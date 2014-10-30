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
		m_lanes.push_back(lane);
		
	}
}

DisplayManager::~DisplayManager() {
	//ClearAllMessages();
	//CloseHandle(m_mutex);

	for(std::list<Lane*>::iterator l=m_lanes.begin();
		l!=m_lanes.end(); ++l)
	{
		delete *l;
	}
	m_lanes.clear();
	//dont' forget we might have pending messages
	for(std::list<Message*>::iterator msg = m_pendingMessages.begin();
		msg!=m_pendingMessages.end();++msg) {
		
		delete *msg;
	}
	m_pendingMessages.clear();
}

void DisplayManager::AddMessage(const std::wstring& msg,
		const std::wstring& name,
		const int& repetitions,
		const float& scroll_time,
		const int& y)
{
	//MutexLock lock(m_mutex);

	Message* newText = new Message( msg, y, 1);
	m_pendingMessages.push_back(newText);
	
}

void DisplayManager::RemoveMessage(const std::wstring& name)
{
	//MutexLock lock(m_mutex);
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
	int screen_width = 0;
	int screen_height = 0;
	if(!m_pendingMessages.empty()){
		screen_width = GetDeviceCaps(hdc, HORZRES);
		screen_height = GetDeviceCaps(hdc,VERTRES);
	}
	for(std::list<Message*>::iterator i=m_pendingMessages.begin();
		i!=m_pendingMessages.end();++i)
	{
		bool handled_msg = false;
		Message* msg = *i;
		for(std::list<Lane*>::iterator lane=m_lanes.begin();
			lane!=m_lanes.end();++lane)
		{
			Gdiplus::RectF bb;
			graphics.MeasureString(msg->Text().c_str(), -1, &font, origin, &bb);
			msg->W(static_cast<int>(bb.Width));
			msg->H(static_cast<int>(bb.Height));
			msg->X(static_cast<int>(screen_width));
			if((*lane)->AddMesssage(msg)) {
				handled_msg=true;
				break;
			}
		}
		if(!handled_msg) {
			delete msg;
		}
	}
	m_pendingMessages.clear();

	for(std::list<Lane*>::iterator lane = m_lanes.begin();
		lane!=m_lanes.end(); ++lane)
	{
		std::list<Message*>& messages = (*lane)->m_messages;
		for(std::list<Message*>::iterator i = messages.begin();
			i!=messages.end(); ++i)
		{
			Message* msg = (*i);
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
	for(std::list<Message*>::iterator i=m_messages.begin();
		i!=m_messages.end();++i)
	{
		delete *i;
	}
}

void Lane::Render(HDC hdc, const int screen_w, const int screen_h)
{

}
bool Lane::Update(float dt, const int screen_w, const int screen_h)
{
	for(std::list<Message*>::iterator msg=m_messages.begin();
		msg!=m_messages.end();)
	{
		if((*msg)->Update(dt, screen_w, screen_h))
		{
			delete (*msg);
			msg = m_messages.erase(msg);
		}else{
			++msg;
		}
	}

	return m_messages.empty();
}

bool Lane::AddMesssage(Message* msg)
{
	//for now, just one message per lane
	if(!m_messages.empty()){
		return false;
	}
	/*
	//We want to calculate if we add this msg to this lane, will it "hit" the message
	//already in the lane (if there is one) during its lifetime. If it does, return
	//'false' to indicate we should place this in another lane
	if(m_messages.empty()) {
		msg->Y(m_y);
		m_messages.push_back(msg);
		return true;
	}

	//we only need be concerned with 'hitting' the last message in this lane
	Message* m1 = m_messages.back();
	Message* m2 = msg;
	float scroll_time = m2->ScrollTime();
	int screen_width = m2->X();//true only when messages added
	float v1 = (m1->W()+screen_width)/scroll_time;
	float v2 = (m2->W()+screen_width)/scroll_time;
	if(m1->X()>m1->W() && v1>=v2) {
		//new message will never "catch up" so add it
		msg->Y(m_y);
		m_messages.push_back(msg);
		return true;
	}
	//TODO:
	//solve to see if new messge will "catch up"
	int m1_w = m1->W();
	int m1_x = m1->X();
	int m2_w = m2->W();
	int m2_x = m2->X();


	if(!m_messages.empty())
	{
		return false;
	}
	*/
	//add this msg to the lane and return 'true' to indicate it's added
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