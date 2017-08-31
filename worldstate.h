#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "memoryapi.h"
#include "entitydata.h"

class WorldState : public QObject
{
    Q_OBJECT
public:
    explicit WorldState(QObject *parent = nullptr);
    QVector <EntityData> entityArray;
    QMap <EntityData::type, QString> checkBoxRelations;
private:
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
