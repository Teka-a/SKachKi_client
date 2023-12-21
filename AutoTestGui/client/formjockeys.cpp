#include "formjockeys.h"
#include "ui_formjockeys.h"

FormJockeys::FormJockeys(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormJockeys)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->name->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->surname->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->age->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->p1->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->p2->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->p3->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->moreInfoCol->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    connect(Client::getInstance(), &Client::receivedJockeys, this, &FormJockeys::setInfo);
}

FormJockeys::~FormJockeys()
{
    delete ui;
}

void FormJockeys::setInfo(QVector<Jockey> jockeys)
{
    QWidget *window = new QWidget;

    QVBoxLayout *layoutMain = new QVBoxLayout(window);

    for (int i = 0; i < jockeys.size(); ++i) {
        QWidget *item = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout(item);

        QLabel *name = new QLabel(jockeys[i].getName());
        QLabel *surname = new QLabel(jockeys[i].getSurname());
        QLabel *age = new QLabel(jockeys[i].getAge());
        QLabel *p1 = new QLabel(jockeys[i].getPlaces1());
        QLabel *p2 = new QLabel(jockeys[i].getPlaces2());
        QLabel *p3 = new QLabel(jockeys[i].getPlaces3());
        QPushButton *moreInfo = new QPushButton("Подробнее");
        connect(moreInfo, &QPushButton::clicked, this, &FormJockeys::showMoreInfo);
        moreInfo->setProperty("id", jockeys[i].getId());
        layout->addWidget(name);
        layout->addWidget(surname);
        layout->addWidget(age);
        layout->addWidget(p1);
        layout->addWidget(p2);
        layout->addWidget(p3);
        layout->addWidget(moreInfo);
        layoutMain->addWidget(item);
    }

    layoutMain->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    ui->scrollArea->setWidget(window);
    FormJockeys::show();

}

void FormJockeys::showMoreInfo()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << buttonSender->property("id");
    requestDetailedInfoJockey(QString::number(buttonSender->property("id").toInt()));
}
