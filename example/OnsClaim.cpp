#include "ONSFactory.h"
#include <iostream>

#include "restclient-cpp/restclient.h"

#include <jsoncons/json.hpp>
#include <openssl/md5.h>
// For convenience
using jsoncons::json;

using namespace std;
using namespace ons;

//md5加密方法
string calculatemd5(string msg)
{
	string result;
	const char* test = msg.c_str();
	int i;

	MD5_CTX md5;

	MD5_Init (&md5);
	MD5_Update (&md5, (const unsigned char *) test, msg.length());
	unsigned char buffer_md5[16];
	MD5_Final ( buffer_md5, &md5);
	printf("Input: %s", test);
	printf("\nMD5: ");
	char buf[32];
	for (i=0;i<16;i++){
		sprintf(buf, "%02x", buffer_md5[i]);
		result.append( buf );
	}
	//std::cout <<"\nResult:"<< result[i]<<endl;
	return result;
}



//创建消费消息的实例
//pushConsumer拉取到消息后，会主动调用该实例的consumeMessage 函数
class ONSCLIENT_API MyMsgListener : public MessageOrderListener
{
public:
    MyMsgListener()
    {
    }
    virtual ~MyMsgListener()
    {
    }

    virtual OrderAction consume(Message &message, ConsumeOrderContext &context)
    {
		std::string APPAK = "3458D3A6A232AFDD"; //自定义的key
		std::string APPSK = "4BB2C635901468BF0E6ADB24E508B79F";//自定义的sk 
		std::string msgId = message.getMsgID();
		std::string topic = message.getTopic();

		
		json data; // empty object
		data["msgType"] = "POLICY_CLAIM"; // 自定义消息的类型
		data["topic"] = topic;
		data["tag"] = message.getTag();
		data["key"] = message.getKey(); 
		data["msgID"] = msgId;
		data["body"] = message.getBody();
		std::string hash_ret = calculatemd5(data.to_string().append(APPSK));
		data["sign"] = hash_ret; //实现简单的md5鉴权
		data["appak"] = APPAK;
		std::string url = "Your url";
		
		RestClient::Response r = RestClient::post(url, "text/json", data.to_string());		
		
		std::cout << "Topic: " << topic << " /MsgId: " << msgId <<std::endl;
		std::cout << "Resonse Code: " << r.code <<std::endl;
	//	std::cout << "Resonse Body: " << r.body <<std::endl;
		std::cout << "md5 of 'grape': " << hash_ret << std::endl;
		
		//根据http的返回消费消息
		if( r.code == 200 && r.body == "Success"){
			std::cout << "CONSUME SUCCESS !!!: " << std::endl;
			//根据业务需求，消费消息
			return Success;
		}
		return Suspend;
    }
};


int main(int argc, char* argv[])
{
    //OrderConsumer创建和工作需要的参数，必须输入
    ONSFactoryProperty factoryInfo;
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumerId, "CID-");//在ONS控制台申请的consumerId
    factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics,"S-" );// 在ONS 控制台申请的msg topic
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "asdfadsf");// ONS AccessKey
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey,  "adsfadsf");//ONS SecretKey
	factoryInfo.setFactoryProperty(ONSFactoryProperty::ONSAddr,  "http://jbponsaddr-internal.aliyun.com:8080/rocketmq/nsaddr4client-internal");//ONS SecretKey
	factoryInfo.setFactoryProperty(ONSFactoryProperty::LogPath, "/tmp/log");

    // 创建orderConsumer
    OrderConsumer* orderConsumer = ONSFactory::getInstance()->createOrderConsumer(factoryInfo);
    MyMsgListener  msglistener;
    //指定orderConsumer 订阅的消息topic和消息tag
    orderConsumer->subscribe(factoryInfo.getPublishTopics(), "*",&msglistener );

    // 注册消息监听的处理实例，orderConsumer 拉取到消息后，会调用该类的consumeMessage 函数

    //启动orderConsumer
    orderConsumer->start();
	
	//一直监听
	while (1)
	{
		int x;	
		cin >> x;
	}

    for(volatile int i = 0; i < 1000000000; ++i) {
        //wait
    }

    //销毁orderConsumer, 在应用退出前，必须销毁Consumer 对象，否则会导致内存泄露等问题
    orderConsumer->shutdown();

   return 0;
}
