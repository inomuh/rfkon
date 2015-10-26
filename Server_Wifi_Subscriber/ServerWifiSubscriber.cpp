/*
 * ServerWifiSubscriber.cpp
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
 *  ServerWifiSubscriber fonksiyonunda, önce gerekli DDS Entity'ler oluşturulmuş
 *  ve bu oluşturulan Entity'ler hedeflenen haberleşme karakteristiğine uygun
 *  bir şekilde konfigüre edilmiştir.
 *  Sonsuz döngü içerisinde de, düğümden gelecek Wifi mesajlarına Subscribe
 *  olmaya çalışır.
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
#include <ctime>
#include <fstream>

#include "ccpp_dds_dcps.h"
#include "CheckStatus.h"
#include "ccpp_KonSensData.h"
#include "os.h"
#include "DDSEntityManagerKonSens.h"

#include <mongo/client/dbclient.h>
#include <mongo/bson/bson.h>

#include "example_main.h"

#define TERMINATION_MESSAGE -1

using namespace DDS;
using namespace KonSensData;

/* entry point exported and demangled so symbol can be found in shared library */
extern "C"
{
OS_API_EXPORT
int ServerWifiSubscriber(int argc, char *argv[]);
}


int ServerWifiSubscriber(int argc, char *argv[])
{  
  // DDS değişkenleri
  SampleInfoSeq_var               infoSeq = new SampleInfoSeq();
  SampleLostStatus                lostSample;
  LivelinessChangedStatus         liveliness;
  WifiSeqSeq                      msgList;
  ReturnCode_t                    status = -1;
  DDSEntityManager                mgrWifiSub;

  // Zaman ile alakalı değişkenler
  os_time                         delay_2ms = { 0, 2000000 };
  os_time                         delay_100ms = { 0, 100000000 };


  // !!! Düğüm tarafından Publish edilmiş olan Wifi mesajına Subscribe olacak
  // Topic yaratılıyor ve o Topic'e ait konfigürasyon ayarları yapılıyor.

  // Domain participant yaratılıyor
  mgrWifiSub.createParticipant("KonSensData_WifiSeq_Participant");

  // WifiSeq tipi yaratılıyor
  WifiSeqTypeSupport_var wifiSeqTs = new WifiSeqTypeSupport();
  mgrWifiSub.registerType(wifiSeqTs.in());

  // Topic yaratılıyor
  char sub_topic_name[] = "KonSensData_WifiSeq_Topic";
  mgrWifiSub.createTopic(sub_topic_name);

  // Subscriber yaratılıyor
  mgrWifiSub.createSubscriber();

  // DataWriter yaratılıyor
  mgrWifiSub.createReader(KEEP_ALL_HISTORY_QOS,
                          BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS);

  // Yaratılan DataReader, WifiSeq tipi için özelleştiriliyor
  DataReader_var dreader = mgrWifiSub.getReader();
  WifiSeqDataReader_var wifiSeqReader =
      WifiSeqDataReader::_narrow(dreader.in());
  checkHandle(wifiSeqReader.in(), "WifiSeqDataReader::_narrow");

  cout << "=== [Subscriber Wifi] Ready ..." << endl;

  // Subscribe edilen Wifi mesaj içeriği gerekli ayrıştırma işlemli yapıldıktan
  // sonra Log dosyalarına kaydetliyecektir
  ofstream outFile11, outFile12, outFile13, outFile15, outFile16, outFile25, outFile26;

  outFile11.open ( "Node11_Wifi.txt", ios::out );
  if( !outFile11 )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }
  else
  {
    outFile11 << "MessageID" << "	"
              << "Before scan" << "							"
              << "After scan" << "							"
              << "Pub time" << "							"
              << "Sub time" << endl;
  }

  outFile12.open ( "Node12_Wifi.txt", ios::out );
  if( !outFile12 )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }
  else
  {
    outFile12 << "MessageID" << "	"
              << "Before scan" << "							"
              << "After scan" << "							"
              << "Pub time" << "							"
              << "Sub time" << endl;
  }

  outFile15.open ( "Node15_Wifi.txt", ios::out );
  if( !outFile15 )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }
  else
  {
    outFile15 << "MessageID" << "	"
              << "Before scan" << "							"
              << "After scan" << "							"
              << "Pub time" << "							"
              << "Sub time" << endl;
  }

  outFile16.open ( "Node16_Wifi.txt", ios::out );
  if( !outFile16 )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }
  else
  {
    outFile16 << "MessageID" << "	"
              << "Before scan" << "							"
              << "After scan" << "							"
              << "Pub time" << "							"
              << "Sub time" << endl;
  }

  outFile13.open ( "Node13_Wifi.txt", ios::out );
  if( !outFile13 )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }
  else
  {
    outFile13 << "MessageID" << "	"
              << "Before scan" << "							"
              << "After scan" << "							"
              << "Pub time" << "							"
              << "Sub time" << endl;
  }

  outFile25.open ( "Node25_Wifi.txt", ios::out );
  if( !outFile25 )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }
  else
  {
    outFile25 << "MessageID" << "	"
              << "Before scan" << "							"
              << "After scan" << "							"
              << "Pub time" << "							"
              << "Sub time" << endl;
  }

  outFile26.open ( "Node26_Wifi.txt", ios::out );
  if( !outFile26 )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }
  else
  {
    outFile26 << "MessageID" << "	"
              << "Before scan" << "							"
              << "After scan" << "							"
              << "Pub time" << "							"
              << "Sub time" << endl;
  }

  cout << "   Node files are ready to take the Wifi messages...   " << endl;


  // MongoDB açılıyor
  mongo::client::initialize();
  mongo::DBClientConnection c;
  c.connect("localhost");

  while (true)
  {
    // Wifi mesajı Subscribe ediliyor
    status = wifiSeqReader->take(msgList,
                                 infoSeq,
                                 LENGTH_UNLIMITED,
                                 ANY_SAMPLE_STATE,
                                 ANY_VIEW_STATE,
                                 ANY_INSTANCE_STATE);
    checkStatus(status, "wifiSeqReader::take");

    status = wifiSeqReader->get_sample_lost_status(lostSample);
    checkStatus(status, "wifiSeqReader::get_sample_lost_status");

    status = wifiSeqReader->get_liveliness_changed_status(liveliness);
    checkStatus(status, "wifiSeqReader::get_liveliness_changed_status");

    for (DDS::ULong j = 0; j < msgList.length(); j++)
    {
      if(infoSeq[j].valid_data)
      {
        // Gelen mesaj içerikleri terminale yazdırılıyor
        cout << "---------------------------------------------"
             << endl;

        cout << "=== [Subscriber] message received :" << endl;
        cout << "    Lost Sample Status: " << lostSample.total_count
             << endl;
        cout << "    Liveliness Changed Status  : "
             << liveliness.not_alive_count << endl;
        cout << "    Node  : " << msgList[j].userID << endl;
        cout << "    messageID  : " << msgList[j].messageID << endl;
        cout << "    Timestamp [0]: \"" << msgList[j].timestamp[0][1]
            << "." << msgList[j].timestamp[0][0] << "\"" << endl;
        cout << "    Timestamp [1]: \"" << msgList[j].timestamp[1][1]
            << "." << msgList[j].timestamp[1][0] << "\"" << endl;
        cout << "    size: " << msgList[j].messages.length() << endl;

        cout << "---------------------------------------------"
             << endl;

        // Tarama öncesi alınan zaman etiketi (timestamp[0])
        char buffer1[80];
        time_t tempTime1 =
            static_cast<time_t>(msgList[j].timestamp[0][1]);
        strftime(buffer1,80,"%Y	%m	%d	%I	%M	%S",
                 localtime(&tempTime1));
        string strTimeOfBeforeScan(buffer1);
        cout << "    timestamp before pub scans the wifi: "
             << strTimeOfBeforeScan << endl;

        // Tarama sonrası alınan zaman etiketi (timestamp[1])
        char buffer2[80];
        time_t tempTime2 =
            static_cast<time_t>(msgList[j].timestamp[1][1]);
        strftime(buffer2,80,"%Y	%m	%d	%I	%M	%S",
                 localtime(&tempTime2));
        string strTimeOfAfterScan(buffer2);
        cout << "    timestamp after pub scans the wifi:  "
             << strTimeOfAfterScan << endl;


        SampleInfo* info = &(infoSeq[j]);

        // Publish edilme zamanı işleniyor
        char buffer3[80];
        time_t tempTime3 =
            static_cast<time_t>(info->source_timestamp.sec);
        strftime(buffer3,80,"%Y	%m	%d	%I	%M	%S",
                 localtime(&tempTime3));
        string strTimeOfPub(buffer3);
        cout << "	   timestamp when publishes:  "
             << info->source_timestamp.sec << '.';
        cout << setfill('0') << setw(9)
             << (info->source_timestamp.nanosec) << " " << endl;

        // Subscribe edilme zamanı işleniyor
        char buffer4[80];
        time_t tempTime4 =
            static_cast<time_t>(info->reception_timestamp.sec);
        strftime(buffer4,80,"%Y	%m	%d	%I	%M	%S",
                 localtime(&tempTime4));
        string strTimeOfSub(buffer4);
        cout << "	   timestamp when subscribes:  "
             << info->reception_timestamp.sec << '.';
        cout << setfill('0') << setw(9)
             << (info->reception_timestamp.nanosec) << " " << endl;

        cout << "----------------------------------" << endl;

        // Wifi mesajı, yukarıdaki işlemler sonucu hazırlanmış format ile
        // birlikte Log dosyalarına yazdırılıyor
        switch(msgList[j].userID)
        {
        case 11:
          if(strTimeOfBeforeScan != "1970	01	01	02	00	00" ||
             strTimeOfAfterScan != "1970	01	01	02	00	00")
          {
            outFile11 << msgList[j].messageID << "	";
            outFile11 << strTimeOfBeforeScan << "	"
                      << msgList[j].timestamp[0][0] << "	";
            outFile11 << strTimeOfAfterScan << "	"
                      << msgList[j].timestamp[1][0] << "	";
            outFile11 << strTimeOfPub << "	"
                      << info->source_timestamp.nanosec << "	";
            outFile11 << strTimeOfSub << "	"
                      << info->reception_timestamp.nanosec << endl;
          }
          break;

        case 12:
          if(strTimeOfBeforeScan != "1970	01	01	02	00	00" ||
             strTimeOfAfterScan != "1970	01	01	02	00	00")
          {
            outFile12 << msgList[j].messageID << "	";
            outFile12 << strTimeOfBeforeScan << "	"
                      << msgList[j].timestamp[0][0] << "	";
            outFile12 << strTimeOfAfterScan << "	"
                      << msgList[j].timestamp[1][0] << "	";
            outFile12 << strTimeOfPub << "	"
                      << info->source_timestamp.nanosec << "	";
            outFile12 << strTimeOfSub << "	"
                      << info->reception_timestamp.nanosec << endl;
          }
          break;

        case 13:
          if(strTimeOfBeforeScan != "1970	01	01	02	00	00" ||
             strTimeOfAfterScan != "1970	01	01	02	00	00")
          {
            outFile13 << msgList[j].messageID << "	";
            outFile13 << strTimeOfBeforeScan << "	"
                      << msgList[j].timestamp[0][0] << "	";
            outFile13 << strTimeOfAfterScan << "	"
                      << msgList[j].timestamp[1][0] << "	";
            outFile13 << strTimeOfPub << "	"
                      << info->source_timestamp.nanosec << "	";
            outFile13 << strTimeOfSub << "	"
                      << info->reception_timestamp.nanosec << endl;
          }
          break;


        case 15:
          if(strTimeOfBeforeScan != "1970	01	01	02	00	00" ||
             strTimeOfAfterScan != "1970	01	01	02	00	00")
          {
            outFile15 << msgList[j].messageID << "	";
            outFile15 << strTimeOfBeforeScan << "	"
                      << msgList[j].timestamp[0][0] << "	";
            outFile15 << strTimeOfAfterScan << "	"
                      << msgList[j].timestamp[1][0] << "	";
            outFile15 << strTimeOfPub << "	"
                      << info->source_timestamp.nanosec << "	";
            outFile15 << strTimeOfSub << "	"
                      << info->reception_timestamp.nanosec << endl;
          }
          break;

        case 16:
          if(strTimeOfBeforeScan != "1970	01	01	02	00	00" ||
             strTimeOfAfterScan != "1970	01	01	02	00	00")
          {
            outFile16 << msgList[j].messageID << "	";
            outFile16 << strTimeOfBeforeScan << "	"
                      << msgList[j].timestamp[0][0] << "	";
            outFile16 << strTimeOfAfterScan << "	"
                      << msgList[j].timestamp[1][0] << "	";
            outFile16 << strTimeOfPub << "	"
                      << info->source_timestamp.nanosec << "	";
            outFile16 << strTimeOfSub << "	"
                      << info->reception_timestamp.nanosec << endl;
          }
          break;

        case 25:
          if(strTimeOfBeforeScan != "1970	01	01	02	00	00" ||
             strTimeOfAfterScan != "1970	01	01	02	00	00")
          {
            outFile25 << msgList[j].messageID << "	";
            outFile25 << strTimeOfBeforeScan << "	"
                      << msgList[j].timestamp[0][0] << "	";
            outFile25 << strTimeOfAfterScan << "	"
                      << msgList[j].timestamp[1][0] << "	";
            outFile25 << strTimeOfPub << "	"
                      << info->source_timestamp.nanosec << "	";
            outFile25 << strTimeOfSub << "	"
                      << info->reception_timestamp.nanosec << endl;
          }
          break;

        case 26:

          if(strTimeOfBeforeScan != "1970	01	01	02	00	00" ||
             strTimeOfAfterScan != "1970	01	01	02	00	00")
          {
            outFile26 << msgList[j].messageID << "	";
            outFile26 << strTimeOfBeforeScan << "	"
                      << msgList[j].timestamp[0][0] << "	";
            outFile26 << strTimeOfAfterScan << "	"
                      << msgList[j].timestamp[1][0] << "	";
            outFile26 << strTimeOfPub << "	"
                      << info->source_timestamp.nanosec << "	";
            outFile26 << strTimeOfSub << "	"
                      << info->reception_timestamp.nanosec << endl;
          }
          break;

        default:
          cout << "	Unidentified User ID...	" << endl;
        }


        for(int i=0; i < msgList[j].messages.length(); i++)
        {
          // Terminalde Subscribe edilen mesajlar yazdırılıyor
          cout << "    devID  : " << msgList[j].messages[i].devID
               << endl;
          cout << "    dBm : \"" << msgList[j].messages[i].dbm
               << "\"" << endl;
          cout << "    hostname: "
               << msgList[j].messages[i].hostName << endl;

          // Veritabanına kaydetme işlemi gerçekleştiriliyor
          mongo::BSONObjBuilder b;
          mongo::StatusWith<mongo::Date_t> now  =
              mongo::dateFromISOString
              (mongo::timeToISOString
               (static_cast<time_t>(info->reception_timestamp.sec)));
          b.append("timestamp", now.getValue() );
          b.append("hostname", msgList[j].messages[0].hostName);
          b.append("devid", msgList[j].messages[i].devID );
          b.append("dbm", msgList[j].messages[i].dbm );
          b.append("type","WiFi"); /*EAX: Bluetooth Type, Wifi Type, BLE Type */
          mongo::BSONObj p = b.obj();
          c.insert("inovasyon.konsens",p);

        }

      }

      else
        cout << "Invalid data!" << endl;

      if(msgList[j].userID == TERMINATION_MESSAGE) {
        cout << "Termination message received: exiting..." << endl;
        //closed = true;
      }

    }
    status = wifiSeqReader->return_loan(msgList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
    os_nanoSleep(delay_100ms);

  }

  os_nanoSleep(delay_2ms);

  // Hafıza temizle işlemleri gerçekleştiriliyor
  mgrWifiSub.deleteReader();
  mgrWifiSub.deleteSubscriber();
  mgrWifiSub.deleteTopic();
  mgrWifiSub.deleteParticipant();


  // Log dosyaları kapatılıyor
  outFile11.close();
  outFile12.close();
  outFile15.close();
  outFile16.close();
  outFile13.close();
  outFile25.close();
  outFile26.close();

  return 0;
}

int OSPL_MAIN (int argc, char *argv[])
{
  return ServerWifiSubscriber (argc, argv);
}
