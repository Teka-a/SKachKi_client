#ifndef FORMHORSES_H
#define FORMHORSES_H

#include <QWidget>
#include <QPushButton>
#include "client.h"
#include "communication.h"

namespace Ui {
class FormHorses;
}

class FormHorses : public QWidget
{
    Q_OBJECT

public:
    explicit FormHorses(QWidget *parent = nullptr);
    ~FormHorses();

private slots:
    void setInfo(QVector<Horse> horses);
    void showMoreInfo();

private:
    Ui::FormHorses *ui;
};

#endif // FORMHORSES_H
