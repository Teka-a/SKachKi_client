#include "background.h"

void requestOpenKey(QString userLogin)
{
    QString request = "requestOpenKey&" + userLogin + "&";
    Client::getInstance()->sendToServer(request);
}

void sendShortTermOpenKey(Point shortTermOpenKey, QString to)
{
    QString request = "shortTermOpenKey&" + Client::getInstance()->getLogin() + "&" + to + "&" + QString::fromStdString(shortTermOpenKey.x.str()) + "&" + QString::fromStdString(shortTermOpenKey.y.str()) + "&";
    Client::getInstance()->sendToServer(request);
}

void generateShortTermKey()
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    //qDebug() << partsUuid[3];
    bool ok;
    uint1024_t shortTermPrivateKey {std::to_string(partsUuid[3].toLongLong(&ok, 16))};
    //qDebug() << "Generate short" << QString::fromStdString(shortTermPrivateKey.str());
    Client::getInstance()->setMyServerShortTermPrivateKey(shortTermPrivateKey);
    Point shortTermOpenKey = Curve::getInstance()->countComposition(shortTermPrivateKey);
    sendShortTermOpenKey(shortTermOpenKey);
}
