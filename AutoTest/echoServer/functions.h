#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <QRegularExpression>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTcpSocket>
#include <QMap>
#include "curve.h"
#include "database.h"
#include "kuznechik.h"
#include "streebog.h"

class Functions
{
private:
    QSqlDatabase *db;
    Curve *secp256k1;
    Point shortTermOpenKey;
    QString sendOpenKey(QString login);
    QString sendShortTermOpenKey(QString login);
    void generateShortTermKey(uint1024_t &shortTermPrivateKey);
    Point mqv(Point longTermOpenKey, Point shortTermOpenKey);

    QString sendOwnerInfo(QString login);
    QString sendJockeyInfo(QString login);

    QString getPlaces(QString jockeyId, QString place);

    QString sendContestsGeneralInfo();
    QString sendContestDetailedInfo(QString contestId);


    QString sendHorsesGeneralInfo();
    QString sendHorseDetailedInfo(QString horseId);

    QString sendJockeysGeneralInfo();
    QString sendJockeyDetailedInfo(QString jockeyId);

    QString send3FutureContests();
    QString sendHorsesForContest(QString contestId);

    QString regForContest(QString contestId, QString horseId, QString jockeyLogin);
    QString sendPlaces();
    QString sendPassedNotAddedContests();

    QString checkPassword(QString login, QString password);

    QString regNewUser(QString name, QString surname, QString statusId,
                       QString phone, QString address, QString birthDate,
                       QString login, QString password, QString ltok);
    QString regNewContest(QString name, QString date, QString time, QString placeId, QString status, QString info);
    QString regNewPlace(QString name, QString address, QString description);
    QString regNewHorse(QString name, QString sex, QString ownerId, QString available, QString birthDate);

    QString changeHorseAvailability(QString horseId, QString status);

    QString sendParticipantsForAddInfo(QString contestId);
    QString addInfo(QStringList info);
public:
    Functions();
    QString parse(QString dataFromClient, QMap<QTcpSocket*, QVector<QString>> &sockets, QTcpSocket* socket);
    QString convertTextToHex(QString text);

    void deleteTestUser();
    void deleteTestParticipant();
    void deleteTestPlace();
    void deleteTestHorse();
    void deleteTestContest();


};

#endif // FUNCTIONS_H
