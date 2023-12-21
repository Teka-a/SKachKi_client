#include "mytcpserver.h"

Curve * Curve::pInstance=0;
CurveDestroyer Curve::destroyer;
int Curve::a;
int Curve::b;
uint256_t Curve::p;
Point Curve::G;
Point Curve::O;

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
}

/*конструктор для запуска сервера*/
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 54345)) {
        qDebug() << "SKachKi server is not started";
    } else {
        //server_status=1;
        //Functions f;
        //qDebug() << f.parse("setInfo&11&2&00:10:00&12&1&00:07:06&10&3&00:18:05&", sockets, mTcpSocket);

        qDebug() << "SKachKi server is started";
        //Curve *curveSecp256k1 = Curve::getInstance();
        //Database *db = Database::getInstance();
        //Point i = curveSecp256k1->countComposition(7041);
        //qDebug() << "Here mullt: " << QString::fromStdString(i.x.str()) << " " << QString::fromStdString(i.y.str());
        //QString m = "lisalisa";
        //m = m.toUtf8().toHex();
        //qDebug() << "Hash" << getHash512(m);

    }
}


/*создание слота для нового подключения*/
void MyTcpServer::slotNewConnection()
{
    QTcpSocket* mTcpSocket = mTcpServer->nextPendingConnection();
    list.push_back(mTcpSocket);
    QVector<QString> params {"4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048", "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea", "anonymous"};
    sockets[mTcpSocket] = params;
    qDebug() << mTcpSocket;
    //mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
    connect(mTcpSocket, &QTcpSocket::readyRead,this,&MyTcpServer::slotServerRead);
    connect(mTcpSocket,&QTcpSocket::disconnected,this,&MyTcpServer::slotClientDisconnected);

}

/*чтение строки от пользователя*/
void MyTcpServer::slotServerRead()
{
    Functions f;
    QTcpSocket* mTcpSocket = (QTcpSocket*)sender();
    //qDebug() << mTcpSocket;
    QVector<QString> params = sockets[mTcpSocket];
    QString res = "";
    while (mTcpSocket->bytesAvailable()>0) {
        QByteArray array = mTcpSocket->readAll();
        res.append(array);
    }
    QString array = decryptKuznechikOFB(res, params[0], params[1]);
    QByteArray hex = QByteArray::fromHex(array.toUtf8());
    array = hex.data();

    res = f.parse(array, sockets, mTcpSocket);
    QStringList list = res.split("&", Qt::SkipEmptyParts);
    res = encryptKuznechikOFB(res.toUtf8().toHex(), params[0], params[1]);
    if (list[1] == "failed") {
        qDebug() << "Params have been set to default";
        params = {"4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048", "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea", "anonymous"};
        sockets[mTcpSocket] = params;
    }
    //qDebug() << "Encrypted" << res;
    mTcpSocket->write(res.toUtf8());

}

/*возможность подключения нескольких клиентов*/
void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* mTcpSocket = (QTcpSocket*)sender();
    //qDebug() << "dicsonnected";
    list.removeAt(list.indexOf(mTcpSocket));
    sockets.remove(mTcpSocket);
    mTcpSocket->close();
    qDebug() << sockets.size();
}
