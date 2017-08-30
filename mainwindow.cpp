#include "mainwindow.h"
#include "interactivemap.h"
#include "sidepanel.h"
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setMinimumSize(800,600);
    //showMaximized();
    worldstate = new WorldState(this);

    QMenu *pFile = new QMenu("File");
    QMenu *pHelp = new QMenu("Help");

    pFile->addAction("Load dump", worldstate, SLOT(loadDump()));
    pFile->addAction("Load State", worldstate, SLOT(loadState()));
    pFile->addAction("Save state", worldstate, SLOT(saveState()));

    menuBar()->addMenu(pFile);
    menuBar()->addMenu(pHelp);

    QSplitter* spl = new QSplitter(Qt::Horizontal);

    InteractiveMap* map = new InteractiveMap;
    SidePanel* panel = new SidePanel;
    panel->setMinimumWidth(200);
    panel->setMaximumWidth(panel->minimumWidth());
    panel->resize(panel->minimumWidth(),panel->height());

    spl->addWidget(map);
    spl->addWidget(panel);

    setCentralWidget(spl);
}

MainWindow::~MainWindow()
{
}
