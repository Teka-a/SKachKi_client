#ifndef FORMREGCONTEST_H
#define FORMREGCONTEST_H

#include <QWidget>
#include "client.h"
#include "communication.h"


namespace Ui {
class FormRegContest;
}

class FormRegContest : public QWidget
{
    Q_OBJECT

public:
    explicit FormRegContest(QWidget *parent = nullptr);
    ~FormRegContest();

private slots:
    void setInfoHorses(QString contestId, QString contestName, QVector<Horse> horsesList);
    void on_regButton_clicked();

    void on_horsesBox_activated(int index);

private:
    Ui::FormRegContest *ui;
    int horseSelectedId = -1;
    int contestId = -1;
    QVector<Horse> horses {Horse("1", "HorseDefault", "HorseDefault", "HorseDefault", "HorseDefault", "t"), Horse("1", "HorseDefault", "HorseDefault", "HorseDefault", "HorseDefault", "t")};
};

#endif // FORMREGCONTEST_H
