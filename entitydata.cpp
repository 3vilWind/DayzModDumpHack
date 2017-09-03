#include "entitydata.h"

EntityData::EntityData()
{
    entityType = type::invalid;
}

EntityData::EntityData(QString n, QPointF c, type t)
{
    name = n;
    coords = c;
    entityType = t;
}

QString EntityData::shortDescription()
{
    return name;
}

QString EntityData::fullDescription()
{
    return name;
}
