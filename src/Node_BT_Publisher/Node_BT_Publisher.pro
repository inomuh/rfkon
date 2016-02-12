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
    NodeBtHandler.cpp \
    DDSEntityManagerKonSens.cpp \
    Timeout.cpp

DISTFILES += \
    Node_BT_Publisher.pro.user \
    KonSensData.idl

HEADERS += \
    ccpp_KonSensData.h \
    CheckStatus.h \
    KonSensData.h \
    KonSensDataDcps.h \
    KonSensDataDcps_impl.h \
    KonSensDataSplDcps.h \
    DDSEntityManagerKonSens.h \
    NodeBtHandler.cpp.autosave \
    Timeout.h \
    DDSEntityManagerKonSens.h.autosave

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
                /usr/include/boost

DEPENDPATH +=   /home/bolatu/opt/HDE/x86_64.linux/include \
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \
                /usr/include/boost
