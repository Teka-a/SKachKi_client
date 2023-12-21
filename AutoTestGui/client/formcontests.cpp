#include "formcontests.h"
#include "ui_formcontests.h"

FormContests::FormContests(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormContests)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->name->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->date->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->time->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->place->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->moreInfoCol->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    connect(Client::getInstance(), &Client::receivedContests, this, &FormContests::setInfo);

}

FormContests::~FormContests()
{
    delete ui;
}


void FormContests::setInfo(QVector<Contest> contests)
{
    qDebug() << "here";
    QWidget *window = new QWidget;

    QVBoxLayout *layoutMain = new QVBoxLayout(window);
    /*
    QWidget *item = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(item);
    QLabel *nameCol = new QLabel("Название");
    QLabel *dateCol = new QLabel("Дата");
    QLabel *timeCol = new QLabel("Время");
    QLabel *placeCol = new QLabel("Место");
    QLabel *moreInfoCol = new QLabel("Подробности");
    layout->addWidget(nameCol);
    layout->addWidget(dateCol);
    layout->addWidget(timeCol);
    layout->addWidget(placeCol);
    layout->addWidget(moreInfoCol);
    layoutMain->addWidget(item);
    */
    for (int i = 0; i < contests.size(); ++i) {
        QWidget *item = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout(item);

        QLabel *name = new QLabel(contests[i].getName());
        QLabel *date = new QLabel(contests[i].getRawDate());
        QLabel *time = new QLabel(contests[i].getRawTime());
        QLabel *place = new QLabel(contests[i].getPlace());
        QPushButton *moreInfo = new QPushButton("Подробнее");
        /*
        if (contests[i].isPassed())
            connect(moreInfo, &QPushButton::clicked, this, &FormContests::showMoreInfoPassed);
        else
            connect(moreInfo, &QPushButton::clicked, this, &FormContests::showMoreInfoFuture);*/
        connect(moreInfo, &QPushButton::clicked, this, &FormContests::showMoreInfo);
        moreInfo->setProperty("id", contests[i].getId());
        layout->addWidget(name);
        layout->addWidget(date);
        layout->addWidget(time);
        layout->addWidget(place);
        layout->addWidget(moreInfo);
        layoutMain->addWidget(item);
    }

    layoutMain->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    ui->scrollArea->setWidget(window);
    FormContests::show();

}


void FormContests::showMoreInfo()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestDetailedInfoContest(QString::number(buttonSender->property("id").toInt()));
}

/*
void FormContests::showMoreInfoFuture()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestDetailedInfoContest(QString::number(buttonSender->property("id").toInt()));
}

void FormContests::showMoreInfoPassed()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestDetailedInfoContest(QString::number(buttonSender->property("id").toInt()));
}*/
