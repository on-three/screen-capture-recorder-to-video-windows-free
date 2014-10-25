#include "ScrollingText.h"

#include <windows.h>
#include <gdiplus.h>
#include "MutexLock.h"
using namespace Gdiplus;

ScrollingTextManager::ScrollingTextManager() {
	//m_mutex = CreateMutex( 
    //    NULL,              // default security attributes
    //    FALSE,             // initially not owned
    //    NULL);             // unnamed mutex 
}

ScrollingTextManager::~ScrollingTextManager() {
	//ClearAllMessages();
	//CloseHandle(m_mutex);
}

void ScrollingTextManager::AddMessage(const std::wstring& msg,
		const std::wstring& name,
		const int& repetitions,
		const float& scroll_time,
		const int& y)
{
	//MutexLock lock(m_mutex);
	ScrollingText* newText = new ScrollingText(msg, y, repetitions, scroll_time);
	std::map<std::wstring, ScrollingText*>::iterator txt = m_text.find(name);
	if(txt!=m_text.end()){
		delete txt->second;
	}
	m_text[name]=newText;
}

void ScrollingTextManager::RemoveMessage(const std::wstring& name)
{
	//MutexLock lock(m_mutex);
	std::map<std::wstring, ScrollingText*>::iterator txt = m_text.find(name);
	if(txt!=m_text.end()){
		delete txt->second;
		m_text.erase(txt);
	}
}
void ScrollingTextManager::ClearAllMessages(void)
{
	//MutexLock lock(m_mutex);
	for(std::map<std::wstring, ScrollingText*>::iterator i = m_text.begin();
		i!=m_text.end();++i)
	{
		delete i->second;
	}
	m_text.clear();
}

bool ScrollingTextManager::Update(const float dt,
	const int screen_w, const int screen_h)
{
	//MutexLock lock(m_mutex);
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
	//MutexLock lock(m_mutex);

	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily,
		36,
		FontStyle::FontStyleRegular,
		Gdiplus::UnitPixel);
	GraphicsPath path;
	StringFormat strformat;
	
	for(std::map<std::wstring, ScrollingText*>::iterator i = m_text.begin();
		i!=m_text.end(); ++i)
	{
		Gdiplus::RectF origin(0,0,848.0, 480.0);
		if(i->second->W()<0 || i->second->H()<0) {
			Gdiplus::RectF bb;
			//graphics.MeasureString(i->second->Text().c_str(), -1, &font, origin,&strformat, &bb);
			//(const WCHAR *,INT,const Gdiplus::Font *,const Gdiplus::RectF &,Gdiplus::RectF *
			//graphics.MeasureString(i->second->Text().c_str(), -1, &font, origin, &bb);
			graphics.MeasureString(i->second->Text().c_str(), -1, &font, origin, &bb);
			i->second->W(static_cast<int>(bb.Width));
			i->second->H(static_cast<int>(bb.Height));
		}
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
		,m_repetitions(other.m_repetitions)
		,m_cumulativeTime(0.0f)
		,m_scroll_time(6.0f)
		,m_cycle(0)
{

}

ScrollingText::ScrollingText()
	:m_text(std::wstring(TEXT("TESTING")))
		,m_x(0)
		,m_y(0)
		,m_w(-1)
		,m_h(-1)
		,m_repetitions(0)
		,m_cumulativeTime(0.0f)
		,m_scroll_time(10.0f)
		,m_cycle(0)
{

}

ScrollingText::ScrollingText(const std::wstring& text,
		const int y,
		const int repetitions,
		const float scroll_time)
		:m_text(text)
		,m_x(0)
		,m_y(y)
		,m_w(-1)
		,m_h(-1)
		,m_repetitions(repetitions)
		,m_cumulativeTime(0.0f)
		,m_scroll_time(scroll_time)
		,m_cycle(0)
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
	float v = (static_cast<float>(screen_w)+static_cast<float>(m_w))/m_scroll_time;
	m_x -= static_cast<int>(v*dt);

	//reset if we've move completely across the screen
	if(m_x <= -1*m_w) {
		++m_cycle;
		m_x = screen_w;
	}

	return m_repetitions==0 ? false : m_cycle >= m_repetitions;
}
