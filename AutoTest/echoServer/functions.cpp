#include "functions.h"

Functions::Functions()
{
    Database *db = Database::getInstance();
    Curve *secp256k1 = Curve::getInstance();
}


//Authentication
QString Functions::checkPassword(QString login, QString password)
{
    QString responce = "auth&";
    QSqlQuery query;
    query.prepare("SELECT password FROM users "
                  "WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    QSqlRecord rec = query.record();
    const int key = rec.indexOf("password");

    QString hashDB = "";
    while(query.next()){
        hashDB = query.value(key).toString();

    }
    password = password.toUtf8().toHex();
    QString hash = getHash512(password);
    //qDebug() << hash;
    //qDebug() << hashDB;
    if (hash == hashDB) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    //add status
    query.prepare("SELECT status.name, users.login FROM users "
                  "INNER JOIN status ON users.status_id = status.id "
                  "WHERE users.login = :login");
    query.bindValue(":login", login);
    query.exec();
    //qDebug() << query.exec();
    //qDebug() << query.lastError();
    QString userStatus = "";

    while(query.next()){
        userStatus = query.value(0).toString();
    }
    //qDebug() << userStatus;
    responce += "&" + userStatus;
    responce = responce.simplified();
    responce.replace( " ", "" );

    //qDebug() << "checkPass" << responce;
    return responce;
}

QString Functions::sendOpenKey(QString login)
{

    QString responce = "openKey&" + login + "&";

    QSqlQuery query;
    query.prepare("SELECT ltok FROM users "
                  "WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    QSqlRecord rec = query.record();
    const int key = rec.indexOf("ltok");

    while(query.next()){
        //qDebug() << query.value(0);
        responce += query.value(key).toString();
        //qDebug() << responce;
    }
    //qDebug() << "openKey" << responce;
    return responce;
}

void Functions::generateShortTermKey(uint1024_t &shortTermPrivateKey)
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    //qDebug() << partsUuid[3];
    bool ok;
    shortTermPrivateKey = partsUuid[3].toLongLong(&ok, 16);
    //qDebug() << QString::fromStdString(shortTermPrivateKey.str());
    this->shortTermOpenKey = Curve::getInstance()->countComposition(shortTermPrivateKey);
}

QString Functions::sendShortTermOpenKey(QString login)
{
    QString responce = "shortTermOpenKey&Server&" + login + "&";

    responce += QString::fromStdString(this->shortTermOpenKey.x.str()) + "&" + QString::fromStdString(this->shortTermOpenKey.y.str()) + "&";
    return responce;
}

void Functions::deleteTestUser()
{
    QSqlQuery query;
    QString queryState = "DELETE FROM users "
                         "WHERE login = 'Test'";
    query.prepare(queryState);
    query.exec();
}
void Functions::deleteTestParticipant()
{
    QSqlQuery query;
    QString queryState = "DELETE FROM c_u_h "
                         "WHERE contest_id = 4 AND horse_id = 5 AND jockey_id = 3";
    query.prepare(queryState);
    query.exec();
}
void Functions::deleteTestPlace()
{
    QSqlQuery query;
    QString queryState = "DELETE FROM hippodrome "
                         "WHERE name = 'Test'";
    query.prepare(queryState);
    query.exec();
}
void Functions::deleteTestHorse()
{
    QSqlQuery query;
    QString queryState = "DELETE FROM horses "
                         "WHERE name = 'Test'";
    query.prepare(queryState);
    query.exec();
}
void Functions::deleteTestContest()
{
    QSqlQuery query;
    QString queryState = "DELETE FROM contests "
                         "WHERE name = 'Test'";
    query.prepare(queryState);
    query.exec();
}



Point Functions::mqv(Point longTermOpenKey, Point shortTermOpenKey)
{
    //Server's privates keys
    uint1024_t dA = 83666;
    uint1024_t kA;
    generateShortTermKey(kA);
    //Server's open keys
    //shortTerm
    Point rA = this->shortTermOpenKey;
    //qDebug() << "Server short open: " << QString::fromStdString(rA.x.str()) << " " << QString::fromStdString(rA.y.str());
    uint1024_t xA = (uint1024_t)rA.x;
    //longTerm
    Point qA = secp256k1->countComposition(83666);
    //qDebug() << "Server long open: " << QString::fromStdString(qA.x.str()) << " " << QString::fromStdString(qA.y.str());
    uint256_t aA = qA.x;
    //Client's open keys
    //shortTerm
    Point rB = shortTermOpenKey;
    //qDebug() << "Client short open: " << QString::fromStdString(rB.x.str()) << " " << QString::fromStdString(rB.y.str());
    uint1024_t xB = (uint1024_t)rB.x;
    //longTerm
    Point qB = longTermOpenKey;
    //qDebug() << "Client long open: " << QString::fromStdString(qB.x.str()) << " " << QString::fromStdString(qB.y.str());
    uint1024_t aB = (uint1024_t)qB.x;


    //qDebug() << "MQV stage 1";

    uint1024_t sA = xA*aA*dA;
    sA += kA;
    sA %= (uint1024_t)secp256k1->getN().toStdString();
    //qDebug() << "sA =" << QString::fromStdString(sA.str());


    //qDebug() << "MQV stage 2";


    Point uA = secp256k1->countComposition(aB, qB);
    uA = secp256k1->countComposition(xB, uA);
    uA = secp256k1->addPoint(rB, uA);
    //qDebug() << "uA" << QString::fromStdString(uA.x.str()) << QString::fromStdString(uA.y.str());

    //qDebug() << "MQV stage 3";
    Point W = secp256k1->countComposition(sA, uA);
    //qDebug() << "W" << QString::fromStdString(W.x.str()) << QString::fromStdString(W.y.str());
    return W;

}


//Contest info
//contestsList& id & name & date & time & status & place
//              id & name & date & time & status & place
QString Functions::sendContestsGeneralInfo()
{
    QString responce = "contestsList";
    QSqlQuery query;
    QString queryState = "SELECT c.id, c.name AS contestname, c.date, c.time, c.status, h.name AS placename FROM contests AS c "
                         "JOIN hippodrome AS h ON h.id = c.hippodrome_id "
                         "ORDER BY c.date DESC";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("contestname");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int status = rec.indexOf("status");
    const int place = rec.indexOf("placename");


    QString contests = "";
    while(query.next()){
        contests += "&" + query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(place).toString() +
                    "&" + query.value(status).toString();
    }
    responce += contests;
    //qDebug() << "Contests" << responce;
    return responce;
}


QString Functions::getPlaces(QString jockeyId, QString place)
{
    QString responce = "jockeysList";
    QSqlQuery query;
    QString queryState = "SELECT COUNT(id) AS c FROM c_u_h "
                         "WHERE place = :place AND jockey_id = :jockeyId";
    query.prepare(queryState);
    query.bindValue(":place", place);
    query.bindValue(":jockeyId", jockeyId);
    query.exec();
    QString c = "";
    QSqlRecord rec = query.record();
    const int count = rec.indexOf("c");
    while(query.next()){
        c += query.value(count).toString();
    }
    return c;
}

//Jockey info
//jockeysList&  id & name & surname & age & p1 & p2 & p3
//              id & name & surname & age & p1 & p2 & p3
QString Functions::sendJockeysGeneralInfo()
{
    QString responce = "jockeysList";
    QSqlQuery query;
    QString queryState = "SELECT id, name, surname, birth_date FROM users "
                         "WHERE status_id = 1";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("name");
    const int surname = rec.indexOf("surname");
    const int date = rec.indexOf("birth_date");


    QString jockeys = "";
    QDate current = QDate::currentDate();

    while(query.next()){
        int age = current.year() - QDate::fromString(query.value(date).toString(), "yyyy-MM-dd").year();
        jockeys += "&" + query.value(id).toString() +
                   "&" + query.value(name).toString() +
                   "&" + query.value(surname).toString() + "&" + QString::number(age) +
                   "&" + getPlaces(query.value(id).toString(), "1") +
                   "&" + getPlaces(query.value(id).toString(), "2") +
                   "&" + getPlaces(query.value(id).toString(), "3");
    }
    responce += jockeys;
    //qDebug() << "Jockeys" << responce;
    return responce;
}

QString Functions::sendJockeyDetailedInfo(QString jockeyId)
{
    QString responce = "jockeyDetailedInfo";
    QSqlQuery query;
    QString queryState = "SELECT id, name, surname, birth_date FROM users "
                         "WHERE id = :jockeyId";
    query.prepare(queryState);
    query.bindValue(":jockeyId", jockeyId);
    query.exec();

    QSqlRecord rec = query.record();
    const int idJockey = rec.indexOf("id");
    const int nameJockey = rec.indexOf("name");
    const int surnameJockey = rec.indexOf("surname");
    const int dateBirth = rec.indexOf("birth_date");

    QString jockeyInfo = "";
    QDate current = QDate::currentDate();
    while (query.next()) {
        int age = current.year() - QDate::fromString(query.value(dateBirth).toString(), "yyyy-MM-dd").year();
        jockeyInfo += "&" + query.value(idJockey).toString() +
                     "&" + query.value(nameJockey).toString() +
                     "&" + query.value(surnameJockey).toString() +
                     "&" + QString::number(age);
    }
    //qDebug() << "jockey info" << jockeyInfo;
    responce += jockeyInfo;
    queryState = "SELECT contest_id, c_u_h.place AS place_stat, contests.name AS name, date, time, hippodrome.name AS place FROM c_u_h "
                 "JOIN contests ON contests.id=c_u_h.contest_id "
                 "JOIN hippodrome ON hippodrome.id=contests.hippodrome_id "
                 "WHERE jockey_id = :jockeyId";
    query.prepare(queryState);
    query.bindValue(":jockeyId", jockeyId);
    query.exec();
    rec = query.record();
    const int id = rec.indexOf("contest_id");
    const int status = rec.indexOf("place_stat");
    const int name = rec.indexOf("name");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int place = rec.indexOf("place");
    QString result = "";
    QString contests = "";
    while(query.next()) {

        //qDebug() << "Date" << current << QDate::fromString(query.value(date).toString(), "yyyy-MM-dd");
        if (current <= QDate::fromString(query.value(date).toString(), "yyyy-MM-dd"))
            result = "Зарегистрирован";
        else {
            qDebug() << query.value(status).toString() << (query.value(status).toString() == "1");
            if (query.value(status).toString() == "1")
                result = "Победитель";
            else if (query.value(status).toString() == "2" || query.value(status).toString() == "3")
                result = "Призёр";
            else
                result = "Участник";
        }
        contests += "&" + query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(place).toString() +
                    "&" + result;
    }

    //qDebug() << contests;
    if (contests != "")
        responce += contests;
    else
        responce += "&0&no&2024-09-27&14:00:00.000&no&no";
    //qDebug() << responce;
    return responce;
}


//Horses info
//horsesList&   id & name & sex & age & owner &
//              id & name & sex & age & owner &
QString Functions::sendHorsesGeneralInfo()
{
    QString responce = "horsesList";

    QSqlQuery query;
    QString queryState = "SELECT h.id AS id, h.name AS name, h.sex AS sex, h.birth_date AS date, users.name AS ownerName, users.surname AS ownerSurname FROM horses AS h "
                         "JOIN users ON h.owner_id = users.id";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("name");
    const int sex = rec.indexOf("sex");
    const int date = rec.indexOf("date");
    const int ownerName = rec.indexOf("ownerName");
    const int ownerSurname = rec.indexOf("ownerSurname");

    QString horses = "";
    QDate current = QDate::currentDate();
    while (query.next()) {
        int age = current.year() - QDate::fromString(query.value(date).toString(), "yyyy-MM-dd").year();
        horses += "&" + query.value(id).toString() +
                  "&" + query.value(name).toString() +
                  "&" + query.value(sex).toString() +
                  "&" + QString::number(age) +
                  "&" + query.value(ownerName).toString() + " " + query.value(ownerSurname).toString();
    }
    responce += horses;
    //qDebug() << "Horses" << responce;
    return responce;
}

//contestsDetailedInfo & ContId & ContName & ContDate & ContTime & ContRegStatus &
//                       HippId & HippName & HippAddr & HippDesc
//                       Horse & Jockey & time & place
QString Functions::sendContestDetailedInfo(QString contestId)
{
    QString responce = "contestsDetailedInfo&";
    //get info about contest: name, date, time, reg_status
    QSqlQuery query;
    QString queryState = "SELECT * FROM contests "
                         "WHERE id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);

    query.exec();
    //qDebug() << query.executedQuery();
    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("name");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int status = rec.indexOf("status");
    const int placeId = rec.indexOf("hippodrome_id");
    QString place_id = "";
    QString contest = "";
    while(query.next()){
        contest +=  query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(status).toString() + "&";
        place_id = query.value(placeId).toString();
    }
    //qDebug() << contest;
    responce += contest;
    //qDebug() << place_id;
    //get info about hippodrome: name, address, description
    QString queryState2 = "SELECT * FROM hippodrome "
                          "WHERE id = :hippodromeId";
    query.prepare(queryState2);
    query.bindValue(":hippodromeId", place_id.toInt());
    //qDebug() << query.executedQuery();
    query.exec();
    //qDebug() << query.executedQuery();
    rec = query.record();
    const int idPlace = rec.indexOf("id");
    const int namePlace = rec.indexOf("name");
    const int address = rec.indexOf("address");
    const int description = rec.indexOf("description");

    QString place = "";
    while(query.next()){
        place +=  query.value(idPlace).toString() +
                   "&" + query.value(namePlace).toString() +
                   "&" + query.value(address).toString() +
                   "&" + query.value(description).toString();
    }
    //qDebug() << place;
    responce += place;
    //get info about participants: surname name, horse name, time, place
    queryState = "SELECT c_u_h.chip_time AS time, c_u_h.place AS place, horses.name AS horse, users.name AS name, users.surname AS surname FROM c_u_h "
                 "JOIN horses ON horses.id = c_u_h.horse_id "
                 "JOIN users  ON users.id = c_u_h.jockey_id "
                 "WHERE contest_id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    //qDebug() << query.executedQuery();
    query.exec();

    rec = query.record();
    const int chipTime = rec.indexOf("time");
    const int jockeyName = rec.indexOf("name");
    const int jockeySurname = rec.indexOf("surname");
    const int horse = rec.indexOf("horse");
    const int placeGetted = rec.indexOf("place");

    QString participants = "";
    QString defaultTime = "00:00:00";
    QString defaultPlace = "-";
    int count = 0;
    while(query.next()){
        count += 1;
        if (query.value(chipTime).toString() != "")
            defaultTime = query.value(chipTime).toString();
        if (query.value(placeGetted).toString() != "0")
            defaultPlace = query.value(placeGetted).toString();

        participants += "&" + query.value(horse).toString() +
                   "&" + query.value(jockeyName).toString() + " " + query.value(jockeySurname).toString() +
                   "&" + defaultTime +
                   "&" + defaultPlace;
    }
    while (count < 7) {
        count += 1;
        participants += "&-&-&00:00:00&-";
    }
    participants += "&";
    //qDebug() << participants;
    responce += participants;
    //qDebug() << responce;
    return responce;
}

//horseDetailedInfo &
QString Functions::sendHorseDetailedInfo(QString horseId)
{
    QString responce = "horseDetailedInfo";
    //get info about horse: name, sex, age, owner
    QSqlQuery query;
    QString queryState = "SELECT h.id AS id, h.name AS name, h.sex AS sex, h.birth_date AS date, users.name AS ownerName, users.surname AS ownerSurname, h.available FROM horses AS h "
                         "JOIN users ON h.owner_id = users.id "
                         "WHERE h.id = :horseId";
    query.prepare(queryState);
    query.bindValue(":horseId", horseId);
    query.exec();

    QSqlRecord rec = query.record();
    const int idHorse = rec.indexOf("id");
    const int nameHorse = rec.indexOf("name");
    const int sexHorse = rec.indexOf("sex");
    const int dateHorse = rec.indexOf("date");
    const int ownerNameHorse = rec.indexOf("ownerName");
    const int ownerSurnameHorse = rec.indexOf("ownerSurname");
    const int available = rec.indexOf("available");

    QString horseInfo = "";
    QDate current = QDate::currentDate();
    while (query.next()) {
        int age = current.year() - QDate::fromString(query.value(dateHorse).toString(), "yyyy-MM-dd").year();
        horseInfo += "&" + query.value(idHorse).toString() +
                  "&" + query.value(nameHorse).toString() +
                  "&" + query.value(sexHorse).toString() +
                  "&" + QString::number(age) +
                  "&" + query.value(ownerNameHorse).toString() + " " + query.value(ownerSurnameHorse).toString() +
                  "&" + query.value(available).toString() ;
    }
    //qDebug() << "horse info" << horseInfo;
    responce += horseInfo;
    //get info about contests            
    queryState = "SELECT contest_id, c_u_h.place AS place_stat, contests.name AS name, date, time, hippodrome.name AS place FROM c_u_h "
                 "JOIN contests ON contests.id=c_u_h.contest_id "
                 "JOIN hippodrome ON hippodrome.id=contests.hippodrome_id "
                 "WHERE horse_id = :horseId";
    query.prepare(queryState);
    query.bindValue(":horseId", horseId);
    query.exec();
    rec = query.record();
    const int id = rec.indexOf("contest_id");
    const int status = rec.indexOf("place_stat");
    const int name = rec.indexOf("name");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int place = rec.indexOf("place");
    QString result = "";
    QString contests = "";
    while(query.next()) {

        //qDebug() << "Date" << current << QDate::fromString(query.value(date).toString(), "yyyy-MM-dd");
        if (current <= QDate::fromString(query.value(date).toString(), "yyyy-MM-dd"))
            result = "Зарегистрирован";
        else {
            //qDebug() << query.value(status).toString() << (query.value(status).toString() == "1");
            if (query.value(status).toString() == "1")
                result = "Победитель";
            else if (query.value(status).toString() == "2" || query.value(status).toString() == "3")
                result = "Призёр";
            else
                result = "Участник";
        }
        contests += "&" + query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(place).toString() +
                    "&" + result;
    }

    //qDebug() << contests;
    if (contests != "")
        responce += contests;
    else
        responce += "&0&no&2024-09-27&14:00:00.000&no&no";
    return responce;
}


QString Functions::send3FutureContests()
{
    QString responce = "contests3";
    QSqlQuery query;
    QString queryState = "SELECT c.id, c.name AS contestname, c.date, c.time, c.status, h.name AS placename FROM contests AS c "
                         "JOIN hippodrome AS h ON h.id = c.hippodrome_id "
                         "WHERE c.date > current_date";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("contestname");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int status = rec.indexOf("status");
    const int place = rec.indexOf("placename");

    int count = 0;
    QString contests = "";
    while(query.next()){
        count += 1;
        contests += "&" + query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(place).toString() +
                    "&" + query.value(status).toString();
    }
    if (count < 3) {
        contests += "&end";
    }
    responce += contests;
    //qDebug() << "Contests" << responce;
    return responce;
}


//horsesForContest&contName& horseId & horseName & horseSex & horseAge & horseOwner
QString Functions::sendHorsesForContest(QString contestId)
{
    QString responce = "horsesForContest&" + contestId + "&";
    QSqlQuery query;

    //select h.id, h.name AS horseName, h.sex, h.age, h.available, u.name, u.surname FROM horses AS h
    //inner join users AS u ON u.id = h.owner_id
    QString queryState = "SELECT horse_id FROM c_u_h "
                         "WHERE contest_id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("horse_id");
    QVector<QString> horseIdsRegistered;
    while(query.next()){
        qDebug() << query.value(id).toString();
        horseIdsRegistered.append(query.value(id).toString());
    }

    queryState = "SELECT name FROM contests "
                 "WHERE id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    query.exec();
    rec = query.record();
    const int name = rec.indexOf("name");
    while(query.next()){
        responce += query.value(name).toString();
    }
    queryState = "SELECT h.id, h.name AS horseName, h.sex, h.birth_date AS age, h.available, u.name, u.surname FROM horses AS h "
                 "INNER JOIN users AS u ON u.id = h.owner_id";

    query.prepare(queryState);
    query.exec();

    rec = query.record();
    const int horseId = rec.indexOf("id");
    const int horseName = rec.indexOf("horseName");
    const int horseSex = rec.indexOf("sex");
    const int horseAge = rec.indexOf("age");
    const int horseAvailable = rec.indexOf("available");
    const int ownerName = rec.indexOf("name");
    const int ownerSurname = rec.indexOf("surname");
    QString horses = "";
    QDate current = QDate::currentDate();
    while (query.next()) {
        //qDebug() << query.value(horseAvailable).toString();
        if (query.value(horseAvailable).toString() != "true")
            continue;
        QString id = query.value(horseId).toString();
        int i = 0;
        for (i = 0; i < horseIdsRegistered.size(); ++i) {
            //qDebug() << id << horseIdsRegistered[i] << (id == horseIdsRegistered[i]) << i;
            if (id == horseIdsRegistered[i])
                break;
        }
        if (i == horseIdsRegistered.size()) {
            int age = current.year() - QDate::fromString(query.value(horseAge).toString(), "yyyy-MM-dd").year();
            qDebug() << "Age" << QString::number(age);
            horses += "&" + id +
                      "&" + query.value(horseName).toString() +
                      "&" + query.value(horseSex).toString() +
                      "&" + QString::number(age) +
                      "&" + query.value(ownerName).toString() + " " + query.value(ownerSurname).toString();
        }

    }

    responce += horses;
    //qDebug() << responce;
    return responce;
}
//add horse:
//INSERT INTO horses (name, sex, age, owner_id, available)
//VALUES ('Дженифер', 'Кобыла', 4, 5, true);

//altr horse name:
//UPDATE horses
//SET name = 'Звездочка',
//WHERE id = 1;

QString Functions::regForContest(QString contestId, QString horseId, QString jockeyLogin)
{
    QString responce = "registration&";
    QSqlQuery query;
    QString queryState = "SELECT c_u_h.horse_id AS horseid, users.login AS login FROM c_u_h "
                         "JOIN users ON c_u_h.jockey_id = users.id "
                         "WHERE contest_id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    query.exec();
    QSqlRecord rec = query.record();
    const int horse = rec.indexOf("horseid");
    const int login = rec.indexOf("login");
    int count = 0;
    while(query.next()){
        count += 1;
        if (query.value(horse).toString() == horseId) {
            qDebug() <<"horse in use";
            responce += "error&horseAlreadyInUse";
            return responce;
        }
        if (query.value(login).toString() == jockeyLogin) {
            qDebug() << "jockey In use";
            responce += "error&jockeyAlreadyInUse";
            return responce;
        }
    }
    if (count >= 6) {
        responce += "error&noPlace";
        return responce;
    }
    queryState = "SELECT id FROM users "
                 "WHERE login = :login";
    query.prepare(queryState);
    query.bindValue(":login", jockeyLogin);
    query.exec();
    rec = query.record();
    const int jockId = rec.indexOf("id");
    QString jockeyId = "";
    while(query.next()){
        jockeyId += query.value(jockId).toString();
    }
    qDebug() << jockeyId;
    //qDebug() << contestId << horseId << jockeyId;
    queryState = "INSERT INTO c_u_h (contest_id, horse_id, jockey_id) "
                 "VALUES (:cid, :hid, :jid)";
    query.prepare(queryState);
    query.bindValue(":cid", contestId);
    query.bindValue(":hid", horseId);
    query.bindValue(":jid", jockeyId);

    query.exec();
    //qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT * FROM c_u_h "
                 "WHERE contest_id = :cid AND horse_id = :hid AND jockey_id = :jid";
    query.prepare(queryState);
    query.bindValue(":cid", contestId);
    query.bindValue(":hid", horseId);
    query.bindValue(":jid", jockeyId);
    //qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        //qDebug() << query.value(0).toString();
        c += 1;
    }
    if (c == 1) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
    //queryState
}

QString Functions::regNewUser(QString name, QString surname, QString statusId, QString phone, QString address, QString birthDate, QString login, QString password, QString ltok)
{
    QString responce = "userRegistration&";

    password = password.toUtf8().toHex();
    password = getHash512(password);
    //qDebug() << "Hash" << password;
    QSqlQuery query;
    QString queryState = "INSERT INTO users (name, surname, phone, login, password, status_id, ltok, address, birth_date) "
                         "VALUES (:name, :surname, :phone, :login, :password, :status_id, :ltok, :address, :birth_date)";
    query.prepare(queryState);
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":phone", phone);
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":status_id", statusId);
    query.bindValue(":ltok", ltok);
    query.bindValue(":address", address);
    query.bindValue(":birth_date", birthDate);
    query.exec();

    //qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT id FROM users "
                 "WHERE login = :login";
    query.prepare(queryState);
    query.bindValue(":login", login);
    //qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        c += 1;
    }
    if (c > 0) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
}


QString Functions::regNewContest(QString name, QString date, QString time, QString placeId, QString status, QString info)
{
    QString responce = "contestRegistration&";
    QSqlQuery query;
    QString queryState = "INSERT INTO contests (name, date, time, hippodrome_id, status, info) "
                         "VALUES (:name, :date, :time, :place, :status, :info)";
    query.prepare(queryState);
    query.bindValue(":name", name);
    query.bindValue(":date", date);
    query.bindValue(":time", time);
    query.bindValue(":place", placeId);
    query.bindValue(":status", status);
    query.bindValue(":info", info);
    query.exec();

    //qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT id FROM contests "
                 "WHERE name = :name";
    query.prepare(queryState);
    query.bindValue(":name", name);
    //qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        c += 1;
    }
    if (c > 0) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
}


QString Functions::sendPlaces()
{
    QString responce = "places&";
    QSqlQuery query;
    QString queryState = "SELECT * FROM hippodrome";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("name");
    const int address = rec.indexOf("address");
    const int description = rec.indexOf("description");


    QString places = "";
    while(query.next()){
        places += "&" + query.value(id).toString() +
                  "&" + query.value(name).toString() +
                  "&" + query.value(address).toString() +
                  "&" + query.value(description).toString();
    }
    responce += places;
    //qDebug() << "Places" << responce;

    return responce;
}


QString Functions::sendOwnerInfo(QString login)
{
    //owner: id, name, surname
    QString responce = "ownerInfo&";
    QSqlQuery query;
    QString queryState = "SELECT id, name, surname FROM users "
                         "WHERE login = :login";
    //qDebug() << login;
    query.prepare(queryState);
    query.bindValue(":login", login);
    query.exec();
    //qDebug() << query.executedQuery();
    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int surname = rec.indexOf("surname");
    const int name = rec.indexOf("name");
    QString ownerInfo = "";
    QString ownerId = "";
    while (query.next()) {
        ownerId = query.value(id).toString();
        ownerInfo = query.value(id).toString() + "&" + query.value(name).toString() + " " + query.value(surname).toString();
    }
    //qDebug() << ownerId;
    responce += ownerInfo;
    //horses: id, name, sex, age, available

    queryState = "SELECT * FROM horses "
                 "WHERE owner_id = :ownerId";
    query.prepare(queryState);
    query.bindValue(":ownerId", ownerId);
    query.exec();
    rec = query.record();
    const int idHorse = rec.indexOf("id");
    const int nameHorse = rec.indexOf("name");
    const int dateHorse = rec.indexOf("birth_date");
    const int sexHorse = rec.indexOf("sex");
    const int availableHorse = rec.indexOf("available");
    QString horses = "";
    QDate current = QDate::currentDate();
    while (query.next()) {
        int age = current.year() - QDate::fromString(query.value(dateHorse).toString(), "yyyy-MM-dd").year();

        horses += "&" + query.value(id).toString() +
                  "&" + query.value(nameHorse).toString() +
                  "&" + query.value(sexHorse).toString() +
                  "&" + QString::number(age) +
                  "&" + query.value(availableHorse).toString();
    }
    //qDebug() << "horses" << horses;
    responce += horses;

    return responce;


}


QString Functions::sendJockeyInfo(QString login)
{

}


QString Functions::regNewPlace(QString name, QString address, QString description)
{
    QString responce = "placeRegistration&";
    QSqlQuery query;
    QString queryState = "INSERT INTO hippodrome (name, address, description) "
                         "VALUES (:name, :address, :description)";
    query.prepare(queryState);
    query.bindValue(":name", name);
    query.bindValue(":address", address);
    query.bindValue(":description", description);
    query.exec();

    //qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT id FROM hippodrome "
                 "WHERE name = :name";
    query.prepare(queryState);
    query.bindValue(":name", name);
    //qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        c += 1;
    }
    if (c > 0) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
}


QString Functions::changeHorseAvailability(QString horseId, QString status)
{
    QString responce = "changeHorseAvailability&";
    QSqlQuery query;
    QString queryState = "UPDATE horses "
                         "SET available = :status "
                         "WHERE id = :horseId";
    query.prepare(queryState);
    query.bindValue(":horseId", horseId);
    query.bindValue(":status", status);
    query.exec();

    queryState = "SELECT available FROM horses "
                 "WHERE id = :horseId";
    query.prepare(queryState);
    query.bindValue(":horseId", horseId);
    query.exec();

    bool isChanged = false;
    while (query.next()) {
        if (query.value(0).toString() == status)
            isChanged = true;
    }
    //qDebug() << "res is changed" << isChanged;

    if (isChanged)
        responce += "success";
    else
        responce += "failed";

    return responce;
}


QString Functions::regNewHorse(QString name, QString sex, QString ownerId, QString available, QString birthDate)
{
    QString responce = "horseRegistration&";
    QSqlQuery query;
    QString queryState = "INSERT INTO horses (name, sex, owner_id, available, birth_date) "
                         "VALUES (:name, :sex, :ownerId, :available, :birthDate)";
    query.prepare(queryState);
    query.bindValue(":name", name);
    query.bindValue(":sex", sex);
    query.bindValue(":ownerId", ownerId);
    query.bindValue(":available", available);
    query.bindValue(":birthDate", birthDate);
    query.exec();

    //qDebug() << "insert" << query.executedQuery();

    queryState = "SELECT id FROM horses "
                 "WHERE name = :name";
    query.prepare(queryState);
    query.bindValue(":name", name);
    //qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        c += 1;
    }
    if (c > 0) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
}


QString Functions::sendPassedNotAddedContests()
{
    QString responce = "passedNotAddedContests";
    QSqlQuery query;
    QString queryState = "SELECT c.id, c.name AS contestname, c.date, c.time, c.status, h.name AS placename FROM contests AS c "
                         "JOIN hippodrome AS h ON h.id = c.hippodrome_id "
                         "WHERE c.date < now()::date AND c.info = 'not added'";
    query.prepare(queryState);
    query.exec();
    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("contestname");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int status = rec.indexOf("status");
    const int place = rec.indexOf("placename");


    QString contests = "";
    while(query.next()){
        contests += "&" + query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(place).toString() +
                    "&" + query.value(status).toString();
    }
    //qDebug() << contests;
    if (contests == "")
        responce += "&0&no&2024-08-13&04:05:00.000&no&no";
    else
        responce += contests;
    return responce;
}

//id, place, time
QString Functions::addInfo(QStringList info)
{
    QString responce = "infoAddition&";
    QSqlQuery query;
    QString queryState = "";
    int count = (info.size() - 1) / 3;
    //set info
    for (int i = 0; i < count; ++i) {
        queryState = "UPDATE c_u_h "
                     "SET place = :place, "
                     "chip_time = :time "
                     "WHERE id = :pairId";
        query.prepare(queryState);
        query.bindValue(":pairId", info[i * 3 + 1]);
        query.bindValue(":place", info[i * 3 + 2]);
        query.bindValue(":time", info[i * 3 + 3]);
        query.exec();
    }

    queryState = "SELECT contest_id FROM c_u_h "
                 "WHERE id = :pairId";
    query.prepare(queryState);
    query.bindValue(":pairId", info[1]);
    query.exec();
    QSqlRecord rec = query.record();
    const int id = rec.indexOf("contest_id");
    QString contId = "";
    while (query.next()) {
        contId = query.value(id).toString();
    }
    queryState = "UPDATE contests "
                 "SET info = 'added' "
                 "WHERE id = :contId";
    query.prepare(queryState);
    query.bindValue(":contId", contId);
    query.exec();

    queryState = "SELECT info FROM contests "
                 "WHERE id = :contId";
    query.prepare(queryState);
    query.bindValue(":contId", contId);
    query.exec();
    rec = query.record();
    const int infoAdded = rec.indexOf("info");
    bool infoStat = false;
    while (query.next()) {
        if (query.value(infoAdded).toString() == "added")
            infoStat = true;
    }
    if (infoStat)
        responce += "success";
    else
        responce += "failed";
    //update cont status to added
    return responce;
}

// contest name & idPair & jocNameSurname & horseName
QString Functions::sendParticipantsForAddInfo(QString contestId)
{
    QString responce = "participantsForAddInfo&";
    QSqlQuery query;
    QString queryState = "SELECT c.id AS id, ct.name AS contestName, h.name AS horse, u.name AS jocName, u.surname AS jocSurname FROM c_u_h AS c "
                         "JOIN horses AS h ON h.id=c.horse_id "
                         "JOIN users AS u ON u.id = c.jockey_id "
                         "JOIN contests AS ct ON ct.id=c.contest_id "
                         "WHERE c.contest_id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    query.exec();
    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int contestName = rec.indexOf("contestName");
    const int horseName = rec.indexOf("horse");
    const int jockeyName = rec.indexOf("jocName");
    const int jockeySurname = rec.indexOf("jocSurname");


    QString ctName = "";

    QString participants = "";
    while(query.next()){
        ctName = query.value(contestName).toString();
        participants += "&" + query.value(id).toString() +
                        "&" + query.value(horseName).toString() +
                        "&" + query.value(jockeyName).toString() + " " + query.value(jockeySurname).toString();
    }
    //qDebug() << ctName << participants;
    responce += ctName;
    if (participants == "")
        responce += "&0&no&no";
    else
        responce += participants;
    return responce;
}

QString Functions::parse(QString dataFromClient, QMap<QTcpSocket*, QVector<QString>> &sockets, QTcpSocket* socket)
{
    //qDebug() << "data client" << dataFromClient;

    QStringList list = dataFromClient.split("&", Qt::SkipEmptyParts);
    //requestOpenKey&userLogin&
    if(list[0] == "requestOpenKey"){
        //requestOpenKey&user1
        return sendOpenKey(list[1]);
    }
    //shortTermOpenKey&userLogin&Server&stok.x&stok.y&
    else if(list[0] == "shortTermOpenKey"){
        if(list[2] == "Server"){
            QStringList keyInfo = sendOpenKey(list[1]).split("&", Qt::SkipEmptyParts);
            Point longTermOpenKey;
            Point shortTermOpenKey;
            longTermOpenKey.x = (uint256_t)keyInfo[2].toStdString();
            longTermOpenKey.y = (uint256_t)keyInfo[3].toStdString();

            shortTermOpenKey.x = (uint256_t)list[3].toStdString();
            shortTermOpenKey.y = (uint256_t)list[4].toStdString();
            //qDebug() << "Before: " << QString::fromStdString(shortTermOpenKey.x.str()) << " " << QString::fromStdString(shortTermOpenKey.y.str());
            //qDebug() << "Before: " << QString::fromStdString(longTermOpenKey.x.str()) << " " << QString::fromStdString(longTermOpenKey.y.str());
            Point W = mqv(longTermOpenKey, shortTermOpenKey);
            //

            qDebug() << "bef" << sockets[socket] << socket;
            QString x = "";
            QString y = "";
            for (int i = 0; i < 64; ++i) {
                x += QString::fromStdString(W.x.str())[i];
                y += QString::fromStdString(W.y.str())[i];
            }
            sockets[socket] = {x, y, list[1]};
            qDebug() << "aft" << sockets[socket] << sockets.size();
            //qDebug() << "After: " << QString::fromStdString(shortTermOpenKey.x.str()) << " " << QString::fromStdString(shortTermOpenKey.y.str());
            //qDebug() << "Before: " << QString::fromStdString(longTermOpenKey.x.str()) << " " << QString::fromStdString(longTermOpenKey.y.str());
            return sendShortTermOpenKey(list[1]);
        }
    }
    //auth&userLogin&password&
    else if (list[0] == "auth") {
        sockets[socket][2] = list[2];
        return checkPassword(list[1], list[2]);
    }
    else if (list[0] == "requestContest") {
        if (list[1] == "generalInfo") {
            return sendContestsGeneralInfo();
        }
        else if (list[1] == "mainPage") {
            return send3FutureContests();
        }
        else {
            return sendContestDetailedInfo(list[1]);
        }
    }
    else if (list[0] == "requestHorse") {
        if (list[1] == "generalInfo") {
            return sendHorsesGeneralInfo();
        }
        else {
            return sendHorseDetailedInfo(list[1]);
        }
    }
    else if (list[0] == "requestJockey") {
        if (list[1] == "generalInfo") {
            return sendJockeysGeneralInfo();
        }
        else {
            return sendJockeyDetailedInfo(list[1]);
        }
    }
    else if (list[0] == "requestHorsesForContest") {
        return sendHorsesForContest(list[1]);
    }
    else if (list[0] == "regForContest") {
        return regForContest(list[1], list[2], list[3]);
    }
    else if (list[0] == "regNewUser") {
        return regNewUser(list[1], list[2], list[3],
                          list[4], list[5], list[6],
                          list[7], list[8], list[9] + "&" + list[10]);
    }
    else if (list[0] == "regNewContest") {
        return regNewContest(list[1], list[2], list[3], list[4], list[5], list[6]);
    }
    else if (list[0] == "regNewPlace") {
        return regNewPlace(list[1], list[2], list[3]);
    }
    else if (list[0] == "regNewHorse") {
        return regNewHorse(list[1], list[2], list[3], list[4], list[5]);
    }
    else if (list[0] == "requestPlaces") {
        return sendPlaces();
    }
    else if (list[0] == "requestOwnerInfo") {
        return sendOwnerInfo(list[1]);
    }
    else if (list[0] == "changeHorseAvailability") {
        return changeHorseAvailability(list[1], list[2]);
    }
    else if (list[0] == "requestPassedNotAddedContests") {
        return sendPassedNotAddedContests();
    }
    else if (list[0] == "requestParticipants") {
        return sendParticipantsForAddInfo(list[1]);
    }
    else if (list[0] == "setInfo") {
        return addInfo(list);
    }

    return "get it!";

}
