
#include "ONSFactory.h"
using namespace std;
using namespace ons;

//创建消费消息的实例
//pushConsumer拉取到消息后，会主动调用该实例的consumeMessage 函数
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
            //根据业务需求，消费消息
        return CommitMessage; //CONSUME_SUCCESS;
    }
};


int main(int argc, char* argv[])
{
    //pushConsumer创建和工作需要的参数，必须输入
    ONSFactoryProperty factoryInfo;
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumerId, "");//在ONS控制台申请的consumerId
    factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics,"" );// 在ONS 控制台申请的msg topic
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "");// ONS AccessKey
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey,  "");//ONS SecretKey

    // 创建pushConsumer
    PushConsumer* pushConsumer = ONSFactory::getInstance()->createPushConsumer(factoryInfo);
    MyMsgListener  msglistener;
    //指定pushConsumer 订阅的消息topic和消息tag
    pushConsumer->subscribe(factoryInfo.getPublishTopics(), "*",&msglistener );

    // 注册消息监听的处理实例，pushConsumer 拉取到消息后，会调用该类的consumeMessage 函数
    //MyMsgListener  msglistener;
    //pushConsumer->registerMessageListener(&msglistener);

    //启动pushConsumer
    pushConsumer->start();


    //销毁pushConsumer, 在应用退出前，必须销毁Consumer 对象，否则会导致内存泄露等问题
    pushConsumer->shutdown();

   return 0;
}
