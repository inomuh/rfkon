#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <fstream>

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

    SampleInfoSeq_var               infoSeq = new SampleInfoSeq(); // for serverReqWriter
    ServerReq                       *serverReq = new ServerReq();
    ReturnCode_t                    status;
    ServerReqSeq                    serverReqSeq;
    DDSEntityManager                mgr_pub;
    DDSEntityManager                mgr_sub;
//    bool isMatlabCommandReceived = false;
//    bool isMatlabCommandSent = false;
//    bool isMatlabResponseSent = false;


    // ********************Publisher**************
    // create domain participant
    mgr_pub.createParticipant("KonSensData_ServerReq_Participant_Server_to_Node");

    //create type
    ServerReqTypeSupport_var mgrPublisherTS = new ServerReqTypeSupport();
    mgr_pub.registerType(mgrPublisherTS.in());

    //create Topic
    char pub_topic_name[] = "KonSensData_ServerReq_Topic_Server_to_Node";
    mgr_pub.createTopic(pub_topic_name, RELIABLE_RELIABILITY_QOS, VOLATILE_DURABILITY_QOS);

    //create Publisher
    mgr_pub.createPublisher();

    // create DataWriter :
    // If autodispose_unregistered_instances is set to true (default value),
    // you will have to start the subscriber before the publisher
    bool autodispose_unregistered_instances = false;
    mgr_pub.createWriter(autodispose_unregistered_instances, KEEP_LAST_HISTORY_QOS, 1);

    // Publish Events
    DataWriter_var ServerRequesterWriter = mgr_pub.getWriter();
    ServerReqDataWriter_var serverReqWriter = ServerReqDataWriter::_narrow(ServerRequesterWriter.in());
    cout << "=== [Publisher] Ready ..." << endl;



    // ********************Subscriber**************
    // create domain participant
    mgr_sub.createParticipant("KonSensData_ServerReq_Participant_Node_to_Server");

    //create type
    ServerReqTypeSupport_var mgrSubscriberTS = new ServerReqTypeSupport();
    mgr_sub.registerType(mgrSubscriberTS.in());

    //create Topic
    char sub_topic_name[] = "KonSensData_ServerReq_Topic_Node_to_Server";
    mgr_sub.createTopic(sub_topic_name, RELIABLE_RELIABILITY_QOS, VOLATILE_DURABILITY_QOS);

    //create Subscriber
    mgr_sub.createSubscriber();

    // create DataReader
    mgr_sub.createReader(KEEP_LAST_HISTORY_QOS, 1);

    DataReader_var dreader = mgr_sub.getReader();
    ServerReqDataReader_var serverReqReader = ServerReqDataReader::_narrow(dreader.in());
    checkHandle(serverReqReader.in(), "ServerReqDataReader::_narrow");
    cout << "=== [Subscriber] Ready ..." << endl;



    serverReq->requestID = 0;

    // TCP/IP and DDS Writer

    if (argc < 2 || argc > 4) {
        printf("usage: server <port> [<ip>]\n");
        exit(1);
    }

    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    if (argc == 3) {
        acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }
    else {
        acceptor = new TCPAcceptor(atoi(argv[1]));
    }

///    acceptor = new TCPAcceptor(9999, "localhost");

    if (acceptor->start() == 0) {

        while (1) {           
            stream = acceptor->accept();
            if (stream != NULL) {
                ssize_t len;
                char line[256];
                //bool isReset = false;
                bool isMatlabCommandReceived = false;
                bool isMatlabCommandSent = false;
                bool isMatlabResponseSent = false;

                while ((len = stream->receive(line, sizeof(line))) > 0)
                {
                    isMatlabCommandReceived = true;
                    line[len] = 0;
                    printf("received - %s\n", line);
                    //stream->send(line, len);

                    serverReq->request = DDS::string_dup(line);
                    status = serverReqWriter->write(*serverReq, DDS::HANDLE_NIL);
                    checkStatus(status, "ServerReqDataWriter::write");
                    isMatlabCommandSent = true;
                    serverReq->requestID++;
                    cout << "=== [Publisher] message is sent :" << endl;

                    cout << "Debug1" << endl;

                    if(isMatlabCommandReceived && isMatlabCommandSent)
                    {
                        while(!isMatlabResponseSent)
                        {
                            //stringstream ss;
                            char* c_temp_message;
                            //string str_temp_message;

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
                                cout << status << endl;
                                cout << "=== [Subscriber] message received :" << endl;
                                cout << "    Response Request Message is received  : " << serverReqSeq[j].request << endl;
                                cout << "    Response RequestID is received : \"" << serverReqSeq[j].requestID << "\"" << endl;

                                c_temp_message = DDS::string_dup(serverReqSeq[j].request);
                                //ss << c_temp_message;
                                //ss >> str_temp_message;
                                string str_temp_message(c_temp_message);

                                // sending response message, is received from Server, to the Matlab Simulator back
                                stream->send(str_temp_message.c_str(), str_temp_message.size());
                                cout << "   Response Message is sent to Matlab Simulator " << str_temp_message << endl;

                                isMatlabResponseSent = true;
                            }
                        }

                        status = serverReqReader->return_loan(serverReqSeq, infoSeq);
                        checkStatus(status, "MsgDataReader::return_loan");

                        os_nanoSleep(delay_2ms);
                        }
                    }

                }

                delete stream;
            }



        }
    }

    os_nanoSleep(delay_2ms);


    //cleanup
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


