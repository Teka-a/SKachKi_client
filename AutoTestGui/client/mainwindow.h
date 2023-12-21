#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include "authform.h"
#include "infocontest.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void anons();
    void renderInfo3Contests(QVector<Contest> contests);
    void on_centralContestMoreInfo_clicked();

private:
    Ui::MainWindow *ui;
    AuthForm *authForm;
    InfoContest *infoContestWindow;
    void setNoInfoRight();
    void setNoInfoCentral();
    void setNoInfoLeft();

    void setInfoRight(Contest contest);
    void setInfoCentral(Contest contest);
    void setInfoLeft(Contest contest);

};
#endif // MAINWINDOW_H
