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
    isCached = false;
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

        if(!isCached)
        {
            cache = QPixmap(image->size()*4);
            cache.setDevicePixelRatio(4);
            cache.fill(Qt::transparent);
            QPainter cachePaint(&cache);
            for(QMap<EntityData::type, EntityRange>::const_iterator it = worldState->entityRanges.cbegin(); it!=worldState->entityRanges.cend();++it)
            {
                if(getFilterValue(it.key()))
                {
                    for(QVector<EntityData>::const_iterator i = it.value().start; i!= it.value().end; ++i)
                    {
                        float x = (*i).coords.x();
                        float y = (*i).coords.y();
                        x = (((x) / (15360.0f / 975.0f)));
                        y = (((15360.0f - y) / (15360.0f / 970.0f)) - 4);

                        QFont font("Arial");
                        QPen  pen;
                        pen.setWidthF(4/scale);
                        pen.setStyle(Qt::SolidLine);
                        font.setPointSizeF(qMax(float(8*1/scale),2.0f));
                        cachePaint.setFont(font);
                        cachePaint.setPen(pen);

                        if(getFilterValue(QString("name")))
                            cachePaint.drawText(x,y,(*i).name);
                        cachePaint.drawPoint(x,y);
                    }
                }
            }
            isCached = true;
        }
        painter->drawPixmap(0,0,cache);
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
        isCached = false;
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
    if(pe->button() & Qt::LeftButton)
        startMove = pe->pos();
}

void InteractiveMap::mouseMoveEvent(QMouseEvent *pe)
{
    if(pe->buttons() & Qt::LeftButton)
    {
        updateTranslate(pe->pos() - startMove);
        startMove = pe->pos();

        update();
    }

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

void InteractiveMap::updateCache()
{
    isCached = false;
    update();
}

void InteractiveMap::loadDump()
{
    delete worldState;

    worldState = new WorldState;
    worldState->loadDump("123", "134");
    isActive = true;
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
