/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    helloworld.cpp
 * @date    08.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <stdio.h>
#include <string>
#include <iostream>

#include "server.h"
//#include "jsonrpc/procedure.h"
//#include <jsonrpc/specificationwriter.h>
#include "procedure.h"
#include "specificationwriter.h"

using namespace jsonrpc;
using namespace std;

int main(int argc, char** argv)
{

    TestServer* server = new TestServer();
    HttpClient *httpClient = new HttpClient("http://localhost:8080");
    Client* client = new Client(httpClient);

    cout << SpecificationWriter::toString(server->GetProtocolHanlder()->GetProcedures()) << endl;

    try {
        server->StartListening();

        Json::Value v;
        v["name"] = "Peter";
        Json::Value result = client->CallMethod("sayHello", v);

        if(result.asString() != "Hello: Peter!") {
            cerr << "sayHello returned " << result.asString() << " but should be \"Hello: Peter!\"" << endl;
            return -1;
        }

        v["name"] = "Peter Spiess-Knafl";
        result = client->CallMethod("sayHello", v);
        if(result.asString() != "Hello: Peter Spiess-Knafl!") {
            cerr << "sayHello returned " << result.asString() << " but should be \"Hello: Peter Spiess-Knafl!\"" << endl;
            return -2;
        }

        delete server;
        delete client;
        delete httpClient;

        cout << argv[0] << " passed" << endl;

        return 0;

    } catch(jsonrpc::JsonRpcException e) {

        cerr << "Exception occured: " << e.what() << endl;
        delete server;
        delete client;
        delete httpClient;
        return -999;
    }
}
