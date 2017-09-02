#ifndef INTERACTIVEMAP_H
#define INTERACTIVEMAP_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPixmap>
#include <QPainter>
#include <QSettings>

#include "worldstate.h"

class InteractiveMap : public QWidget
{
protected:
    virtual void paintEvent(QPaintEvent* pe);

public:
    InteractiveMap(QWidget* pwgt = nullptr);
    InteractiveMap(WorldState* ws, QWidget* pwgt = nullptr);
    virtual ~InteractiveMap();


private:
    void updateScale(const qreal value, const QPointF& dpos);
    void updateTranslate(const QPointF& value);

    bool getFilterValue(EntityData::type t);
    bool getFilterValue(QString t);

    void updateFilterEntities();

    WorldState* worldState;
    QPainter*   painter;
    QPixmap*    image;
    void mousePressEvent  (QMouseEvent* pe);
    void mouseMoveEvent   (QMouseEvent* pe);
    void wheelEvent       (QWheelEvent *pe);

    qreal scale;
    QPointF translate;
    QPoint startMove;
};

#endif // INTERACTIVEMAP_H
