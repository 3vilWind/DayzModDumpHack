#ifndef ENTITYDATA_H
#define ENTITYDATA_H

#include <QString>
#include <QMap>
#include <QPointF>

class EntityData
{
public:
    enum class type {airplane, car, motorcycle, ship, helicopter, parachute, tank,
                     tent, stash, fence, ammoBox, campFire, crashSite, animals,
                     players, zombies, stuff, hedgehog, invalid};

    type entityType;

    EntityData();
    EntityData(QString n, QPointF c, type t = type::stuff);

    QString shortDescription();
    QString fullDescription();

    QString name;
    QPointF coords;
    QMap<QString, QString> additionalFields;
};
#endif // ENTITYDATA_H
