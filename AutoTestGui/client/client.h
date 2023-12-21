#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include <QVector>
#include <QPair>
#include <QMap>
#include <QCoreApplication>
#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include "curve.h"
#include "contest.h"
#include "kuznechik.h"
#include "horse.h"
#include "jockey.h"
#include "place.h"

struct ParticipantPair
{
    QString horseName;
    QString jockey;
    QTime time;
    QString place;
};


class Client;



class ClientDestroyer
{
private:
    Client * pInstance;
public:
    /**
         * @brief clientDestroyer::~clientDestroyer деструктор
         */
    ~ClientDestroyer();
    /**
        * @brief clientDestroyer::initialize копирует ссылку на объект класса client
         * для последующей работы с ней в классе clientDestroyer
        * @param p
        */
    void initialize(Client * p);
};


class Client: public QObject
{
    Q_OBJECT
private:
    static Client * pInstance;
    static ClientDestroyer destroyer;
    static QTcpSocket * mTcpSocket;
    //general info
    QString login;
    QString openKey;
    QString userStatus;
    QString password;
    uint1024_t privateKey;
    QString serverStatus;
    //info for communication with server
    uint1024_t myServerShortTermPrivateKey;
    Point serverOpenKey;
    QString serverKey;
    QString serverIV;

protected:
    Client(QObject *parent = nullptr);
    ~Client();

    Client(const Client& );
    Client& operator = (Client &);
    friend class ClientDestroyer;

public:
    static Client* getInstance();
    void sendToServer(QString message);

    //set general info
    void setOpenKey(QString openKey);
    void setPrivateKey(uint1024_t privateKey);
    void setLogin(QString login);
    void setServerKey(QString key);
    void setServerIV(QString IV);
    void setPassword(QString password);
    void setUserStatus(QString status);
    void setMyServerShortTermPrivateKey(uint1024_t privateKey);
    //get general info
    uint1024_t getPrivateKey();
    QString getLogin();
    QString getUserStatus();
    QString getServerStatus();
    QString getServerKey();
    QString getServerIV();
    QString getPassword();
    uint1024_t getMyServerShortTermPrivateKey();

    QString getConnectionStatus();

    void resetPersonalData();
    void disconnect();

signals:
    void receivedMyOpenKey(Point openKeyPoint);
    void receivedServerShortTermOpenKey(Point longTermOpenKey, Point shortTermOpenKey);
    void receivedUserOpenKey(Point openKeyPoint, QString login);
    void receivedShortTermOpenKey(QString login, Point longTermOpenKey, Point shortTermOpenKey);
    void receivedAuthResult(QString status, QString userStatus);

    void receivedContestsAddInfo(QVector<Contest> contests);
    void receivedContestParticipants(QString contestName, QVector<QVector<QString>> particiants);

    void receivedContests(QVector<Contest> contests);
    void receivedHorses(QVector<Horse> horses);
    void receivedJockeys(QVector<Jockey> jockeys);

    void receivedContest(Contest contest, Place hippodrome, QVector<ParticipantPair> participants);
    void receivedHorse(Horse horse, QVector<Contest> contests);
    void receivedJockey(Jockey jockey, QVector<Contest> contests);


    void receivedPlaces(QVector<Place> places);
    void receivedFutureContestsForMain(QVector<Contest> contests);
    void receivedHorsesForContest(QString contestId, QString contestName, QVector<Horse> horsesList);
    void registrationStatus(QString status);
    void registrationContestStatus(QString status);
    void registrationPlaceStatus(QString status);
    void registrationHorseStatus(QString status);
    void changeHorseStatus(QString status);
    void addInfoStatus(QString status);

    void receivedOwnerForAccount(QString id, QString owner, QVector<Horse> horses);
    void receivedJockeyForAccount(QString id, QString jockey, QVector<Contest> contests);
public slots:
    void slotServerRead();
};


#endif // CLIENT_H
