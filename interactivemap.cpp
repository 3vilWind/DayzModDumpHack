#include "interactivemap.h"
#include "settingsmanager.h"
#include <QDebug>
#include <QtGlobal>
#include <QMap>

InteractiveMap::InteractiveMap(QWidget* pwgt) : QWidget(pwgt)
{
    painter = new QPainter();
    image   = new QPixmap("D:/bigmap.jpg");

    scale = 1;

    QPalette pal;
    pal.setColor(QPalette::Background,QColor(Qt::GlobalColor::black));
    setPalette(pal);
    setAutoFillBackground(true);

    worldState = nullptr;
    isActive = false;
}

InteractiveMap::~InteractiveMap()
{
    delete worldState;
    delete painter;
    delete image;
}

void InteractiveMap::paintEvent(QPaintEvent *pe)
{
    if(isActive)
    {
        painter->begin(this);
    //////////////////////////////////////////////////
        QTransform mat;
        painter->setTransform(mat);
        painter->scale(scale, scale);
        painter->translate(translate);

        painter->drawPixmap(0,0, *image);

        for(QMap<EntityData::type, QMap<float, QPicture>>::iterator it = renderData.begin();it!=renderData.end();++it)
        {
            if(getFilterValue(it.key()))
            {
                //painter->scale(scale, scale);
                painter->drawPicture(0, 0, it.value()[scale]);
            }
        }

        /*for(QMap<EntityData::type, EntityRange>::const_iterator it = worldState->entityRanges.cbegin(); it!=worldState->entityRanges.cend();++it)
        {
            if(getFilterValue(it.key()))
            {
                for(QVector<EntityData>::const_iterator i = it.value().start; i!= it.value().end; ++i)
                {
                    float x = (*i).coords.x();
                    float y = (*i).coords.y();
                    x = (((x) / (15360.0f / 975.0f)));
                    y = (((15360.0f - y) / (15360.0f / 970.0f)) - 4);
                    painter->setPen(QPen(Qt::red, SettingsManager::instance().value("pen").toInt(), Qt::SolidLine));
                    painter->setFont(QFont("Arial", SettingsManager::instance().value("slider").toInt()));

                    if(getFilterValue(QString("name")))
                        painter->drawText(x,y,(*i).name);
                }
            }
        }*/
    //////////////////////////////////////////////////
        painter->end();
    }

}

void InteractiveMap::updateScale(qreal value, const QPointF& dpos)
{
    qreal newScale = scale * value;
    if(newScale >= minScale && newScale <=maxScale)
    {
        scale=newScale;
        translate+=dpos/scale;
        update();
    }
}

void InteractiveMap::updateTranslate(const QPointF& value)
{
    QPointF newV = translate + (value * 1/scale);
    translate = newV;

    //if(newX >= -image->width() && newX <= image->width())
    //    translate.setX(newX);

    //if(newY >= -image->height() && newY <= image->height())
    //    translate.setY(newY);
}

void InteractiveMap::mousePressEvent(QMouseEvent *pe)
{
    startMove = pe->pos();
}

void InteractiveMap::mouseMoveEvent(QMouseEvent *pe)
{
    updateTranslate(pe->pos() - startMove);
    startMove = pe->pos();

    update();
}

void InteractiveMap::wheelEvent(QWheelEvent *pe)
{
    float dScale = (pe->angleDelta().y() < 0) ? 1/scaleStep : scaleStep;

    QPointF nPos = pe->pos() * (dScale);
    QPointF dPos = pe->pos() - nPos;

    updateScale(dScale,dPos);
}

bool InteractiveMap::getFilterValue(EntityData::type t)
{
    return SettingsManager::instance().value(QVariant(static_cast<int>(t)).toString(), false).toBool();
}

bool InteractiveMap::getFilterValue(QString t)
{
    return SettingsManager::instance().value(t, false).toBool();
}

void InteractiveMap::updateFilterEntities()
{

}

void InteractiveMap::loadDump()
{
    delete worldState;

    worldState = new WorldState;
    worldState->loadDump("123", "134");
    cacheRenderData();
    //isActive = true;
    //worldState->loadDump(dumpFile, idxFile);
}

void InteractiveMap::loadState()
{
    delete worldState;

    worldState = new WorldState;
    worldState->loadState("123");
    isActive = true;
    //worldState->loadState(stateFile);
}

void InteractiveMap::cacheRenderData()
{
    cachingInput = new QVector<EntityLayer>;

    for(QMap<EntityData::type, EntityRange>::iterator it = worldState->entityRanges.begin(); it!=worldState->entityRanges.end(); ++it)
    {
        for(float i = minScale; i<=maxScale;i*=scaleStep)
        {
            renderData[it.key()][i] = QPicture();
            EntityLayer obj(&(*it), i, &renderData[it.key()][i]);
            cachingInput->append(obj);
        }
    }
    QFuture<void> future = QtConcurrent::map(*cachingInput, &EntityLayer::renderToPixmap);
    QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
    connect(watcher, SIGNAL(finished()), this, SLOT(dataReady()));
    //connect(&watcher, &QFutureWatcher<void>::finished, this, &InteractiveMap::dataReady);
    connect(watcher, &QFutureWatcher<void>::progressValueChanged, this, &InteractiveMap::debug);
    watcher->setFuture(future);
}

void InteractiveMap::debug(int a)
{
    qDebug() << a;
}

void InteractiveMap::dataReady()
{
    qDebug() << "READY";
    isActive = true;
    update();
}

void EntityLayer::renderToPixmap()
{
    //pixmap->fill(Qt::transparent);
    QPainter painter(pixmap);


    for(QVector<EntityData>::const_iterator i = range->start; i!= range->end; ++i)
    {
        //painter.scale(1/scale, 1/scale);


        QFont f("Arial");
        QPen  p;
        p.setWidthF(4/scale);
        p.setStyle(Qt::SolidLine);
        painter.setPen(p);
        f.setPointSizeF(qMax(float(8*1/scale),2.0f));
        painter.setFont(f);
        //painter.setRenderHint(QPainter::Antialiasing);
        float x = (*i).coords.x();
        float y = (*i).coords.y();
        x = (((x) / (15360.0f / 975.0f)));
        y = (((15360.0f - y) / (15360.0f / 970.0f)) - 4);

        //if(SettingsManager::instance().value(QString("name"), false).toBool())
        painter.drawText(x,y,(*i).name);
        painter.drawPoint(x,y);
    }
}
