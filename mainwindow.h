#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QTreeWidget>

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "parsejsonfile.h"
#include "qtconfigitem.h"

enum ConfigType{
    cfgType_null = 0,
    cfgType_module = 1,
    cfgType_section,
    cfgType_feature
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushBtn_BrowseBildDir_clicked();

    void on_pushBtn_BrowseConfig_clicked();

private:
    Ui::MainWindow *ui;

    QtConfigItem parseConfigFile(QString strConfigFile);    
    QtConfigItem m_ConfigItem;
    int itemCount;

    void initTreeWidget();
    void addChild2Tree(QTreeWidgetItem *parent, QtConfigItem config);
    void addItems2Tree();
};

#endif // MAINWINDOW_H
