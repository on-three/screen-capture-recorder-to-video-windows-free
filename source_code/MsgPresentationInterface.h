/*
MsgPresentationInterface.h

on_three
Sat, Oct 18th 2014
*/


#ifndef __MSGPRESENTATIONINTERFACE_H__
#define __MSGPRESENTATIONINTERFACE_H__

//purely abstract interface to separate the directx filter and jsonrpc server
class MsgPresentationInterface
{
public:
	virtual void process(const std::wstring& msg) = 0;
};

#endif