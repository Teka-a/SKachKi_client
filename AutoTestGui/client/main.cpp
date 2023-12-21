#include "mainform.h"
#include "dialogsform.h"
#include "client.h"
#include "curve.h"
#include <QApplication>
#include <QStyleFactory>
#include "formnoconnection.h"

Client * Client::pInstance = 0;
ClientDestroyer Client::destroyer;
QTcpSocket * Client::mTcpSocket;
Curve * Curve::pInstance=0;
CurveDestroyer Curve::destroyer;
int Curve::a;
int Curve::b;
uint256_t Curve::p;
Point Curve::G;
Point Curve::O;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(194, 165, 140));
    darkPalette.setColor(QPalette::Disabled, QPalette::Button, QColor(105, 105, 105));
    darkPalette.setColor(QPalette::ButtonText, Qt::black);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(255, 218, 185));
    darkPalette.setColor(QPalette::Highlight, QColor(255, 218, 185));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    QTcpSocket *mTcpSocket = new QTcpSocket();
    mTcpSocket->connectToHost("192.168.0.5", 54345);
    bool connected = mTcpSocket->waitForConnected();
    mTcpSocket->close();
    if (!connected) {
        FormNoConnection *f = new FormNoConnection;
        f->show();
        return app.exec();
    }
    else {
        Client *cl = Client::getInstance();
        MainForm window;
        return app.exec();
    }
    //Client *cl = Client::getInstance();
    //MainForm window;
    //qDebug() << "end";

}
