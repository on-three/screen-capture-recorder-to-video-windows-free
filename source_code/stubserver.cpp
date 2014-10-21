/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    stubserver.cpp
 * @date    02.05.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/
#include <jsonrpc/rpc.h>
#include <iostream>
#include <jsonrpc/connectors/httpserver.h>
#include "stubserver.h"
using namespace jsonrpc;
using namespace std;



//class CPushPinDesktop;

/*
class MyStubServer : public AbstractMyStubServer
{
    public:
        MyStubServer();

        virtual void notifyServer();
        virtual std::string sayHello(const std::string& name);
        virtual int addNumbers(const int& param1, const int& param2);
};
*/
MyStubServer::MyStubServer(MsgPresentationInterface* captureDevice)
	:AbstractMyStubServer(new HttpServer(8080))
	,m_captureDevice(captureDevice)
{
}

std::string MyStubServer::RemoveStaticMessage(const std::string& name)
{
	std::wstring freindlyName(name.begin(), name.end());
	return m_captureDevice->RemoveStaticMessage(freindlyName);
}
std::string MyStubServer::StaticMessage(const int& h, 
			const double& lifetime, 
			const std::string& msg, 
			const std::string& name, 
			const int& w, const int& x, const int& y)
{
	std::wstring message(msg.begin(), msg.end());
	std::wstring freindlyName(name.begin(), name.end());
	return m_captureDevice->StaticMessage(h, 
		message, freindlyName, 
		w, x, y, static_cast<float>(lifetime));
	//return std::string("success");
}

std::string MyStubServer::RemoveScrollingMessage(const std::string& name)
{
	std::wstring freindlyName(name.begin(), name.end());
	return m_captureDevice->RemoveScrollingMessage(freindlyName);
}
std::string MyStubServer::ScrollingMessage(const int& h, 
			const double& lifetime, 
			const std::string& msg, 
			const std::string& name, 
			const int& w, const int& x, const int& y)
{
	std::wstring message(msg.begin(), msg.end());
	std::wstring freindlyName(name.begin(), name.end());
	return m_captureDevice->ScrollingMessage(h, 
		message, freindlyName, 
		w, x, y, static_cast<float>(lifetime));
	//return std::string("success");
}

/*
int main()
{
    MyStubServer s;
    s.StartListening();

    getchar();

    s.StopListening();

    return 0;
}
*/