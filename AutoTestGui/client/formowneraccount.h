#ifndef FORMOWNERACCOUNT_H
#define FORMOWNERACCOUNT_H

#include <QWidget>
#include "client.h"
#include "communication.h"

namespace Ui {
class FormOwnerAccount;
}

class FormOwnerAccount : public QWidget
{
    Q_OBJECT

public:
    explicit FormOwnerAccount(QWidget *parent = nullptr);
    ~FormOwnerAccount();
private slots:
    void setInfo(QString ownerId, QString ownerWelcome, QVector<Horse> horses);
    void changeAvailabilityTrue();
    void changeAvailabilityFalse();
    void receivedChangeStatus(QString status);
    void receivedRegStatus(QString status);
    void reset();

    void on_pushButton_clicked();

private:
    Ui::FormOwnerAccount *ui;
    QString ownerId = "0";
};

#endif // FORMOWNERACCOUNT_H
