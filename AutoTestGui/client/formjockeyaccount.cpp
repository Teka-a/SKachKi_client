#include "formjockeyaccount.h"
#include "ui_formjockeyaccount.h"

FormJockeyAccount::FormJockeyAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormJockeyAccount)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    connect(Client::getInstance(), &Client::receivedJockeyForAccount, this, &FormJockeyAccount::setInfo);
}

FormJockeyAccount::~FormJockeyAccount()
{
    delete ui;
}

void FormJockeyAccount::setInfo(QString ownerId, QString jockeyWelcome, QVector<Contest> contests)
{

}
