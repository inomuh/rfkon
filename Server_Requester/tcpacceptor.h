/*
 * TcpAcceptor.h
 *
 * GLOBAL DEĞİŞKENLER:
 *  Yok
 *
 * ENUM KULLANIMI:
 *  Yok
 *
 * FONKSİYON PROTOTİPLERİ:
 *   int        start();
 *   TcpStream* accept();
 *
 * AMAÇ:
 *  TcpAcceptor sınıfı, TCP/IP soket mekanizmasını kapsülleyerek PASİF bir
 *  şekilde istemciden bağlantıyı kabul eder. Bu sınıf ayrıca, sunucu uygulaması
 *  istemci uygulaması ile başarılı bir bağlantı kurarsa TcpStream nesnelerini
 *  üretir.
 *
 * NOTLAR:
 *  Yazar: Uğur Bolat
 *  Tarih: 15.10.2015
 *
 *   Versiyon: v_1.0
 *    Güncelleme Tarihi: 15.10.2015
 *
 */

#ifndef __tcpacceptor_h__
#define __tcpacceptor_h__

#include <string>
#include <netinet/in.h>
#include "tcpstream.h"

using namespace std;

class TcpAcceptor
{
public:
  TcpAcceptor(int port, const char* address="");
  ~TcpAcceptor();

  int        start();
  TcpStream* accept();

private:
  TcpAcceptor() {}

  int    lsd_;
  int    port_;
  string address_;
  bool   listening_;

};

#endif
