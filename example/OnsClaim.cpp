#include "ONSFactory.h"
#include <iostream>

#include "restclient-cpp/restclient.h"

#include <jsoncons/json.hpp>
#include <openssl/md5.h>
// For convenience
using jsoncons::json;

using namespace std;
using namespace ons;

//md5���ܷ���
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



//����������Ϣ��ʵ��
//pushConsumer��ȡ����Ϣ�󣬻��������ø�ʵ����consumeMessage ����
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
		std::string APPAK = "3458D3A6A232AFDD"; //�Զ����key
		std::string APPSK = "4BB2C635901468BF0E6ADB24E508B79F";//�Զ����sk 
		std::string msgId = message.getMsgID();
		std::string topic = message.getTopic();

		
		json data; // empty object
		data["msgType"] = "POLICY_CLAIM"; // �Զ�����Ϣ������
		data["topic"] = topic;
		data["tag"] = message.getTag();
		data["key"] = message.getKey(); 
		data["msgID"] = msgId;
		data["body"] = message.getBody();
		std::string hash_ret = calculatemd5(data.to_string().append(APPSK));
		data["sign"] = hash_ret; //ʵ�ּ򵥵�md5��Ȩ
		data["appak"] = APPAK;
		std::string url = "Your url";
		
		RestClient::Response r = RestClient::post(url, "text/json", data.to_string());		
		
		std::cout << "Topic: " << topic << " /MsgId: " << msgId <<std::endl;
		std::cout << "Resonse Code: " << r.code <<std::endl;
	//	std::cout << "Resonse Body: " << r.body <<std::endl;
		std::cout << "md5 of 'grape': " << hash_ret << std::endl;
		
		//����http�ķ���������Ϣ
		if( r.code == 200 && r.body == "Success"){
			std::cout << "CONSUME SUCCESS !!!: " << std::endl;
			//����ҵ������������Ϣ
			return Success;
		}
		return Suspend;
    }
};


int main(int argc, char* argv[])
{
    //OrderConsumer�����͹�����Ҫ�Ĳ�������������
    ONSFactoryProperty factoryInfo;
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumerId, "CID-");//��ONS����̨�����consumerId
    factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics,"S-" );// ��ONS ����̨�����msg topic
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "asdfadsf");// ONS AccessKey
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey,  "adsfadsf");//ONS SecretKey
	factoryInfo.setFactoryProperty(ONSFactoryProperty::ONSAddr,  "http://jbponsaddr-internal.aliyun.com:8080/rocketmq/nsaddr4client-internal");//ONS SecretKey
	factoryInfo.setFactoryProperty(ONSFactoryProperty::LogPath, "/tmp/log");

    // ����orderConsumer
    OrderConsumer* orderConsumer = ONSFactory::getInstance()->createOrderConsumer(factoryInfo);
    MyMsgListener  msglistener;
    //ָ��orderConsumer ���ĵ���Ϣtopic����Ϣtag
    orderConsumer->subscribe(factoryInfo.getPublishTopics(), "*",&msglistener );

    // ע����Ϣ�����Ĵ���ʵ����orderConsumer ��ȡ����Ϣ�󣬻���ø����consumeMessage ����

    //����orderConsumer
    orderConsumer->start();
	
	//һֱ����
	while (1)
	{
		int x;	
		cin >> x;
	}

    for(volatile int i = 0; i < 1000000000; ++i) {
        //wait
    }

    //����orderConsumer, ��Ӧ���˳�ǰ����������Consumer ���󣬷���ᵼ���ڴ�й¶������
    orderConsumer->shutdown();

   return 0;
}
