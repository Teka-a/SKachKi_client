#ifndef FORMDETAILINFOJOCKEY_H
#define FORMDETAILINFOJOCKEY_H

#include <QWidget>
#include <QPushButton>
#include "client.h"
#include "communication.h"

namespace Ui {
class FormDetailInfoJockey;
}

class FormDetailInfoJockey : public QWidget
{
    Q_OBJECT

public:
    explicit FormDetailInfoJockey(QWidget *parent = nullptr);
    ~FormDetailInfoJockey();

private slots:
    void renderInfo(Jockey jockey, QVector<Contest> contests);
    void showMoreInfo();

private:
    Ui::FormDetailInfoJockey *ui;
};

#endif // FORMDETAILINFOJOCKEY_H
