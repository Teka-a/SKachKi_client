#include "place.h"

Place::Place(QString id, QString name, QString address, QString description)
{
    this->id = id.toInt();
    this->name = name;
    this->address = address;
    this->description = description;
}

int Place::generateRandomNum()
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    bool ok;
    return partsUuid[1].toInt(&ok, 16);
}

QString Place::removeRandomNum()
{
    int originId = this->id - this->randomNum;
    return QString::number(originId);
}

QString Place::getId()
{
    return QString::number(this->id);
}


QString Place::getAddress()
{
    return this->address;
}


QString Place::getName()
{
    return this->name;
}


QString Place::getDescription()
{
    return this->description;
}


QString Place::getShortDescription()
{
    QString shortDescription = "";
    for (int i = 0; i < 30; ++i) {
        shortDescription += this->description[i];
    }
    return shortDescription;
}
