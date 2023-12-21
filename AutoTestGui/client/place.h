#ifndef PLACE_H
#define PLACE_H

#include <QString>
#include <QRegularExpression>
#include <QUuid>
#include <QDebug>

class Place
{
private:
    int randomNum;
    int id;
    QString name;
    QString address;
    QString description;
    int generateRandomNum();
public:
    Place(QString id, QString name, QString address, QString description);

    QString getId();
    QString getName();
    QString getAddress();
    QString getDescription();
    QString getShortDescription();

    QString removeRandomNum();
};

#endif // PLACE_H
