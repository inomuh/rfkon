/*
 * CheckStatus.h
 *
 * AMAÇ:
 *  DDS konfigürasyon ayarları yapılırken kullanılan fonksiyonlar bir status
 *  döndürür. CheckStatus, bu statusleri kontrol eden fonksiyonları içerir.
 *
 * NOTLAR:
 *  Yazar: Uğur Bolat
 *  Tarih: 15.10.2015
 *
 *   Versiyon: v_1.0
 *    Güncelleme Tarihi: 15.10.2015
 *
 */

#ifndef __CHECKSTATUS_H__
#define __CHECKSTATUS_H__

#include "ccpp_dds_dcps.h"
#include <iostream>

using namespace std;

// Returns the name of an error code.
string getErrorName(DDS::ReturnCode_t status);

// Check the return status for errors. If there is an error, then terminate.
void checkStatus(DDS::ReturnCode_t status, const char *info);

// Check whether a valid handle has been returned. If not, then terminate.
void checkHandle(void *handle, string info);

#endif
