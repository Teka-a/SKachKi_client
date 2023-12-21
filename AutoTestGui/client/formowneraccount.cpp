#include "formowneraccount.h"
#include "ui_formowneraccount.h"

FormOwnerAccount::FormOwnerAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormOwnerAccount)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->label->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    reset();
    ui->nameLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->sexLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->age->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
    ui->availableLabel->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");

    ui->regResult->setText("");
    ui->availableChangeRes->setText("");

    connect(Client::getInstance(), &Client::receivedOwnerForAccount, this, &FormOwnerAccount::setInfo);
    connect(Client::getInstance(), &Client::registrationHorseStatus, this, &FormOwnerAccount::receivedRegStatus);
    connect(Client::getInstance(), &Client::changeHorseStatus, this, &FormOwnerAccount::receivedChangeStatus);
}

FormOwnerAccount::~FormOwnerAccount()
{
    delete ui;
}


void FormOwnerAccount::receivedRegStatus(QString status)
{
    reset();
    if (status == "s")
        ui->regResult->setText("Лошадь успешно зарегистрирована. Обновите страницу.");
    else if (status == "f")
        ui->regResult->setText("Лошадь не была зарегистрирована ввиду загадочных обстоятельств(");

}


void FormOwnerAccount::receivedChangeStatus(QString status)
{
    reset();
    if (status == "s")
        ui->availableChangeRes->setText("Изменения внесены. Обновите страницу.");
    else if (status == "f")
        ui->availableChangeRes->setText("Изменения не внесены ввиду загадочных обстоятельств(");


}


void FormOwnerAccount::reset()
{
    ui->enterName->clear();
    ui->availableChangeRes->clear();
    ui->regResult->setText("");
    ui->availableChangeRes->setText("");
    ui->enterDateBirth->setDate(QDate::fromString("2000-01-01", "yyyy-MM-dd"));
    ui->sex_2->setCurrentIndex(0);
    ui->isAvailable->setChecked(false);

}

void FormOwnerAccount::setInfo(QString ownerId, QString ownerWelcome, QVector<Horse> horses)
{
    reset();
    qDebug() << "here";
    ui->welcome->setText("Добро пожаловать, " + ownerWelcome + "!");
    ui->welcome->setAlignment(Qt::AlignCenter);
    this->ownerId = ownerId;
    QWidget *window = new QWidget;

    QVBoxLayout *layoutMain = new QVBoxLayout(window);
    if (horses[0].getName() == "no") {
        QLabel *item = new QLabel("Нет информации");
        item->setAlignment(Qt::AlignCenter);
        layoutMain->addWidget(item);
    }
    else {
        for (int i = 0; i < horses.size(); ++i) {
            QWidget *item = new QWidget;
            QHBoxLayout *layout = new QHBoxLayout(item);

            QLabel *name = new QLabel(horses[i].getName());
            QLabel *age = new QLabel(horses[i].getAge());
            QLabel *sex = new QLabel(horses[i].getSex());
            QLabel *available = new QLabel(horses[i].getAvailability());
            QPushButton *changeAvailable = new QPushButton("Изменить");
            qDebug() << "horse" << horses[i].getAvailability();
            if (horses[i].getAvailability() == "да") {
                connect(changeAvailable, &QPushButton::clicked, this, &FormOwnerAccount::changeAvailabilityFalse);

                changeAvailable->setProperty("id", horses[i].getId());
            }
            else {
                connect(changeAvailable, &QPushButton::clicked, this, &FormOwnerAccount::changeAvailabilityTrue);
                changeAvailable->setProperty("id", horses[i].getId());
            }

            layout->addWidget(name);
            layout->addWidget(sex);
            layout->addWidget(age);
            layout->addWidget(available);
            layout->addWidget(changeAvailable);
            layoutMain->addWidget(item);
        }
    }
    layoutMain->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    ui->scrollArea->setWidget(window);
    FormOwnerAccount::show();
}


void FormOwnerAccount::changeAvailabilityTrue()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << "changeAvailabilityTrue" << buttonSender->property("id");
    changeHorseAvailability(QString::number(buttonSender->property("id").toInt()), "true");
}


void FormOwnerAccount::changeAvailabilityFalse()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    qDebug() << "changeAvailabilityFalse" << buttonSender->property("id");
    changeHorseAvailability(QString::number(buttonSender->property("id").toInt()), "false");
}

void FormOwnerAccount::on_pushButton_clicked()
{
    QString available = ui->isAvailable->isChecked() ? "true" : "false";
    regNewHorse(this->ownerId,
                ui->enterName->text(),
                ui->sex_2->currentText(),
                ui->enterDateBirth->date().toString(),
                available);
}

