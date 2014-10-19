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

        virtual std::string StaticMessage(const int& h, 
			const std::string& msg, 
			const std::string& name, 
			const int& w, const int& x, const int& y);
private:
	MsgPresentationInterface* const m_captureDevice;

};

#endif //__STUBSERVER_H__
