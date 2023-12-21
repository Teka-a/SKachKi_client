#ifndef FORMDETAILINFO_H
#define FORMDETAILINFO_H

#include <QWidget>
#include "client.h"
#include "communication.h"
#include "formregcontest.h"

namespace Ui {
class FormDetailInfo;
}

class FormDetailInfo : public QWidget
{
    Q_OBJECT

public:
    explicit FormDetailInfo(QWidget *parent = nullptr);
    ~FormDetailInfo();

private:
    Ui::FormDetailInfo *ui;
    FormRegContest *regForm;
    QString contestId = "0";

private slots:
    void setDetailedInfo(Contest contest, Place hippodrome, QVector<ParticipantPair> participants);

    void on_regButton_clicked();
};

#endif // FORMDETAILINFO_H
