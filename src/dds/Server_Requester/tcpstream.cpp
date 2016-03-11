/*
 * tcpstream.cpp
 *
 * GLOBAL DEĞİŞKENLER:
 *  Yok
 *
 * ENUM KULLANIMI:
 *      enum {
 *       connectionClosed = 0,
 *       connectionReset = -1,
 *       connectionTimedOut = -2
 *      };
 *
 * FONKSİYON PROTOTİPLERİ:
 *  public:
 *   ssize_t send(const char* buffer, size_t len);
 *   ssize_t receive(char* buffer, size_t len, int timeout=0);
 *   string getPeerIP();
 *   int    getPeerPort();
 *
 *  private:
 *   bool waitForReadEvent(int timeout);
 *   TcpStream(int sd, struct sockaddr_in* address);
 *   TcpStream();
 *   TcpStream(const TcpStream& stream);
 *
 * AMAÇ:
 *  TcpStream sınıfı, ağ I/O mekanizmasının yaratılmasını sağlar. Ayrıca
 *  IP addresslerini ve TCP Port bilgisini döndürür.
 *
 * NOTLAR:
 *  Yazar: Uğur Bolat
 *  Tarih: 15.10.2015
 *
 *   Versiyon: v_1.0
 *    Güncelleme Tarihi: 15.10.2015
 *
 */

#include <arpa/inet.h>
#include "tcpstream.h"

TcpStream::TcpStream(int sd, struct sockaddr_in* address) : sd_(sd) {
  char ip[50];
  inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
  peerIp = ip;
  peerPort_ = ntohs(address->sin_port);
}

TcpStream::~TcpStream()
{
  close(sd_);
}

ssize_t TcpStream::send(const char* buffer, size_t len)
{
  return write(sd_, buffer, len);
}

ssize_t TcpStream::receive(char* buffer, size_t len, int timeout)
{
  if (timeout <= 0) return read(sd_, buffer, len);

  if (waitForReadEvent(timeout) == true)
  {
    return read(sd_, buffer, len);
  }
  return connectionTimedOut;

}

string TcpStream::getPeerIP()
{
  return peerIp;
}

int TcpStream::getPeerPort()
{
  return peerPort_;
}

bool TcpStream::waitForReadEvent(int timeout)
{
  fd_set sdset;
  struct timeval tv;

  tv.tv_sec = timeout;
  tv.tv_usec = 0;
  FD_ZERO(&sdset);
  FD_SET(sd_, &sdset);
  if (select(sd_+1, &sdset, NULL, NULL, &tv) > 0)
  {
    return true;
  }
  return false;
}
