#include "mainwindow.h"
#include "sidepanel.h"
#include "settingsmanager.h"
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMenu>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setMinimumSize(800,600);
    SettingsManager initSettings;

    map = new InteractiveMap;

    QMenu *pFile = new QMenu("File");
    QMenu *pHelp = new QMenu("Help");

    pFile->addAction("Load dump", this, &MainWindow::loadDump);
    pFile->addAction("Load State", this, &MainWindow::loadState);
    QAction* saveAct = pFile->addAction("Save state", this, &MainWindow::saveWorldState);
    saveAct->setEnabled(false);
    QAction* closeAct = pFile->addAction("Close state", this, &MainWindow::closeWorldState);
    closeAct->setEnabled(false);
    pFile->addAction("Exit", QCoreApplication::instance(), SLOT(quit()));

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
    connect(map, SIGNAL(saveStateChanged(bool)), saveAct, SLOT(setEnabled(bool)));
    connect(map, SIGNAL(saveStateChanged(bool)), closeAct, SLOT(setEnabled(bool)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::loadDump()
{
    QString dump = QFileDialog::getOpenFileName(nullptr, "Load .dmp file"),
            idx  = QFileDialog::getOpenFileName(nullptr, "Load .idx file");
    if(!dump.isEmpty() && !idx.isEmpty())
    map->loadDump(dump, idx);
}

void MainWindow::loadState()
{
    QString st = QFileDialog::getOpenFileName(nullptr, "Load state file");
    if(!st.isEmpty())
        map->loadState(st);
}

void MainWindow::saveWorldState()
{
    QString st = QFileDialog::getSaveFileName(nullptr, "Save state");
    if(!st.isEmpty())
        map->saveState(st);
}

void MainWindow::closeWorldState()
{
    map->closeState();
}
