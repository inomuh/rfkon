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

/* entry point exported and demangled so symbol can be found in shared library */
extern "C"
{
OS_API_EXPORT
int NodeWifiHandler (int argc, char *argv[]);
}

int NodeWifiHandler (int argc, char *argv[])
{

    ReturnCode_t                        status;
    InstanceHandle_t                    userHandle;
    DDSEntityManager                    mgrReqSub;
    DDSEntityManager                    mgrReqPub;
    DDSEntityManager                    mgrWifiPub;
    WifiSeq                             *wifiSeqInstance;
    ServerReqSeq                        serverReqSeq;
    ServerReq                           *serverReq = new ServerReq();
    SampleInfoSeq_var                   infoSeq = new SampleInfoSeq();

    DDS::Duration_t                     waitAckTime = { 0, 800000000 }; //800ms
    int                                 refreshRate = 60;
    long int                            messageIDCount = 0;
    Time_t                              tsBeforeTheScan = { 0, 0 };
    Time_t                              tsAfterTheScan = { 0, 0 };

    struct timeval                      tsConverter;

    char                                hostName[1024];
    gethostname(hostName, 1024);
    vector<string>                      modemIdHolder;
    vector<int>                         dbmHolder;
    string                              wifiFileContenHolder;
    string                              ifconfigFileContentHolder;
    boost::filesystem::ifstream         fInWifiScan, fInIfconfig;
    string                              ip, subnet, gateway, dns, nodeName;


    // 'refreshRateCounter' is a control variable that enables the DDS
    // application toboth publish the Wifi messages and
    // subscribe the ServerRequester messages.
    int refreshRateCounter = -1;


    /* Creating the 'wifiSeqWriter' function to publish the Wifi scan
     * messages */

    // create domain participant
    mgrWifiPub.createParticipant("KonSensData_WifiSeq_Participant");

    //create type
    WifiSeqTypeSupport_var wifiSeqTS = new WifiSeqTypeSupport();
    mgrWifiPub.registerType(wifiSeqTS.in());

    //create Topic
    char wifiPubTopicName[] = "KonSensData_WifiSeq_Topic";
    mgrWifiPub.createTopic(wifiPubTopicName);

    //create Publisher
    mgrWifiPub.createPublisher();

    //create DataWriter
    bool autodisposeUnregisteredInstances = false;
    mgrWifiPub.createWriter(autodisposeUnregisteredInstances,
                            KEEP_ALL_HISTORY_QOS,
                            BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS);

    // Publish Events
    DataWriter_var dWriterWifPub = mgrWifiPub.getWriter();
    WifiSeqDataWriter_var wifiSeqWriter =
            WifiSeqDataWriter::_narrow(dWriterWifPub.in());
    cout << "=== [Publisher of KonSensData_WifiSeq_Topic] Ready ..." << endl;

    wifiSeqInstance = new WifiSeq();
    checkHandle(wifiSeqInstance, "new konsensSensDugWifiSeqTopic WifiSeq");
    wifiSeqInstance->userID = 11;


    /* Creating 'serverReqReader' to receive the respone messages from Server */

    // create domain participant
    mgrReqSub.createParticipant(
                "KonSensData_ServerReq_Participant_Server_to_Node");

    //create type
    ServerReqTypeSupport_var mgrSubscriberTS = new ServerReqTypeSupport();
    mgrReqSub.registerType(mgrSubscriberTS.in());

    //create Topic
    char reqSubTopicName[] = "KonSensData_ServerReq_Topic_Server_to_Node";
    mgrReqSub.createTopic(reqSubTopicName, RELIABLE_RELIABILITY_QOS,
                          VOLATILE_DURABILITY_QOS);

    //create Subscriber
    mgrReqSub.createSubscriber();

    // create DataReader
    mgrReqSub.createReader(KEEP_LAST_HISTORY_QOS, 1);

    DataReader_var dReadeSub = mgrReqSub.getReader();
    ServerReqDataReader_var serverReqReader =
            ServerReqDataReader::_narrow(dReadeSub.in());
    checkHandle(serverReqReader.in(), "ServerReqDataReader::_narrow");
    cout << "=== [Subscriber of KonSensData_ServerReq_Topic_Server_to_Node]"
            " Ready ..." << endl;




    /* Creating serverReqWriter to send the respone messages to Server */

    // create domain participant
    mgrReqPub.createParticipant(
                "KonSensData_ServerReq_Participant_Node_to_Server");

    //create type
    ServerReqTypeSupport_var mgrPublisherTS = new ServerReqTypeSupport();
    mgrReqPub.registerType(mgrPublisherTS.in());

    //create Topic
    char reqPubTopicName[] = "KonSensData_ServerReq_Topic_Node_to_Server";
    mgrReqPub.createTopic(reqPubTopicName);

    //create Publisher
    mgrReqPub.createPublisher();

    //create DataWriter
    //bool autodispose_unregistered_instances = false;
    mgrReqPub.createWriter(autodisposeUnregisteredInstances,
                           KEEP_LAST_HISTORY_QOS, 1);

    // Publish Events
    DataWriter_var dWriterPub = mgrReqPub.getWriter();
    ServerReqDataWriter_var serverReqWriter =
            ServerReqDataWriter::_narrow(dWriterPub.in());
    cout << "=== [Publisher of KonSensData_ServerReq_Topic_Node_to_Server] "
            "Ready ..." << endl;

    Timeout timeLimit(std::chrono::milliseconds(refreshRate*1000));

    while(1)
    {
        if (timeLimit.isExpired() || refreshRateCounter == -1)
        {

            /* Publishing the Wifi scan messages */
            cout << "-----------------------------------" << endl;
            cout << " Timeout is expired and sending Wifi messages..." << endl;

            wifiSeqInstance->messageID = messageIDCount;

            // saving the time into timestamp[0] before the Wifi scan
            gettimeofday(&tsConverter, NULL);
            tsBeforeTheScan.sec = tsConverter.tv_sec;
            tsBeforeTheScan.nanosec = (tsConverter.tv_usec * 1000);
            cout << " timestamp[0] (before the scan) = " << tsBeforeTheScan.sec
                 << '.';
            cout << setfill('0') << setw(9) << (tsBeforeTheScan.nanosec)
                 << endl;

            system("sudo iwlist wlan0 scanning >> wifi_rssi.txt");
            system("sudo ifconfig wlan0 down");

            // opening the wifi_rssi file to read.
            fInWifiScan.open("wifi_rssi.txt",ios::in);

            stringstream ssWifi;
            ssWifi << fInWifiScan.rdbuf();
            wifiFileContenHolder = ssWifi.str();
            system("rm wifi_rssi.txt");
            try
            {
                boost::regex exp( "Signal level=.*?([0-9]+)") ;
                boost::regex expAd( "Address: ([0-9A-F:]{17})" ) ;
                boost::match_results<string::const_iterator> whatAd;
                string::const_iterator startAd =
                        wifiFileContenHolder.begin() ;
                string::const_iterator finishAd =
                        wifiFileContenHolder.end() ;

                while ( boost::regex_search(startAd, finishAd,
                                            whatAd, expAd) )
                {
                    modemIdHolder.push_back(whatAd[1]);
                    startAd = whatAd[0].second ;
                }

                boost::match_results<string::const_iterator> what;

                string::const_iterator start = wifiFileContenHolder.begin();
                string::const_iterator finish = wifiFileContenHolder.end();

                while ( boost::regex_search(start, finish, what, exp) )
                {
                    string foundDbm(what[1]);
                    dbmHolder.push_back(atoi(foundDbm.c_str()));
                    start = what[0].second ;
                }

                cout << " Number of Wifi Network that has been founded: "
                     << dbmHolder.size() << endl;

                // saving the time into timestamp[1] after the Wifi scan
                gettimeofday(&tsConverter, NULL);
                tsAfterTheScan.sec = tsConverter.tv_sec;
                tsAfterTheScan.nanosec = (tsConverter.tv_usec * 1000);
                cout << " timestamp[1] (after the scan) = "
                     << tsAfterTheScan.sec << '.';
                cout << setfill('0') << setw(9) << (tsAfterTheScan.nanosec)
                     << endl;

                // preparing instances of the 'wifiSeqInstance'
                // which will contain the Wifi data
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

                wifiSeqWriter->wait_for_acknowledgments(waitAckTime);
                status = wifiSeqWriter->write(*wifiSeqInstance,DDS::HANDLE_NIL);

                checkStatus(status, "konsensSensDugWifiSeqTopic::"
                                    "WifiSeqDataWriter::write_w_timestamp");

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

            // restart the counter
            refreshRateCounter = 0;
            cout << refreshRateCounter << endl;

        }


        else
        {
            /* Subscribing the Matlab commands from Server */

            status = serverReqReader->take(serverReqSeq,
                                           infoSeq,
                                           LENGTH_UNLIMITED,
                                           ANY_SAMPLE_STATE,
                                           ANY_VIEW_STATE,
                                           ANY_INSTANCE_STATE);
            checkStatus(status, "severReqDataReader::take");

            // isDataReceived is a control variable that enables
            // the ServerRequester Publisher
            // to check before sends the Response message
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

                    // creating a rapidjson constructor
                    Document d;
                    if(d.Parse(serverReqSeq[j].request).HasParseError())
                        cout << " Parsing Error!" << endl;

                    StringBuffer nodeIdBuffer;
                    Writer<StringBuffer> nodeIdWriter(nodeIdBuffer);
                    d["NodeID"].Accept(nodeIdWriter);

                    string tempNodeId = nodeIdBuffer.GetString();

                    if (tempNodeId == "\"SensDug11\"")
                    {

                        StringBuffer buffer;
                        Value::ConstMemberIterator itr = d.FindMember("Reset");
                        if(itr != d.MemberEnd())
                        {
                            // Modify it by DOM.
                            Value& s = d["Reset"];
                            s.SetBool(true);

                            // Stringify the DOM
                            Writer<StringBuffer> writer(buffer);
                            d.Accept(writer);

                            cout << "    Request Message is modified to  : "
                                 << buffer.GetString() << endl;

                            // Sending back the modified message to Server
                            serverReq->request = DDS::string_dup(buffer.GetString());
                            serverReq->requestID = serverReqSeq[j].requestID;


                            if(!isDataReceived && status == DDS::RETCODE_OK)
                            {
                                ReturnCode_t tempStatus = serverReqWriter->write(
                                            *serverReq, DDS::HANDLE_NIL);
                                checkStatus(tempStatus, "severReqDataReader::write");
                                isDataReceived = true;

                                cout << "    Response Request Message is sent : "
                                     << serverReq->request << endl;
                                cout << "    Response RequestID is sent : \""
                                     << serverReq->requestID << "\"" << endl;
                            }

                            // rebooting !!!
                            system("sudo reboot");
                        }

                        itr = d.FindMember("Ref Rate");
                        if(itr != d.MemberEnd())
                        {
                            string refreshRateString;
                            int refreshRateInt;
                            // Modify it by DOM.
                            Value& s = d["Status"];
                            s.SetBool(true);

                            Value& tempRefreshRate = d["Ref Rate"];

                            // Stringify the DOM
                            Writer<StringBuffer> writer(buffer);
                            d["Ref Rate"].Accept(writer);

                            refreshRateString = buffer.GetString();


                            refreshRateInt = atoi(refreshRateString.c_str());


                            //cout << refreshRateString.length()<< endl;

                            if (refreshRateString != "\"-1\"")
                            {
                                refreshRate = refreshRateInt;
                                timeLimit.setMaxDuration(std::chrono::milliseconds
                                                         (refreshRate*1000));
                            }

                            else
                            {
                                string str = to_string(refreshRate);
                                tempRefreshRate.SetString(str.c_str(), str.length());
                            }

                        }

                        itr = d.FindMember("Alive");
                        if(itr != d.MemberEnd())
                        {
                            // Modify it by DOM.
                            Value& s = d["Alive"];
                            s.SetBool(true);

                            // Stringify the DOM
                            Writer<StringBuffer> writer(buffer);
                            d.Accept(writer);
                        }

                        itr = d.FindMember("IP");
                        if(itr != d.MemberEnd())
                        {
                            //String Ip, subnet, gateway, dns, nodeName;
                            cout << "-----------------------------------" << endl;

                            system("cat /etc/network/interfaces >> ifconfig.txt");
                            system("cat /etc/resolv.conf >> ifconfig.txt");
                            system("echo -n hostname >> ifconfig.txt && hostname "
                                   ">> ifconfig.txt");

                            // wifi rssi açılıyor.
                            fInIfconfig.open("ifconfig.txt",ios::in);
                            stringstream ssIfconfig;
                            ssIfconfig << fInIfconfig.rdbuf();
                            ifconfigFileContentHolder = ssIfconfig.str();
                            system("rm ifconfig.txt");

                            // parsing the ifconfig data
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

                            }
                            catch ( boost::bad_expression & ex )
                            {
                                cout << ex.what() ;
                            }

                            ifconfigFileContentHolder.clear();
                            fInIfconfig.close();

                            cout << "-----------------------------------" << endl;

                            // Modify it by DOM.
                            Value& s = d["Status"];
                            s.SetBool(true);

                            //                         // Stringify the DOM
                            //                         Writer<StringBuffer> writer(buffer);
                            //                         d.Accept(writer);

                        }


                        // Stringify the DOM
                        StringBuffer responseBuffer;
                        Writer<StringBuffer> writer(responseBuffer);
                        d.Accept(writer);

                        // Output {"project":false}
                        cout << "    Request Message is modified to  : "
                             << responseBuffer.GetString() << endl;

                        // Sending back the modified message to Server
                        serverReq->request = DDS::string_dup(responseBuffer.GetString());
                        serverReq->requestID = serverReqSeq[j].requestID;


                        if(!isDataReceived && status == DDS::RETCODE_OK)
                        {
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

        }

        // to able to observe the readalbe output on the terminal
        fake_delay();
    }


    /* Leave the room by disposing and unregistering the message instance. */
    status = wifiSeqWriter->dispose(*wifiSeqInstance, userHandle);
    checkStatus(status, "WifiSeqDataWriter::dispose");
    status = wifiSeqWriter->unregister_instance(*wifiSeqInstance, userHandle);
    checkStatus(status, "WifiSeqDataWriter::unregister_instance");


    /* Release the data-samples. */
    delete wifiSeqInstance;
    delete serverReq;

    // cleanup
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
