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

        virtual void notifyServer();
        virtual std::string sayHello(const std::string& name);
        virtual int addNumbers(const int& param1, const int& param2);
private:
	MsgPresentationInterface* const m_captureDevice;

};

#endif //__STUBSERVER_H__
