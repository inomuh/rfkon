/*
 * tcpstream.h
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

#ifndef __tcpstream_h__
#define __tcpstream_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

using namespace std;

class TcpStream
{
  int     sd_;
  string  peerIp;
  int     peerPort_;

public:
  friend class TcpAcceptor;
  friend class TcpConnector;

  ~TcpStream();

  ssize_t send(const char* buffer, size_t len);
  ssize_t receive(char* buffer, size_t len, int timeout=0);

  string getPeerIP();
  int    getPeerPort();

  enum {
    connectionClosed = 0,
    connectionReset = -1,
    connectionTimedOut = -2
  };

private:
  bool waitForReadEvent(int timeout);

  TcpStream(int sd, struct sockaddr_in* address);
  TcpStream();
  TcpStream(const TcpStream& stream);
};

#endif
