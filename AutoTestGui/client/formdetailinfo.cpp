#include "formdetailinfo.h"
#include "ui_formdetailinfo.h"

FormDetailInfo::FormDetailInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDetailInfo)
{
    regForm = new FormRegContest();
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    connect(Client::getInstance(), &Client::receivedContest, this, &FormDetailInfo::setDetailedInfo);

}

FormDetailInfo::~FormDetailInfo()
{
    delete ui;
}



void FormDetailInfo::setDetailedInfo(Contest contest, Place hippodrome, QVector<ParticipantPair> participants)
{
    this->contestId = contest.getId();
    ui->ContestName->setText(contest.getName());
    ui->contestDate->setText("Дата: " + contest.getFullDate());
    ui->contestTime->setText("Время: " + contest.getFullTime());
    ui->contestPlace->setText("Место: " + contest.getPlace());
    ui->contestDescription->setText("Описание: " + hippodrome.getDescription() + "\n" + hippodrome.getAddress());
    ui->contestPlacesJ->setText("Всего мест: 6");
    int registered = 0;
    for (int i = 0; i < 6; ++i) {
        if (participants[i].horseName == "-") {
            break;
        }
        else {
            registered += 1;
        }
    }
    ui->contestLJ->setText("Осталось мест: " + QString::number(6 - registered));
    ui->jockey->setStyleSheet("color : rgb(166, 166, 166);");
    ui->horse->setStyleSheet("color : rgb(166, 166, 166);");
    ui->ContestName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->Registered->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    if (contest.isPassed()) {
        ui->Registered->setText("Результаты");
        ui->place->setVisible(true);
        ui->time->setVisible(true);

        ui->contestLJ->hide();
        ui->contestPlacesJ->hide();

        ui->p1->setVisible(true);
        ui->t1->setVisible(true);
        if (participants[0].jockey == "-") {
            ui->p1->setText(" ");
            ui->j1->setText(participants[0].jockey);
            ui->h1->setText(participants[0].horseName);
            ui->t1->setText(" ");
        } else {
            ui->p1->setText("1");
            ui->j1->setText(participants[0].jockey);
            ui->h1->setText(participants[0].horseName);
            ui->t1->setText(participants[0].time.toString());
        }


        ui->p2->setVisible(true);
        ui->t2->setVisible(true);
        if (participants[1].jockey == "-") {
            ui->p2->setText(" ");
            ui->j2->setText(participants[1].jockey);
            ui->h2->setText(participants[1].horseName);
            ui->t2->setText(" ");
        } else {
            ui->p2->setText("2");
            ui->j2->setText(participants[1].jockey);
            ui->h2->setText(participants[1].horseName);
            ui->t2->setText(participants[1].time.toString());
        }

        ui->p3->setVisible(true);
        ui->t3->setVisible(true);
        if (participants[2].jockey == "-") {
            ui->p3->setText(" ");
            ui->j3->setText(participants[2].jockey);
            ui->h3->setText(participants[2].horseName);
            ui->t3->setText(" ");
        } else {
            ui->p3->setText("3");
            ui->j3->setText(participants[2].jockey);
            ui->h3->setText(participants[2].horseName);
            ui->t3->setText(participants[2].time.toString());
        }

        ui->p4->setVisible(true);
        ui->t4->setVisible(true);
        if (participants[3].jockey == "-") {
            ui->p4->setText(" ");
            ui->j4->setText(participants[3].jockey);
            ui->h4->setText(participants[3].horseName);
            ui->t4->setText(" ");
        } else {
            ui->p4->setText("4");
            ui->j4->setText(participants[3].jockey);
            ui->h4->setText(participants[3].horseName);
            ui->t4->setText(participants[3].time.toString());
        }

        ui->p5->setVisible(true);
        ui->t5->setVisible(true);
        if (participants[4].jockey == "-") {
            ui->p5->setText(" ");
            ui->j5->setText(participants[4].jockey);
            ui->h5->setText(participants[4].horseName);
            ui->t5->setText(" ");
        } else {
            ui->p5->setText("5");
            ui->j5->setText(participants[4].jockey);
            ui->h5->setText(participants[4].horseName);
            ui->t5->setText(participants[4].time.toString());
        }


        ui->p6->setVisible(true);
        ui->t6->setVisible(true);
        if (participants[5].jockey == "-") {
            ui->p6->setText(" ");
            ui->j6->setText(participants[5].jockey);
            ui->h6->setText(participants[5].horseName);
            ui->t6->setText(" ");
        } else {
            ui->p6->setText("6");
            ui->j6->setText(participants[5].jockey);
            ui->h6->setText(participants[5].horseName);
            ui->t6->setText(participants[5].time.toString());
        }

    }
    else {
        ui->Registered->setText("Зарегистрированы");
        ui->place->hide();
        ui->time->hide();

        ui->contestLJ->setVisible(true);
        ui->contestPlacesJ->setVisible(true);

        ui->p1->hide();
        ui->j1->setText(participants[0].jockey);
        ui->h1->setText(participants[0].horseName);
        ui->t1->hide();

        ui->p2->hide();
        ui->j2->setText(participants[1].jockey);
        ui->h2->setText(participants[1].horseName);
        ui->t2->hide();

        ui->p3->hide();
        ui->j3->setText(participants[2].jockey);
        ui->h3->setText(participants[2].horseName);
        ui->t3->hide();

        ui->p4->hide();
        ui->j4->setText(participants[3].jockey);
        ui->h4->setText(participants[3].horseName);
        ui->t4->hide();

        ui->p5->hide();
        ui->j5->setText(participants[4].jockey);
        ui->h5->setText(participants[4].horseName);
        ui->t5->hide();

        ui->p6->hide();
        ui->j6->setText(participants[5].jockey);
        ui->h6->setText(participants[5].horseName);
        ui->t6->hide();
    }

    qDebug() << contest.getFullDate() << contest.getFullTime();
    qDebug() << hippodrome.getName();
    qDebug() << participants[0].horseName;
    qDebug() << Client::getInstance()->getUserStatus();
    if (Client::getInstance()->getUserStatus() != "jockey") {
        ui->regButton->setVisible(false);
    }
    FormDetailInfo::show();
}



void FormDetailInfo::on_regButton_clicked()
{
    qDebug() << "reg";
    requestHorsesForContest(this->contestId);
    FormDetailInfo::close();
}

