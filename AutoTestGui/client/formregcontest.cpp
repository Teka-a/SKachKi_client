#include "formregcontest.h"
#include "ui_formregcontest.h"

FormRegContest::FormRegContest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormRegContest)
{

    ui->setupUi(this);

    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    connect(Client::getInstance(), &Client::receivedHorsesForContest, this, &FormRegContest::setInfoHorses);

}


void FormRegContest::setInfoHorses(QString contestId, QString contestName, QVector<Horse> horsesList)
{
    //qDebug() << contestId << contestName;
    ui->contestName->setText(contestName);
    this->contestId = contestId.toInt();
    horses = horsesList;
    for (int i = 0; i < horsesList.size(); ++i) {
        ui->horsesBox->addItem(horsesList[i].getName());

        qDebug() << horsesList[i].getName();
    }
    FormRegContest::show();
}


FormRegContest::~FormRegContest()
{
    delete ui;
}


void FormRegContest::on_regButton_clicked()
{
    regForContest(QString::number(this->contestId), QString::number(this->horseSelectedId), Client::getInstance()->getLogin());
}


void FormRegContest::on_horsesBox_activated(int index)
{
    qDebug() << index;
    if (index > 0) {
        Horse horse = horses[index - 1];
        qDebug() << horse.getName();
        ui->horseInfo->setText(horse.getName());
        ui->horseAge->setText("Возраст: " + horse.getAge());
        ui->horseSex->setText("Пол: " + horse.getSex());
        ui->horseOwner->setText("Владелец: " + horse.getOwner());
        horseSelectedId = horse.getId().toInt();
    }
    else {
        ui->horseInfo->setText("");
        ui->horseAge->setText("");
        ui->horseSex->setText("");
        ui->horseOwner->setText("");
        horseSelectedId = -1;
    }
}

