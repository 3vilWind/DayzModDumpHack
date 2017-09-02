#include "interactivemap.h"
#include "settingsmanager.h"
#include <QDebug>
#include <QtGlobal>
#include <QMap>

InteractiveMap::InteractiveMap(QWidget* pwgt) : QWidget(pwgt)
{
}

InteractiveMap::InteractiveMap(WorldState *ws, QWidget* pwgt) : QWidget(pwgt)
{
    painter = new QPainter();
    image   = new QPixmap("D:/bigmap.jpg");

    scale = 1;

    QPalette pal;
    pal.setColor(QPalette::Background,QColor(Qt::GlobalColor::black));
    setPalette(pal);
    setAutoFillBackground(true);
    worldState = ws;
}

InteractiveMap::~InteractiveMap()
{
    delete painter;
    delete image;
}

void InteractiveMap::paintEvent(QPaintEvent *pe)
{
    painter->begin(this);
//////////////////////////////////////////////////
    QTransform mat;
    painter->setTransform(mat);
    painter->scale(scale, scale);
    painter->translate(translate);

    painter->drawPixmap(0,0, *image);

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
                painter->setPen(QPen(Qt::red, SettingsManager::instance().value("pen").toInt(), Qt::SolidLine));
                painter->setFont(QFont("Arial", SettingsManager::instance().value("slider").toInt()));

                if(getFilterValue(QString("name")))
                    painter->drawText(x,y,(*i).name);
            }
        }
    }
//////////////////////////////////////////////////
    painter->end();
}

void InteractiveMap::updateScale(qreal value, const QPointF& dpos)
{
    qreal newScale = scale * value;
    if(newScale >= 0.5 && newScale <= 8)
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
    float dScale = (pe->angleDelta().y() < 0) ? 0.5 : 2;

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
