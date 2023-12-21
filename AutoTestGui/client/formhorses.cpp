#include "formhorses.h"
#include "ui_formhorses.h"

FormHorses::FormHorses(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormHorses)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->name->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->age->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->sex->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->owner->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->moreInfoCol->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    connect(Client::getInstance(), &Client::receivedHorses, this, &FormHorses::setInfo);
}

FormHorses::~FormHorses()
{
    delete ui;
}


void FormHorses::setInfo(QVector<Horse> horses)
{
    QWidget *window = new QWidget;

    QVBoxLayout *layoutMain = new QVBoxLayout(window);
    /*
    QWidget *item = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(item);
    QLabel *nameCol = new QLabel("Кличка");
    QLabel *sexCol = new QLabel("Пол");
    QLabel *ageCol = new QLabel("Возраст");
    QLabel *ownCol = new QLabel("Владелец");
    QLabel *moreInfoCol = new QLabel("Подробности");
    layout->addWidget(nameCol);
    layout->addWidget(sexCol);
    layout->addWidget(ageCol);
    layout->addWidget(ownCol);
    layout->addWidget(moreInfoCol);
    layoutMain->addWidget(item);
    */
    for (int i = 0; i < horses.size(); ++i) {
        QWidget *item = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout(item);

        QLabel *name = new QLabel(horses[i].getName());
        QLabel *sex = new QLabel(horses[i].getSex());
        QLabel *age = new QLabel(horses[i].getAge());
        QLabel *owner = new QLabel(horses[i].getOwner());
        QPushButton *moreInfo = new QPushButton("Подробнее");
        connect(moreInfo, &QPushButton::clicked, this, &FormHorses::showMoreInfo);
        moreInfo->setProperty("id", horses[i].getId());
        layout->addWidget(name);
        layout->addWidget(sex);
        layout->addWidget(age);
        layout->addWidget(owner);
        layout->addWidget(moreInfo);
        layoutMain->addWidget(item);
    }

    layoutMain->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    ui->scrollArea->setWidget(window);
    FormHorses::show();

}

void FormHorses::showMoreInfo()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestDetailedInfoHorse(QString::number(buttonSender->property("id").toInt()));
}
