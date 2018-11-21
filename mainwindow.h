#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QFileDialog>

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "parsejsonfile.h"
#include "qtconfigitem.h"

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
    void add2TreeWidget();
    QtConfigItem m_ConfigItem;
    int itemCount;
};

#endif // MAINWINDOW_H
