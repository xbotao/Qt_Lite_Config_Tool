#include "parsejsonfile.h"

ParseJsonFile::ParseJsonFile()
{

}

QJsonObject ParseJsonFile::parse(QString strFileName)
{
    QFile loadFile(strFileName);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open projects json";
//        return null;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
//        return;
    }

    QJsonObject rootObj = jsonDoc.object();
    return  rootObj;

    //输出所有key，这一步是非必须的，放最后的话，你可能读不到任何key
//    QStringList keys = rootObj.keys();
//    for(int i = 0; i < keys.size(); i++)
//    {
//        qDebug() << "key" << i << " is:" << keys.at(i);
//    }

}
