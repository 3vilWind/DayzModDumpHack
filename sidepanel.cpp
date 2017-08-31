#include "sidepanel.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSignalMapper>
#include <QDebug>
#include <QSettings>

SidePanel::SidePanel(QWidget *parent) : QWidget(parent)
{
    psigManager = new QSignalMapper(this);
    connect(psigManager, SIGNAL(mapped(QWidget*)), this, SLOT(updateCheckBox(QWidget*)));
////////////////////////////////////VEHICLES///////////////////////////////
    QVBoxLayout*    panelLayout = new QVBoxLayout;

    QGroupBox*  vehBox = new QGroupBox("Vehicles");
    QVBoxLayout*    vehLayout = new QVBoxLayout;

    QVector<QCheckBox*> veh;
    veh.append(new QCheckBox("Cars"));
    veh.append(new QCheckBox("Motorcycles"));
    veh.append(new QCheckBox("Airplanes"));
    veh.append(new QCheckBox("Ships"));
    veh.append(new QCheckBox("Helicopters"));
    veh.append(new QCheckBox("Parachutes"));
    veh.append(new QCheckBox("Tanks"));

    mapCheckBoxes(veh, vehLayout);

    vehBox->setLayout(vehLayout);
    panelLayout->addWidget(vehBox);

////////////////////////////////BUILDINGS///////////////////////////////////
    QGroupBox*      buildBox = new QGroupBox("Buildings");
    QVBoxLayout*    buildLayout = new QVBoxLayout;

    QVector<QCheckBox*> build;
    build.append(new QCheckBox("Tents"));
    build.append(new QCheckBox("Stashes"));
    build.append(new QCheckBox("Hedgehog"));
    build.append(new QCheckBox("Wooden Fences"));
    build.append(new QCheckBox("Ammo Boxes"));
    build.append(new QCheckBox("Camp Fires"));
    build.append(new QCheckBox("Crash Sites"));
    mapCheckBoxes(build, buildLayout);

    buildBox->setLayout(buildLayout);
    panelLayout->addWidget(buildBox);
///////////////////////////////ENTITIES////////////////////////////////////////
    QGroupBox*      entBox = new QGroupBox("Entities");
    QVBoxLayout*    entLayout = new QVBoxLayout;

    QVector<QCheckBox*> en;
    en.append(new QCheckBox("Animals"));
    en.append(new QCheckBox("Players"));
    en.append(new QCheckBox("Useless Stuff"));

    mapCheckBoxes(en, entLayout);

    entBox->setLayout(entLayout);
    panelLayout->addWidget(entBox);
//////////////////////////////////////////////////////////////////////////////
    auto m = veh + build + en;
    loadCheckBoxes(m);
    panelLayout->addStretch(1);
    setLayout(panelLayout);
}

void SidePanel::updateCheckBox(QWidget *wgt)
{
    QCheckBox* d = static_cast<QCheckBox*>(wgt);

    QSettings settings;
    settings.setValue(d->text(),d->isChecked());
}

void SidePanel::mapCheckBoxes(QVector<QCheckBox *> &v, QVBoxLayout *bl)
{
    for(QVector<QCheckBox*>::iterator it = v.begin(); it!=v.end(); ++it)
    {
        connect((*it), SIGNAL(clicked()), psigManager, SLOT(map()));
        psigManager->setMapping((*it), (*it));
        bl->addWidget(*it);
    }
}

void SidePanel::loadCheckBoxes(QVector<QCheckBox *> &v)
{
    QSettings settings;
    for(QVector<QCheckBox*>::iterator it = v.begin(); it!=v.end(); ++it)
    {
        (*it)->setChecked(settings.value((*it)->text(), false).toBool());
    }
}
