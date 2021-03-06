/*
StaticText.h
on_three
Saturday, Oct 18th 2014
*/
#ifndef __STATICTEXT_H__
#define __STATICTEXT_H__

#include <string>
#include <map>
//#include <WinDef.h>//for HDC
#include <Unknwn.h>
#include <objidl.h>
#include <windows.h>

class StaticText
{
public:
	StaticText(const StaticText& other);;
	StaticText();
	StaticText(const std::wstring& text,
		const int x, const int y,
		const int w, const int h,
		const float lifetime=0.0f);

	std::wstring Text(void){return m_text;};
	int X(void)const{return m_x;};
	int Y(void)const{return m_y;};
	int W(void)const{return m_w;};
	int H(void)const{return m_h;};
	float Lifetime(void)const{return m_lifetime;};

	void Render(HDC hdc);
	bool Update(const float dt, const int screen_w, const int screen_h);

private:
	std::wstring m_text;
	int m_x;
	int m_y;
	int m_w;
	int m_h;

	const float m_lifetime;//time to life in seconds; 0 is infinite.
	float m_cumulativeTime;
};

class StaticTextManager
{
public:
	StaticTextManager();
	~StaticTextManager();

public:
	void AddMessage(const std::wstring& msg,
		const std::wstring& name,
		const int x, const int y,
		const int w, const int h,
		const float lifetime);
	void RemoveMessage(const std::wstring& name);
	void ClearAllMessages(void);

	bool Update(const float dt,
		const int screen_w, const int screen_h);
	void Render(HDC hdc);

private:
	std::map<std::wstring, StaticText*> m_text;
	
	//Control access to message list
	HANDLE m_mutex;
};

#endif