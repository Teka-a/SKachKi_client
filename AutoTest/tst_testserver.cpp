#include <QtTest>

// add necessary includes here
#include "./echoServer/database.h"
#include "./echoServer/functions.h"
#include "./echoServer/curve.h"
#include "./echoServer/streebog.h"
#include "./echoServer/kuznechik.h"
#include "./echoServer/formatfunctions.h"

class TestServer : public QObject
{
    Q_OBJECT

public:
    TestServer();
    ~TestServer();
private:
    Functions *f;
    QTcpSocket *socket;
    QList<QTcpSocket*> list;
    QMap<QTcpSocket*, QVector<QString>> sockets;

    QSqlDatabase *db;
    Curve *secp256k1;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_database_connection();
    void test_check_password();
    //test detail info
    void test_jockey_detailed_info();
    void test_horse_detailed_info();
    void test_contest_detailed_info();
    //test reg
    void test_reg_new_participant();
    void test_reg_new_horse();
    void test_reg_new_user();
    void test_reg_new_contest();
    void test_reg_new_place();
    //test crypto parts
    void test_composition_count();
    void test_hash_generation();
    void test_encryption();
};

TestServer::TestServer()
{
    Database *db = Database::getInstance();
    QVector<QString> params {"4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048", "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea", "anonymous"};
    this->sockets[this->socket] = params;

}

TestServer::~TestServer()
{

}

void TestServer::initTestCase()
{
    qDebug() << "tests are started";
}

void TestServer::cleanupTestCase()
{

}
//Test database connection
void TestServer::test_database_connection()
{
    QCOMPARE(Database::getInstance()->isOpen(), true);
}

//Test authorization

void TestServer::test_check_password()
{
    QTcpSocket *socket;
    QMap<QTcpSocket*, QVector<QString>> sockets;
    QVector<QString> params {"4645d95fc0beec2c432f8914b62d4efd3e5e37f14b097aead67de417c220b048", "2492ac996667e0ebdf45d95fc0beec2c432f8914b62d4efd3e5e37f14b097aea", "anonymous"};
    sockets[socket] = params;
    QCOMPARE(this->f->parse("auth&ohno&password&", sockets, socket), "auth&success&owner");
}

//Test request detailed info

void TestServer::test_contest_detailed_info()
{
    QCOMPARE(this->f->parse("requestContest&2", sockets, socket), "contestsDetailedInfo&2&Go, Horse, go!&2023-10-08&16:00:00.000&closed&1&FIREST&Moscow, Pushkinskaya&The best place ever!&Снежок&Puf Paf&00:07:49.000&1&Звездочка&Pif Paf&00:10:15.000&2&-&-&00:00:00&-&-&-&00:00:00&-&-&-&00:00:00&-&-&-&00:00:00&-&-&-&00:00:00&-&");
}


void TestServer::test_jockey_detailed_info()
{
    QCOMPARE(this->f->parse("requestJockey&2", sockets, socket), "jockeyDetailedInfo&2&Do&Do&35&0&no&2024-09-27&14:00:00.000&no&no");
}


void TestServer::test_horse_detailed_info()
{
    QCOMPARE(this->f->parse("requestHorse&2", sockets, socket), "horseDetailedInfo&2&Снежок&Конь&2&Do Do&true&1&First place&2024-08-13&04:05:00.000&FIREST&Зарегистрирован&2&Go, Horse, go!&2023-10-08&16:00:00.000&FIREST&Победитель");
}

//Test register new things

void TestServer::test_reg_new_participant()
{
    QCOMPARE(this->f->parse("regForContest&4&5&Joc", sockets, socket), "registration&success");
    this->f->deleteTestParticipant();
}


void TestServer::test_reg_new_horse()
{
    QCOMPARE(this->f->parse("regNewHorse&Test&Test&5&true&Sat Jan 1 2000&", sockets, socket), "horseRegistration&success");
    this->f->deleteTestHorse();
}


void TestServer::test_reg_new_user()
{
    QCOMPARE(this->f->parse("regNewUser&Test&Test&2&Test&Test&Sat Jan 1 2000&Test&Test&109592412646496367956651201864237637973894672309776077807076095113283485589680&40027922089108786430240755143524622377039268935779570037004788920888510605775&", sockets, socket), "userRegistration&success");
    this->f->deleteTestUser();
}


void TestServer::test_reg_new_contest()
{
    QCOMPARE(this->f->parse("regNewContest&Test&Sat Jan 1 2000&00:00:00&1&Test&Test&", sockets, socket), "contestRegistration&success");
    this->f->deleteTestContest();
}

void TestServer::test_reg_new_place()
{
    QCOMPARE(this->f->parse("regNewPlace&Test&Test&Test", sockets, socket), "placeRegistration&success");
    this->f->deleteTestPlace();
}

//Test cryptogrphy functions

void TestServer::test_composition_count()
{
    Curve *curveSecp256k1 = Curve::getInstance();
    Point test = curveSecp256k1->countComposition(7041);
    Point check;
    check.x = 0;
    check.y = 0;
    uint256_t x {"33377799271553325024409104842395608980167680478077059948369948261639792004383"};
    uint256_t y {"109554916784570073974933658659026726591929811574826193101606868203920878416598"};
    check.x += x;
    check.y += y;
    QCOMPARE(test.x, check.x);
    QCOMPARE(test.y, check.y);
}


void TestServer::test_hash_generation()
{
    QString m = "passsword";
    m = m.toUtf8().toHex();
    QCOMPARE(getHash512(m), "ce7c7826e58f3a4d426c4f7d762272df0632927d8fd24e5d862f168d41a326a5456ca20d0e172a9e8b4220a49ad073ef1869705b0f6cf0eaffd110d40a94cfcc");
}


void TestServer::test_encryption()
{
    QString testPlaintext = "1122334455667700ffeeddccbbaa998800112233445566778899aabbcceeff0a112233445566778899aabbcceeff0a002233445566778899aabbcceeff0a0011";
    QString testKey = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
    QString testVI = "1234567890abcef0a1b2c3d4e5f0011223344556677889901213141516171819";
    QString testCipherText = "81800a59b1842b24ff1f795e897abd95ed5b47a7048cfab48fb521369d9326bf66a257ac3ca0b8b1c80fe7fc10288a13203ebbc066138660a0292243f6903150";
    QCOMPARE(testCipherText, encryptKuznechikOFB(testPlaintext, testKey, testVI));
}

QTEST_APPLESS_MAIN(TestServer)

#include "tst_testserver.moc"
