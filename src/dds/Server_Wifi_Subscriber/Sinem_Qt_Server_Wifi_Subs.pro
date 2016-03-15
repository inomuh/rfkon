TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ServerWifiSubscriber.cpp \
    KonSensDataSplDcps.cpp \
    KonSensDataDcps.cpp \
    KonSensDataDcps_impl.cpp \
    KonSensData.cpp \
    DDSEntityManagerKonSens.cpp \
    CheckStatus.cpp

DISTFILES += \
    Sinem_Qt_Server_Wifi_Subs.pro.user \
    KonSensData.idl

HEADERS += \
    KonSensDataSplDcps.h \
    KonSensDataDcps.h \
    KonSensDataDcps_impl.h \
    KonSensData.h \
    DDSEntityManagerKonSens.h \
    CheckStatus.h \
    ccpp_KonSensData.h

LIBS +=     -L/opt/HDE/x86_64.linux/lib \
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
            -L/opt/mongodb/lib \
            -lmongoclient \
            -lboost_thread \
            -lboost_system \
            -lboost_regex \

INCLUDEPATH +=  /opt/HDE/x86_64.linux/include /opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ /opt/HDE/x86_64.linux/examples/include \
                /opt/mongodb/include
DEPENDPATH += /opt/HDE/x86_64.linux/include /opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ /opt/HDE/x86_64.linux/examples/include \
                /opt/mongodb/include
