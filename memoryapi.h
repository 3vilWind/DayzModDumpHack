#ifndef MEMORYAPI_H
#define MEMORYAPI_H

#include <QObject>
#include <QByteArray>
#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
class MemoryRange;

class MemoryAPI : public QObject
{
    Q_OBJECT
public:
    explicit MemoryAPI(QObject *parent = nullptr);
    MemoryAPI(QString pathDump, QString pathIDX);

    quint32 readPtr      (quint32 offset);
    qint32  readInt      (quint32 offset);
    float   readFloat    (quint32 offset);
    QString readStringAscii(quint32 offset, quint32 size);
    QString readArmaString(quint32 offset);

    void loadIDX        (QString path);
    void loadDump       (QString path);
private:
    QVector <MemoryRange> memoryRelations;
    quint32 convertVirtToPhys(quint32 virt);
    QByteArray readVirtMem(quint32 baseAddr, quint32 size);
    QFile dumpFile;

signals:

public slots:
};

class MemoryRange
{
public:
    MemoryRange() {}
    MemoryRange(quint32 virt, quint32 phys, quint32 len) : baseVirtualAddress(virt), basePhysicalAddress(phys), size(len) {}
    bool inRange(quint32 addr) { return (addr >= baseVirtualAddress) && (addr < baseVirtualAddress + size); }

    quint32 getVirtualAddress(){ return baseVirtualAddress; }
    quint32 getPhysicalAddress(){ return basePhysicalAddress; }
    quint32 getSize(){ return size; }

    quint32 setVirtualAddress(quint32 addr){ baseVirtualAddress = addr; }
    quint32 setPhysicalAddress(quint32 addr){ basePhysicalAddress = addr; }
    quint32 setSize(quint32 len){ size = len; }
private:
    quint32 baseVirtualAddress;
    quint32 basePhysicalAddress;
    quint32 size;
};

#endif // MEMORYAPI_H
