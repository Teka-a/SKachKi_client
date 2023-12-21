#include "formadminaccount.h"
#include "ui_formadminaccount.h"

FormAdminAccount::FormAdminAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAdminAccount)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->label->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    reset();
    ui->secretKey->setText("");
    connect(Client::getInstance(), &Client::receivedPlaces, this, &FormAdminAccount::setUp);
    connect(Client::getInstance(), &Client::registrationStatus, this, &FormAdminAccount::receivedRegStatus);
    connect(Client::getInstance(), &Client::registrationContestStatus, this, &FormAdminAccount::receivedContestRegStatus);
    connect(Client::getInstance(), &Client::registrationPlaceStatus, this, &FormAdminAccount::receivedPlaceRegStatus);

}

FormAdminAccount::~FormAdminAccount()
{
    delete ui;
}

void FormAdminAccount::setUp(QVector<Place> places)
{
    placesList = places;
    for (int i = 0; i < places.size(); ++i) {
        ui->chosePlace->addItem(places[i].getName());
    }
    FormAdminAccount::show();
}

void FormAdminAccount::reset()
{
    ui->enterName->clear();
    ui->enterSurname->clear();
    ui->enterAddress->clear();
    ui->enterPhoneNumber->clear();
    ui->enterLogin->clear();
    ui->enterPassword->clear();
    ui->enterDateBirth->setDate(QDate::fromString("2000-01-01", "yyyy-MM-dd"));
    ui->status->setCurrentIndex(0);
    ui->secretKey->clear();

    ui->choseNameContest->clear();
    ui->choseRegStatus->setCurrentIndex(0);
    ui->chosePlace->setCurrentIndex(0);
    ui->choseDateTime->setDate(QDate::fromString("2000-01-01", "yyyy-MM-dd"));
    ui->choseDateTime->setTime(QTime::fromString("00:00:00"));
    ui->contRegStat->clear();

    ui->regPlaceStatus->clear();
    ui->placeAddress->clear();
    ui->placeDesc->clear();
    ui->placeName->clear();
}

void FormAdminAccount::receivedRegStatus(QString status)
{
    reset();
    if (status == "s")
        ui->secretKey->setText("Пользователь успешно зарегистрирован. Выбран секретный ключ: " + QString::fromStdString(this->privateKey.str()));
    else if (status == "f")
        ui->secretKey->setText("Пользователь не был зарегистрирован ввиду загадочных обстоятельств(");

}

void FormAdminAccount::receivedContestRegStatus(QString status)
{
    reset();
    if (status == "s")
        ui->contRegStat->setText("Соревнование успешно добавлено.");
    else if (status == "f")
        ui->contRegStat->setText("Соревнование не было добавлено ввиду загадочных обстоятельств(");

}

void FormAdminAccount::receivedPlaceRegStatus(QString status)
{
    reset();
    if (status == "s")
        ui->regPlaceStatus->setText("Ипподром успешно добавлен.");
    else if (status == "f")
        ui->regPlaceStatus->setText("Ипподром не был добавлен ввиду загадочных обстоятельств(");

}

QString FormAdminAccount::generateLTOK(){

    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    bool ok;
    this->privateKey = (uint1024_t)std::to_string(partsUuid[2].toLongLong(&ok, 16));
    qDebug() << "secretKey" << QString::fromStdString(privateKey.str());

    Point p = Curve::getInstance()->countComposition(privateKey);
    QString ltok = QString::fromStdString(p.x.str()) + "&" + QString::fromStdString(p.y.str());
    return ltok;

}

void FormAdminAccount::on_pushButton_clicked()
{
    QString name = ui->enterName->text();
    QString surname = ui->enterSurname->text();
    QString address = ui->enterAddress->text();
    QString phone = ui->enterPhoneNumber->text();
    //qDebug() << ui->status->currentText();
    QString status = userStatId[ui->status->currentText()];
    QString date = ui->enterDateBirth->date().toString();
    //qDebug() << ui->enterDateBirth->date();
    QString login = ui->enterLogin->text();
    QString password = ui->enterPassword->text();
    //name, surname, phone, login, password, statusId, ltok, address, birthdate
    QString ltok = generateLTOK();
    regNewUser(name, surname, phone, login, password, status, ltok, address, date);
}


void FormAdminAccount::on_pushButton_2_clicked()
{
    QString contestName = ui->choseNameContest->text();
    QString contestDate = ui->choseDateTime->date().toString();
    QString contestTime = ui->choseDateTime->time().toString();
    QString contestPlace = placesList[ui->chosePlace->currentIndex() - 1].getId();
    QString info = "not added";
    QString regStatus = ui->choseRegStatus->currentText();

    regNewContest(contestName, contestDate, contestTime, contestPlace, regStatus, info);
}


void FormAdminAccount::on_pushButton_3_clicked()
{
    QString placeName = ui->placeName->text();
    QString placeAddress = ui->placeAddress->text();
    QString placeDesc = ui->placeDesc->text();

    regNewPlace(placeName, placeAddress, placeDesc);
}


void FormAdminAccount::on_pushButton_4_clicked()
{
    requestPassedNotAddedContests();
}

