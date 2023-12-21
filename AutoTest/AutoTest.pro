QT += testlib sql network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testserver.cpp \
    ./echoServer/database.cpp \
    ./echoServer/functions.cpp \
    ./echoServer/mytcpserver.cpp \
    ./echoServer/curve.cpp \
    ./echoServer/kuznechik.cpp \
    ./echoServer/streebog.cpp \
    ./echoServer/formatfunctions.cpp

HEADERS += \
    ./echoServer/database.h \
    ./echoServer/functions.h \
    ./echoServer/mytcpserver.h \
    ./echoServer/curve.h \
    ./echoServer/kuznechik.h \
    ./echoServer/streebog.h \
    ./echoServer/formatfunctions.h

