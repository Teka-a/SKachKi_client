#ifndef JOCKEY_H
#define JOCKEY_H


#include <QString>
#include <QDate>
#include <QTime>
#include <QRegularExpression>
#include <QUuid>
#include <QDebug>


class Jockey
{
private:
    int randomNum;
    int id;
    QString name;
    QString surname;
    QString age;
    QString p1;
    QString p2;
    QString p3;

    int generateRandomNum();
public:
    Jockey(QString id, QString name, QString surname, QString age, QString p1 = "0", QString p2 = "0", QString p3 = "0");

    QString getId();
    QString getName();
    QString getAge();
    QString getPlaces1();
    QString getPlaces2();
    QString getPlaces3();
    QString getSurname();
    QString removeRandomNum();
};

#endif // JOCKEY_H
