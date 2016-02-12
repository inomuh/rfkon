/*
 * CheckStatus.cpp
 *
 * GLOBAL DEĞİŞKENLER:
 *  Yok
 *
 * ENUM KULLANIMI:
 *  Yok
 *
 * FONKSİYON PROTOTİPLERİ:
 *  string getErrorName(DDS::ReturnCode_t status);
 *  void checkStatus(DDS::ReturnCode_t status, const char *info);
 *  void checkHandle(void *handle, string info);
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

#include "CheckStatus.h"

// Array to hold the names for all ReturnCodes.
string RetCodeName[13] = 
{
  "DDS_RETCODE_OK", "DDS_RETCODE_ERROR", "DDS_RETCODE_UNSUPPORTED",
  "DDS_RETCODE_BAD_PARAMETER", "DDS_RETCODE_PRECONDITION_NOT_MET",
  "DDS_RETCODE_OUT_OF_RESOURCES", "DDS_RETCODE_NOT_ENABLED",
  "DDS_RETCODE_IMMUTABLE_POLICY", "DDS_RETCODE_INCONSISTENT_POLICY",
  "DDS_RETCODE_ALREADY_DELETED", "DDS_RETCODE_TIMEOUT", "DDS_RETCODE_NO_DATA",
  "DDS_RETCODE_ILLEGAL_OPERATION"
};

// Returns the name of an error code.
string getErrorName(DDS::ReturnCode_t status)
{
  return RetCodeName[status];
}

// Check the return status for errors. If there is an error, then terminate.
void checkStatus(DDS::ReturnCode_t status, const char *info)
{
  if (status != DDS::RETCODE_OK && status != DDS::RETCODE_NO_DATA)
  {
    cerr << "Error in " << info << ": " << getErrorName(status).c_str() << endl;
    exit(1);
  }
}

// Check whether a valid handle has been returned. If not, then terminate.
void checkHandle(void *handle, string info)
{
  if (!handle)
  {
    cerr << "Error in " << info.c_str() << ": Creation failed: invalid handle" << endl;
    exit(1);
  }
}
