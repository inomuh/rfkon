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
    DDSEntityManagerKonSens.cpp \
    ServerWifiSubscriber.cpp

DISTFILES += \
    Server_Wifi_Subscriber.pro.user \
    KonSensData.idl

HEADERS += \
    ccpp_KonSensData.h \
    CheckStatus.h \
    KonSensData.h \
    KonSensDataDcps.h \
    KonSensDataDcps_impl.h \
    KonSensDataSplDcps.h \
    DDSEntityManagerKonSens.h


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
            -L/home/bolatu/opt/mongo-cxx-driver/lib \
            -lmongoclient \
            -lboost_thread \
            -lboost_system \
            -lboost_regex\


INCLUDEPATH +=  /home/bolatu/opt/HDE/x86_64.linux/include \
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \
                /home/bolatu/opt/mongo-cxx-driver/include

DEPENDPATH +=   /home/bolatu/opt/HDE/x86_64.linux/include \
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \
                /home/bolatu/opt/mongo-cxx-driver/include

