#ifndef FORMJOCKEYS_H
#define FORMJOCKEYS_H

#include <QWidget>
#include <QPushButton>
#include "client.h"
#include "communication.h"

namespace Ui {
class FormJockeys;
}

class FormJockeys : public QWidget
{
    Q_OBJECT

public:
    explicit FormJockeys(QWidget *parent = nullptr);
    ~FormJockeys();
private slots:
    void setInfo(QVector<Jockey> jockeys);
    void showMoreInfo();

private:
    Ui::FormJockeys *ui;
};

#endif // FORMJOCKEYS_H
