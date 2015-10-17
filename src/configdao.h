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
#include <QPoint>

using namespace std;
using namespace cv;


class ConfigDAO : public QObject
{
    Q_OBJECT
public:
    bool readJSON();
    bool writeJSON();
    static ConfigDAO *Instance();

    bool hasColor(QString colorName);
    ColorSpace insertColorSpace(ColorSpace cs);
    vector<ColorSpace> getColorSpaces() const;

    void updateColorSpace(ColorSpace cs, QString name);
    void save();
    ColorSpace getLastModified() const;

    QPoint getCornerLeftTop() const;
    void setCornerLeftTop(const QPoint &value);

    QPoint getCornerLeftBottom() const;
    void setCornerLeftBottom(const QPoint &value);

    QPoint getCornerRightTop() const;
    void setCornerRightTop(const QPoint &value);

    QPoint getCornerRightBottom() const;
    void setCornerRightBottom(const QPoint &value);


signals:

public slots:

private:
    static ConfigDAO *object;
    QString fileName;
    vector<ColorSpace> colorSpaces;
    QJsonObject mainObjectJSON;
    ColorSpace lastModified;


    QString readFile(const QString &filename);
    ColorSpace jsonToColorSpace(QJsonObject obj);
    QJsonObject colorSpaceToJson(ColorSpace cs);


    explicit ConfigDAO(QObject *parent = 0);
    ConfigDAO(ConfigDAO const&) {};
    ConfigDAO& operator=(ConfigDAO const&){};


    QPoint cornerLeftTop;
    QPoint cornerLeftBottom;
    QPoint cornerRightTop;
    QPoint cornerRightBottom;


    QPoint jsonToQPoint(QJsonObject obj);
    QJsonObject qPointToJson(QPoint p);
};

#endif // CONFIGDAO_H
