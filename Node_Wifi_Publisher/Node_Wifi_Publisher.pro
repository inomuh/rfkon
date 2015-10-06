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
    NodeWifiHandler.cpp \
    DDSEntityManagerKonSens.cpp \
    Timeout.cpp

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
            -L/usr/lib \
            -lboost_filesystem \
            -lboost_system \
            -lboost_regex \



INCLUDEPATH +=  /opt/HDE/x86_64.linux/include /opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ /opt/HDE/x86_64.linux/examples/include \
                /usr/include/boost
DEPENDPATH += /opt/HDE/x86_64.linux/include /opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ /opt/HDE/x86_64.linux/examples/include \
                /usr/include/boost

DISTFILES += \
    Node_Wifi_Publisher.pro.user \
    KonSensData.idl

HEADERS += \
    ccpp_KonSensData.h \
    CheckStatus.h \
    KonSensData.h \
    KonSensDataDcps.h \
    KonSensDataDcps_impl.h \
    KonSensDataSplDcps.h \
    DDSEntityManagerKonSens.h \
    Timeout.h
