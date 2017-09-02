#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "memoryapi.h"
#include "entitydata.h"

class EntityRange
{
public:
    EntityRange(): start(0), end(0) {}
    EntityRange(const QVector<EntityData>::const_iterator& s, const QVector<EntityData>::const_iterator& e): start(s), end(e){}
    QVector<EntityData>::const_iterator start;
    QVector<EntityData>::const_iterator end;
};

class WorldState : public QObject
{
    Q_OBJECT
public:
    explicit WorldState(QObject *parent = nullptr);

    QMap <EntityData::type, EntityRange> entityRanges;
private:
    QVector <EntityData> entityArray;
    QVector<quint32> masterOffsets;
    QVector<quint32> tableOffsets;
    quint32          objTableAddress;
    void handleEntity   (quint32 entityAddress, MemoryAPI& mem);
    //void handleInventory(quint32 inventoryAddress, MemoryAPI& mem);

public slots:
    void loadDump();
    void loadState();
    void saveState();
};

#endif // WORLDSTATE_H
