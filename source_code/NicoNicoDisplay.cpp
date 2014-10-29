#include "NicoNicoDisplay.h"

#include <windows.h>
#include <gdiplus.h>
#include "MutexLock.h"
using namespace Gdiplus;

namespace NicoNico
{

const int LANE_COUNT = 15;
const int LANE_OFFSET = 30;

DisplayManager::DisplayManager() {
	//m_mutex = CreateMutex( 
    //    NULL,              // default security attributes
    //    FALSE,             // initially not owned
    //    NULL);             // unnamed mutex

	//Just add N allocated lanes for now
	//in the future they should be added and removed as needed.
	for(int l=0;l<LANE_COUNT;++l)
	{
		Lane* lane = new Lane(l*(LANE_OFFSET+1));
		//std::wstring t(TEXT("test"));
		//lane->AddMesssage(new Message(t,100,0));
		m_lanes.push_back(lane);
		
	}
}

DisplayManager::~DisplayManager() {
	//ClearAllMessages();
	//CloseHandle(m_mutex);

	//for(std::list<Lane*>::iterator l=m_lanes.begin();
	//	l!=m_lanes.end(); ++l)
	//{
	//	delete *l;
	//}
	//m_lanes.clear();
	/*
	//dont' forget we might have pending messages
	for(std::list<Message*>::iterator msg = m_pendingMessages.begin();
		msg!=m_pendingMessages.end();++msg) {
		
		delete *msg;
	}
	m_pendingMessages.clear();
	*/
}

void DisplayManager::AddMessage(const std::wstring& msg,
		const std::wstring& name,
		const int& repetitions,
		const float& scroll_time,
		const int& y)
{
	//MutexLock lock(m_mutex);

	//std::wstring t(TEXT("newtext"));
	Message* newText = new Message( msg, y, 1);
	for(std::list<Lane*>::iterator l=m_lanes.begin();
		l!=m_lanes.end(); ++l)
	{
		if((*l)->AddMesssage(newText)) {
			break;
		}
	}
	//m_pendingMessages.push_back(newText);
	
}

void DisplayManager::RemoveMessage(const std::wstring& name)
{
	//MutexLock lock(m_mutex);
	/*std::map<std::wstring, Message*>::iterator txt = m_text.find(name);
	if(txt!=m_text.end()){
		delete txt->second;
		m_text.erase(txt);
	}*/
}
void DisplayManager::ClearAllMessages(void)
{
	//MutexLock lock(m_mutex);
	for(std::list<Lane*>::iterator lane = m_lanes.begin();
		lane!=m_lanes.end();++lane)
	{
		(*lane)->ClearMessages();
	}
	//dont' forget we might have pending messages
	for(std::list<Message*>::iterator msg = m_pendingMessages.begin();
		msg!=m_pendingMessages.end();++msg) {
		
		delete *msg;
	}
	m_pendingMessages.clear();
}

bool DisplayManager::Update(const float dt,
	const int screen_w, const int screen_h)
{
	//MutexLock lock(m_mutex);
	for(std::list<Lane*>::iterator lane = m_lanes.begin();
		lane!=m_lanes.end();)
	{
		if((*lane)->Update(dt, screen_w, screen_h)) {
			//delete *lane;
			//lane = m_lanes.erase(lane);
			++lane;
		}else{
			++lane;
		}
	}
	return false;
}
void DisplayManager::Render(HDC hdc)
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
	Gdiplus::RectF origin(0,0,848.0, 480.0);

	//Before drawing, calculate the size of any pending messages for this render context
	//and add them to a lane as appropriate
	/*
	for(std::list<Message*>::iterator i=m_pendingMessages.begin();
		i!=m_pendingMessages.end();++i)
	{
		for(std::list<Lane*>::iterator lane=m_lanes.begin();
			lane!=m_lanes.end();++lane)
		{
			(*lane)->AddMesssage(*i);
		}
	}
	*/
		/*
		Message* msg = *i;
		Gdiplus::RectF bb;
		graphics.MeasureString(msg->Text().c_str(), -1, &font, origin, &bb);
		msg->W(static_cast<int>(bb.Width));
		msg->H(static_cast<int>(bb.Height));
		bool lane_found=false;
		for(std::list<Lane*>::iterator lane=m_lanes.begin();
			lane!=m_lanes.end();++lane)
		{
			if((*lane)->AddMesssage(msg))
			{
				lane_found=true;
				//break;
			}
		}
		if(!lane_found) {
			//for now we're going to just drop messages we can't fit
			//delete msg;
		}
		
	}*/
	//m_pendingMessages.clear();

	for(std::list<Lane*>::iterator lane = m_lanes.begin();
		lane!=m_lanes.end(); ++lane)
	{
		std::list<Message*>& messages = (*lane)->m_messages;
		for(std::list<Message*>::iterator i = messages.begin();
			i!=messages.end(); ++i)
		{
			Message* msg = (*i);
			
			if(msg->W()<0 || msg->H()<0) {
				Gdiplus::RectF bb;
				graphics.MeasureString(msg->Text().c_str(), -1, &font, origin, &bb);
				msg->W(static_cast<int>(bb.Width));
				msg->H(static_cast<int>(bb.Height));
			}
			path.AddString(msg->Text().c_str(), -1, &fontFamily, 
			FontStyleRegular, 36, Gdiplus::Point(msg->X(), msg->Y()), &strformat );
		}
	}
	Pen pen(Color::Black, 4);
    graphics.DrawPath(&pen, &path);
    SolidBrush brush(Color::Yellow);
    graphics.FillPath(&brush, &path);
}

Lane::Lane(const int y)
	:m_y(y)
{

}
Lane::~Lane()
{
	//for(std::list<Message*>::iterator i=m_messages.begin();
	//	i!=m_messages.end();++i)
	//{
	//	delete *i;
	//}
}

void Lane::Render(HDC hdc, const int screen_w, const int screen_h)
{

}
bool Lane::Update(float dt, const int screen_w, const int screen_h)
{
	bool addmsg = false;
	for(std::list<Message*>::iterator msg=m_messages.begin();
		msg!=m_messages.end();)
	{
		if((*msg)->Update(dt, screen_w, screen_h))
		{
			delete (*msg);
			msg = m_messages.erase(msg);
			//debug
			addmsg = true;
		}else{
			++msg;
		}
	}

	
	//debug
	if(addmsg) {
		//std::wstring t(TEXT("test"));
		//Message* newText = new Message(t, 100, 1);
		//this->AddMesssage(newText);
	}

	return m_messages.empty();
}

bool Lane::AddMesssage(Message* msg)
{
	//for now, just one message per lane
	if(!m_messages.empty()){
		return false;
	}
	msg->Y(m_y);
	m_messages.push_back(msg);
	return true;
}

void Lane::ClearMessages(void)
{
	for(std::list<Message*>::iterator msg=m_messages.begin();
		msg!=m_messages.end();++msg)
	{
		delete (*msg);
	}
	m_messages.clear();
}

Message::Message(const Message& other)
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

Message::Message()
	:m_text(std::wstring(TEXT("TESTING")))
		,m_x(0)
		,m_y(0)
		,m_w(-1)
		,m_h(-1)
		,m_repetitions(1)
		,m_cumulativeTime(0.0f)
		,m_scroll_time(10.0f)
		,m_cycle(0)
{

}

Message::Message(const std::wstring& text,
		const int y,
		const int repetitions,
		const float scroll_time)
		:m_text(text)
		,m_x(0)
		,m_y(y)
		,m_w(-1)
		,m_h(-1)
		,m_repetitions(1)
		,m_cumulativeTime(0.0f)
		,m_scroll_time(10.0f)
		,m_cycle(0)
{

}

void Message::Render(HDC hdc) {

}
bool Message::Update(const float dt, const int screen_w, const int screen_h) {
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

}