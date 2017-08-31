#include "sidepanel.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSignalMapper>
#include <QDebug>
#include <QSettings>

FilterCheckBox::FilterCheckBox(QString name, EntityData::type et, QWidget *parent): QCheckBox(parent)
{
    setText(name);
    filterType = QVariant(static_cast<int>(et)).toString();

    loadSetting();

    connect(this, SIGNAL(clicked(bool)),this,SLOT(updateSetting(bool)));
}

FilterCheckBox::FilterCheckBox(QString name, QString et, QWidget *parent): QCheckBox(parent)
{
    setText(name);
    filterType = et;

    loadSetting();

    connect(this, SIGNAL(clicked(bool)),this,SLOT(updateSetting(bool)));
}

void FilterCheckBox::loadSetting()
{
    QSettings settings;
    setChecked(settings.value(QVariant(filterType).toString(), false).toBool());
}

void FilterCheckBox::updateSetting(bool value)
{
    QSettings settings;
    settings.setValue(QVariant(filterType).toString(), value);
}

SidePanel::SidePanel(QWidget *parent) : QWidget(parent)
{
////////////////////////////////////VEHICLES///////////////////////////////
    QVBoxLayout*    panelLayout = new QVBoxLayout;

    QGroupBox*  vehBox = new QGroupBox("Vehicles");
    QVBoxLayout*    vehLayout = new QVBoxLayout;

    QVector<FilterCheckBox*> veh;
    veh.append(new FilterCheckBox("Cars", EntityData::type::car));
    veh.append(new FilterCheckBox("Motorcycles", EntityData::type::motorcycle));
    veh.append(new FilterCheckBox("Airplanes", EntityData::type::airplane));
    veh.append(new FilterCheckBox("Ships", EntityData::type::ship));
    veh.append(new FilterCheckBox("Helicopters", EntityData::type::helicopter));
    veh.append(new FilterCheckBox("Parachutes", EntityData::type::parachute));
    veh.append(new FilterCheckBox("Tanks", EntityData::type::tank));

    mapCheckBoxes(veh, vehLayout);

    vehBox->setLayout(vehLayout);
    panelLayout->addWidget(vehBox);

////////////////////////////////BUILDINGS///////////////////////////////////
    QGroupBox*      buildBox = new QGroupBox("Buildings");
    QVBoxLayout*    buildLayout = new QVBoxLayout;

    QVector<FilterCheckBox*> build;
    build.append(new FilterCheckBox("Tents", EntityData::type::tent));
    build.append(new FilterCheckBox("Stashes", EntityData::type::stash));
    build.append(new FilterCheckBox("Hedgehog", EntityData::type::hedgehog));
    build.append(new FilterCheckBox("Wooden Fences", EntityData::type::fence));
    build.append(new FilterCheckBox("Ammo Boxes", EntityData::type::ammoBox));
    build.append(new FilterCheckBox("Camp Fires", EntityData::type::campFire));
    build.append(new FilterCheckBox("Crash Sites", EntityData::type::crashSite));
    mapCheckBoxes(build, buildLayout);

    buildBox->setLayout(buildLayout);
    panelLayout->addWidget(buildBox);
///////////////////////////////ENTITIES////////////////////////////////////////
    QGroupBox*      entBox = new QGroupBox("Entities");
    QVBoxLayout*    entLayout = new QVBoxLayout;

    QVector<FilterCheckBox*> en;
    en.append(new FilterCheckBox("Animals", EntityData::type::animals));
    en.append(new FilterCheckBox("Players", EntityData::type::players));
    en.append(new FilterCheckBox("Useless Stuff", EntityData::type::stuff));

    mapCheckBoxes(en, entLayout);

    entBox->setLayout(entLayout);
    panelLayout->addWidget(entBox);
//////////////////////////Other/////////////////////////////////////////////
    QGroupBox*      othBox = new QGroupBox("Other");
    QVBoxLayout*    othLayout = new QVBoxLayout;

    QVector<FilterCheckBox*> oth;
    oth.append(new FilterCheckBox("Object Names", "name"));

    mapCheckBoxes(oth, othLayout);

    othBox->setLayout(othLayout);
    panelLayout->addWidget(othBox);
//////////////////////////////////////////////////////////////////////////////

    panelLayout->addStretch(1);
    setLayout(panelLayout);
}

void SidePanel::mapCheckBoxes(QVector<FilterCheckBox *> &v, QVBoxLayout *bl)
{
    for(QVector<FilterCheckBox*>::iterator it = v.begin(); it!=v.end(); ++it)
    {
        bl->addWidget(*it);
    }
}
