/*
 * NodeBtHandler.cpp
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
 *  NodeBtHandler fonksiyonunda, önce gerekli DDS Entity'ler oluşturulmuş ve
 *  bu oluşturulan Entity'ler hedeflenen haberleşme karakteristiğine uygun
 *  bir şekilde konfigüre edilmiştir.
 *  Sonsuz döngü içerisinde de, ilk önce Timeout limiti aşılıp aşılmadığı
 *  kontrol edilir. Eğer aşılmışsa BT mesajı Publish edilir, aşılmamışsa da
 *  Matlab'dan gelebilecek olan mesaja Subscribe olmaya çalışır.
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
#include <sys/time.h>
#include <string>
#include <fstream>
#include <sstream>

#include "Timeout.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>

#include "ccpp_dds_dcps.h"
#include "ccpp_KonSensData.h"
#include "os.h"
#include "CheckStatus.h"
#include "DDSEntityManagerKonSens.h"

#include "example_main.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using namespace DDS;
using namespace KonSensData;
using namespace std;
using namespace rapidjson;


#define FAKEDELAY

void FakeDelay()
{
#ifdef FAKEDELAY
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
}

extern "C"
{
OS_API_EXPORT
int NodeBtHandler (int argc, char *argv[]);
}

int NodeBtHandler (int argc, char *argv[])
{
  // DDS değişkenleri
  DDSEntityManager                    mgrBtPub;
  DDSEntityManager                    mgrReqSub;
  ReturnCode_t                        status;
  SampleInfoSeq_var                   infoSeq = new SampleInfoSeq();
  BtSeq*                              btSeqInstance = new BtSeq();
  ServerReqSeq                        serverReqSeq;

  // Zaman ile alakalı değişkenler
  long int                            messageIDCount = 0;
  Time_t                              tsBeforeTheScan = { 0, 0 };
  Time_t                              tsAfterTheScan = { 0, 0 };
  //Time_t                              tsWifiPub = { 0, 0 };
  struct timeval                      tsConverter;
  DDS::Duration_t                     waitAckTime = { 0, 800000000 }; //800ms
  int                                 refreshRate = 60;

  // Veri tutucular (data structures)
  vector<string>                      btMacHolder;
  vector<int>                         btRssiHolder;
  string                              btFileContenHolder;

  // Bluetooth tarama sonuçlarının yazdırıldığı dosyadan okuma yapacak
  // olan değişken
  boost::filesystem::ifstream         fIn;

  // Bluetooth tarama sıklığı ayarlayan değişken
  int                                 refreshRateCounter = -1;

  char                                hostName[1024];
  gethostname(hostName, 1024);


  // !!! Bluetooth tarama mesajlarını Publish edecek Topic yaratılıyor
  // ve o Topic'e ait konfigürasyon ayarları yapılıyor.

  // Domain participant yaratılıyor
  mgrBtPub.createParticipant ("KonSens_BtSeq_Participant");

  // BtSeq tipi yaratılıyor
  BtSeqTypeSupport_var btSeqTs = new BtSeqTypeSupport();
  mgrBtPub.registerType(btSeqTs.in());

  // Topic yaratılıyor
  char btPubTopicName[] = "KonSensData_BtSeq_Topic";
  mgrBtPub.createTopic(btPubTopicName);

  // Publisher yaratılıyor
  mgrBtPub.createPublisher();

  // DataWriter yaratılıyor
  bool autodispose_unregistered_instances = false;
  mgrBtPub.createWriter(autodispose_unregistered_instances,
                        KEEP_ALL_HISTORY_QOS,
                        BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS);

  // Yaratılan DataWriter, BtSeq tipi için özelleştiriliyor
  DataWriter_var dWriter = mgrBtPub.getWriter ();
  BtSeqDataWriter_var btSeqWriter = BtSeqDataWriter::_narrow(dWriter.in());

  // Düğüm numarasını atanıyor
  btSeqInstance->userID = 13;

  // Publish edilecek olan mesajlara zaman etiketi takabilmek için
  // btSeqInstance değişkeni register ediliyor
  //userHandle = btSeqWriter->register_instance_w_timestamp(*btSeqInstance,
  //                                                        tsWifiPub);

  cout << "=== [Publisher of KonSensData_BtSeq_Topic] Ready ..." << endl;


  // !!! Sunucudan gelen komutlara Subscribe olacak olan Topic yaratılıyor
  // ve o Topic için gerekli konfigürasyon ayarları yapılıyor

  // Domain participant yaratılıyor
  mgrReqSub.createParticipant(
        "KonSensData_ServerReq_Participant_Server_to_Node");

  // ServerReq tipi yaratılıyor.
  ServerReqTypeSupport_var mgrSubscriberTS = new ServerReqTypeSupport();
  mgrReqSub.registerType(mgrSubscriberTS.in());

  // Topic yaratılıyor
  char reqSubTopicName[] = "KonSensData_ServerReq_Topic_Server_to_Node";
  mgrReqSub.createTopic(reqSubTopicName, RELIABLE_RELIABILITY_QOS,
                        VOLATILE_DURABILITY_QOS);

  // Subscriber yaratılıyor
  mgrReqSub.createSubscriber();

  // DataReader yaratılıyor
  mgrReqSub.createReader(KEEP_LAST_HISTORY_QOS, 1);

  // Yaratılan DataReader, ServerReq tipi için özelleştiriliyor.
  DataReader_var dReaderSub = mgrReqSub.getReader();
  ServerReqDataReader_var serverReqReader =
      ServerReqDataReader::_narrow(dReaderSub.in());
  checkHandle(serverReqReader.in(), "ServerReqDataReader::_narrow");

  cout << "=== [Subscriber KonSensData_ServerReq_Topic_Server_to_Node]"
          " Ready ..." << endl;


  // Bluetooth aktif hale getiriliyor.
  system("sudo hciconfig -a hci0 up");
  stringstream ssBtName;
  ssBtName << "sudo bt-adapter -a hci0 --set Name \"" << hostName << "\"";
  system(ssBtName.str().c_str());
  system("sudo bt-adapter -a hci0 --set Discoverable 1");

  // Yenileme sıklığını belirleyecek olan 'timelimit' değişkeni yaratılıyor.
  Timeout timelimit(std::chrono::milliseconds(refreshRate*1000));


  while (true)
  {
    if (timelimit.isExpired() || refreshRateCounter == -1)
    {

      // BT mesajını Publish etmek için hazırlık yapılıyor.

      cout << "-----------------------------------" << endl;

      btSeqInstance->messageID = messageIDCount;

      // Tarama öncesi alınan zaman etiketi (timestamp[0])
      gettimeofday(&tsConverter, NULL);
      tsBeforeTheScan.sec = tsConverter.tv_sec;
      tsBeforeTheScan.nanosec = (tsConverter.tv_usec * 1000);
      cout << " timestamp[0] (before the scan) = " << tsBeforeTheScan.sec
           << '.';
      cout << setfill('0') << setw(9) << (tsBeforeTheScan.nanosec)
           << endl;

      // BT taraması yapılıyor ve ardından tarama sonuçları 'bt_rssi'
      // dosyasına yazdırılıyor.
      system("sudo hciconfig hci0 reset");
      FakeDelay();
      system("sudo bt-adapter -a hci0 -d >> bt_rssi.txt");
      FakeDelay();

      cout << "Bluetooth message is publishing..." << endl;

      try
      {
        // BT tarama dosyası okunuyor
        fIn.open ("bt_rssi.txt", ios::in);
        stringstream ssBt;
        ssBt << fIn.rdbuf();
        btFileContenHolder = ssBt.str();
        system("rm bt_rssi.txt");

        // Okunan dosya boost kütüphane yardımıyla ayrıştırılıyor
        boost::regex expAd( "Address: ([0-9A-F:]{17})" ) ;
        boost::regex expBt( "RSSI:.*?([0-9]+)") ;
        boost::match_results<string::const_iterator> whatAd;

        string::const_iterator startAd = btFileContenHolder.begin();
        string::const_iterator finishAd = btFileContenHolder.end();

        while (boost::regex_search(startAd, finishAd, whatAd, expAd))
        {
          btMacHolder.push_back(whatAd[1]);
          startAd = whatAd[0].second ;
        }

        boost::match_results<string::const_iterator> whatBt;
        startAd = btFileContenHolder.begin() ;
        finishAd = btFileContenHolder.end() ;

        while (boost::regex_search(startAd, finishAd, whatBt, expBt))
        {
          string foundRssi(whatBt[1]);
          btRssiHolder.push_back(atoi(foundRssi.c_str()));
          startAd = whatBt[0].second ;
        }

        cout << "Number of BT connection that has been found: "
             << btRssiHolder.size() << endl;
        cout << "MessageID: " << btSeqInstance->messageID << endl;

        // Tarama sonrası alınan zaman etiketi (timestamp[1])
        gettimeofday(&tsConverter, NULL);
        tsAfterTheScan.sec = tsConverter.tv_sec;
        tsAfterTheScan.nanosec =( tsConverter.tv_usec * 1000);
        cout << " timestamp[1] (after the scan) = "
             << tsAfterTheScan.sec << '.';
        cout << setfill('0') << setw(9) << (tsAfterTheScan.nanosec)
             << endl;


        // Ayrıştırılan BT tarama dosyası ve alınan zaman etiketleri,
        // Publish edilecek olan mesaj değişkenlerine kaydediliyor.
        btSeqInstance->timestamp[0][0] = tsBeforeTheScan.nanosec;
        btSeqInstance->timestamp[0][1] = tsBeforeTheScan.sec;
        btSeqInstance->timestamp[1][0] = tsAfterTheScan.nanosec;
        btSeqInstance->timestamp[1][1] = tsAfterTheScan.sec;

        btSeqInstance->messages.length(btMacHolder.size());
        for(int i = 0; i < btMacHolder.size(); i++)
        {
          Msg msg;
          msg.devID = DDS::string_dup(btMacHolder[i].c_str());
          msg.hostName = DDS::string_dup(hostName);
          msg.dbm = -btRssiHolder[i];
          btSeqInstance->messages[i] = msg;
        }

        // Publish edilmeden önce, bir önceki mesajın acknowlegde mesajı
        // bekleniyor
        btSeqWriter->wait_for_acknowledgments(waitAckTime);

        status = btSeqWriter->write(*btSeqInstance,
                                    DDS::HANDLE_NIL);
        checkStatus(status, "BtSeqDataWriter::write");
        messageIDCount++;
      }

      catch ( boost::bad_expression & ex )
      {
        std::cout << ex.what() ;
        break;
      }

      btMacHolder.clear();
      btRssiHolder.clear();
      fIn.close();

      cout << "-----------------------------------" << endl;

      // Tarama sıklığını belirleyen değişken sıfırlanıyor
      timelimit.setTimerToZero();
      refreshRateCounter = 0;
      cout << refreshRateCounter << endl;

    } // BT Publisher kısmının sonu


    // Sunucu tarafından gönderilen Matlab komutlarına Subscribe olunuyor
    else
    {
      status = serverReqReader->take(serverReqSeq,
                                     infoSeq,
                                     LENGTH_UNLIMITED,
                                     ANY_SAMPLE_STATE,
                                     ANY_VIEW_STATE,
                                     ANY_INSTANCE_STATE);
      checkStatus(status, "severReqDataReader::read");

      for (DDS::ULong j = 0; j < serverReqSeq.length(); j++)
      {
        if(infoSeq[j].valid_data)
        {
          cout << "=== [Subscriber] message received :" << endl;
          cout << "    Received Request Message  : "
               << serverReqSeq[j].request << endl;
          cout << "    Received RequestID : \""
               << serverReqSeq[j].requestID << "\"" << endl;

          // Rapidjson yapılandırıcısı yaratılıyor
          Document d;
          if(d.Parse(serverReqSeq[j].request).HasParseError())
            cout << " Parsing Error!" << endl;

          StringBuffer nodeIdBuffer;
          Writer<StringBuffer> nodeIdWriter(nodeIdBuffer);
          d["NodeID"].Accept(nodeIdWriter);

          string tempNodeId = nodeIdBuffer.GetString();

          // Subscribe olunan mesajın düğüme ait olup olmadığı kontrol ediliyor
          if (tempNodeId == "\"SensDug13\"")
          {
            StringBuffer buffer;

            Value::ConstMemberIterator itr = d.FindMember("SetRefreshRate");
            // Ref Rate komutunun gelip gelmediği kontrol ediliyor
            if(itr != d.MemberEnd())
            {
              string refreshRateString;
              int refreshRateInt;

              // Document formatındaki JSON mesajı StrinBuffer'a dönüştürülüyor
              Writer<StringBuffer> writer(buffer);
              d["SetRefreshRate"].Accept(writer);
              refreshRateString = buffer.GetString();

              // Gelen mesajda fazladan çift tırnak ile bulunuyor
              // Örneğin, ""15""
              // Bu yüzden ilk son karakterler kırpılıyor
              refreshRateString =
                  refreshRateString.substr(1, refreshRateString.size()-1);
              // Refresh rate değeri stringden integera çevriliyor
              refreshRateInt = atoi(refreshRateString.c_str());

              refreshRate = refreshRateInt;
              timelimit.setMaxDuration(std::chrono::milliseconds
                                       (refreshRate*1000));

            }
          }
          else
            cout << "Invalid NodeID!" << endl;
        }
      }

      status = serverReqReader->return_loan(serverReqSeq, infoSeq);
      checkStatus(status, "ServerReqDataReader::return_loan");

      refreshRateCounter++;
      cout << refreshRateCounter << endl;

    } // Matlab komutuna Subscribe olma kısmının sonu

    // Terminalde akacak olan çıktıları dah gözle görülebilir bir şekilde
    // yazdırmak için koyulmuştur
    FakeDelay();
  }

  // Hafıza temizle işlemleri gerçekleştiriliyor

  mgrBtPub.deleteWriter();
  mgrBtPub.deletePublisher();
  mgrBtPub.deleteTopic();
  mgrBtPub.deleteParticipant();

  mgrReqSub.deleteReader();
  mgrReqSub.deleteSubscriber();
  mgrReqSub.deleteTopic();
  mgrReqSub.deleteParticipant();

  return 0;
}

int OSPL_MAIN (int argc, char *argv[])
{
  return NodeBtHandler (argc, argv);
}
