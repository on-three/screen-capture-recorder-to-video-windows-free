/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    servertemplate.h
 * @date    01.05.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SERVERTEMPLATE_H
#define SERVERTEMPLATE_H

#define TEMPLATE_SERVER_STUB "\
/**\n\
 * THIS FILE IS GENERATED BY jsonrpcstub, DO NOT CHANGE IT!!!!!\n\
 */\n\
\n\
#ifndef _<STUBNAME>_H_\n\
#define _<STUBNAME>_H_\n\
\n\
#include <jsonrpc/rpc.h>\n\
\n\
class <stubname> : public jsonrpc::AbstractServer<<stubname>>\n\
{\n\
    public:\n\
        <stubname>(jsonrpc::AbstractServerConnector* conn) :\n\
            jsonrpc::AbstractServer<<stubname>>(conn) \n\
        {\n\
<procedurebindings>\n\
        }\n\
        \n\
<proceduredefinitions>\n\
<abstractdefinitions>\n\
};\n\
#endif //_<STUBNAME>_H_\n\
"

#define TEMPLATE_SERVER_METHODBINDING "\
            this->bindAndAddMethod(new jsonrpc::Procedure(\"<rpcprocedurename>\", <paramtype>, <returntype>, <parameterlist> NULL), &<stubname>::<procedurename>I);"
#define TEMPLATE_SERVER_NOTIFICATIONBINDING "\
            this->bindAndAddNotification(new jsonrpc::Procedure(\"<rpcprocedurename>\", <paramtype>, <parameterlist> NULL), &<stubname>::<procedurename>I);"

#define TEMPLATE_SERVER_METHODDEFINITION "\
        inline virtual void <procedurename>I(const Json::Value& request, Json::Value& response) \n\
        {\n\
            response = this-><procedurename>(<parametermapping>);\n\
        }\n\
"
#define TEMPLATE_SERVER_NOTIFICAITONDEFINITION "\
        inline virtual void <procedurename>I(const Json::Value& request) \n\
        {\n\
            this-><procedurename>(<parametermapping>);\n\
        }\n\
"

#define TEMPLATE_SERVER_ABSTRACTDEFINITION "\
        virtual <returntype> <procedurename>(<parameterlist>) = 0;\n\
"

#endif // SERVERTEMPLATE_H
