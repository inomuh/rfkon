/*
 * DDSEntityManagerKonSens.h
 *
 * AMAÇ:
 *  DDS'e ait Entity'leri (Domain, Topic, Publisher, Writer etc.) yaratmada
 *  bize kolaylık sağlaması için oluşturulmuş bir nesnedir. Bu kapsülleme
 *  sayesinde oluşturalacak olan DDS değişkenleri kolayca konfigüre edilebilir.
 *
 * NOTLAR:
 *  Yazar: Uğur Bolat
 *  Tarih: 15.10.2015
 *
 *   Versiyon: v_1.0
 *    Güncelleme Tarihi: 15.10.2015
 *
 */

#ifndef SRC_DDSENTITYMANAGERKONSENS_H_
#define SRC_DDSENTITYMANAGERKONSENS_H_

#include "ccpp_dds_dcps.h"
#include "CheckStatus.h"
using namespace DDS;

class DDSEntityManager
{

  /* Generic DDS entities */
  DomainParticipantFactory_var 		dpf;
  DomainParticipant_var 		participant;
  Topic_var 				topic;
  Publisher_var 			publisher;
  Subscriber_var 			subscriber;
  DataWriter_var 			writer;
  DataReader_var 			reader;

  /* QosPolicy holders */
  TopicQos 				reliable_topic_qos;
  TopicQos 				setting_topic_qos;
  PublisherQos 				pub_qos;
  DataWriterQos 			dw_qos;
  DataReaderQos				dr_qos;
  SubscriberQos 			sub_qos;

  DomainId_t 				domain;
  ReturnCode_t 				status;

  DDS::String_var 			partition;
  DDS::String_var 			typeName;

public:
  void createParticipant(const char *partitiontName);
  void deleteParticipant();
  void registerType(TypeSupport *ts);
  void createTopic(char *topicName);
  void createTopic(char *topicName,
                   DDS::ReliabilityQosPolicyKind reliability_kind,
                   DDS::DurabilityQosPolicyKind durability_kind);
  void deleteTopic();
  void createPublisher();
  void deletePublisher();
  void createWriter();
  void createWriter(bool autodispose_unregistered_instances);
  void createWriter(bool autodispose_unregistered_instances,
                    DDS::HistoryQosPolicyKind history_kind);
  void createWriter(bool autodispose_unregistered_instances,
                    DDS::HistoryQosPolicyKind history_kind,
                    int history_depth);
  void createWriter(bool autodispose_unregistered_instances,
                    DDS::HistoryQosPolicyKind history_kind,
                    DDS::DestinationOrderQosPolicyKind destination_order);
  void createWriter(bool autodispose_unregistered_instances,
                    DDS::HistoryQosPolicyKind history_kind,
                    int history_depth,
                    DDS::DestinationOrderQosPolicyKind destination_order);
  void deleteWriter();
  void createSubscriber();
  void deleteSubscriber();
  void createReader();
  void createReader(DDS::HistoryQosPolicyKind history_kind,
                    DDS::DestinationOrderQosPolicyKind destination_order);
  void createReader(DDS::HistoryQosPolicyKind history_kind);
  void createReader(DDS::HistoryQosPolicyKind history_kind,
                    int history_depth,
                    DDS::DestinationOrderQosPolicyKind destination_order);
  void createReader(DDS::HistoryQosPolicyKind history_kind,
                    int history_depth);
  void deleteReader();
  DataReader_ptr getReader();
  DataWriter_ptr getWriter();
  Publisher_ptr getPublisher();
  Subscriber_ptr getSubscriber();
  Topic_ptr getTopic();
  DomainParticipant_ptr getParticipant();
  ~DDSEntityManager();
};



#endif /* SRC_DDSENTITYMANAGERKONSENS_H_ */
