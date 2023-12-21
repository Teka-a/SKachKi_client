#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QWidget>
#include <QDebug>

#include "communication.h"
#include "mqv.h"

namespace Ui {
class AuthForm;
}

class AuthForm : public QWidget
{
    Q_OBJECT

public:
    explicit AuthForm(QWidget *parent = nullptr);
    Ui::AuthForm *ui;
    ~AuthForm();

private slots:
    void on_enterButton_clicked();
    void checkSecretKey(Point openKey);
    void shareSecret(Point longTermOpenKey, Point shortTermOpenKey);
    void authenticate(QString status, QString userStatus);
signals:
    void successLogin();
private:

    Curve *secp256k1;
    void clean();
};

#endif // AUTHFORM_H
