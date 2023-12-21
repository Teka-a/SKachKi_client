#ifndef HORSE_H
#define HORSE_H

#include <QString>
#include <QRegularExpression>
#include <QUuid>
#include <QDebug>


class Horse
{
private:
    int randomNum;
    int id;
    QString name;
    QString sex;
    int age;
    QString owner;
    bool isAvailable;
    int generateRandomNum();
public:
    Horse(QString id, QString name, QString sex, QString age, QString owner, QString availability);

    QString getId();
    QString getName();
    QString getSex();
    QString getAge();
    QString getOwner();
    QString getAvailability();
    QString removeRandomNum();
};

#endif // HORSE_H
