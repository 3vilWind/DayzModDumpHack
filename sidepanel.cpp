#include "sidepanel.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>

SidePanel::SidePanel(QWidget *parent) : QWidget(parent)
{
    /*QPalette pal;
    pal.setColor(QPalette::Background,QColor(Qt::GlobalColor::black));
    setPalette(pal);
    setAutoFillBackground(true);*/
////////////////////////////////////VEHICLES///////////////////////////////
    QVBoxLayout*    panelLayout = new QVBoxLayout;

    QGroupBox*      vehBox = new QGroupBox("Vehicles");
    QVBoxLayout*    vehLayout = new QVBoxLayout;
    QCheckBox*      cars = new QCheckBox("Cars");
    QCheckBox*      moto = new QCheckBox("Motorcycles");
    QCheckBox*      airp = new QCheckBox("Airplanes");
    QCheckBox*      ship = new QCheckBox("Ships");
    QCheckBox*      heli = new QCheckBox("Helicopters");
    QCheckBox*      para = new QCheckBox("Parachutes");
    QCheckBox*      tank = new QCheckBox("Tanks");

    vehLayout->addWidget(cars);
    vehLayout->addWidget(moto);
    vehLayout->addWidget(airp);
    vehLayout->addWidget(ship);
    vehLayout->addWidget(heli);
    vehLayout->addWidget(para);
    vehLayout->addWidget(tank);

    vehBox->setLayout(vehLayout);
    panelLayout->addWidget(vehBox);

////////////////////////////////BUILDINGS///////////////////////////////////
    QGroupBox*      buildBox = new QGroupBox("Buildings");
    QVBoxLayout*    buildLayout = new QVBoxLayout;
    QCheckBox*      tent = new QCheckBox("Tents");
    QCheckBox*      stas = new QCheckBox("Stashes");
    QCheckBox*      fenc = new QCheckBox("Wooden Fences");
    QCheckBox*      ammo = new QCheckBox("Ammo Boxes");
    QCheckBox*      fire = new QCheckBox("Camp Fires");
    QCheckBox*      cras = new QCheckBox("Crash Sites");

    buildLayout->addWidget(tent);
    buildLayout->addWidget(stas);
    buildLayout->addWidget(fenc);
    buildLayout->addWidget(ammo);
    buildLayout->addWidget(fire);
    buildLayout->addWidget(cras);

    buildBox->setLayout(buildLayout);
    panelLayout->addWidget(buildBox);
///////////////////////////////ENTITIES////////////////////////////////////////
    QGroupBox*      entBox = new QGroupBox("Entities");
    QVBoxLayout*    entLayout = new QVBoxLayout;
    QCheckBox*      anim = new QCheckBox("Animals");
    QCheckBox*      play = new QCheckBox("Players");
    QCheckBox*      zomb = new QCheckBox("Zombies");
    /*QCheckBox*      ammo = new QCheckBox("Ammo Boxes");
    QCheckBox*      fire = new QCheckBox("Camp Fires");
    QCheckBox*      cras = new QCheckBox("Crash Sites");*/

    entLayout->addWidget(anim);
    entLayout->addWidget(play);
    entLayout->addWidget(zomb);
    /*entLayout->addWidget(ammo);
    entLayout->addWidget(fire);
    entLayout->addWidget(cras);*/

    entBox->setLayout(entLayout);
    panelLayout->addWidget(entBox);
//////////////////////////////////////////////////////////////////////////////
    panelLayout->addStretch(1);

    setLayout(panelLayout);
}
