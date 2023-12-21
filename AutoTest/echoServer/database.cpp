
#include "database.h"

Database * Database::pInstance=0;
DatabaseDestroyer Database::destroyer;

DatabaseDestroyer::~DatabaseDestroyer()
{
    delete pInstance;
}

void DatabaseDestroyer::initialize(Database * p)
{
    pInstance = p;
}

Database * Database::getInstance()
{
    if (!pInstance) {
        pInstance = new Database();
        destroyer.initialize(pInstance);
    }
    return pInstance;
}

Database::Database()
{
    database = QSqlDatabase::addDatabase("QPSQL");
    //database.setHostName("192.168.0.102");
    //database.setPort(5432);
    database.setDatabaseName("server");
    database.setUserName("server");
    database.setPassword("server");

    if (!database.open()){
        qDebug() << "Database is not connected" << database.lastError().text();
    }
    else
        qDebug() << "Database is connected";



}

void Database::closeDatabase()
{
    if (database.open()) {
        database.close();
        qDebug() << "closed";
    }
    else
        qDebug() << "already closed";
}

bool Database::isOpen()
{
    if (database.open()) {
        return true;
    }
    else
        return false;
}

Database::~Database()
{

}
