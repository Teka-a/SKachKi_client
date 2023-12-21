#ifndef CONTEST_H
#define CONTEST_H


#include <QString>
#include <QDate>
#include <QTime>
#include <QRegularExpression>
#include <QUuid>
#include <QDebug>

class Contest
{
private:
    int randomNum;
    int id;
    QString name;
    QString startDate;
    QString startTime;
    QString place;
    QString regStatus;


    int generateRandomNum();

public:
    Contest(QString id, QString name, QString startDate, QString startTime, QString place, QString regStatus);
    //Get info
    QString getId();
    QString getName();
    QString getRawDate();
    QString getRawTime();
    QString getFullDate();
    QString getFullTime();
    QString getPlace();
    QString getRegStatus();
    bool isPassed();
    QString removeRandomNum();
};


#endif // CONTEST_H
