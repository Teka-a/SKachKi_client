#ifndef FORMJOCKEYACCOUNT_H
#define FORMJOCKEYACCOUNT_H

#include <QWidget>
#include "client.h"
#include "communication.h"

namespace Ui {
class FormJockeyAccount;
}

class FormJockeyAccount : public QWidget
{
    Q_OBJECT

public:
    explicit FormJockeyAccount(QWidget *parent = nullptr);
    ~FormJockeyAccount();
private slots:
    void setInfo(QString ownerId, QString jockeyWelcome, QVector<Contest> contests);
private:
    Ui::FormJockeyAccount *ui;
};

#endif // FORMJOCKEYACCOUNT_H
