#include "worldstate.h"
#include <QDebug>
#include <algorithm>
#include <QCoreApplication>
#include <QProgressDialog>
#include <QWidget>

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

void WorldState::loadDump(const QString& dumpFile, const QString& idxFile)
{
    QProgressDialog progress;
    progress.setCancelButton(nullptr);
    progress.setLabelText("Loading dump...");
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimum(0);

    progress.setMaximum(masterOffsets.length()+2);
    progress.show();

    MemoryAPI mem("D:/ArmA2OA.exe_8000.dmp","D:/ArmA2OA.exe_8000.dmp.idx");
    progress.setValue(1);

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

                QCoreApplication::processEvents();
            }
        }
        progress.setValue(progress.value()+1);
    }
    initRanges();
    progress.setValue(progress.value()+1);
}

void WorldState::loadState(const QString &stateFile)
{

}

void WorldState::handleEntity(quint32 entityAddress, MemoryAPI &mem)
{
    QString objType;
    QString objName;
    float coordX;
    float coordY;

    try{
        quint32 obj1 = entityAddress;
        quint32 pCfgVehicle = mem.readPtr(obj1 + 0x3C);
        quint32 obj3 = mem.readPtr(pCfgVehicle + 0x30);
        quint32 pObjType = mem.readPtr(pCfgVehicle + 0x6C);
        //quint32 pEntityInventory = mem.readPtr(obj1 + 0x21C);

        objType = mem.readArmaString(pObjType);
        objName = mem.readStringAscii(obj3 + 0x8, 25);

        quint32 coords = mem.readPtr(obj1 + 0x18);
        coordX = mem.readFloat(coords + 0x28);
        coordY = mem.readFloat(coords + 0x30);
    }catch(int a)
    {
        return;
    }


    EntityData ed(objName, QPointF(coordX, coordY));

    if(objType == "car")
        ed.entityType = EntityData::type::car;
    else if(objType == "motorcycle")
        ed.entityType = EntityData::type::motorcycle;
    else if(objType == "airplane")
        ed.entityType = EntityData::type::airplane;
    else if(objType == "helicopter")
        ed.entityType = EntityData::type::helicopter;
    else if(objType == "ship")
        ed.entityType = EntityData::type::ship;
    else if(objType == "tank")
        ed.entityType = EntityData::type::tank;
    else if(objType == "parachute")
        ed.entityType = EntityData::type::parachute;
    else if(objName.indexOf("TentStorage")!=-1)
        ed.entityType = EntityData::type::tent;
    else if(objName.indexOf("Stash")!=-1)
        ed.entityType = EntityData::type::stash;
    else if(objName.indexOf("WoodenGate")!=-1 || objName.indexOf("WoodenFence")!=-1)
        ed.entityType = EntityData::type::fence;
    else if(objName.indexOf("DZ_MedBox")!=-1 || objName.indexOf("DZ_AmmoBox")!=-1)
        ed.entityType = EntityData::type::ammoBox;
    else if(objName.indexOf("Hedgehog_DZ")!=-1)
        ed.entityType = EntityData::type::hedgehog;
    else if(objName.indexOf("Land_Camp_Fire_DZ")!= -1)
        ed.entityType = EntityData::type::campFire;
    else if(objName.indexOf("CrashSite")!= -1)
        ed.entityType = EntityData::type::crashSite;
    else if(objName.indexOf("WildBoar")== 0 || objName.indexOf("Rabbit")== 0 ||
            objName.indexOf("Cow")== 0 || objName.indexOf("Sheep")== 0 ||
            objName.indexOf("Goat")== 0 || objName.indexOf("Hen")== 0)
        ed.entityType = EntityData::type::animals;
    else if(objName.indexOf("Survivor2_DZ")!= -1 || objName.indexOf("Sniper1_DZ")!=-1 ||
            objName.indexOf("Camo1_DZ")!=-1 || objName.indexOf("Survivor3_DZ")!=-1 ||
            objName.indexOf("Bandit1_DZ")!= -1)
        ed.entityType = EntityData::type::players;
    else
        ed.entityType = EntityData::type::stuff;

    entityArray.append(ed);
}

void WorldState::initRanges()
{
    std::sort(entityArray.begin(),entityArray.end(), [](const EntityData& lhs, const EntityData& rhs){ return lhs.entityType < rhs.entityType; });
    EntityData::type currentType = EntityData::type::invalid;
    EntityRange currentRange;
    for(QVector<EntityData>::const_iterator it = entityArray.cbegin(); it != entityArray.cend(); ++it)
    {
        if(currentType == (*it).entityType)
            ++currentRange.end;
        else
        {
            if(currentType != EntityData::type::invalid)
                entityRanges[currentType] = currentRange;

            currentRange = EntityRange(it, it+1);
            currentType = (*it).entityType;
        }
    }
    if(!entityRanges.contains(currentType))
        entityRanges[currentType] = currentRange;
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
