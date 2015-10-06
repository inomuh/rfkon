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


#define FAKE_DELAY

void fake_delay()
{
#ifdef FAKE_DELAY
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
}

/* entry point exported and demangled so symbol can be found in shared
 * library */
extern "C"
{
OS_API_EXPORT
int NodeBtHandler (int argc, char *argv[]);
}

int NodeBtHandler (int argc, char *argv[])
{
    //Genel
    char hostname[1024];
    gethostname(hostname, 1024);
    os_time delay_1s = { 1, 0 };

    //BT
    DDSEntityManager                    mgrBtPub;
    DDSEntityManager                    mgrReqSub;
    ReturnCode_t                        status;
    SampleInfoSeq_var                   infoSeq = new SampleInfoSeq();
    BtSeq*                              btSeqInstance = new BtSeq();
    ServerReqSeq                        serverReqSeq;
    InstanceHandle_t                    userHandle;

    long int                            messageIDCount = 0;
    Time_t                              tsBeforeTheScan = { 0, 0 };
    Time_t                              tsAfterTheScan = { 0, 0 };
    Time_t                              tsWifiPub = { 0, 0 };

    struct timeval                      tsConverter;
    DDS::Duration_t                     waitAckTime = { 0, 800000000 }; //800ms
    int                                 refreshRate = 60;

    char                                hostName[1024];
    gethostname(hostName, 1024);
    vector<string>                      btMacHolder;
    vector<int>                         btRssiHolder;
    string                              btFileContenHolder;
    boost::filesystem::ifstream         fIn;
    int                                 refreshRateCounter = -1;

    // create domain participant
    mgrBtPub.createParticipant("KonSens_BtSeq_Participant");

    //create type
    BtSeqTypeSupport_var btSeqTs = new BtSeqTypeSupport();
    mgrBtPub.registerType(btSeqTs.in());

    //create Topic
    char btPubTopicName[] = "KonSensData_BtSeq_Topic";
    mgrBtPub.createTopic(btPubTopicName);

    //create Publisher
    mgrBtPub.createPublisher();

    // create DataWriter :
    // If autodispose_unregistered_instances is set to true (default value),
    // you will have to start the subscriber before the publisher
    bool autodispose_unregistered_instances = false;
    mgrBtPub.createWriter(autodispose_unregistered_instances,
                          KEEP_ALL_HISTORY_QOS,
                          BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS);

    // Publish Events
    DataWriter_var dWriter = mgrBtPub.getWriter();
    BtSeqDataWriter_var btSeqWriter = BtSeqDataWriter::_narrow(dWriter.in());

    btSeqInstance->userID = 11;

    /* Register a WifiSeq message for this user (pre-allocating resources for
     * it!!) */
    userHandle = btSeqWriter->register_instance_w_timestamp(*btSeqInstance,
                                                            tsWifiPub);
    cout << "=== [Publisher of KonSensData_BtSeq_Topic] Ready ..." << endl;


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
    mgrReqSub.createReader(KEEP_LAST_HISTORY_QOS,
                           1,
                           BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS);

    DataReader_var dReaderSub = mgrReqSub.getReader();
    ServerReqDataReader_var serverReqReader =
            ServerReqDataReader::_narrow(dReaderSub.in());
    checkHandle(serverReqReader.in(), "ServerReqDataReader::_narrow");
    cout << "=== [Subscriber KonSensData_ServerReq_Topic_Server_to_Node]"
            " Ready ..." << endl;


    //Bluetooth Aktif hale getiriliyor.
    system("sudo hciconfig -a hci0 up");
    stringstream ssBtName;
    ssBtName << "sudo bt-adapter -a hci0 --set Name \"" << hostname << "\"";
    system(ssBtName.str().c_str());
    system("sudo bt-adapter -a hci0 --set Discoverable 1");

    Timeout timelimit(std::chrono::milliseconds(refreshRate*1000));

    while(1){

        if (timelimit.isExpired() || refreshRateCounter == -1)
        {

            /* Publishing the Wifi scan messages */

            cout << "-----------------------------------" << endl;

            btSeqInstance->messageID = messageIDCount;

            // timestamp[0]
            gettimeofday(&tsConverter, NULL);
            tsBeforeTheScan.sec = tsConverter.tv_sec;
            tsBeforeTheScan.nanosec = (tsConverter.tv_usec * 1000);
            cout << " timestamp[0] (before the scan) = " << tsBeforeTheScan.sec
                 << '.';
            cout << setfill('0') << setw(9) << (tsBeforeTheScan.nanosec)
                 << endl;

            //BT
            system("sudo hciconfig hci0 reset");
            os_nanoSleep(delay_1s);
            system("sudo bt-adapter -a hci0 -d >> bt_rssi.txt");
            os_nanoSleep(delay_1s);


            cout << "DDS sent operation" << endl;

            try
            {

                //BT
                fIn.open("bt_rssi.txt", ios::in);
                stringstream ssBt;
                ssBt << fIn.rdbuf();
                btFileContenHolder = ssBt.str();
                system("rm bt_rssi.txt");

                boost::regex expAd( "Address: ([0-9A-F:]{17})" ) ;
                boost::regex expBt( "RSSI:.*?([0-9]+)") ;
                boost::match_results<string::const_iterator> whatAd;

                string::const_iterator startAd = btFileContenHolder.begin();
                string::const_iterator finishAd = btFileContenHolder.end();

                while ( boost::regex_search(startAd, finishAd, whatAd, expAd) )
                {
                    btMacHolder.push_back(whatAd[1]);
                    startAd = whatAd[0].second ;

                }


                boost::match_results<string::const_iterator> whatBt;

                startAd = btFileContenHolder.begin() ;
                finishAd = btFileContenHolder.end() ;

                while ( boost::regex_search(startAd, finishAd, whatBt, expBt) )
                {
                    string foundRssi(whatBt[1]);
                    btRssiHolder.push_back(atoi(foundRssi.c_str()));
                    startAd = whatBt[0].second ;
                }

                cout << "Number of BT connection that has been found: "
                     << btRssiHolder.size() << endl;
                cout << "MessageID: " << btSeqInstance->messageID << endl;

                gettimeofday(&tsConverter, NULL);
                tsAfterTheScan.sec = tsConverter.tv_sec;
                tsAfterTheScan.nanosec =( tsConverter.tv_usec * 1000);
                cout << " timestamp[1] (after the scan) = "
                     << tsAfterTheScan.sec << '.';
                cout << setfill('0') << setw(9) << (tsAfterTheScan.nanosec)
                     << endl;

                btSeqInstance->timestamp[0][0] = tsBeforeTheScan.nanosec;
                btSeqInstance->timestamp[0][1] = tsBeforeTheScan.sec;
                btSeqInstance->timestamp[1][0] = tsAfterTheScan.nanosec;
                btSeqInstance->timestamp[1][1] = tsAfterTheScan.sec;

                /**
            * DDS related stuff - EAX
            **/
                btSeqInstance->messages.length(btMacHolder.size());
                for(int i = 0; i < btMacHolder.size(); i++)
                {
                    Msg msg;
                    msg.devID = DDS::string_dup(btMacHolder[i].c_str());
                    msg.hostName = DDS::string_dup(hostName);
                    msg.dbm = -btRssiHolder[i];
                    btSeqInstance->messages[i] = msg;
                }

                btSeqWriter->wait_for_acknowledgments(waitAckTime);

                ReturnCode_t status = btSeqWriter->write(*btSeqInstance,
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

            timelimit.setTimerToZero();

            // restart the counter
            refreshRateCounter = 0;
            cout << refreshRateCounter << endl;


        }
        /* end loop seq */



        /* Subscribing the Matlab commands from Server */
        else
        {

            status = serverReqReader->take(serverReqSeq,
                                           infoSeq,
                                           LENGTH_UNLIMITED,
                                           ANY_SAMPLE_STATE,
                                           ANY_VIEW_STATE,
                                           ANY_INSTANCE_STATE);
            checkStatus(status, "severReqDataReader::read");

            // isDataReceived is a control variable that enables
            // the ServerRequester Publisher
            // to check before sends the Response message
            // bool isDataReceived = false;

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
                    d.Parse(serverReqSeq[j].request);

                    Value::ConstMemberIterator itr = d.FindMember("Ref Rate");
                    if(itr != d.MemberEnd())
                    {
                        string refreshRateString;
                        int refreshRateInt;

                        Value& tempRefreshRate = d["Ref Rate"];
                        refreshRateString = tempRefreshRate.GetString();
                        stringstream(refreshRateString) >> refreshRateInt;

                        if (refreshRateInt != -1)
                        {
                            refreshRate = refreshRateInt;
                            timelimit.setMaxDuration(std::chrono::milliseconds
                                                     (refreshRate*1000));
                        }
                    }
                }

            }


            status = serverReqReader->return_loan(serverReqSeq, infoSeq);
            checkStatus(status, "MsgDataReader::return_loan");

            refreshRateCounter++;
            cout << refreshRateCounter << endl;

        }

        fake_delay();
    }

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
