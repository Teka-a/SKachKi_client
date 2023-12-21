#ifndef FORMCONTESTSADDINFO_H
#define FORMCONTESTSADDINFO_H

#include <QWidget>
#include <QPushButton>
#include "client.h"
#include "communication.h"
#include "formaddinfo.h"
namespace Ui {
class FormContestsAddInfo;
}

class FormContestsAddInfo : public QWidget
{
    Q_OBJECT

public:
    explicit FormContestsAddInfo(QWidget *parent = nullptr);
    ~FormContestsAddInfo();
private slots:
    void setInfo(QVector<Contest> contests);
    void showMoreInfo();
private:
    Ui::FormContestsAddInfo *ui;
    FormAddInfo *addInfoWindow;
};

#endif // FORMCONTESTSADDINFO_HaddInfoWindow
