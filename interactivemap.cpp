#include "interactivemap.h"
#include <QDebug>
#include <QtGlobal>

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

    for(auto it = worldState->entityArray.begin(); it!=worldState->entityArray.end();++it)
    {
        if(((*it).entityType) == EntityData::type::car)
            qDebug() << (*it).name;
    }
//////////////////////////////////////////////////
    painter->end();
}

void InteractiveMap::updateScale(qreal value, const QPointF& dpos)
{
    qreal newScale = scale * value;
    if(newScale >= 0.5 && newScale <= 16)
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
