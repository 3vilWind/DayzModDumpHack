#ifndef INTERACTIVEMAP_H
#define INTERACTIVEMAP_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPixmap>
#include <QPainter>
#include <QSettings>
#include <QMap>
#include <QtConcurrent>
#include <QPicture>
#include "worldstate.h"

class EntityLayer
{
public:
    EntityLayer() {}
    EntityLayer(EntityRange *r, float s, QPicture* pix): range(r), scale(s), pixmap(pix) {}
    void renderToPixmap();
private:
    EntityRange* range;
    float       scale;
    QPicture*    pixmap;
};

class InteractiveMap : public QWidget
{
    Q_OBJECT
protected:
    virtual void paintEvent(QPaintEvent* pe);

public:
    InteractiveMap(QWidget* pwgt = nullptr);
    virtual ~InteractiveMap();

    bool isActive;

    const float minScale = 0.5;
    const float maxScale = 8.0;
    const float scaleStep= 2.0;
public slots:
    void loadState();
    void loadDump();
    void dataReady();
    void debug(int a);
private:
    WorldState* worldState;

    void updateScale(const qreal value, const QPointF& dpos);
    void updateTranslate(const QPointF& value);

    bool getFilterValue(EntityData::type t);
    bool getFilterValue(QString t);

    void updateFilterEntities();

    void cacheRenderData();

    QMap<EntityData::type, QMap<float, QPicture>> renderData;

    QPainter*   painter;
    QPixmap*    image;
    void mousePressEvent  (QMouseEvent* pe);
    void mouseMoveEvent   (QMouseEvent* pe);
    void wheelEvent       (QWheelEvent *pe);

    qreal scale;
    QPointF translate;
    QPoint startMove;


    QVector<EntityLayer> *cachingInput;
};

#endif // INTERACTIVEMAP_H
