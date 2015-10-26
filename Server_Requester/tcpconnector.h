/*
 * tcpconnector.h
 *
 * GLOBAL DEĞİŞKENLER:
 *  Yok
 *
 * ENUM KULLANIMI:
 *  Yok
 *
 * FONKSİYON PROTOTİPLERİ:
 *  TCPStream* connect(const char* server, int port);
 *  TCPStream* connect(const char* server, int port, int timeout);
 *
 * AMAÇ:
 *  TcpConnector sınıfı, TCP/IP soket mekanizmasını kapsülleyerek AKTİF bir
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

#ifndef __tcpconnector_h__
#define __tcpconnector_h__

#include <netinet/in.h>
#include "tcpstream.h"

class TcpConnector
{
public:
  TcpStream* connect(const char* server, int port);
  TcpStream* connect(const char* server, int port, int timeout);

private:
  int resolveHostName(const char* host, struct in_addr* addr);
};

#endif
