#ifndef FORMADMINACCOUNT_H
#define FORMADMINACCOUNT_H

#include <QWidget>
#include "client.h"
#include "communication.h"
#include <QMap>
#include "mqv.h"

namespace Ui {
class FormAdminAccount;
}

class FormAdminAccount : public QWidget
{
    Q_OBJECT

public:
    explicit FormAdminAccount(QWidget *parent = nullptr);
    ~FormAdminAccount();

private slots:
    void on_pushButton_clicked();
    void receivedRegStatus(QString status);
    void receivedContestRegStatus(QString status);
    void receivedPlaceRegStatus(QString status);
    void setUp(QVector<Place> places);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::FormAdminAccount *ui;
    uint1024_t privateKey = 0;
    QString generateLTOK();
    void reset();
    QVector<Place> placesList;
    QMap<QString, QString> userStatId {{"Администратор", "3"},
                                      {"Владелец лошади", "2"},
                                      {"Жокей", "1"}};
};

#endif // FORMADMINACCOUNT_H
