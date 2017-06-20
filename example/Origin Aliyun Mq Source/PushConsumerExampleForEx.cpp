
#include "ONSFactory.h"
using namespace std;
using namespace ons;

//����������Ϣ��ʵ��
//pushConsumer��ȡ����Ϣ�󣬻��������ø�ʵ����consumeMessage ����
class ONSCLIENT_API MyMsgListener : public MessageListener
{
public:
    MyMsgListener()
    {
    }
    virtual ~MyMsgListener()
    {
    }

    virtual Action consume(Message &message, ConsumeContext &context) 
    {
            //����ҵ������������Ϣ
        return CommitMessage; //CONSUME_SUCCESS;
    }
};


int main(int argc, char* argv[])
{
    //pushConsumer�����͹�����Ҫ�Ĳ�������������
    ONSFactoryProperty factoryInfo;
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumerId, "");//��ONS����̨�����consumerId
    factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics,"" );// ��ONS ����̨�����msg topic
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "");// ONS AccessKey
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey,  "");//ONS SecretKey

    // ����pushConsumer
    PushConsumer* pushConsumer = ONSFactory::getInstance()->createPushConsumer(factoryInfo);
    MyMsgListener  msglistener;
    //ָ��pushConsumer ���ĵ���Ϣtopic����Ϣtag
    pushConsumer->subscribe(factoryInfo.getPublishTopics(), "*",&msglistener );

    // ע����Ϣ�����Ĵ���ʵ����pushConsumer ��ȡ����Ϣ�󣬻���ø����consumeMessage ����
    //MyMsgListener  msglistener;
    //pushConsumer->registerMessageListener(&msglistener);

    //����pushConsumer
    pushConsumer->start();


    //����pushConsumer, ��Ӧ���˳�ǰ����������Consumer ���󣬷���ᵼ���ڴ�й¶������
    pushConsumer->shutdown();

   return 0;
}
