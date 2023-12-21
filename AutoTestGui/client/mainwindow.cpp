#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    this->setStyleSheet("QGroupBox::title { color : rgb(255, 218, 185); font-size: 150px;} ");

    ui->centralContest->setStyleSheet("QGroupBox::title{font-size:40pt;}");
    infoContestWindow = new InfoContest;
    authForm = new AuthForm;
    authForm->show();

    connect(authForm, &AuthForm::successLogin, this, &MainWindow::anons);
    connect(Client::getInstance(), &Client::receivedFutureContestsForMain, this, &MainWindow::renderInfo3Contests);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::anons()
{
    MainWindow::show();
}


void MainWindow::setNoInfoRight()
{
    ui->rightContest->setTitle(" ");
    ui->rightContestDate->setText(" ");
    ui->rightContestTime->setText("Нет информации");
    ui->rightContestTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->rightContestPlace->setText(" ");
    ui->rightContestRegStatus->setText(" ");
    ui->rightContestMoreInfo->setDisabled(true);
}


void MainWindow::setNoInfoCentral()
{
    ui->centralContest->setTitle(" ");
    ui->centralContestDate->setText(" ");
    ui->centralContestTime->setText("Нет информации");
    ui->centralContestTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->centralContestPlace->setText(" ");
    ui->centralContestRegStatus->setText(" ");
    ui->centralContestMoreInfo->setDisabled(true);
}


void MainWindow::setNoInfoLeft()
{
    ui->leftContest->setTitle(" ");
    ui->leftContestDate->setText(" ");
    ui->leftContestTime->setText("Нет информации");
    ui->leftContestTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->leftContestPlace->setText(" ");
    ui->leftContestRegStatus->setText(" ");
    ui->leftContestMoreInfo->setDisabled(true);
}


void MainWindow::setInfoRight(Contest contest)
{
    ui->rightContest->setTitle(contest.getName());
    ui->rightContestDate->setText("Дата: " + contest.getRawDate());
    ui->rightContestTime->setText("Время: " + contest.getRawTime());
    ui->rightContestPlace->setText("Место: " + contest.getPlace());
    ui->rightContestRegStatus->setText("Регистрация: " + contest.getRegStatus());
    ui->rightContestMoreInfo->setStyleSheet("background-color: rgb(194, 165, 140)");
    ui->rightContestMoreInfo->setProperty("id", contest.getId());
}


void MainWindow::setInfoCentral(Contest contest)
{
    ui->centralContest->setTitle(contest.getName());
    ui->centralContestDate->setText("Дата: " + contest.getRawDate());
    ui->centralContestTime->setText("Время: " + contest.getRawTime());
    ui->centralContestPlace->setText("Место: " + contest.getPlace());
    ui->centralContestRegStatus->setText("Регистрация: " + contest.getRegStatus());
    ui->centralContestMoreInfo->setStyleSheet("background-color: rgb(194, 165, 140)");
    ui->centralContestMoreInfo->setProperty("id", contest.getId());
}


void MainWindow::setInfoLeft(Contest contest)
{
    ui->leftContest->setTitle(contest.getName());
    ui->leftContestDate->setText("Дата: " + contest.getRawDate());
    ui->leftContestTime->setText("Время: " + contest.getRawTime());
    ui->leftContestPlace->setText("Место: " + contest.getPlace());
    ui->leftContestRegStatus->setText("Регистрация: " + contest.getRegStatus());
    ui->leftContestMoreInfo->setStyleSheet("background-color: rgb(194, 165, 140)");
    ui->leftContestMoreInfo->setProperty("id", contest.getId());
}


void MainWindow::renderInfo3Contests(QVector<Contest> contests)
{
    if (contests.size() == 3) {
        setInfoRight(contests[0]);
        setInfoCentral(contests[1]);
        setInfoLeft(contests[2]);
    }
    else if (contests.size() == 2) {
        setNoInfoRight();
        setInfoCentral(contests[0]);
        setInfoLeft(contests[1]);
    }
    else if (contests.size() == 1) {
        setNoInfoRight();
        setInfoCentral(contests[0]);
        setNoInfoLeft();
    }
    else {
        setNoInfoRight();
        setNoInfoCentral();
        setNoInfoLeft();
    }
}


void MainWindow::on_centralContestMoreInfo_clicked()
{
    qDebug() << ui->centralContestMoreInfo->property("id").toInt();
    requestDetailedInfoContest(QString::number(ui->centralContestMoreInfo->property("id").toInt()));
}

