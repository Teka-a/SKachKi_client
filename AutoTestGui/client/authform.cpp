#include "authform.h"
#include "ui_authform.h"

AuthForm::AuthForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthForm)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    Curve *secp256k1 = Curve::getInstance();
    connect(Client::getInstance(), &Client::receivedMyOpenKey, this, &AuthForm::checkSecretKey);
    connect(Client::getInstance(), &Client::receivedServerShortTermOpenKey, this, &AuthForm::shareSecret);
    connect(Client::getInstance(), &Client::receivedAuthResult, this, &AuthForm::authenticate);

}

AuthForm::~AuthForm()
{
    delete ui;
}

void AuthForm::clean(){
    ui->login->setText("");
    ui->password->setText("");
    ui->secretKey->setText("");
}


void AuthForm::on_enterButton_clicked()
{
    QString login = ui->login->text();
    QString pass = ui->password->text();
    QString key = ui->secretKey->text();

    Client::getInstance()->setLogin(login);
    Client::getInstance()->setPrivateKey((uint1024_t)key.toStdString());
    Client::getInstance()->setPassword(pass);

    qDebug() << login;
    qDebug() << pass;
    qDebug() << key;

    requestOpenKey(login);

}

void AuthForm::checkSecretKey(Point openKey)
{
    qDebug() << "checkSecretKey";
    uint1024_t privateKey = Client::getInstance()->getPrivateKey();

    Point calculatedOpenKey = secp256k1->countComposition(privateKey);

    if(openKey.x != calculatedOpenKey.x || openKey.y != calculatedOpenKey.y){
        ui->error->setText("Неверные реквизиты для входа");
        Client::getInstance()->resetPersonalData();
        clean();
    }
    else{
        uint1024_t shortTermPrivateKey = generateShortTermPrivateKey();
        Client::getInstance()->setMyServerShortTermPrivateKey(shortTermPrivateKey);
        Point shortTermOpenKey = Curve::getInstance()->countComposition(shortTermPrivateKey);
        sendShortTermOpenKey(shortTermOpenKey);
    }
}

void AuthForm::shareSecret(Point longTermOpenKey, Point shortTermOpenKey)
{
    qDebug() << "shareSecret";
    uint1024_t dA = Client::getInstance()->getPrivateKey();
    uint1024_t kA = Client::getInstance()->getMyServerShortTermPrivateKey();
    Point qA = Curve::getInstance()->countComposition(dA);
    Point rA = Curve::getInstance()->countComposition(kA);
    Point W = MQV("Server", dA, kA, qA, rA, longTermOpenKey, shortTermOpenKey);
    QString x = "";
    QString y = "";
    for (int i = 0; i < 64; ++i) {
        x += QString::fromStdString(W.x.str())[i];
        y += QString::fromStdString(W.y.str())[i];
    }
    Client::getInstance()->setServerKey(x);
    Client::getInstance()->setServerIV(y);
    sendCredentials();
}

void AuthForm::authenticate(QString status, QString userStatus)
{
    qDebug() << "auth";
    if (status == "success"){
        Client::getInstance()->setUserStatus(userStatus);
        this->close();
        emit successLogin();
        request3Contests();
        qDebug() << "requested";
        //emit successLogin();
    } else if (status == "failed"){
        Client::getInstance()->resetPersonalData();
        ui->error->setText("Неверные реквизиты для входа");
        clean();
    }
}


