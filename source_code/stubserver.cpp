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

std::wstring stdUTF8ToWideString(const std::string& str) {
	int wchars_num =  MultiByteToWideChar( CP_UTF8 , 0 , str.c_str() , -1, NULL , 0 );
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar( CP_UTF8 , 0 , str.c_str() , -1, wstr , wchars_num );
	std::wstring ws(wstr);
	delete [] wstr;
	return ws;
}

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
	std::wstring message = stdUTF8ToWideString(msg);
	std::wstring freindlyName = stdUTF8ToWideString(name);
	return m_captureDevice->StaticMessage(h, 
		message, freindlyName, 
		w, x, y, static_cast<float>(lifetime));
	//return std::string("success");
}

std::string MyStubServer::RemoveScrollingMessage(const std::string& name)
{
	std::wstring freindlyName = stdUTF8ToWideString(name);
	return m_captureDevice->RemoveScrollingMessage(freindlyName);
}
std::string MyStubServer::ScrollingMessage(const std::string& msg,
	const std::string& name,
	const int& repetitions,
	const double& scroll_time,
	const int& y)
{
	std::wstring message = stdUTF8ToWideString(msg);
	std::wstring freindlyName = stdUTF8ToWideString(name);
	return m_captureDevice->ScrollingMessage(message, freindlyName, 
		repetitions, static_cast<float>(scroll_time), y);
	//return std::string("success");
}

std::string MyStubServer::ClearAll(const int& arg) {
	return m_captureDevice->ClearAll(arg);
}

std::string MyStubServer::AddNicoNicoMsg(const std::string& msg)
{
	//std::wstring message(msg.begin(), msg.end());
	std::wstring message = stdUTF8ToWideString(msg);
	return m_captureDevice->AddNicoNicoMsg(message);
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