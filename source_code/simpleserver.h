#pragma once
#ifndef __SIMPLESERVER_H__
#define __SIMPLESERVER_H__

#include <stdio.h>
#include <string>
#include <iostream>
#include <jsonrpc/rpc.h>
#include <connectors/httpserver.h>

class SampleServer : public AbstractServer<SampleServer>
{
    public:
        SampleServer() :
            AbstractServer<SampleServer>(new HttpServer(8080))//, true, "ssl_cert.pem"))
        {
            this->bindAndAddMethod(new Procedure("sayHello", PARAMS_BY_NAME, JSON_STRING, "name", JSON_STRING, NULL), &SampleServer::sayHello);
            this->bindAndAddNotification(new Procedure("notifyServer", PARAMS_BY_NAME, NULL), &SampleServer::notifyServer);
        }

        //methodssl_cert.pem
        void sayHello(const Json::Value& request, Json::Value& response)
        {
            response = "Hello: " + request["name"].asString();
        }

        //notification
        void notifyServer(const Json::Value& request)
        {
            cout << "server received some Notification" << endl;
        }

};

#endif