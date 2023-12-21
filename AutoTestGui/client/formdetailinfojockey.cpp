#include "formdetailinfojockey.h"
#include "ui_formdetailinfojockey.h"

FormDetailInfoJockey::FormDetailInfoJockey(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDetailInfoJockey)
{
    ui->setupUi(this);
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->name->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->stat->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->date->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->time->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->place->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->moreInfoCol->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    ui->nameLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->surnameLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->ageLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    connect(Client::getInstance(), &Client::receivedJockey, this, &FormDetailInfoJockey::renderInfo);
}

FormDetailInfoJockey::~FormDetailInfoJockey()
{
    delete ui;
}


void FormDetailInfoJockey::renderInfo(Jockey jockey, QVector<Contest> contests)
{
    qDebug() << jockey.getId() << jockey.getName() << jockey.getSurname() << jockey.getAge();
    ui->nameJoc->setText(jockey.getName());
    ui->surnameJoc->setText(jockey.getSurname());
    ui->ageJoc->setText(jockey.getAge());
    QWidget *window = new QWidget;

    QVBoxLayout *layoutMain = new QVBoxLayout(window);
    if (contests[0].getName() == "no") {
        qDebug() << "set no info";
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
            QLabel *status = new QLabel(contests[i].getRegStatus());
            QPushButton *moreInfo = new QPushButton("Подробнее");
            connect(moreInfo, &QPushButton::clicked, this, &FormDetailInfoJockey::showMoreInfo);
            moreInfo->setProperty("id", contests[i].getId());
            layout->addWidget(name);
            layout->addWidget(date);
            layout->addWidget(time);
            layout->addWidget(place);
            layout->addWidget(status);
            layout->addWidget(moreInfo);
            layoutMain->addWidget(item);
        }
    }
    layoutMain->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    ui->scrollArea->setWidget(window);
    FormDetailInfoJockey::show();
}

void FormDetailInfoJockey::showMoreInfo()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestDetailedInfoContest(QString::number(buttonSender->property("id").toInt()));
}
