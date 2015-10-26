/*
 * NodeWifiHandler.cpp
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
 *  NodeWifiHandler fonksiyonunda, önce gerekli DDS Entity'ler oluşturulmuş ve
 *  bu oluşturulan Entity'ler hedeflenen haberleşme karakteristiğine uygun
 *  bir şekilde konfigüre edilmiştir.
 *  Sonsuz döngü içerisinde de, ilk önce Timeout limiti aşılıp aşılmadığı
 *  kontrol edilir. Eğer aşılmışsa Wifi mesajı Publish edilir, aşılmamışsa da
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

#include "example_main.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "DDSEntityManagerKonSens.h"


using namespace DDS;
using namespace KonSensData;
using namespace rapidjson;
using namespace std;

#define FAKE_DELAY

void fake_delay()
{
#ifdef FAKE_DELAY
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
}

extern "C"
{
OS_API_EXPORT
int NodeWifiHandler (int argc, char *argv[]);
}

int NodeWifiHandler (int argc, char *argv[])
{

  // DDS değişkenleri
  ReturnCode_t                        status;
  InstanceHandle_t                    userHandle;
  DDSEntityManager                    mgrReqSub;
  DDSEntityManager                    mgrReqPub;
  DDSEntityManager                    mgrWifiPub;
  WifiSeq                             *wifiSeqInstance = new WifiSeq();;
  ServerReqSeq                        serverReqSeq;
  ServerReq                           *serverReq = new ServerReq();
  SampleInfoSeq_var                   infoSeq = new SampleInfoSeq();

  // Zaman ile alakalı değişkenler
  DDS::Duration_t                     waitAckTime = { 0, 800000000 }; //800ms
  int                                 refreshRate = 60;
  long int                            messageIDCount = 0;
  Time_t                              tsBeforeTheScan = { 0, 0 };
  Time_t                              tsAfterTheScan = { 0, 0 };
  struct timeval                      tsConverter;

  // Veri tutucular (data structures)
  vector<string>                      modemIdHolder;
  vector<int>                         dbmHolder;
  string                              wifiFileContenHolder;
  string                              ifconfigFileContentHolder;


  // Wifi tarama sonuçlarının yazdırıldığı dosyadan okuma yapacak
  // olan değişken
  boost::filesystem::ifstream         fInWifiScan, fInIfconfig;

  // Matlab komutları ayrıştırılmasında kullanılacak olan değişkenler
  string                              ip, subnet, gateway, dns, nodeName;

  char                                hostName[1024];
  gethostname(hostName, 1024);

  // Bluetooth tarama sıklığı ayarlayan değişken
  int refreshRateCounter = -1;


  // !!! Wifi tarama mesajlarını Publish edecek Topic yaratılıyor
  // ve o Topic'e ait konfigürasyon ayarları yapılıyor

  // Domain participant yaratılıyor
  mgrWifiPub.createParticipant("KonSensData_WifiSeq_Participant");

  // WifiSeq tipi yaratılıyor
  WifiSeqTypeSupport_var wifiSeqTS = new WifiSeqTypeSupport();
  mgrWifiPub.registerType(wifiSeqTS.in());

  // Topic yaratılıyor
  char wifiPubTopicName[] = "KonSensData_WifiSeq_Topic";
  mgrWifiPub.createTopic(wifiPubTopicName);

  // Publisher yaratılıyor
  mgrWifiPub.createPublisher();

  // DataWriter yaratılıyor
  bool autodisposeUnregisteredInstances = false;
  mgrWifiPub.createWriter(autodisposeUnregisteredInstances,
                          KEEP_ALL_HISTORY_QOS,
                          BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS);

  // Yaratılan DataWriter, WifiSeq tipi için özelleştiriliyor
  DataWriter_var dWriterWifPub = mgrWifiPub.getWriter();
  WifiSeqDataWriter_var wifiSeqWriter =
      WifiSeqDataWriter::_narrow(dWriterWifPub.in());

  // Düğüm numarası atanıyor
  wifiSeqInstance->userID = 13;

  cout << "=== [Publisher of KonSensData_WifiSeq_Topic] Ready ..." << endl;


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
  DataReader_var dReadeSub = mgrReqSub.getReader();
  ServerReqDataReader_var serverReqReader =
      ServerReqDataReader::_narrow(dReadeSub.in());
  checkHandle(serverReqReader.in(), "ServerReqDataReader::_narrow");

  cout << "=== [Subscriber of KonSensData_ServerReq_Topic_Server_to_Node]"
          " Ready ..." << endl;


  // !!! Sunucudan gelen komutlar ayrıştırılıp işlendikten sonra Response
  // mesajını Publish edecek olan Topic yaratılıyor

  // Domain participant yaratılıyor
  mgrReqPub.createParticipant(
        "KonSensData_ServerReq_Participant_Node_to_Server");

  // ServerReq tipi yaratılıyor.
  ServerReqTypeSupport_var mgrPublisherTS = new ServerReqTypeSupport();
  mgrReqPub.registerType(mgrPublisherTS.in());

  // Topic yaratılıyor
  char reqPubTopicName[] = "KonSensData_ServerReq_Topic_Node_to_Server";
  mgrReqPub.createTopic(reqPubTopicName, RELIABLE_RELIABILITY_QOS,
                        VOLATILE_DURABILITY_QOS);

  // Publisher yaratılıyor
  mgrReqPub.createPublisher();

  // DataWriter yaratılıyor
  // Not: Kullanılan autodisposeUnregisteredInstances değişkeni
  // WifiSeqDataWriter yaratılırken ki kullananla aynıdır
  mgrReqPub.createWriter(autodisposeUnregisteredInstances,
                         KEEP_LAST_HISTORY_QOS, 1);

  // Yaratılan DataReader, ServerReq tipi için özelleştiriliyor.
  DataWriter_var dWriterPub = mgrReqPub.getWriter();
  ServerReqDataWriter_var serverReqWriter =
      ServerReqDataWriter::_narrow(dWriterPub.in());

  cout << "=== [Publisher of KonSensData_ServerReq_Topic_Node_to_Server] "
          "Ready ..." << endl;

  // Yenileme sıklığını belirleyecek olan 'timelimit' değişkeni yaratılıyor.
  Timeout timeLimit(std::chrono::milliseconds(refreshRate*1000));

  while(true)
  {
    if (timeLimit.isExpired() || refreshRateCounter == -1)
    {

      // Wifi mesajını Publish etmek için hazırlık yapılıyor.
      cout << "-----------------------------------" << endl;

      wifiSeqInstance->messageID = messageIDCount;

      // Tarama öncesi alınan zaman etiketi (timestamp[0])
      gettimeofday(&tsConverter, NULL);
      tsBeforeTheScan.sec = tsConverter.tv_sec;
      tsBeforeTheScan.nanosec = (tsConverter.tv_usec * 1000);
      cout << " timestamp[0] (before the scan) = " << tsBeforeTheScan.sec
           << '.';
      cout << setfill('0') << setw(9) << (tsBeforeTheScan.nanosec)
           << endl;

      // Wifi taraması yapılıyor ve ardından tarama sonuçları 'wifi_rssi'
      // dosyasına yazdırılıyor.
      system("sudo iwlist wlan0 scanning >> wifi_rssi.txt");

      // Wifi bağlantısı kapatılıyor
      system("sudo ifconfig wlan0 down");

      // Wifi tarama dosyası okunuyor
      fInWifiScan.open("wifi_rssi.txt",ios::in);

      stringstream ssWifi;
      ssWifi << fInWifiScan.rdbuf();
      wifiFileContenHolder = ssWifi.str();
      system("rm wifi_rssi.txt");
      try
      {
        // Okunan dosya boost kütüphane yardımıyla ayrıştırılıyor
        boost::regex exp( "Signal level=.*?([0-9]+)") ;
        boost::regex expAd( "Address: ([0-9A-F:]{17})" ) ;
        boost::match_results<string::const_iterator> whatAd;
        string::const_iterator startAd =
            wifiFileContenHolder.begin() ;
        string::const_iterator finishAd =
            wifiFileContenHolder.end() ;

        while (boost::regex_search(startAd, finishAd,
                                   whatAd, expAd))
        {
          modemIdHolder.push_back(whatAd[1]);
          startAd = whatAd[0].second ;
        }

        boost::match_results<string::const_iterator> what;

        string::const_iterator start = wifiFileContenHolder.begin();
        string::const_iterator finish = wifiFileContenHolder.end();

        while (boost::regex_search(start, finish, what, exp))
        {
          string foundDbm(what[1]);
          dbmHolder.push_back(atoi(foundDbm.c_str()));
          start = what[0].second ;
        }

        cout << " Number of Wifi Network that has been founded: "
             << dbmHolder.size() << endl;

        // Tarama sonrası alınan zaman etiketi (timestamp[1])
        gettimeofday(&tsConverter, NULL);
        tsAfterTheScan.sec = tsConverter.tv_sec;
        tsAfterTheScan.nanosec = (tsConverter.tv_usec * 1000);
        cout << " timestamp[1] (after the scan) = "
             << tsAfterTheScan.sec << '.';
        cout << setfill('0') << setw(9) << (tsAfterTheScan.nanosec)
             << endl;

        // Ayrıştırılan Wifi tarama dosyası ve alınan zaman etiketleri,
        // Publish edilecek olan mesaj değişkenlerine kaydediliyor
        wifiSeqInstance->timestamp[0][0] = tsBeforeTheScan.nanosec;
        wifiSeqInstance->timestamp[0][1] = tsBeforeTheScan.sec;
        wifiSeqInstance->timestamp[1][0] = tsAfterTheScan.nanosec;
        wifiSeqInstance->timestamp[1][1] = tsAfterTheScan.sec;

        wifiSeqInstance->messages.length(modemIdHolder.size());
        cout << " MessageID: " << wifiSeqInstance->messageID << endl;
        for(int i = 0; i < modemIdHolder.size(); i++)
        {
          Msg msg;
          msg.devID = DDS::string_dup(modemIdHolder[i].c_str());
          msg.hostName = DDS::string_dup(hostName);
          msg.dbm = -dbmHolder[i];
          wifiSeqInstance->messages[i] = msg;

        }

        // Publish edilmeden önce, bir önceki mesajın acknowlegde mesajı
        // bekleniyor
        wifiSeqWriter->wait_for_acknowledgments(waitAckTime);
        status = wifiSeqWriter->write(*wifiSeqInstance,DDS::HANDLE_NIL);

        checkStatus(status, "konsensSensDugWifiSeqTopic::"
                            "WifiSeqDataWriter::write_w_timestamp");

        // Wifi bağlantısı tekrar açılıyor
        system("sudo ifconfig wlan0 up");

        messageIDCount++;
      }

      catch ( boost::bad_expression & ex )
      {
        cout << ex.what() ;
      }

      modemIdHolder.clear();
      dbmHolder.clear();
      wifiFileContenHolder.clear();
      fInWifiScan.close();

      cout << "-----------------------------------" << endl;

      timeLimit.setTimerToZero();

      // Tarama sıklığını belirleyen değişken sıfırlanıyor
      refreshRateCounter = 0;
      cout << refreshRateCounter << endl;

    } // Wifi Publisher kısmının sonu


    // Sunucu tarafından gönderilen Matlab komutlarına Subscribe olunuyor
    else
    {
      status = serverReqReader->take(serverReqSeq,
                                     infoSeq,
                                     LENGTH_UNLIMITED,
                                     ANY_SAMPLE_STATE,
                                     ANY_VIEW_STATE,
                                     ANY_INSTANCE_STATE);
      checkStatus(status, "severReqDataReader::take");

      // isDataReceived değişkeni bir kontrol değişkeni olup, Matlab komut
      // bilgisini içeren mesaja Subscribe olunduysa true, olunmadıysa false
      // değerini tutar
      bool isDataReceived = false;

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

            // Reset komutunun gelip gelmediği kontrol ediliyor.
            Value::ConstMemberIterator itr = d.FindMember("Reset");
            if(itr != d.MemberEnd())
            {
              // Resetin değeri falsedan trueya çevriliyor
              Value& s = d["Reset"];
              s.SetBool(true);

              // Document formatındaki JSON mesajı StrinBuffer'a dönüştürülüyor
              Writer<StringBuffer> writer(buffer);
              d.Accept(writer);

              cout << "    Request Message is modified to  : "
                   << buffer.GetString() << endl;

              // Publish edilecek Response mesajı hazırlanıyor
              string str = buffer.GetString();
              str.append("\n");
              serverReq->request = DDS::string_dup(str.c_str());
              serverReq->requestID = serverReqSeq[j].requestID;

              if(!isDataReceived && status == DDS::RETCODE_OK)
              {
                // Response mesajı gönderiliyor
                ReturnCode_t tempStatus = serverReqWriter->write(
                      *serverReq, DDS::HANDLE_NIL);
                checkStatus(tempStatus, "severReqDataReader::write");
                isDataReceived = true;

                cout << "    Response Request Message is sent : "
                     << serverReq->request << endl;
                cout << "    Response RequestID is sent : \""
                     << serverReq->requestID << "\"" << endl;

              }

              // Sistem yeniden başlatılıyor !!!
              cout << "reboot" << endl;
              system("sudo reboot");
            }

            // GetRefreshRate ve SetRefreshRate komutundan herhangi birinin
            // gelip gelmediği kontrol ediliyor
            itr = d.FindMember("SetRefreshRate");
            if(itr != d.MemberEnd())
            {
              string refreshRateString;
              int refreshRateInt;

              // Status değeri falsedan trueya çevriliyor
              Value& s = d["Status"];
              s.SetBool(true);

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

              // Refresh rate değişkeni güncelleniyor
              refreshRate = refreshRateInt;
              timeLimit.setMaxDuration(std::chrono::milliseconds
                                       (refreshRate*1000));

            }

            itr = d.FindMember("GetRefreshRate");
            if(itr != d.MemberEnd())
            {
              // Status değeri falsedan trueya çevriliyor
              Value& s = d["Status"];
              s.SetBool(true);

              Value& tempRefreshRate = d["GetRefreshRate"];

              // Güncel refresh rate değişken bilgisi alınıyor
              string str = to_string(refreshRate);
              tempRefreshRate.SetString(str.c_str(), str.length());
            }

            // GetAliveStatus komutunun gönderilip gönderilmediği kontrol
            // ediliyor.
            itr = d.FindMember("Alive");
            if(itr != d.MemberEnd())
            {
              // Alive değeri truedan false çevriliyor
              Value& s = d["Alive"];
              s.SetBool(true);

              // Document formatındaki JSON mesajı StrinBuffer'a dönüştürülüyor
              Writer<StringBuffer> writer(buffer);
              d.Accept(writer);
            }

            // GetNetStatus komutunun gelip gelmediği kontrol ediliyor
            itr = d.FindMember("IP");
            if(itr != d.MemberEnd())
            {
              cout << "-----------------------------------" << endl;

              // Ağ durum bilgisi ifconfig.txt dosyasına yazdırılıyor
              system("cat /etc/network/interfaces >> ifconfig.txt");
              system("cat /etc/resolv.conf >> ifconfig.txt");
              system("echo -n hostname >> ifconfig.txt && hostname "
                     ">> ifconfig.txt");

              // ifconfig.txt dosyası açılıyor
              fInIfconfig.open("ifconfig.txt",ios::in);
              stringstream ssIfconfig;
              ssIfconfig << fInIfconfig.rdbuf();
              ifconfigFileContentHolder = ssIfconfig.str();
              system("rm ifconfig.txt");

              // Dosya içeriği ayrıştırılıyor
              try
              {
                boost::regex expIp
                    ("address (\\d{1,3}(\\.\\d{1,3}){3})");
                boost::regex expSubnet
                    ("netmask (\\d{1,3}(\\.\\d{1,3}){3})");
                boost::regex expGateway
                    ("gateway (\\d{1,3}(\\.\\d{1,3}){3})");
                boost::regex expDns
                    ("nameserver (\\d{1,3}(\\.\\d{1,3}){3})");
                boost::regex expNoneName
                    ("hostname.*");

                string::const_iterator startAd =
                    ifconfigFileContentHolder.begin() ;
                string::const_iterator finishAd =
                    ifconfigFileContentHolder.end() ;

                boost::match_results<string::const_iterator> whatIp;
                if ( boost::regex_search(startAd, finishAd,
                                         whatIp, expIp) )
                {
                  ip = whatIp[0];
                  cout << ip << endl;
                  string chopped_str = "address ";
                  string::size_type i = ip.find(chopped_str);
                  if (i != string::npos)
                    ip.erase(i, chopped_str.length());

                  Value& tempIp = d["IP"];
                  tempIp.SetString(ip.c_str(), ip.length());
                }

                startAd = ifconfigFileContentHolder.begin() ;
                finishAd = ifconfigFileContentHolder.end() ;

                boost::match_results<string::const_iterator> whatSubnet;
                if ( boost::regex_search(startAd, finishAd,
                                         whatSubnet, expSubnet) )
                {
                  subnet = whatSubnet[0];
                  cout << subnet << endl;
                  string chopped_str = "netmask ";
                  string::size_type i = subnet.find(chopped_str);
                  if (i != string::npos)
                    subnet.erase(i, chopped_str.length());

                  Value& tempSubnet = d["Subnet"];
                  tempSubnet.SetString(subnet.c_str(),
                                       subnet.length());

                }

                startAd = ifconfigFileContentHolder.begin() ;
                finishAd = ifconfigFileContentHolder.end() ;
                boost::match_results<string::const_iterator> whatGateway;

                if ( boost::regex_search(startAd, finishAd,
                                         whatGateway, expGateway) )
                {
                  gateway = whatGateway[0];
                  cout << gateway << endl;
                  string chopped_str = "gateway ";
                  string::size_type i = gateway.find(chopped_str);
                  if (i != string::npos)
                    gateway.erase(i, chopped_str.length());

                  Value& tempGateway = d["Gateway"];
                  tempGateway.SetString(gateway.c_str(),
                                        gateway.length());


                }

                startAd = ifconfigFileContentHolder.begin() ;
                finishAd = ifconfigFileContentHolder.end() ;
                boost::match_results<string::const_iterator> whatDns;

                if ( boost::regex_search(startAd, finishAd,
                                         whatDns, expDns) )
                {
                  dns = whatDns[0];
                  cout << dns << endl;
                  string chopped_str = "nameserver ";
                  string::size_type i = dns.find(chopped_str);
                  if (i != string::npos)
                    dns.erase(i, chopped_str.length());

                  Value& tempDns = d["DNS"];
                  tempDns.SetString(dns.c_str(), dns.length());
                }

                startAd = ifconfigFileContentHolder.begin() ;
                finishAd = ifconfigFileContentHolder.end() ;
                boost::match_results<string::const_iterator> whatNodeName;

                if ( boost::regex_search(startAd, finishAd,
                                         whatNodeName, expNoneName) )
                {
                  nodeName = whatNodeName[0];
                  cout << nodeName << endl;
                  string chopped_str = "hostname";
                  string::size_type i = nodeName.find(chopped_str);
                  if (i != string::npos)
                    nodeName.erase(i, chopped_str.length());
                  nodeName.erase(remove(nodeName.begin(),
                                        nodeName.end(),
                                        '\n'),
                                 nodeName.end());

                  Value& tempNodeName = d["Name"];
                  tempNodeName.SetString(nodeName.c_str(),
                                         nodeName.length());
                }

                cout << ip << endl;
                cout << subnet << endl;
                cout << dns << endl;
                cout << nodeName << endl;
              } // Dosya ayrıştırma işlemi bitti

              catch ( boost::bad_expression & ex )
              {
                cout << ex.what() ;
              }

              ifconfigFileContentHolder.clear();
              fInIfconfig.close();

              cout << "-----------------------------------" << endl;

              // GetNetStatus komutuna ait status değeri falsedan trueya
              // çevriliyor.
              Value& s = d["Status"];
              s.SetBool(true);
            }

            // Matlab'dan gelen mesaj içeriğinin ayrıştırılıp düzenlenme işlemi
            // tamamlandı ve sunucuya Response mesajını Publish etmek için
            // aşağıdaki adımlar gerçekleştiriliyor

            // Document formatındaki JSON mesajı StrinBuffer'a dönüştürülüyor
            StringBuffer responseBuffer;
            Writer<StringBuffer> writer(responseBuffer);
            d.Accept(writer);

            cout << "    Request Message is modified to  : "
                 << responseBuffer.GetString() << endl;

            // Response mesajı hazırlanıyor
            string str = responseBuffer.GetString();
            str.append("\n");
            serverReq->request = DDS::string_dup(str.c_str());
            //serverReq->request = DDS::string_dup("Hello World\n");
            serverReq->requestID = serverReqSeq[j].requestID;

            if(!isDataReceived && status == DDS::RETCODE_OK)
            {
              // Response mesajı Publish ediliyor
              ReturnCode_t tempStatus = serverReqWriter->write(
                    *serverReq, DDS::HANDLE_NIL);
              checkStatus(tempStatus, "severReqDataReader::write");
              isDataReceived = true;

              cout << "    Response Request Message is sent : "
                   << serverReq->request << endl;
              cout << "    Response RequestID is sent : \""
                   << serverReq->requestID << "\"" << endl;
            }
          }

          else
            cout << "Invalid NodeID!" << endl;
        }

      }

      status = serverReqReader->return_loan(serverReqSeq, infoSeq);
      checkStatus(status, "MsgDataReader::return_loan");

      refreshRateCounter++;
      cout << refreshRateCounter << endl;

    } // Matlab komutuna Subscribe olma kısmının sonu

    // Terminalde akacak olan çıktıları dah gözle görülebilir bir şekilde
    // yazdırmak için koyulmuştur
    fake_delay();
  }

  // Hafıza temizle işlemleri gerçekleştiriliyor

  status = wifiSeqWriter->dispose(*wifiSeqInstance, userHandle);
  checkStatus(status, "WifiSeqDataWriter::dispose");
  status = wifiSeqWriter->unregister_instance(*wifiSeqInstance, userHandle);
  checkStatus(status, "WifiSeqDataWriter::unregister_instance");

  delete wifiSeqInstance;
  delete serverReq;

  mgrWifiPub.deleteWriter();
  mgrWifiPub.deletePublisher();
  mgrWifiPub.deleteTopic();
  mgrWifiPub.deleteParticipant();

  mgrReqSub.deleteReader();
  mgrReqSub.deleteSubscriber();
  mgrReqSub.deleteTopic();
  mgrReqSub.deleteParticipant();

  mgrReqPub.deletePublisher();
  mgrReqPub.deleteWriter();
  mgrReqPub.deleteTopic();
  mgrReqPub.deleteParticipant();

  return 0;
}


int OSPL_MAIN (int argc, char *argv[])
{
  return NodeWifiHandler (argc, argv);
}
