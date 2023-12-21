#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#include <QString>
#include "client.h"

void requestOpenKey(QString userLogin);
void sendShortTermOpenKey(Point shortTermOpenKey, QString to = "Server");
void sendCredentials();

void requestOwnerInfo();
void requestJockeyInfo();

void requestGeneralInfoContests();
void requestGeneralInfoHorses();
void requestGeneralInfoJockeys();

void requestDetailedInfoContest(QString id);
void requestDetailedInfoHorse(QString id);
void requestDetailedInfoJockey(QString id);

void request3Contests();
void requestHorsesForContest(QString id);
void requestPlaces();
void regForContest(QString contestId, QString horseId, QString jockeyLogin);
void requestPassedNotAddedContests();
void requestParticipants(QString contestId);

void regNewUser(QString name, QString surname, QString phone, QString login, QString password, QString statusId, QString ltok, QString address, QString birthdate);
void regNewContest(QString name, QString date, QString time, QString placeId, QString status, QString info);
void regNewPlace(QString name, QString address, QString description);
void regNewHorse(QString ownerId, QString name, QString sex, QString birthDate, QString available);

void changeHorseAvailability(QString horseId, QString status);

void setResults(QString info);

#endif // COMMUNICATION_H
