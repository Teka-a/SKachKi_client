#include "client.h"
#include <QDebug>
#include <QCoreApplication>

ClientDestroyer::~ClientDestroyer(){
    delete pInstance;
}

void ClientDestroyer::initialize(Client * p){
    pInstance = p;
}

Client::Client(QObject *parent){
    mTcpSocket = new QTcpSocket(this);
    mTcpSocket->connectToHost("192.168.0.102", 54345);
    if(mTcpSocket->waitForConnected() ){
        QString num = "51739109251054544900652827515386315264922377181505051896146948101942003159361";
        this->serverOpenKey.x = (uint256_t)num.toStdString();
        num = "12864400938217234001090395423358871809299620525603885367207698537354801609454";
        this->serverKey = "4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048";
        this->serverIV = "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea";
        this->serverOpenKey.y = (uint256_t)num.toStdString();
        this->serverStatus = "active";
        this->userStatus = "anonymous";
    }
    else{
        this->serverStatus = "inactive";

        disconnect();
    }
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &Client::slotServerRead);
}

Client::~Client(){
    mTcpSocket->close();
}

Client* Client::getInstance(){
    if (!pInstance)
    {
        pInstance = new Client();
        destroyer.initialize(pInstance);
    }
    return pInstance;
}

void Client::disconnect()
{
    this->~Client();
}

void Client::sendToServer(QString message){
    //qDebug() << "Send to server" << message;
    message = encryptKuznechikOFB(message.toUtf8().toHex(), this->serverKey, this->serverIV);
    //qDebug() << "Send to server" << message;
    mTcpSocket->write(message.toUtf8());
    //mTcpSocket->write(message.toUtf8());
}

//set general info

void Client::setOpenKey(QString openKey)
{
    this->openKey = openKey;
}

void Client::setPrivateKey(uint1024_t privateKey)
{
    this->privateKey = privateKey;
}

void Client::setLogin(QString login)
{
    this->login = login;
}

void Client::setServerKey(QString key)
{
    this->serverKey = key;
}

void Client::setServerIV(QString IV)
{
    this->serverIV = IV;
}

void Client::setUserStatus(QString status)
{
    this->userStatus = status;
}


void Client::setMyServerShortTermPrivateKey(uint1024_t privateKey)
{
    this->myServerShortTermPrivateKey = privateKey;
}

void Client::setPassword(QString password)
{
    this->password = password;
}

//get general info

uint1024_t Client::getPrivateKey()
{
    return this->privateKey;
}

QString Client::getLogin()
{
    return this->login;
}

QString Client::getUserStatus()
{
    return this->userStatus;
}

QString Client::getServerStatus()
{
    return this->serverStatus;
}

QString Client::getServerKey()
{
    return this->serverKey;
}

QString Client::getServerIV()
{
    return this->serverIV;
}

QString Client::getPassword()
{
    return this->password;
}

uint1024_t Client::getMyServerShortTermPrivateKey()
{
    return this->myServerShortTermPrivateKey;
}

void Client::resetPersonalData()
{
    this->login = "";
    this->password = "";
    this->privateKey = 0;
    this->userStatus = "anonymous";
    this->serverKey = "4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048";
    this->serverIV = "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea";

}

QString Client::getConnectionStatus(){
    QHostAddress ip6Address =  mTcpSocket->peerAddress();
    bool conversionOK = false;
    QHostAddress ip4Address(ip6Address.toIPv4Address(&conversionOK));
    QString ip4String;
    if (conversionOK)
    {
        ip4String = ip4Address.toString();
    }
    return mTcpSocket->peerName();
}

void Client::slotServerRead(){
    QString array = "";
    while(mTcpSocket->bytesAvailable()>0)
    {
        QByteArray res = mTcpSocket->readAll();
        array.append(res);

        //qDebug() << "slotServerRead before";
        //qDebug() << array;
        array = decryptKuznechikOFB(array, this->serverKey, this->serverIV);
        QByteArray hex = QByteArray::fromHex(array.toUtf8());
        array = hex.data();

        qDebug() << "slotServerRead after";
        qDebug() << array;
        QStringList list = array.split("&", Qt::SkipEmptyParts);
        if(list[0] == "openKey"){
            //qDebug() << "sir";
            Point openKeyPoint;
            openKeyPoint.x = (uint256_t)list[2].toStdString();
            openKeyPoint.y = (uint256_t)list[3].toStdString();
            if(list[1] == this->login){
                //qDebug() << "1";
                emit receivedMyOpenKey(openKeyPoint);
                //qDebug() << "send";
            }
            else{
                emit receivedUserOpenKey(openKeyPoint, login);
            }
        }
        else if (list[0] == "shortTermOpenKey"){
            if(list[2] == this->login){
                QString from;
                Point longTermOpenKey;
                Point shortTermOpenKey;
                shortTermOpenKey.x = (uint256_t)list[3].toStdString();
                shortTermOpenKey.y = (uint256_t)list[4].toStdString();
                if(list[1] == "Server"){
                    longTermOpenKey = this->serverOpenKey;
                    emit receivedServerShortTermOpenKey(longTermOpenKey, shortTermOpenKey);
                }
                else {
                    emit receivedShortTermOpenKey(list[1], longTermOpenKey, shortTermOpenKey);
                }

            }
        }
        else if (list[0] == "auth"){
            emit receivedAuthResult(list[1], list[2]);
        }
        else if (list[0] == "contestsList") {
            QVector<Contest> contestsList;
            int contests = (list.size() - 1) / 6;
            for (int i = 0; i < contests; ++i) {
                Contest cont(list[i * 6 + 1],
                             list[i * 6 + 2],
                             list[i * 6 + 3],
                             list[i * 6 + 4],
                             list[i * 6 + 5],
                             list[i * 6 + 6]);
                contestsList.append(cont);
            }
            emit receivedContests(contestsList);
        }
        else if (list[0] == "horsesList") {
            QVector<Horse> horsesList;
            int horses = (list.size() - 1) / 5;
            for (int i = 0; i < horses; ++i) {
                Horse horse (list[i * 5 + 1],
                             list[i * 5 + 2],
                             list[i * 5 + 3],
                             list[i * 5 + 4],
                             list[i * 5 + 5], "true");
                horsesList.append(horse);
            }
            qDebug() << "horseszlist";
            emit receivedHorses(horsesList);
        }
        else if (list[0] == "jockeysList") {
            QVector<Jockey> jockeysList;
            int jockeys = (list.size() - 1) / 7;
            for (int i = 0; i < jockeys; ++i) {
                Jockey jockey (list[i * 7 + 1],
                               list[i * 7 + 2],
                               list[i * 7 + 3],
                               list[i * 7 + 4],
                               list[i * 7 + 5],
                               list[i * 7 + 6],
                               list[i * 7 + 7]);
                jockeysList.append(jockey);
            }
            qDebug() << "jockeysList";
            emit receivedJockeys(jockeysList);
        }
        else if (list[0] == "contestsDetailedInfo") {
            //contestsDetailedInfo & ContId & ContName & ContDate & ContTime & ContRegStatus &
            //                       HippId & HippName & HippAddr & HippDesc
            //                       Horse & Jockey & time & place
            QVector<ParticipantPair> participants;
            Contest contest(list[1], list[2], list[3], list[4], list[7], list[5]);
            Place hippodrome(list[6], list[7], list[8], list[9]);
            int contests = (list.size() - 10) / 4;
            int start = 10;
            for (int i = 0; i < contests; ++i) {
                ParticipantPair pair;
                pair.horseName = list[start];
                pair.jockey = list[start + 1];
                pair.time = QTime::fromString(list[start + 2]);
                pair.place = list[start + 3];
                participants.append(pair);
                start += 4;
            }
            qDebug() << "ee";
            emit receivedContest(contest, hippodrome, participants);
        }
        else if (list[0] == "horseDetailedInfo") {
            Horse horse(list[1], list[2], list[3], list[4], list[5], list[6]);
            QVector<Contest> contests;
            int contestsNum = (list.size() - 7) / 6;
            int start = 7;
            for  (int i = 0; i < contestsNum; ++i) {
                Contest contest(list[start],
                                list[start+1],
                                list[start+2],
                                list[start+3],
                                list[start+4],
                                list[start+5]);
                start += 6;
                contests.append(contest);
            }
            emit receivedHorse(horse, contests);
        }
        else if (list[0] == "jockeyDetailedInfo") {
            Jockey jockey(list[1], list[2], list[3], list[4]);
            QVector<Contest> contests;
            int contestsNum = (list.size() - 5) / 6;
            int start = 5;
            for  (int i = 0; i < contestsNum; ++i) {
                Contest contest(list[start],
                                list[start+1],
                                list[start+2],
                                list[start+3],
                                list[start+4],
                                list[start+5]);
                start += 6;
                contests.append(contest);
            }
            emit receivedJockey(jockey, contests);


        }
        else if (list[0] == "contests3") {
            qDebug() << "3 cont getted";
            QVector<Contest> contestsList;
            for (int i = 0; i < 3; ++i) {
                if (list[i * 6 + 1] == "end") {
                    break;
                }
                else {
                    Contest cont(list[i * 6 + 1],
                                 list[i * 6 + 2],
                                 list[i * 6 + 3],
                                 list[i * 6 + 4],
                                 list[i * 6 + 5],
                                 list[i * 6 + 6]);
                    contestsList.append(cont);
                }
            }
            emit receivedFutureContestsForMain(contestsList);
        }
        else  if (list[0] == "horsesForContest") {
            qDebug() << "horses";
            //horsesForContest&contName& horseId & horseName & horseSex & horseAge & horseOwner
            QString contestName = list[2];
            QString contestId = list[1];
            QVector<Horse> horsesList;
            int horses = (list.size() - 2) / 5;
            for (int i = 0; i < horses; ++i) {
                Horse horse(list[i * 5 + 3],
                            list[i * 5 + 4],
                            list[i * 5 + 5],
                            list[i * 5 + 6],
                            list[i * 5 + 7], "t");
                horsesList.append(horse);
            }
            emit receivedHorsesForContest(contestId, contestName, horsesList);
        }
        else if (list[0] == "registration") {
            qDebug() << list[0] << list[1] << list.size();
        }
        else if (list[0] == "userRegistration") {
            if (list[1] == "success")
                emit registrationStatus("s");
            else if (list[1] == "failed")
                emit registrationStatus("f");
        }
        else if (list[0] == "contestRegistration") {
            if (list[1] == "success")
                emit registrationContestStatus("s");
            else if (list[1] == "failed")
                emit registrationContestStatus("f");
        }
        else if (list[0] == "placeRegistration") {
            if (list[1] == "success")
                emit registrationPlaceStatus("s");
            else if (list[1] == "failed")
                emit registrationPlaceStatus("f");
        }
        else if (list[0] == "places") {
            QVector<Place> places;
            int placesNum = (list.size() - 1) / 4;
            for (int i = 0; i < placesNum; ++i) {
                Place place (list[i * 4 + 1],
                            list[i * 4 + 2],
                            list[i * 4 + 3],
                            list[i * 4 + 4]);
                places.append(place);
            }
            emit receivedPlaces(places);
        }
        else if (list[0] == "ownerInfo") {
            QVector<Horse> horsesList;
            int horses = (list.size() - 3) / 5;
            for (int i = 0; i < horses; ++i) {
                Horse horse(list[i * 5 + 3],
                            list[i * 5 + 4],
                            list[i * 5 + 5],
                            list[i * 5 + 6],
                            list[2],
                            list[i * 5 + 7]);

                horsesList.append(horse);
            }
            qDebug() << "owner";
            emit receivedOwnerForAccount(list[1], list[2], horsesList);
        }
        else if (list[0] == "horseRegistration") {
            if (list[1] == "success")
                emit registrationHorseStatus("s");
            else if (list[1] == "failed")
                emit registrationHorseStatus("f");
        }
        else if (list[0] == "changeHorseAvailability") {
            if (list[1] == "success")
                emit changeHorseStatus("s");
            else if (list[1] == "failed")
                emit changeHorseStatus("f");
        }
        else if (list[0] == "passedNotAddedContests") {
            QVector<Contest> contestsList;
            int contests = (list.size() - 1) / 6;
            for (int i = 0; i < contests; ++i) {
                Contest cont(list[i * 6 + 1],
                             list[i * 6 + 2],
                             list[i * 6 + 3],
                             list[i * 6 + 4],
                             list[i * 6 + 5],
                             list[i * 6 + 6]);
                contestsList.append(cont);
            }
            emit receivedContestsAddInfo(contestsList);
        }
        else if (list[0] == "participantsForAddInfo") {
            QVector<QVector<QString>> participants;
            int count = (list.size() - 2) / 3;
            for (int i = 0; i < count; ++i) {
                QVector<QString> pair {list[i * 3 + 2], list[i * 3 + 3], list[i * 3 + 4]};
                participants.push_back(pair);
            }
            emit receivedContestParticipants(list[1], participants);
        }
        else if (list[0] == "infoAddition") {
            if (list[1] == "success")
                emit addInfoStatus("s");
            else if (list[1] == "failed")
                emit addInfoStatus("f");
        }
    }

}
