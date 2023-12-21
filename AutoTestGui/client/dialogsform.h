#ifndef DIALOGSFORM_H
#define DIALOGSFORM_H

#include <QWidget>

namespace Ui {
class dialogsForm;
}

class dialogsForm : public QWidget
{
    Q_OBJECT

public:
    explicit dialogsForm(QWidget *parent = nullptr);
    ~dialogsForm();

private:
    Ui::dialogsForm *ui;
};

#endif // DIALOGSFORM_H
