TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    CheckStatus.cpp \
    KonSensData.cpp \
    KonSensDataDcps.cpp \
    KonSensDataDcps_impl.cpp \
    KonSensDataSplDcps.cpp \
    tcpacceptor.cpp \
    DDSEntityManagerKonSens.cpp \
    tcpconnector.cpp \
    tcpstream.cpp \
    ServerRequester.cpp \
    Timeout.cpp

DISTFILES += \
    Server_Requester.pro.user \
    KonSensData.idl

HEADERS += \
    ccpp_KonSensData.h \
    CheckStatus.h \
    KonSensData.h \
    KonSensDataDcps.h \
    KonSensDataDcps_impl.h \
    KonSensDataSplDcps.h \
    tcpacceptor.h \
    DDSEntityManagerKonSens.h \
    tcpconnector.h \
    tcpstream.h \
    Timeout.h

LIBS +=     -L/home/bolatu/opt/HDE/x86_64.linux/lib \
            -lddsserialization \
            -lddskernel \
            -lddsutil \
            -lddsdatabase \
            -lddsuser \
            -lddsconf \
            -lddsconfparser \
            -ldcpsgapi \
            -ldcpssacpp \
            -lddsos \
            -L/usr/lib \
            -lboost_filesystem \
            -lboost_system \
            -lboost_regex \



INCLUDEPATH +=  /home/bolatu/opt/HDE/x86_64.linux/include \
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \

DEPENDPATH +=   /home/bolatu/opt/HDE/x86_64.linux/include \
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \
