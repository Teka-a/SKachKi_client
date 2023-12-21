#include "contest.h"

Contest::Contest(QString id, QString name, QString startDate, QString startTime, QString place, QString regStatus)
{
    this->name = name;
    this->startDate = startDate.remove(QRegularExpression("-"));
    for (int i = 0; i < 8; ++i) {
        this->startTime += startTime[i];
    }
    this->place = place;
    this->regStatus = regStatus;


    this->id = id.toInt();
    //this->randomNum = generateRandomNum();
    //this->id += this->randomNum;
}

bool Contest::isPassed()
{
    QDateTime current = QDateTime::currentDateTime();

    if( current.date() > QDate::fromString(this->startDate, "yyyyMMdd")) {
        return true;
    }
    else if (current.date() == QDate::fromString(this->startDate, "yyyyMMdd") && current.time() > QTime::fromString(this->startTime)){
        return true;
    }
    else{
        return false;
    }
}

int Contest::generateRandomNum()
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    bool ok;
    return partsUuid[1].toInt(&ok, 16);
}

QString Contest::removeRandomNum()
{
    int originId = this->id - this->randomNum;
    return QString::number(originId);
}


QString Contest::getId()
{
    return QString::number(this->id);
}


QString Contest::getName()
{
    return this->name;
}


QString Contest::getRawDate()
{
    QString date = QDate::fromString(this->startDate, "yyyyMMdd").toString(Qt::ISODate);
    return date;
}


QString Contest::getRawTime()
{
    return this->startTime;
}


QString Contest::getFullDate()
{
    QString date = QDate::fromString(this->startDate, "yyyyMMdd").toString(Qt::ISODate);
    return date;
}


QString Contest::getFullTime()
{
    return this->startTime;
}


QString Contest::getPlace()
{
    return this->place;
}


QString Contest::getRegStatus()
{
    return this->regStatus;
}

