#ifndef INFOCONTEST_H
#define INFOCONTEST_H

#include <QMainWindow>
#include "client.h"

namespace Ui {
class InfoContest;
}

class InfoContest : public QMainWindow
{
    Q_OBJECT

public:
    explicit InfoContest(QWidget *parent = nullptr);
    ~InfoContest();

private:
    Ui::InfoContest *ui;

private slots:
    void setDetailedInfo(Contest contest, Place hippodrome, QVector<ParticipantPair> participants);
};

#endif // INFOCONTEST_H
