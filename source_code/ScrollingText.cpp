#include "ScrollingText.h"

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;


ScrollingTextManager::~ScrollingTextManager() {
	ClearAllMessages();
}

void ScrollingTextManager::AddMessage(const std::wstring& msg,
	const std::wstring& name,
	const int x, const int y,
	const int w, const int h,
	const float lifetime)
{
	ScrollingText* newText = new ScrollingText(msg, x, y, w, h, lifetime);
	std::map<std::wstring, ScrollingText*>::iterator txt = m_text.find(name);
	if(txt!=m_text.end()){
		delete txt->second;
	}
	m_text[name]=newText;
}

void ScrollingTextManager::RemoveMessage(const std::wstring& name)
{
	std::map<std::wstring, ScrollingText*>::iterator txt = m_text.find(name);
	if(txt!=m_text.end()){
		delete txt->second;
		m_text.erase(txt);
	}
}
void ScrollingTextManager::ClearAllMessages(void)
{
	for(std::map<std::wstring, ScrollingText*>::iterator i = m_text.begin();
		i!=m_text.end();)
	{
		delete i->second;
	}
	m_text.clear();
}

bool ScrollingTextManager::Update(const float dt,
	const int screen_w, const int screen_h)
{
	for(std::map<std::wstring, ScrollingText*>::iterator i = m_text.begin();
		i!=m_text.end();)
	{
		if(i->second->Update(dt, screen_w, screen_h)) {
			delete i->second;
			i = m_text.erase(i);
		}else{
			++i;
		}
	}

	return false;
}
void ScrollingTextManager::Render(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	FontFamily fontFamily(L"Arial");
	GraphicsPath path;
	StringFormat strformat;
	for(std::map<std::wstring, ScrollingText*>::iterator i = m_text.begin();
		i!=m_text.end(); ++i)
	{
		path.AddString(i->second->Text().c_str(), -1, &fontFamily, 
		FontStyleRegular, 36, Gdiplus::Point(i->second->X(), i->second->Y()), &strformat );
	}
	Pen pen(Color::Black, 4);
    graphics.DrawPath(&pen, &path);
    SolidBrush brush(Color::Yellow);
    graphics.FillPath(&brush, &path);
}

ScrollingText::ScrollingText(const ScrollingText& other)
		:m_text(other.m_text)
		,m_x(other.m_x)
		,m_y(other.m_y)
		,m_w(other.m_w)
		,m_h(other.m_h)
		,m_lifetime(other.m_lifetime)
		,m_cumulativeTime(0.0f)
		,m_cycletime(6.0f)
{

}

ScrollingText::ScrollingText()
	:m_text(std::wstring(TEXT("TESTING")))
	,m_x(100)
	,m_y(100)
	,m_w(100)
	,m_h(100)
	,m_lifetime(0.0f)
	,m_cumulativeTime(0.0f)
	,m_cycletime(6.0f)
{

}

ScrollingText::ScrollingText(const std::wstring& text,
		const int x, const int y,
		const int w, const int h,
		const float lifetime)
		:m_text(text)
		,m_x(x)
		,m_y(y)
		,m_w(w)
		,m_h(h)
		,m_lifetime(lifetime)
		,m_cumulativeTime(0.0f)
		,m_cycletime(6.0f)
{

}

void ScrollingText::Render(HDC hdc) {

}
bool ScrollingText::Update(const float dt, const int screen_w, const int screen_h) {
	//first update?
	if(m_cumulativeTime==0.0f) {
		m_x = screen_w;
	}
	m_cumulativeTime+=dt;

	//move message to new position
	//if we wish to travel screen_w pixes in m_cycletime seconds, then the rate is:
	//v = pixels/second = (screen_w+m_w)/m_cycletime
	float v = (static_cast<float>(screen_w)+static_cast<float>(m_w))/m_cycletime;
	m_x -= static_cast<int>(v*dt);

	//reset if we've move completely across the screen
	if(m_x <= -1*m_w) {
		m_x = screen_w;
	}

	return m_lifetime<=0.0f ? false : m_cumulativeTime >= m_lifetime;
}
