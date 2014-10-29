/*
NicoNicoDisplay.h
on_three
Saturday, Oct 18th 2014
*/
#ifndef __NICONICODISPLAY_H__
#define __NICONICODISPLAY_H__

#include <string>
#include <map>
#include <list>
//#include <WinDef.h>//for HDC
#include <Unknwn.h>
#include <objidl.h>
#include <windows.h>

namespace NicoNico
{

class Message
{
public:
	Message(const Message& other);;
	Message();
	Message(const std::wstring& text,
		const int y,
		const int repetitions=0,
		const float scroll_time=10.0f);

	std::wstring Text(void){return m_text;};
	int X(void)const{return m_x;};
	int Y(void)const{return m_y;};
	void Y(const int y){m_y=y;};
	int W(void)const{return m_w;};
	void W(const int w){m_w=w;};
	int H(void)const{return m_h;};
	void H(const int h){m_h=h;};
	float ScrollTime(void)const{return m_scroll_time;};

	void Render(HDC hdc);
	bool Update(const float dt, const int screen_w, const int screen_h);

private:
	std::wstring m_text;
	int m_x;
	int m_y;
	int m_w;
	int m_h;

	const int m_repetitions;//time to life in seconds; 0 is infinite.
	int m_cycle;
	const float m_scroll_time;//time for one scroll in seconds.
	float m_cumulativeTime;
};

class DisplayManager;

class Lane
{
public:
	friend class DisplayManager;
public:
	Lane(const int y);
	~Lane();

public:
	void Render(HDC hdc, const int screen_w, const int screen_h);
	bool Update(float dt, const int screen_w, const int screen_h);

public:
	bool AddMesssage(Message* msg);
	void ClearMessages(void);

private:
	std::list< Message* > m_messages;
	const int m_y;//vertical pos onscreen in pixels

};

class DisplayManager
{
public:
	DisplayManager();
	~DisplayManager();

public:
	void AddMessage(const std::wstring& msg,
		const std::wstring& name,
		const int& repetitions,
		const float& scroll_time,
		const int& y);
	void RemoveMessage(const std::wstring& name);
	void ClearAllMessages(void);

	bool Update(const float dt,
		const int screen_w, const int screen_h);
	void Render(HDC hdc);

private:
	std::list<Lane*> m_lanes;

	//Hold incoming messages in this quque until thier on screen width can be calculated
	//then we can add them to a lane and see them onscreen.
	std::list<Message*> m_pendingMessages;

	//control access to message list
	HANDLE m_mutex;
};

}

#endif