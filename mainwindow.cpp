#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList strLDisType;
    strLDisType << tr("Default") << tr("All") << tr("Available");
    ui->comboDisType->addItems(strLDisType);

    initTreeWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushBtn_BrowseBildDir_clicked()
{
    QString strBuildDir = QFileDialog::getExistingDirectory(nullptr,
                                      tr("select Build Directory"),
                                      tr(""));
    if(!strBuildDir.isEmpty())
    {
        ui->lineEdit_BildDir->setText(strBuildDir);
    }
}

void MainWindow::on_pushBtn_BrowseConfig_clicked()
{
    //选择文件夹
    QString strConfigFile = QFileDialog::getOpenFileName(nullptr,
                                                        tr("select Build Directory"),
                                                        tr("E:/work/5.11.2/src/"),
                                                        tr("config file (*.json)"));
    if(!strConfigFile.isEmpty())
    {
        ui->lineEdit_Config->setText(strConfigFile);
    }

    itemCount = 0;
    m_ConfigItem.childrens.clear();
    m_ConfigItem.strItemName = "root";
    m_ConfigItem.strSection = ".";
    m_ConfigItem.strPurpose = "";

    //遍历工程下的所有配置文件
    QString strAbsPath = QFileInfo(strConfigFile).absolutePath();
    QDir dir(strAbsPath);
    foreach(QFileInfo full, dir.entryInfoList(QDir::Dirs))
    {
        if((full.fileName() == ".")||(full.fileName() == ".."))
            continue;

        QString subConfigFileName = strAbsPath + "/"+full.fileName()+"/configure.json";
        QFile subConfigFile(subConfigFileName);
        if(subConfigFile.exists())
        {
            QtConfigItem item = parseConfigFile(subConfigFileName);
            m_ConfigItem.childrens.append(item);
        }
    }

    addItems2Tree();
}

QtConfigItem MainWindow::parseConfigFile(QString strConfigFile)
{
    QJsonObject jsonObject = ParseJsonFile::parse(strConfigFile);
    QString absolutionPath = QFileInfo(strConfigFile).absolutePath();
    int nnn = absolutionPath.lastIndexOf("/");
    QString strLabel = absolutionPath.right(absolutionPath.length()-1-nnn);


    QtConfigItem configItem;
    configItem.configFile = strConfigFile;
    configItem.strItemName = strLabel;
    configItem.iType = cfgType_null;

    //qDebug() << "\r\n"<<strConfigFile<<"--------------";

    if(jsonObject.contains("module"))
    {
        configItem.iType = cfgType_module;
        configItem.strItemName = jsonObject.value("module").toString()+"_module";
    }

     if(jsonObject.contains("subconfigs"))
     {
         QJsonValue value = jsonObject.value("subconfigs");
         if(value.isArray())
         {
            QJsonArray subArray = value.toArray();
            for(int i=0; i<subArray.size(); i++)
            {
                QString strSubFileName = absolutionPath+"/"+subArray.at(i).toString()+"/configure.json";
                QtConfigItem item = parseConfigFile(strSubFileName);
                configItem.childrens.append(item);
                //qDebug() << i<<" value is:" << subArray.at(i).toString();
            }
         }
     }
     else if(jsonObject.contains("features"))
     {
         QJsonObject featureObj = jsonObject.value("features").toObject();
         QVector<QtConfigItem> sections;

         for(int i=0; i<featureObj.keys().size(); i++)
         {
             QString key = featureObj.keys().at(i);
             QJsonObject obj = featureObj.value(key).toObject();

             if(!obj.contains("purpose"))
                 continue;

             QtConfigItem item;
             item.configFile = strConfigFile;             
             item.strItemName = key;
             item.strPurpose = obj.value("purpose").toString();
             item.iType = cfgType_feature;

             if(obj.contains("section"))
             {
                item.strSection = obj.value("section").toString();
             }

             bool bHasSection = false;
             for(int i=0; i<sections.size(); i++)
             {
                 if(sections.at(i).strItemName == item.strSection)
                 {
                     sections[i].childrens.append(item);
                     bHasSection = true;
                     break;
                 }
             }

             if(!bHasSection)
             {
                QtConfigItem section;
                section.strItemName = item.strSection;
                section.iType = cfgType_section;
                section.childrens.append(item);
                sections.append(section);
             }

             //configItem.childrens.append(item);
             itemCount++;

             QString strItemInfo = QString("%1 %2:%3")
                     .arg(key, -35, '.')
                     .arg(item.strSection)
                     .arg(item.strPurpose);
         }

         configItem.childrens.append(sections);
     }

     return configItem;
}

void MainWindow::initTreeWidget()
{
    ui->treeWidget->setColumnCount(1);
    //ui->treeWidget->setColumnWidth(0, 300);
    ui->treeWidget->setHeaderLabels(QStringList()<<"Name"/*<<"Enabled"*/);
}

void MainWindow::addChild2Tree(QTreeWidgetItem *parent, QtConfigItem config)
{
    foreach (QtConfigItem item, config.childrens) {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem();
        treeItem->setText(0, item.strItemName);
        if(item.iType == cfgType_feature)
        {
            treeItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            treeItem->setCheckState (0, Qt::Checked);   //复选框
        }
        //treeItem->setText(1, QString("%1").arg(item.iType));
        parent->addChild(treeItem);

        if(item.childrens.size() > 0)
        {
            addChild2Tree(treeItem, item);
        }
    }
}

void MainWindow::addItems2Tree()
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, m_ConfigItem.strItemName);
    //treeItem->setText(1, "");
    ui->treeWidget->addTopLevelItem(treeItem);

    addChild2Tree(treeItem, m_ConfigItem);
}
