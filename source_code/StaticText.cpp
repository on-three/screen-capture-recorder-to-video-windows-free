#include "StaticText.h"

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;


StaticTextManager::~StaticTextManager() {
	ClearAllMessages();
}

void StaticTextManager::AddMessage(const std::wstring& msg,
	const std::wstring& name,
	const int x, const int y,
	const int w, const int h,
	const float lifetime)
{
	StaticText* newText = new StaticText(msg, x, y, w, h, lifetime);
	std::map<std::wstring, StaticText*>::iterator txt = m_text.find(name);
	if(txt!=m_text.end()){
		delete txt->second;
	}
	m_text[name]=newText;
}

void StaticTextManager::RemoveMessage(const std::wstring& name)
{
	std::map<std::wstring, StaticText*>::iterator txt = m_text.find(name);
	if(txt!=m_text.end()){
		delete txt->second;
		m_text.erase(txt);
	}
}
void StaticTextManager::ClearAllMessages(void)
{
	for(std::map<std::wstring, StaticText*>::iterator i = m_text.begin();
		i!=m_text.end();)
	{
		delete i->second;
	}
	m_text.clear();
}

bool StaticTextManager::Update(const float dt,
	const int screen_w, const int screen_h)
{
	for(std::map<std::wstring, StaticText*>::iterator i = m_text.begin();
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
void StaticTextManager::Render(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	FontFamily fontFamily(L"Arial");
	GraphicsPath path;
	StringFormat strformat;
	for(std::map<std::wstring, StaticText*>::iterator i = m_text.begin();
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

StaticText::StaticText(const StaticText& other)
		:m_text(other.m_text)
		,m_x(other.m_x)
		,m_y(other.m_y)
		,m_w(other.m_w)
		,m_h(other.m_h)
		,m_lifetime(other.m_lifetime)
		,m_cumulativeTime(0.0f)
{

}

StaticText::StaticText()
	:m_text(std::wstring(TEXT("TESTING")))
	,m_x(100)
	,m_y(100)
	,m_w(100)
	,m_h(100)
	,m_lifetime(0.0f)
	,m_cumulativeTime(0.0f)
{

}

StaticText::StaticText(const std::wstring& text,
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
{

}

void StaticText::Render(HDC hdc) {

}
bool StaticText::Update(const float dt, const int screen_w, const int screen_h) {
	m_cumulativeTime+=dt;
	return m_lifetime<=0.0f ? false : m_cumulativeTime >= m_lifetime;
}
