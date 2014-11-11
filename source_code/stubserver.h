/*
	stubserver.h

	Implementation of abstrac stub server (gen by json-rpc-cpp)
*/
#pragma once
#ifndef __STUBSERVER_H__
#define __STUBSERVER_H__
#include "abstractmystubserver.h"
#include <jsonrpc/rpc.h>
#include <connectors/httpserver.h>
#include "MsgPresentationInterface.h"

//forwards
class CPushPinDesktop;

class MyStubServer : public AbstractMyStubServer
{
    public:
        MyStubServer(MsgPresentationInterface* captureDevice);

        virtual std::string RemoveStaticMessage(const std::string& name);
        virtual std::string StaticMessage(const int& h, 
			const double& lifetime, 
			const std::string& msg, 
			const std::string& name, 
			const int& w, const int& x, const int& y);
		virtual std::string RemoveScrollingMessage(const std::string& name);
        virtual std::string MyStubServer::ScrollingMessage(const std::string& msg,
			const std::string& name,
			const int& repetitions,
			const double& scroll_time,
			const int& y);
		virtual std::string ClearAll(const int& arg);
		virtual std::string AddNicoNicoMsg(const std::string& msg);
private:
	MsgPresentationInterface* const m_captureDevice;

};

#endif //__STUBSERVER_H__
