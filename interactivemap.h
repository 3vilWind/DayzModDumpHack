#ifndef INTERACTIVEMAP_H
#define INTERACTIVEMAP_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPixmap>
#include <QPainter>

class InteractiveMap : public QWidget
{
protected:
    virtual void paintEvent(QPaintEvent* pe);

public:
    InteractiveMap(QWidget* pwgt = nullptr);
    virtual ~InteractiveMap();

    void updateScale(const qreal value, const QPointF& dpos);
    void updateTranslate(const QPointF& value);
private:
    QPainter* painter;
    QPixmap*  image;
    void mousePressEvent  (QMouseEvent* pe);
    void mouseMoveEvent   (QMouseEvent* pe);
    void wheelEvent(QWheelEvent *pe);

    qreal scale;
    QPointF translate;
    QPoint startMove;

};

#endif // INTERACTIVEMAP_H
