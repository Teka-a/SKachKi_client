#include "formnoconnection.h"
#include "ui_formnoconnection.h"

FormNoConnection::FormNoConnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNoConnection)
{
    ui->setupUi(this);
    this->setWindowTitle("SKachKi");
    ui->mainName->setStyleSheet("QLabel { color : rgb(255, 218, 185); }");
}

FormNoConnection::~FormNoConnection()
{
    delete ui;
}
