#ifndef FORMADDINFO_H
#define FORMADDINFO_H

#include <QWidget>
#include "client.h"
#include "communication.h"

namespace Ui {
class FormAddInfo;
}

class FormAddInfo : public QWidget
{
    Q_OBJECT

public:
    explicit FormAddInfo(QWidget *parent = nullptr);
    ~FormAddInfo();
private slots:
    void setInfo(QString contName, QVector<QVector<QString>> participants);
    void on_pushButton_clicked();
    void receivedAddStatus(QString status);
private:
    Ui::FormAddInfo *ui;
    int totalCountOfPaitrs = 0;
    //QPair
    QVector<QVector<QString>> participantsInfo {{"no"}};
    void setPair(int num);
    void reset();
    QPair<QTime, QString> getInfoAboutPair(int num);
    QTime getTime(int num);
    QVector<QTime> sortTime();
};

#endif // FORMADDINFO_H
