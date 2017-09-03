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
#include <QList>
#include <QPicture>
#include <QMutex>
#include <QtConcurrent>
#include <QVector>
#include <QImage>
#include "worldstate.h"

//QString findCloseObjects(QPointF pos);

class CloseObjects
{
public:
    CloseObjects() {}
    CloseObjects(EntityRange *r, QPointF p): range(r), coords(p) {}
    QString findCloseObjects() const;
private:
    EntityRange*    range;
    QPointF         coords;
};

/*class EntityLayer
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
*/
class InteractiveMap : public QWidget
{
    Q_OBJECT
protected:
    virtual void paintEvent(QPaintEvent* pe);

public:
    InteractiveMap(QWidget* pwgt = nullptr);
    virtual ~InteractiveMap();
public slots:
    void loadState(QString stateFile);
    void loadDump(QString dumpFile, QString idxFile);
    void closeState();
    void saveState(QString stateFile);
    void updateCache();
    void sendCloseObjects();

signals:
    void showCloseObjects(QString str);
    void saveStateChanged(bool state);
    //void closeStateChanged(bool state);
private:
    const float minScale = 0.5;
    const float maxScale = 8.0;
    const float scaleStep= 2.0;

    void updateScale(const qreal value, const QPointF& dpos);
    void updateTranslate(const QPointF& value);

    bool getFilterValue(EntityData::type t);
    bool getFilterValue(QString t);

    void mousePressEvent  (QMouseEvent* pe);
    void mouseMoveEvent   (QMouseEvent* pe);
    void wheelEvent       (QWheelEvent *pe);

    void findCloseObjects(QPointF coords);
    QVector<CloseObjects>* input;

    QPainter*   painter;
    QPixmap*    image;
    WorldState* worldState;

    qreal scale;
    QPointF translate;
    QPoint startMove;

    bool isCached;
    QPixmap cache;

    QMutex renderMutex;
    QFutureWatcher<QString> closeObjWatcher;
    QFuture<QString> closeObjFuture;
};

#endif // INTERACTIVEMAP_H
