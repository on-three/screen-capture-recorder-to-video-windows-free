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
	virtual std::string StaticMessage(const int& h, 
		const std::wstring& msg, 
		const std::wstring& name, 
		const int& w, const int& x, const int& y,
		const float lifetime) = 0;
	virtual std::string RemoveStaticMessage(const std::wstring& name) = 0;
};

#endif