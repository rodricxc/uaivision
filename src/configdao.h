#ifndef CONFIGDAO_H
#define CONFIGDAO_H

#include <QObject>
#include <vector>
#include "colorspace.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;
using namespace cv;


class ConfigDAO : public QObject
{
    Q_OBJECT
public:
    bool readJSON();
    bool writeJSON();
    static ConfigDAO *Instance();




signals:

public slots:

private:
    static ConfigDAO *object;
    QString fileName;
    vector<ColorSpace> colorSpaces;
    QJsonObject mainObjectJSON;


    QString readFile(const QString &filename);
    ColorSpace jsonToColorSpace(QJsonObject obj);



    explicit ConfigDAO(QObject *parent = 0);
    ConfigDAO(ConfigDAO const&) {};
    ConfigDAO& operator=(ConfigDAO const&){};

};

#endif // CONFIGDAO_H
