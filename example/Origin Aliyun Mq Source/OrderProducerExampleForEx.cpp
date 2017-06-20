
#include "ONSFactory.h"
#include "ONSClientException.h"
#include <iostream>
using namespace ons;

int main()
{
    //producer���������������Ĳ�������������
    ONSFactoryProperty factoryInfo;
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ProducerId, "PID_xxxxxxxxxx");//��ONS����̨�����producerId
    factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics,"xxxxxxxxxxxx" );// ��ONS ����̨�����msg topic
    factoryInfo.setFactoryProperty(ONSFactoryProperty::MsgContent, "input msg content");//��Ϣ����
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "xxxxxxxxx");//ONS AccessKey
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, "xxxxxxxxxxxxxxxxxxx" );// ONS SecretKey

    //����producer
    OrderProducer *pProducer = ONSFactory::getInstance()->createOrderProducer(factoryInfo);


    //�ڷ�����Ϣǰ���������start����������Producer��ֻ�����һ�μ��ɡ�
    pProducer->start();

    Message msg(
                //Message Topic
                factoryInfo.getPublishTopics(),
                //Message Tag,�����ΪGmail�еı�ǩ������Ϣ�����ٹ��࣬����Consumerָ������������ONS����������
                "TagA",
                //Message Body���κζ�������ʽ�����ݣ�ONS�����κθ�Ԥ����ҪProducer��ConsumerЭ�̺�һ�µ����л��ͷ����л���ʽ
                factoryInfo.getMessageContent()
    );

    // ���ô�����Ϣ��ҵ��ؼ����ԣ��뾡����ȫ��Ψһ��
    // �Է��������޷������յ���Ϣ����£���ͨ��ONS Console��ѯ��Ϣ��������
    // ע�⣺������Ҳ����Ӱ����Ϣ�����շ�
    msg.setKey("ORDERID_100");

    //������Ϣ��ֻҪ�����쳣���ǳɹ�
  std::string key = "abc";  //һ��key����ͬ��key���ǰ���˳���͵�
	try
	{
		SendResultONS sendResult = pProducer->send(msg, key);
    std::cout << "send success" << std::endl;
	}
	catch(ONSClientException & e)
	{
		//��Ӷ�exception�Ĵ���
	}
    // ��Ӧ���˳�ǰ����������Producer���󣬷���ᵼ���ڴ�й¶������
    pProducer->shutdown();

    return 0;
}
