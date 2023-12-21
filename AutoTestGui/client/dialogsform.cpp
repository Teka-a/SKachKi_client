#include "dialogsform.h"
#include "ui_dialogsform.h"

dialogsForm::dialogsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialogsForm)
{
    ui->setupUi(this);
}

dialogsForm::~dialogsForm()
{
    delete ui;
}
