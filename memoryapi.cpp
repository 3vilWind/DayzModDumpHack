#include "memoryapi.h"
#include <limits>
#include <QDebug>

MemoryAPI::MemoryAPI(QObject *parent) : QObject(parent)
{

}

MemoryAPI::MemoryAPI(QString pathDump, QString pathIDX)
{
    //qDebug() << pathDump << pathIDX;
    loadDump(pathDump);
    loadIDX(pathIDX);
}

quint32 MemoryAPI::convertVirtToPhys(quint32 virt)
{
    for(auto it = memoryRelations.begin(); it != memoryRelations.end(); ++it)
    {
        //qDebug() << hex <<  virt << (*it).getVirtualAddress() << (*it).getSize();
        if((*it).inRange(virt))
        {
            const quint32& phBase = (*it).getPhysicalAddress(), vrBase = (*it).getVirtualAddress();

            if(phBase>vrBase)
                return virt + (phBase - vrBase);
            else
                return virt - (vrBase - phBase);
        }
    }
    throw 1;
}

void MemoryAPI::loadIDX(QString path)
{
    QFile file(path);
    //qDebug() <<
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        for(quint8 i = 0; i<2; ++i)
            stream.readLine();

        while(!stream.atEnd())
        {
            quint64 fileAddress, length, virtualAddress;
            stream >> fileAddress >> length >> virtualAddress;
            if(fileAddress > std::numeric_limits<quint32>::max() || virtualAddress > std::numeric_limits<quint32>::max())
                continue;
            memoryRelations.append(MemoryRange(virtualAddress, fileAddress, length));
        }
    }else
        qDebug() << "IDX файл не открывается";
}

void MemoryAPI::loadDump(QString path)
{
    dumpFile.setFileName(path);
    dumpFile.open(QIODevice::ReadOnly);
}

QByteArray MemoryAPI::readVirtMem(quint32 baseAddr, quint32 size)
{
    //qDebug() << hex << baseAddr;
    QByteArray result;
    try{
        quint32 addr = convertVirtToPhys(baseAddr);
        //qDebug() << hex << addr;
        dumpFile.seek(addr);
        result = dumpFile.read(size);

        //qDebug() << hex << result.toUInt(nullptr,16);
    }catch(int i)
    {
        1+1;
        //qDebug() << "Ошибка с конвертацией адреса";
    }
    return result;
}

qint32 MemoryAPI::readInt(quint32 offset)
{
    qint32 a;
    QDataStream ds(readVirtMem(offset, 4));
    ds.setByteOrder(QDataStream::LittleEndian);
    ds >> a;
    return a;
}

quint32 MemoryAPI::readPtr(quint32 offset)
{
    quint32 a;
    QDataStream ds(readVirtMem(offset, 4));
    ds.setByteOrder(QDataStream::LittleEndian);

    ds >> a;
    return a;
}

QString MemoryAPI::readStringAscii(quint32 offset, quint32 size)
{
    return QString(readVirtMem(offset, size));
}

float MemoryAPI::readFloat(quint32 offset)
{
    float a;
    QDataStream ds(readVirtMem(offset, 4));
    ds.setByteOrder(QDataStream::LittleEndian);
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);

    ds >> a;
    return a;
}

QString MemoryAPI::readArmaString(quint32 offset)
{
    const int maxStringLength = 0x40;
    int absoluteLength = readPtr(offset + 0x4);
    if (absoluteLength > maxStringLength)
    {
        return QString("");
    }

    return readStringAscii(offset + 8, absoluteLength);
}
