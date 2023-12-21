#include "infocontest.h"
#include "ui_infocontest.h"


InfoContest::InfoContest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InfoContest)
{
    ui->setupUi(this);

    connect(Client::getInstance(), &Client::receivedContest, this, &InfoContest::setDetailedInfo);
}

InfoContest::~InfoContest()
{
    delete ui;
}

void InfoContest::setDetailedInfo(Contest contest, Place hippodrome, QVector<ParticipantPair> participants)
{
    qDebug() << contest.getFullDate() << contest.getFullTime();
    qDebug() << hippodrome.getName();
    qDebug() << participants[0].horseName;
    InfoContest::show();
}
