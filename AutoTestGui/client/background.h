#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QString>
#include <QRegularExpression>
#include <QUuid>

#include "client.h"

void requestOpenKey(QString userLogin);
void generateShortTermKey();
void sendShortTermOpenKey(Point shortTermOpenKey, QString to = "Server");

#endif // BACKGROUND_H
