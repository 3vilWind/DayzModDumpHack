#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include "entitydata.h"
#include <QWidget>
#include <QGroupBox>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QCheckBox>


class SidePanel : public QWidget
{
    Q_OBJECT
public:
    explicit SidePanel(QWidget *parent = nullptr);


public slots:
    void updateCheckBox(QWidget* wgt);


private:
    QSignalMapper* psigManager;

    void mapCheckBoxes(QVector<QCheckBox *> &v, QVBoxLayout* bl);
    void loadCheckBoxes(QVector<QCheckBox *> &v);
};

#endif // SIDEPANEL_H
