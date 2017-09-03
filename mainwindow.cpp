#include "mainwindow.h"
#include "sidepanel.h"
#include "settingsmanager.h"
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setMinimumSize(800,600);
    SettingsManager initSettings;


    map = new InteractiveMap;

    QMenu *pFile = new QMenu("File");
    QMenu *pHelp = new QMenu("Help");

    pFile->addAction("Load dump", map, &InteractiveMap::loadDump);
    pFile->addAction("Load State", map, &InteractiveMap::loadState);
    //pFile->addAction("Save state", worldstate, SLOT(saveState()));

    menuBar()->addMenu(pFile);
    menuBar()->addMenu(pHelp);

    QSplitter* spl = new QSplitter(Qt::Horizontal);

    SidePanel* panel = new SidePanel;

    panel->setMinimumWidth(200);
    panel->setMaximumWidth(panel->minimumWidth());
    panel->resize(panel->minimumWidth(),panel->height());

    spl->addWidget(map);
    spl->addWidget(panel);

    setCentralWidget(spl);

    connect(panel, SIGNAL(updateMap()), map, SLOT(updateCache()));
}

MainWindow::~MainWindow()
{
}
