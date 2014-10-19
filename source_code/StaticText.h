/*
StaticText.h
on_three
Saturday, Oct 18th 2014
*/
#ifndef __STATICTEXT_H__
#define __STATICTEXT_H__

#include <string>

class StaticText
{
public:
	StaticText(const StaticText& other)
		:m_text(other.m_text)
		,m_x(other.m_x)
		,m_y(other.m_y)
		,m_w(other.m_w)
		,m_h(other.m_h)
	{

	};
	StaticText()
		:m_text(std::wstring(TEXT("TESTING")))
		,m_x(100)
		,m_y(100)
		,m_w(100)
		,m_h(100)
	{}
	StaticText(const std::wstring& text,
		const int x, const int y,
		const int w, const int h)
		:m_text(text)
		,m_x(x)
		,m_y(y)
		,m_w(w)
		,m_h(h)
	{};

	std::wstring Text(void){return m_text;};
	int X(void)const{return m_x;};
	int Y(void)const{return m_y;};
	int W(void)const{return m_w;};
	int H(void)const{return m_h;};

private:
	std::wstring m_text;
	int m_x;
	int m_y;
	int m_w;
	int m_h;
};

#endif