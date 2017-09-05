#include "worldstate.h"
#include <QDebug>
#include <algorithm>
#include <QCoreApplication>
#include <QProgressDialog>
#include <QWidget>
#include <QtXml>

WorldState::WorldState(const QString& dumpFile, const QString& idxFile)
{
    initOffsets();

    QProgressDialog progress;
    progress.setCancelButton(nullptr);
    progress.setLabelText("Loading dump...");
    progress.setModal(true);
    progress.setMinimum(0);

    progress.setMaximum(masterOffsets.length()+2);
    progress.show();

    MemoryAPI mem(dumpFile,idxFile);
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
    worldName = "chernarus";
    progress.setValue(progress.value()+1);
}

WorldState::WorldState(const QString &stateFile)
{
    initOffsets();

    QProgressDialog progress;
    progress.setCancelButton(nullptr);
    progress.setLabelText("Loading dump...");
    progress.setModal(true);
    progress.setMinimum(0);

    progress.setMaximum(masterOffsets.length()+2);
    progress.show();

    QDomDocument domDoc;
    QFile        file(stateFile);

    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement rootElement = domDoc.documentElement();
            worldName = rootElement.attribute("name", "chernarus");
            progress.setMaximum(rootElement.childNodes().count());
            //Iterate entities
            for(QDomNode domNode = rootElement.firstChild(); !domNode.isNull(); domNode = domNode.nextSibling())
            {
                if(domNode.isElement()) {
                    EntityData ent;
                    //ent.name
                    QDomElement entity = domNode.toElement();
                    for(QDomNode entNode = entity.firstChild(); !entNode.isNull(); entNode = entNode.nextSibling())
                    {
                        if(entNode.isElement())
                        {
                            QDomElement entElem = entNode.toElement();
                            if(entElem.tagName() == "name")
                            {
                                ent.name = entElem.text();
                            }else if(entElem.tagName() == "type")
                            {
                                ent.entityType = static_cast<EntityData::type>(entElem.text().toInt());
                            }
                            else if(entElem.tagName() == "coords")
                            {
                                for(QDomElement entText = entElem.firstChild().toElement(); !entText.isNull(); entText = entText.nextSibling().toElement())
                                {
                                    if(entText.tagName() == "x")
                                        ent.coords.setX(entText.text().toFloat());
                                    else if(entText.tagName() == "y")
                                        ent.coords.setY(entText.text().toFloat());
                                }
                            }else if(entElem.tagName() == "options")
                            {
                                for(QDomElement entText = entElem.firstChild().toElement(); !entText.isNull(); entText = entText.nextSibling().toElement())
                                {
                                    ent.additionalFields[entText.tagName()] = entText.text();
                                }
                            }
                        }
                        QCoreApplication::processEvents();
                    }
                    entityArray.append(ent);
                }
                progress.setValue(progress.value()+1);
            }

        }
        file.close();
        initRanges();
    }
}

void WorldState::initOffsets()
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

void WorldState::saveState(const QString &stateFile)
{
    QDomDocument state("MapHackWorldState");
    QDomElement world = makeElement(state, "world");
    world.setAttribute("name", "chernarus");
    state.appendChild(world);

    for(QVector<EntityData>::const_iterator it = entityArray.cbegin(); it != entityArray.cend(); ++it)
    {
        QDomElement entity = makeElement(state, "entity");
        entity.appendChild(makeElement(state, "name", it->shortDescription()));
        entity.appendChild(makeElement(state, "type", QVariant(static_cast<int>(it->entityType)).toString()));
        QDomElement coords = makeElement(state, "coords");
        coords.appendChild(makeElement(state, "x", QVariant(it->coords.x()).toString()));
        coords.appendChild(makeElement(state, "y", QVariant(it->coords.y()).toString()));
        entity.appendChild(coords);

        if(!it->additionalFields.isEmpty())
        {
            QDomElement options = makeElement(state, "options");
            for(QMap<QString,QString>::const_iterator i = it->additionalFields.cbegin(); i!=it->additionalFields.cend(); ++i)
            {
                options.appendChild(makeElement(state, i.key(), i.value()));
            }
            entity.appendChild(options);
        }

        world.appendChild(entity);
    }

    QFile file(stateFile);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << state.toString();
        file.close();
    }
}

QDomElement WorldState::makeElement(QDomDocument &domDoc, const QString &name, const QString &strData)
{
    QDomElement domElement = domDoc.createElement(name);
    if(!strData.isEmpty())
    {
        QDomText domText = domDoc.createTextNode(strData);
        domElement.appendChild(domText);
    }
    return domElement;
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
            objName.indexOf("Bandit1_DZ")!= -1 || objName.indexOf("Soldier1_DZ")!= -1)
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
