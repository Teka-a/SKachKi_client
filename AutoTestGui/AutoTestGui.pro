QT += testlib
QT += core gui
QT += widgets network
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_testclient.cpp \
    ./client/authform.cpp \
    ./client/client.cpp \
    ./client/communication.cpp \
    ./client/contest.cpp \
    ./client/curve.cpp \
    ./client/formaddinfo.cpp \
    ./client/formadminaccount.cpp \
    ./client/formcontests.cpp \
    ./client/formcontestsaddinfo.cpp \
    ./client/formdetailinfo.cpp \
    ./client/formdetailinfohorse.cpp \
    ./client/formdetailinfojockey.cpp \
    ./client/formhorses.cpp \
    ./client/formjockeyaccount.cpp \
    ./client/formjockeys.cpp \
    ./client/formnoconnection.cpp \
    ./client/formowneraccount.cpp \
    ./client/formregcontest.cpp \
    ./client/horse.cpp \
    ./client/jockey.cpp \
    ./client/kuznechik.cpp \
    ./client/mainform.cpp \
    ./client/mqv.cpp \
    ./client/place.cpp

HEADERS += ./client/authform.h \
    ./client/client.h \
    ./client/communication.h \
    ./client/contest.h \
    ./client/curve.h \
    ./client/formaddinfo.h \
    ./client/formadminaccount.h \
    ./client/formcontests.h \
    ./client/formcontestsaddinfo.h \
    ./client/formdetailinfo.h \
    ./client/formdetailinfohorse.h \
    ./client/formdetailinfojockey.h \
    ./client/formhorses.h \
    ./client/formjockeyaccount.h \
    ./client/formjockeys.h \
    ./client/formnoconnection.h \
    ./client/formowneraccount.h \
    ./client/formregcontest.h \
    ./client/horse.h \
    ./client/jockey.h \
    ./client/kuznechik.h \
    ./client/mainform.h \
    ./client/mqv.h \
    ./client/place.h

FORMS += ./client/authform.ui \
    ./client/formaddinfo.ui \
    ./client/formadminaccount.ui \
    ./client/formcontests.ui \
    ./client/formcontestsaddinfo.ui \
    ./client/formdetailinfo.ui \
    ./client/formdetailinfohorse.ui \
    ./client/formdetailinfojockey.ui \
    ./client/formhorses.ui \
    ./client/formjockeyaccount.ui \
    ./client/formjockeys.ui \
    ./client/formnoconnection.ui \
    ./client/formowneraccount.ui \
    ./client/formregcontest.ui \
    ./client/mainform.ui

