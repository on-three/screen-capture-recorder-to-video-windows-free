/**
 * @file main.cpp
 * @date 01.08.2011
 * @author Peter Spiess-Knafl
 * @brief main.cpp
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <jsonrpc/rpc.h>

using namespace jsonrpc;
using namespace std;

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

int main(int argc, char** argv)
{
    try
    {
        SampleServer serv;
        if (serv.StartListening())
        {
            cout << "Server started successfully" << endl;
            getchar();
            serv.StopListening();
        }
        else
        {
            cout << "Error starting Server" << endl;
        }
    }
    catch (jsonrpc::JsonRpcException& e)
    {
        cerr << e.what() << endl;
    }
//curl --data "{\"jsonrpc\":\"2.0\",\"method\":\"sayHello\",\"id\":1,\"params\":{\"name\":\"peter\"}}" localhost:8080
//curl --data "{\"jsonrpc\":\"2.0\",\"method\":\"notifyServer\", \"params\": null}" localhost:8080
}