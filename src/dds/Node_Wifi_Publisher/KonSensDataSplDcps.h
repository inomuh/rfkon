#ifndef KONSENSDATASPLTYPES_H
#define KONSENSDATASPLTYPES_H

#include "ccpp_KonSensData.h"

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>

extern c_metaObject __KonSensData_KonSensData__load (c_base base);

extern c_metaObject __KonSensData_Msg__load (c_base base);
extern const char * __KonSensData_Msg__keys (void);
extern const char * __KonSensData_Msg__name (void);
struct _KonSensData_Msg ;
extern  c_bool __KonSensData_Msg__copyIn(c_base base, struct KonSensData::Msg *from, struct _KonSensData_Msg *to);
extern  void __KonSensData_Msg__copyOut(void *_from, void *_to);
struct _KonSensData_Msg {
    c_string devID;
    c_string hostName;
    c_long dbm;
};

extern c_metaObject __KonSensData_MesSeq__load (c_base base);
typedef c_sequence _KonSensData_MesSeq;

extern c_metaObject __KonSensData_WifiSeq__load (c_base base);
extern const char * __KonSensData_WifiSeq__keys (void);
extern const char * __KonSensData_WifiSeq__name (void);
struct _KonSensData_WifiSeq ;
extern  c_bool __KonSensData_WifiSeq__copyIn(c_base base, struct KonSensData::WifiSeq *from, struct _KonSensData_WifiSeq *to);
extern  void __KonSensData_WifiSeq__copyOut(void *_from, void *_to);
struct _KonSensData_WifiSeq {
    c_long timestamp[2][2];
    c_long userID;
    c_long messageID;
    _KonSensData_MesSeq messages;
};

extern c_metaObject __KonSensData_BtSeq__load (c_base base);
extern const char * __KonSensData_BtSeq__keys (void);
extern const char * __KonSensData_BtSeq__name (void);
struct _KonSensData_BtSeq ;
extern  c_bool __KonSensData_BtSeq__copyIn(c_base base, struct KonSensData::BtSeq *from, struct _KonSensData_BtSeq *to);
extern  void __KonSensData_BtSeq__copyOut(void *_from, void *_to);
struct _KonSensData_BtSeq {
    c_long timestamp[2][2];
    c_long userID;
    c_long messageID;
    _KonSensData_MesSeq messages;
};

extern c_metaObject __KonSensData_ServerReq__load (c_base base);
extern const char * __KonSensData_ServerReq__keys (void);
extern const char * __KonSensData_ServerReq__name (void);
struct _KonSensData_ServerReq ;
extern  c_bool __KonSensData_ServerReq__copyIn(c_base base, struct KonSensData::ServerReq *from, struct _KonSensData_ServerReq *to);
extern  void __KonSensData_ServerReq__copyOut(void *_from, void *_to);
struct _KonSensData_ServerReq {
    c_string request;
    c_long requestID;
};

#endif
