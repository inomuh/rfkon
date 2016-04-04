/*
 * ServerRequester.cpp
 *
 * GLOBAL DEĞİŞKENLER:
 *  Yok
 *
 * ENUM KULLANIMI:
 *  Yok
 *
 * FONKSİYON PROTOTİPLERİ:
 *  void FakeDelay();
 *
 * AMAÇ:
 *  ServerRequester fonksiyonunda, önce gerekli DDS Entity'ler oluşturulmuş
 *  ve bu oluşturulan Entity'ler hedeflenen haberleşme karakteristiğine uygun
 *  bir şekilde konfigüre edilmiştir. Daha sonra Matlab'dan gelecek komutlar
 *  için TCP/IP bağlantısı açılır.
 *  Sonsuz döngü içerisinde de, DDS Bridge görevini gerçekleştirmek üzere
 *  Matlab'dan gelecek komutu bekler. Matlab'dan herhangi bir komut gelmediği
 *  sürece pasif durumdadır. Eğer bir komut gelmiş ise, köprü görevini
 *  üstlenerek gelen mesajı Publish eder ve ilgili düğümden gelecek Response
 *  mesajını beklemeye geçer. Eğer, düğümden gelen Response mesajı gelmiş ise
 *  o mesaja Subscibe olur. Son olarak da, Subscribe olunan Response mesajını
 *  TCP/IP üzerinden Matlab kontrol arayüzüne gönderir.
 *
 *
 * NOTLAR:
 *  Yazar: Uğur Bolat
 *  Tarih: 15.10.2015
 *
 *   Versiyon: v_1.0
 *    Güncelleme Tarihi: 15.10.2015
 *
 */

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <fstream>

#include "Timeout.h"

#include "ccpp_dds_dcps.h"
#include "ccpp_KonSensData.h"
#include "os.h"
#include "CheckStatus.h"

#include "example_main.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "tcpacceptor.h"

#include "DDSEntityManagerKonSens.h"


using namespace rapidjson;
using namespace DDS;
using namespace KonSensData;

/* entry point exported and demangled so symbol can be found in shared library */
extern "C"
{
OS_API_EXPORT
int ServerRequester(int argc, char *argv[]);
}


int ServerRequester(int argc, char *argv[])
{
  os_time delay_2ms = { 0, 2000000 };
  os_time delay_1s = { 1, 0 };
  int expirationTime = 30;  // 30 saniye

  SampleInfoSeq_var               infoSeq = new SampleInfoSeq(); // for serverReqWriter
  ServerReq                       *serverReq = new ServerReq();
  ReturnCode_t                    status;
  ServerReqSeq                    serverReqSeq;
  DDSEntityManager                mgr_pub;
  DDSEntityManager                mgr_sub;
  //    bool isMatlabCommandReceivedThroughTcp = false;
  //    bool isMatlabCommandSent = false;
  //    bool isMatlabResponseSent = false;


  // !!! Matlab'dan aldığı komutu düğüme Publish edecek Topic yaratılıyor
  // ve o Topic'e ait konfigürasyon ayarları yapılıyor

  // Domain participant yaratılıyor
  mgr_pub.createParticipant("KonSensData_ServerReq_Participant_Server_to_Node");

  // ServerReq tipi yaratılıyor
  ServerReqTypeSupport_var mgrPublisherTS = new ServerReqTypeSupport();
  mgr_pub.registerType(mgrPublisherTS.in());

  // Topic yaratılıyor
  char pub_topic_name[] = "KonSensData_ServerReq_Topic_Server_to_Node";
  mgr_pub.createTopic(pub_topic_name,
                      RELIABLE_RELIABILITY_QOS,
                      VOLATILE_DURABILITY_QOS);

  // Publisher yaratılıyor
  mgr_pub.createPublisher();

  // DataReader yaratılıyor
  // Not: autodispose_unregistered_instances değerinin varsayılan değeri truedur.
  // Eğer false yapılmazsa Subscriber her zaman Publisher'dan önce
  // başlatılmak zorundadır
  bool autodispose_unregistered_instances = false;
  mgr_pub.createWriter(autodispose_unregistered_instances,
                       KEEP_LAST_HISTORY_QOS,
                       1);

  // Yaratılan DataWriter, ServerReq tipi için özelleştiriliyor
  DataWriter_var ServerRequesterWriter = mgr_pub.getWriter();
  ServerReqDataWriter_var serverReqWriter =
      ServerReqDataWriter::_narrow(ServerRequesterWriter.in());

  cout << "=== [Publisher] Ready ..." << endl;



  // !!! Düğümden gelen Response mesaja Subscribe olacak Topic yaratılıyor
  // o Topic'e ait konfigürasyon ayarları yapılıyor

  // Domain participant yaratılıyor
  mgr_sub.createParticipant("KonSensData_ServerReq_Participant_Node_to_Server");

  // ServerReq tipi yaratılıyor
  ServerReqTypeSupport_var mgrSubscriberTS = new ServerReqTypeSupport();
  mgr_sub.registerType(mgrSubscriberTS.in());

  // Topic yaratılıyor
  char sub_topic_name[] = "KonSensData_ServerReq_Topic_Node_to_Server";
  mgr_sub.createTopic(sub_topic_name,
                      RELIABLE_RELIABILITY_QOS,
                      VOLATILE_DURABILITY_QOS);

  // Subscriber yaratılıyor
  mgr_sub.createSubscriber();

  // DataWriter yaratılıyor
  mgr_sub.createReader(KEEP_LAST_HISTORY_QOS, 1);

  // Yaratılan DataReader, ServerReq tipi için özelleştiriliyor
  DataReader_var dreader = mgr_sub.getReader();
  ServerReqDataReader_var serverReqReader = ServerReqDataReader::_narrow(dreader.in());
  checkHandle(serverReqReader.in(), "ServerReqDataReader::_narrow");
  cout << "=== [Subscriber] Ready ..." << endl;

  serverReq->requestID = 0;

  // Terminalden girilen IP address ve port numarasını doğruluğu kontrol
  // ediliyor
  //  if (argc < 2 || argc > 4) {
  //    printf("usage: server <port> [<ip>]\n");
  //    exit(1);
  //  }

  TcpStream* stream = NULL;
  TcpAcceptor* acceptor = NULL;
  //  if (argc == 3) {
  //    acceptor = new TcpAcceptor(atoi(argv[1]), argv[2]);
  //  }
  //  else {
  //    acceptor = new TcpAcceptor(atoi(argv[1]));
  //  }

  acceptor = new TcpAcceptor(30000, "localhost");

  // Zaman aşımı kontrolu için Timeout tipinde bir değişken yaratılıyor
  Timeout timeLimit(std::chrono::milliseconds(expirationTime*1000));

  if (acceptor->start() == 0) {

    while (true) {
      stream = acceptor->accept();
      if (stream != NULL) {
        ssize_t len;
        char line[256];

        bool isMatlabCommandReceivedThroughTcp = false;
        bool isMatlabCommandSentThroughDds = false;
        bool isMatlabResponseSentThroughTcp = false;
        bool isMatlabResponseReceivedThroughDds = false;

        timeLimit.setTimerToZero();

        // Matlab GUI'den TCP/IP üzerinden bir komut geldiği anda döngü
        // başlar
        while ((len = stream->receive(line, sizeof(line))) > 0)
        {
          isMatlabCommandReceivedThroughTcp = true;
          isMatlabResponseSentThroughTcp = false;
          isMatlabCommandSentThroughDds = false;
          isMatlabResponseReceivedThroughDds = false;

          timeLimit.setTimerToZero();

          line[len] = 0;
          cout << "----------------------------------------" << endl;
          cout << " [TCP/IP] Matlab command message has been received : "
               << line << endl;

          // Matlab'dan gelen komut düğümlere Publish ediliyor
          serverReq->request = DDS::string_dup(line);
          status = serverReqWriter->write(*serverReq, DDS::HANDLE_NIL);
          // Matlab Command mesajı Publish edilmiş mi kontrol ediliyor
          if (status != DDS::RETCODE_OK && status != DDS::RETCODE_NO_DATA)
          {
            cerr << "Error in " << "ServerReqDataWriter::write"
                 << ": " << getErrorName(status).c_str() << endl;
            //exit(1);
          }
          else
          {
            isMatlabCommandSentThroughDds = true;
            serverReq->requestID++;
            cout << " [DDS] command message has been sent to the Node : "
                 << serverReq->request << endl;
          }

          // Matlab'dan gelen komut alınmış ve o komut düğümlere Publish
          // edilmişse if'in içerisine girecek
          if (isMatlabCommandSentThroughDds && isMatlabCommandReceivedThroughTcp)
          {
            // Düğümden gelen Response mesajı tekrar Matlab GUI'ye gönderilene
            // kadar döngü içerisinde kalır
            while (!isMatlabResponseReceivedThroughDds && !timeLimit.isExpired())
            {
              char* c_temp_message;

              // Düğümden gelen Response mesajına Subscribe oluyor
              status = serverReqReader->take(serverReqSeq,
                                             infoSeq,
                                             LENGTH_UNLIMITED,
                                             ANY_SAMPLE_STATE,
                                             ANY_VIEW_STATE,
                                             ANY_INSTANCE_STATE);
              checkStatus(status, "severReqDataReader::take");


              for (DDS::ULong j = 0; j < serverReqSeq.length(); j++)
              {
                if(infoSeq[j].valid_data)
                {
                  isMatlabResponseReceivedThroughDds = true;
                  cout << status << endl;
                  cout << " Response RequestID = "
                       << serverReqSeq[j].requestID << endl;
                  cout << " [DDS] Response Message has been received from Node : "
                       << serverReqSeq[j].request << endl;
                  c_temp_message = DDS::string_dup(serverReqSeq[j].request);

                  string str_temp_message(c_temp_message);

                  // Düğümden gelen Response mesajı tekrar Matlab GUI'ye
                  // gönderiliyor
                  stream->send(str_temp_message.c_str(),
                               str_temp_message.size());
                  cout << " [TCP/IP] Response Message been sent to Matlab GUI : "
                       << str_temp_message << endl;

                  isMatlabResponseSentThroughTcp = true;
                  cout << "----------------------------------------" << endl;
                }
              }

              status = serverReqReader->return_loan(serverReqSeq, infoSeq);
              checkStatus(status, "MsgDataReader::return_loan");

              os_nanoSleep(delay_2ms);
            }

            if (timeLimit.isExpired() && !isMatlabResponseReceivedThroughDds &&
                !isMatlabResponseSentThroughTcp)
              cout << "Expiration time is exceed!" << endl;

          }

          if (isMatlabCommandSentThroughDds)
            timeLimit.setTimerToZero();

        }

        delete stream;
      }



    }
  }

  os_nanoSleep(delay_2ms);

  // Hafıza temizle işlemleri gerçekleştiriliyor
  mgr_pub.deleteWriter();
  mgr_pub.deletePublisher();
  mgr_pub.deleteTopic();
  mgr_pub.deleteParticipant();

  mgr_sub.deleteReader();
  mgr_sub.deleteSubscriber();
  mgr_sub.deleteTopic();
  mgr_sub.deleteParticipant();
}

int OSPL_MAIN (int argc, char *argv[])
{
  return ServerRequester (argc, argv);
}


