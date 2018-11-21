#ifndef QTCONFIGITEMS_H
#define QTCONFIGITEMS_H

#include <QString>
#include <QVector>

class QtConfigItem
{
public:
    QtConfigItem();

    QString strItemName;
    QString strSection;
    QString strPurpose;
    QString configFile;
    int iType;
//    QVector<QString> listStrOutput;
    QVector<QtConfigItem> childrens;

};

#endif // QTCONFIGITEMS_H
