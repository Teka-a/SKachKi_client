#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "client.h"
#include "authform.h"
#include "formdetailinfo.h"
#include "formcontests.h"
#include "formhorses.h"
#include "formjockeys.h"
#include "formadminaccount.h"
#include "formjockeyaccount.h"
#include "formowneraccount.h"
#include "formdetailinfohorse.h"
#include "formdetailinfojockey.h"
#include "formcontestsaddinfo.h"
namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();
private slots:
    void anons();
    void renderInfo3Contests(QVector<Contest> contests);
    void on_centralContestMoreInfo_clicked();


    void on_contestsButton_clicked();

    void on_accountButton_clicked();

    void on_leftContestMoreInfo_clicked();

    void on_rightContestMoreInfo_clicked();

    void on_horsesButton_clicked();

    void on_jockeysButton_clicked();

private:
    Ui::MainForm *ui;
    AuthForm *authForm;
    FormDetailInfo *infoContestWindow;
    FormContests *contestsWindow;
    FormHorses *horsesWindow;
    FormJockeys *jockeysWindow;
    FormAdminAccount *adminPanel;
    FormJockeyAccount *jockeyPanel;
    FormOwnerAccount *ownerPanel;
    FormDetailInfoHorse *infoHorseWindow;
    FormDetailInfoJockey *infoJockeyWindow;

    FormContestsAddInfo *addInfoWindow;
    void setNoInfoRight();
    void setNoInfoCentral();
    void setNoInfoLeft();

    void setInfoRight(Contest contest);
    void setInfoCentral(Contest contest);
    void setInfoLeft(Contest contest);
};

#endif // MAINFORM_H
