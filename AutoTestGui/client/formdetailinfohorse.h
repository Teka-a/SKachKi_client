#ifndef FORMDETAILINFOHORSE_H
#define FORMDETAILINFOHORSE_H

#include <QWidget>
#include <QPushButton>
#include "client.h"
#include "communication.h"

namespace Ui {
class FormDetailInfoHorse;
}

class FormDetailInfoHorse : public QWidget
{
    Q_OBJECT

public:
    explicit FormDetailInfoHorse(QWidget *parent = nullptr);
    ~FormDetailInfoHorse();

private slots:
    void renderInfo(Horse horse, QVector<Contest> contests);
    void showMoreInfo();

private:
    Ui::FormDetailInfoHorse *ui;
};

#endif // FORMDETAILINFOHORSE_H
