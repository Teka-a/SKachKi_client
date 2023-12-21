#include "formcontestsaddinfo.h"
#include "ui_formcontestsaddinfo.h"

FormContestsAddInfo::FormContestsAddInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormContestsAddInfo)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->name->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->date->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->time->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->place->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->moreInfoCol->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    addInfoWindow = new FormAddInfo;

    connect(Client::getInstance(), &Client::receivedContestsAddInfo, this, &FormContestsAddInfo::setInfo);
}

FormContestsAddInfo::~FormContestsAddInfo()
{
    delete ui;
}


void FormContestsAddInfo::setInfo(QVector<Contest> contests)
{
    QWidget *window = new QWidget;

    QVBoxLayout *layoutMain = new QVBoxLayout(window);
    if (contests[0].getName() == "no") {
        QLabel *item = new QLabel("Нет информации");
        item->setAlignment(Qt::AlignCenter);
        layoutMain->addWidget(item);
    }
    else {
        for (int i = 0; i < contests.size(); ++i) {
            QWidget *item = new QWidget;
            QHBoxLayout *layout = new QHBoxLayout(item);

            QLabel *name = new QLabel(contests[i].getName());
            QLabel *date = new QLabel(contests[i].getRawDate());
            QLabel *time = new QLabel(contests[i].getRawTime());
            QLabel *place = new QLabel(contests[i].getPlace());
            QPushButton *moreInfo = new QPushButton("Добавить");

            connect(moreInfo, &QPushButton::clicked, this, &FormContestsAddInfo::showMoreInfo);
            moreInfo->setProperty("id", contests[i].getId());
            layout->addWidget(name);
            layout->addWidget(date);
            layout->addWidget(time);
            layout->addWidget(place);
            layout->addWidget(moreInfo);
            layoutMain->addWidget(item);
        }
    }


    layoutMain->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    ui->scrollArea->setWidget(window);
    FormContestsAddInfo::show();

}


void FormContestsAddInfo::showMoreInfo()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestParticipants(QString::number(buttonSender->property("id").toInt()));
}
