// Example43.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "atltime.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CString strTime;//用户将ctime转字符串
	CTime curTime = CTime::GetCurrentTime();

	int nYear = curTime.GetYear();
	int nMonth = curTime.GetMonth();
	int nDay = curTime.GetDay();
	int nHour = curTime.GetHour();
	int nMin = curTime.GetMinute();
	int nSec = curTime.GetSecond();

	char ts[256];
	sprintf(ts,  "%d-%d-%d %d:%d:%d\n", nYear, 
		nMonth,
		nDay,
		nHour,
		nMin,
		nSec);
	cout << ts << endl;

	CTime startTime = CTime(2010,7,3,10,20,30);
	cout << "start time \n" << endl;
	

	char es[256];
	CTimeSpan timeSpan;
	timeSpan = curTime - startTime;
	sprintf(ts,  "%d %d %d %d\n", nYear, 
		timeSpan.GetDays(),
		timeSpan.GetHours(),
		timeSpan.GetMinutes(),
		timeSpan.GetSeconds()
		);

	cout << ts << endl;
	strTime = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	wcout << (LPCTSTR)strTime << endl;
	
	
	system("pause");
	

	return 0;
}

