#include "formaddinfo.h"
#include "ui_formaddinfo.h"

FormAddInfo::FormAddInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAddInfo)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainMane->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    connect(Client::getInstance(), &Client::receivedContestParticipants, this, &FormAddInfo::setInfo);
    connect(Client::getInstance(), &Client::addInfoStatus, this, &FormAddInfo::receivedAddStatus);
}

FormAddInfo::~FormAddInfo()
{
    delete ui;
}

void FormAddInfo::receivedAddStatus(QString status)
{

    if (status == "s")
        ui->result->setText("Изменения успешно внесены");
    else if (status == "f")
        ui->result->setText("Изменения не были зарегистрированы ввиду загадочных обстоятельств(");

}

void FormAddInfo::reset()
{
    ui->j1->setText("-");
    ui->h1->setText("-");
    ui->timeEdit->setTime(QTime::fromString("00:00:00"));
    ui->j2->setText("-");
    ui->h2->setText("-");
    ui->timeEdit_2->setTime(QTime::fromString("00:00:00"));
    ui->j3->setText("-");
    ui->h3->setText("-");
    ui->timeEdit_3->setTime(QTime::fromString("00:00:00"));
    ui->j4->setText("-");
    ui->h4->setText("-");
    ui->timeEdit_4->setTime(QTime::fromString("00:00:00"));
    ui->j5->setText("-");
    ui->h5->setText("-");
    ui->timeEdit_5->setTime(QTime::fromString("00:00:00"));
    ui->j6->setText("-");
    ui->h6->setText("-");
    ui->timeEdit_6->setTime(QTime::fromString("00:00:00"));
}

void FormAddInfo::setPair(int num)
{
    if (num == 1) {
        ui->j1->setText(participantsInfo[0][2]);
        ui->h1->setText(participantsInfo[0][1]);
    }
    else if (num == 2) {
        ui->j2->setText(participantsInfo[1][2]);
        ui->h2->setText(participantsInfo[1][1]);
    }
    else if (num == 3) {
        ui->j3->setText(participantsInfo[2][2]);
        ui->h3->setText(participantsInfo[2][1]);
    }
    else if (num == 4) {
        ui->j4->setText(participantsInfo[3][2]);
        ui->h4->setText(participantsInfo[3][1]);
    }
    else if (num == 5) {
        ui->j5->setText(participantsInfo[4][2]);
        ui->h5->setText(participantsInfo[4][1]);
    }
    else if (num == 6) {
        ui->j6->setText(participantsInfo[5][2]);
        ui->h6->setText(participantsInfo[5][1]);
    }
}



void FormAddInfo::setInfo(QString contName, QVector<QVector<QString>> participants)
{
    reset();
    ui->contName->setText(contName);
    this->totalCountOfPaitrs = participants.size();
    this->participantsInfo = participants;

    for (int i = 1; i <= totalCountOfPaitrs; ++i) {
        setPair(i);
    }
    FormAddInfo::show();
}

QPair<QTime, QString> FormAddInfo::getInfoAboutPair(int num)
{
    QPair<QTime, QString> info;
    if (num == 1) {
        info.first = ui->timeEdit->time();
        info.second = this->participantsInfo[0][0];
    }
    else if (num == 2) {
        info.first = ui->timeEdit_2->time();
        info.second = this->participantsInfo[1][0];
    }
    else if (num == 3) {
        info.first = ui->timeEdit_3->time();
        info.second = this->participantsInfo[2][0];
    }
    else if (num == 4) {
        info.first = ui->timeEdit_4->time();
        info.second = this->participantsInfo[3][0];
    }
    else if (num == 5) {
        info.first = ui->timeEdit_5->time();
        info.second = this->participantsInfo[4][0];
    }
    else if (num == 6) {
        info.first = ui->timeEdit_6->time();
        info.second = this->participantsInfo[5][0];
    }
    return info;
}

QTime FormAddInfo::getTime(int num)
{
    if (num == 1) {
        return ui->timeEdit->time();
    }
    else if (num == 2) {
        return ui->timeEdit_2->time();
    }
    else if (num == 3) {
        return ui->timeEdit_3->time();
    }
    else if (num == 4) {
        return ui->timeEdit_4->time();
    }
    else if (num == 5) {
        return ui->timeEdit_5->time();
    }
    else if (num == 6) {
        return ui->timeEdit_6->time();
    }
}

QVector<QTime> FormAddInfo::sortTime()
{
    QVector<QTime> times;
    for (int i = 1; i <= this->totalCountOfPaitrs; ++i) {
        times.push_back(getTime(i));
    }
    qSort(times.begin(), times.end());
    for (int i = 0; i < times.size(); ++i) {
        qDebug() << times[i].toString();
    }
    return times;
}

void FormAddInfo::on_pushButton_clicked()
{
    QVector<QTime> places = sortTime();
    QString info = "";
    for (int i = 1; i <= this->totalCountOfPaitrs; ++i) {
        QPair<QTime, QString> timeId = getInfoAboutPair(i);
        info += timeId.second + "&" + QString::number(places.indexOf(timeId.first) + 1) + "&" + timeId.first.toString() + "&";
    }
    setResults(info);
}

