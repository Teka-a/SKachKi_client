#include "formdetailinfohorse.h"
#include "ui_formdetailinfohorse.h"

FormDetailInfoHorse::FormDetailInfoHorse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDetailInfoHorse)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->name->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->stat->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->date->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->time->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->place->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->moreInfoCol->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    ui->nameLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->ownerLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->sexlabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->ageLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->availableLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    connect(Client::getInstance(), &Client::receivedHorse, this, &FormDetailInfoHorse::renderInfo);

}

FormDetailInfoHorse::~FormDetailInfoHorse()
{
    delete ui;
}


void FormDetailInfoHorse::renderInfo(Horse horse, QVector<Contest> contests)
{
    qDebug() << horse.getId() << horse.getName() << horse.getSex() << horse.getAge() << horse.getOwner();
    ui->nameHorse->setText(horse.getName());
    ui->owner->setText(horse.getOwner());
    ui->age->setText(horse.getAge());
    ui->sex->setText(horse.getSex());
    ui->available->setText(horse.getAvailability());
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
            connect(moreInfo, &QPushButton::clicked, this, &FormDetailInfoHorse::showMoreInfo);
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
    FormDetailInfoHorse::show();
}

void FormDetailInfoHorse::showMoreInfo()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestDetailedInfoContest(QString::number(buttonSender->property("id").toInt()));
}
