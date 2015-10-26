/*
 * tcpacceptor.cpp
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

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "tcpacceptor.h"

TcpAcceptor::TcpAcceptor(int port, const char* address)
  : lsd_(0), port_(port), address_(address), listening_(false) {}

TcpAcceptor::~TcpAcceptor()
{
  if (lsd_ > 0) {
    close(lsd_);
  }
}

int TcpAcceptor::start()
{
  if (listening_ == true) {
    return 0;
  }

  lsd_ = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in address;

  memset(&address, 0, sizeof(address));
  address.sin_family = PF_INET;
  address.sin_port = htons(port_);
  if (address_.size() > 0) {
    inet_pton(PF_INET, address_.c_str(), &(address.sin_addr));
  }
  else {
    address.sin_addr.s_addr = INADDR_ANY;
  }

  int optval = 1;
  setsockopt(lsd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

  int result = bind(lsd_, (struct sockaddr*)&address, sizeof(address));
  if (result != 0) {
    perror("bind() failed");
    return result;
  }

  result = listen(lsd_, 5);
  if (result != 0) {
    perror("listen() failed");
    return result;
  }
  listening_ = true;
  return result;
}

TcpStream* TcpAcceptor::accept()
{
  if (listening_ == false) {
    return NULL;
  }

  struct sockaddr_in address;
  socklen_t len = sizeof(address);
  memset(&address, 0, sizeof(address));
  int sd = ::accept(lsd_, (struct sockaddr*)&address, &len);
  if (sd < 0) {
    perror("accept() failed");
    return NULL;
  }
  return new TcpStream(sd, &address);
}
