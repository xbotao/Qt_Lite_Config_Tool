#ifndef PARSEJSONFILE_H
#define PARSEJSONFILE_H

#include <QString>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>
#include <QtDebug>

class ParseJsonFile
{
public:
    ParseJsonFile();

    static QJsonObject parse(QString strFileName);
};

#endif // PARSEJSONFILE_H
