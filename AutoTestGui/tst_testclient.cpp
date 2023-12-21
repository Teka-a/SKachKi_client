#include <QtTest>
#include <QLabel>

#include <QCoreApplication>

// add necessary includes here
#include "./client/authform.h"
#include "./client/mainform.h"
#include "./client/formnoconnection.h"
#include "./client/kuznechik.h"
#include "./client/curve.h"

class TestClient : public QObject
{
    Q_OBJECT

public:
    TestClient();
    ~TestClient();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case_failed();
    void test_composition_count();
    void test_encryption();

private:
    FormNoConnection *f;
};

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

TestClient::TestClient()
{

}

TestClient::~TestClient()
{

}

void TestClient::initTestCase()
{

}

void TestClient::cleanupTestCase()
{

    f->close();
}

void TestClient::test_case_failed()
{
    f = new FormNoConnection;
    f->show();
    QVERIFY2(f->findChild<QLabel *>("label"), "Not opened");
}

void TestClient::test_composition_count()
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


void TestClient::test_encryption()
{
    QString testPlaintext = "1122334455667700ffeeddccbbaa998800112233445566778899aabbcceeff0a112233445566778899aabbcceeff0a002233445566778899aabbcceeff0a0011";
    QString testKey = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
    QString testVI = "1234567890abcef0a1b2c3d4e5f0011223344556677889901213141516171819";
    QString testCipherText = "81800a59b1842b24ff1f795e897abd95ed5b47a7048cfab48fb521369d9326bf66a257ac3ca0b8b1c80fe7fc10288a13203ebbc066138660a0292243f6903150";
    QCOMPARE(testCipherText, encryptKuznechikOFB(testPlaintext, testKey, testVI));
}


QTEST_MAIN(TestClient)

#include "tst_testclient.moc"
