#include "jockey.h"

Jockey::Jockey(QString id, QString name, QString surname, QString age, QString p1, QString p2, QString p3)
{
    this->id = id.toInt();
    this->name = name;
    this->surname = surname;
    this->age = age;
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}


int Jockey::generateRandomNum()
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    bool ok;
    return partsUuid[1].toInt(&ok, 16);
}


QString Jockey::removeRandomNum()
{
    int originId = this->id - this->randomNum;
    return QString::number(originId);
}

QString Jockey::getId()
{
    return QString::number(this->id);
}

QString Jockey::getName()
{
    return this->name;
}

QString Jockey::getSurname()
{
    return this->surname;
}

QString Jockey::getAge()
{
    return this->age;
}

QString Jockey::getPlaces1()
{
    return this->p1;
}

QString Jockey::getPlaces2()
{
    return this->p2;
}

QString Jockey::getPlaces3()
{
    return this->p3;
}
