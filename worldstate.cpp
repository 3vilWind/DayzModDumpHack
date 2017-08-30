#include "worldstate.h"
#include <QDebug>

WorldState::WorldState(QObject *parent) : QObject(parent)
{
    masterOffsets.append(0x87c + 0x4);
    masterOffsets.append(0xb20 + 0x4);
    masterOffsets.append(0xdc4 + 0x4);

    tableOffsets.append(0x4);
    tableOffsets.append(0xac);
    tableOffsets.append(0x154);
    tableOffsets.append(0x1fc);

    objTableAddress = 0xDAD8C0;
}

void WorldState::loadDump()
{
    MemoryAPI mem("D:/ArmA2OA.exe_8000.dmp","D:/ArmA2OA.exe_8000.dmp.idx");

    for(auto mO = masterOffsets.begin(); mO != masterOffsets.end(); ++mO)
    {
        quint32 entityTableBasePtr = mem.readPtr(objTableAddress) + (*mO);
        for(auto tO = tableOffsets.begin(); tO != tableOffsets.end(); ++tO)
        {
            qint32 size = mem.readInt(entityTableBasePtr + 0x8 +(*tO));

            for(qint32 i = 0; i!=size; ++i)
            {
                quint32 fPtr = mem.readPtr(entityTableBasePtr + (*tO) + 4);
                quint32 entityAddress = mem.readPtr(fPtr + 4 * i);

                handleEntity(entityAddress, mem);
            }
        }
    }
}

void WorldState::handleEntity(quint32 entityAddress, MemoryAPI &mem)
{
    quint32 obj1 = entityAddress;
    quint32 pCfgVehicle = mem.readPtr(obj1 + 0x3C);
    quint32 obj3 = mem.readPtr(pCfgVehicle + 0x30);
    quint32 pObjType = mem.readPtr(pCfgVehicle + 0x6C);
    quint32 pEntityInventory = mem.readPtr(obj1 + 0x21C);

    QString objType = mem.readArmaString(pObjType);
    QString objName = mem.readStringAscii(obj3 + 0x8, 25);

    quint32 coords = mem.readPtr(obj1 + 0x18);
    float coordX = mem.readFloat(coords + 0x28);
    float coordY = mem.readFloat(coords + 0x30);
    //handleInventory(pEntityInventory, mem);

    if(objName.indexOf("TentStorage")!=-1)// || objName.indexOf("Stash")!=-1)
    //if(objType == "car" || objType == "helicopter" || objType == "airplane" || objType == "motorcycle")
        qDebug() << objName << objType << coordX/100 << (15360-coordY)/100;
}

/*void WorldState::handleInventory(quint32 inventoryAddress, MemoryAPI &mem)
{
    qint32 weaponTableSize = mem.readInt(inventoryAddress + 0x10);
    qint32 itemTableSize = mem.readInt(inventoryAddress + 0x1C);
    quint32 weaponBasePtr = mem.readPtr(inventoryAddress + 0xC);
    quint32 itemBasePtr = mem.readPtr(inventoryAddress + 0x18);
    for(qint32 i = 0; i<weaponTableSize; ++i)
    {
        quint32 weaponPtr = weaponBasePtr + (i*4);

    }
}*/

void WorldState::loadState()
{

}

void WorldState::saveState()
{

}
