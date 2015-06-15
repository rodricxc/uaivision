#ifndef CONFIGDAO_H
#define CONFIGDAO_H

#include <QObject>

class ConfigDAO : public QObject
{
    Q_OBJECT
public:
    explicit ConfigDAO(QObject *parent = 0);
    bool readJSON();


signals:

public slots:

};

#endif // CONFIGDAO_H
