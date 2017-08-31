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

    checkBoxRelations[EntityData::type::airplane] = "Airplanes";
    checkBoxRelations[EntityData::type::ammoBox] = "Ammo Boxes";
    checkBoxRelations[EntityData::type::animals] = "Animals";
    checkBoxRelations[EntityData::type::campFire] = "Airplanes";
    checkBoxRelations[EntityData::type::car] = "Airplanes";
    checkBoxRelations[EntityData::type::crashSite] = "Airplanes";
    checkBoxRelations[EntityData::type::fence] = "Airplanes";
}

void WorldState::loadDump()
{
    MemoryAPI mem("D:/ArmA2OA.exe_12204.dmp","D:/ArmA2OA.exe_12204.dmp.idx");

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
    else if(objName.indexOf("DZ_MedBox")!=-1 || objName.indexOf("DZ_AmmoBox"))
        ed.entityType = EntityData::type::ammoBox;
    else if(objName.indexOf("Hedgehog_DZ")!=-1)
        ed.entityType = EntityData::type::hedgehog;
    else if(objName.indexOf("Land_Camp_Fire_DZ")!= -1)
        ed.entityType = EntityData::type::campFire;
    else if(objName.indexOf("CrashSite")!= -1)
        ed.entityType = EntityData::type::crashSite;
    else if(objName.indexOf("WildBoar")!= -1 || objName.indexOf("Rabbit")!= -1 ||
            objName.indexOf("Cow")!= -1 || objName.indexOf("Sheep")!= -1 ||
            objName.indexOf("Goat")!= -1 || objName.indexOf("Hen")!= -1)
        ed.entityType = EntityData::type::animals;
    else if(objName.indexOf("Survivor2_DZ")!= -1 || objName.indexOf("Sniper1_DZ")!=-1 ||
            objName.indexOf("Camo1_DZ")!=-1 || objName.indexOf("Survivor3_DZ")!=-1 ||
            objName.indexOf("Bandit1_DZ")!= -1)
        ed.entityType = EntityData::type::players;
    else
        ed.entityType = EntityData::type::stuff;

    entityArray.append(ed);
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
