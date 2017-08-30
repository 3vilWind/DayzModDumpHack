#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <QObject>
#include <QVector>
#include "memoryapi.h"

class WorldState : public QObject
{
    Q_OBJECT
public:
    explicit WorldState(QObject *parent = nullptr);

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
