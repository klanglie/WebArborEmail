////////////////////////////////////////////////////////////////////////////////
// CDate.h - Copyright 2003 - R. Santor
////////////////////////////////////////////////////////////////////////////////
// My purpose in creating this class was to simplify the handling of dates. I
// work with a high level program generating language called ProIV which uses
// a function called CDATE to simply convert to/from numeric/alpha dates. You
// can also modify the dates by adding and subtracting days because it calcs
// the date by counting the number of days from something like Jan 1, 1775.
// This class manages the date by tracking the number of days after the date
// December 31, 1 B.C.
////////////////////////////////////////////////////////////////////////////////
// The default constructor retrieves the current system date. You may also set
// the date in one of several different ways. Ex:
//    CDate date(7, 15, 2003);
//    CDate date("07152003", "MMDDYYYY");
//    CDate date(731596); // number of days since year 0 (not very useful)
// Modify a date very simply. Ex:
//    date += 200; // set date to 200 days later (Jan 31, 2004 in this case)
//    --date;
//    int diff = date1 - date2; // difference between two dates
// Compare two dates. Ex:
//    if (date2 >= date1) ...
// Convert a date to a string:
//    cout << date << endl; // prints date in default format: 20030715
//    date.Format("Mmmmmmmmm DD, YYYY") // returns "July 15, 2003" (use 9 Ms to
//      insure that maximum length of 'September' is handled)
//    date.Format("DDMMMYY") // returns 15JUL03
// Everything is taken care of for you. Don't worry about the proper number of
// days in a month or in a leap year. It's all handled.
// As you can see, this class is simple yet quite powerful. lucky760@yahoo.com
////////////////////////////////////////////////////////////////////////////////
//  7/15/03 - R. Santor - Initial release
// 11/18/03 - R. Santor - Added day-of-the-week tracking by adding:
//                        GetDayName(), GetDayOfWeek(), DAY_NAMES[]
//                      - Added: GetMonthName()
//                      - Fixed operator const char*() const; by adding:
//                        const char* Format(const char *fmt, char *recvBuf) const;
////////////////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"// (un)comment as necessary

#include "stdafx.h"
#include "CDate.h"
#include <cstdlib>
#include <ctype.h>
#include <sstream>
#include <cstring>
#include <ctime>
using namespace cdate;
using namespace std;

#include <iostream>

CDate::CDate()
: _formatStr(NULL)
{
  time_t ltime;
  time (&ltime);
  
  char today[10] = { 0 };
  strftime(today, 10, "%Y%m%d", localtime(&ltime));
  
  StringToDate(today, "YYYYMMDD");
}

CDate::CDate(int totalDays)
: _daycount(totalDays), _formatStr(NULL)
{
  CalcMDY();
}

CDate::CDate(const CDate &rhs)
: _daycount(rhs._daycount),
  _m(rhs._m), _d(rhs._d), _y(rhs._y),
  _formatStr(NULL)
{
}

CDate::CDate(int m, int d, int y)
: _m(m), _d(d), _y(y), _formatStr(NULL)
{
  CalcDayCount();
}

CDate::CDate(const char *date, const char *fmt)
: _formatStr(NULL)
{
  StringToDate(date, fmt);
}


CDate::~CDate() {
  if (_formatStr != NULL)
    delete [] _formatStr;
}

void CDate::StringToDate(const char *date, const char *fmt) {
  int len = strlen(fmt);
  
  ostringstream d, m, y;
  
  const char *mfmt = strchr(fmt, 'm');
  const char *tmp = strchr(fmt, 'M');
  if (mfmt == NULL || (tmp < mfmt && tmp != NULL))
    mfmt = tmp;

  bool moByName = mfmt != NULL;
  if (moByName)
    moByName = strnicmp(mfmt, "MMM", 3) == 0;  
  
  for (int i = 0; i < len; ++i) {
    switch (toupper(fmt[i])) {
      case 'Y':
        y << date[i];
      break;
      
      case 'M':
        m << date[i];
      break;
      
      case 'D':
        d << date[i];
      break;
    }
  }

  if (!moByName)  
    _m = atoi(m.str().c_str());
  else {
    _m = 0;
    for (int i = 0; i < 12; ++i) {
      if (!strnicmp(MONTH_NAMES[i], m.str().c_str(), m.str().length())) {
        _m = i + 1;
        break;
      }
    }
  }    

  _d = atoi(d.str().c_str());
  _y = atoi(y.str().c_str());
  
  CalcDayCount();
}

const char* CDate::Format(const char *fmt, char *recvBuf) const {
  ostringstream ostr;
  
  ostringstream d, m, y;
  m << _m;
  d << _d;  
  y << _y;

  int dlen = d.str().length();
  int mlen = m.str().length();
  int ylen = y.str().length();
  int days = 0;
  int mos = 0;
  int yrs = 0;
  
  bool moByName = false;
  int moNameLen = strlen(MONTH_NAMES[_m - 1]);
  int moFmtLen = 0;
  
  int i;

  int len = strlen(fmt);  
  for (i = len - 1; i >= 0; --i) {
    switch (toupper(fmt[i])) {
      case 'Y':
        if (yrs < ylen) {
          ostr << y.str()[ylen - yrs - 1];
          ++yrs;
        }
        else
          ostr << '0';
      break;
      
      case 'M':
        if (!moByName && mos == 0) {
          // if there at least 3 in a row, user wants the name of the month
          moByName = strnicmp(fmt + i - 2, "MMM", 3) == 0;
          if (moByName) {
            mlen = 0;
            for (int j = i; j >= 0; --j)
              if (toupper(fmt[j]) == 'M')
                ++mlen;
              else
                break;

            moFmtLen = mlen;

            if (mlen > moNameLen)
              mlen = moNameLen; 
          }
        }
        
        if (mos < mlen || (moByName && mos < moFmtLen)) {
          if (moByName) {
            if ((moFmtLen - mos - 1) >= moNameLen) {
              ostr << ' ';
              ++mlen;
            }
            else {
              if (fmt[i] == 'm')
                ostr << static_cast<char>(tolower(MONTH_NAMES[_m - 1][mlen - mos - 1]));
              else
                ostr << static_cast<char>(toupper(MONTH_NAMES[_m - 1][mlen - mos - 1]));
            }
          }
          else
            ostr << m.str()[mlen - mos - 1];

          ++mos;
        }
        else {
          if (moByName)
            ostr << ' ';
          else              
            ostr << '0';
        }
      break;
      
      case 'D':
        if (days < dlen) {
          ostr << d.str()[dlen - days - 1];
          ++days;
        }
        else
          ostr << '0';
      break;
      
      default:
        ostr << fmt[i];
      break;        
    }
  }

  ostr << ends;

  int j = 0;  
  bool prevspace = false;
  for (i = len - 1; i >= 0; --i) {
    char ch = ostr.str()[i];

    if (!prevspace || ch != ' ') {
      recvBuf[j] = ch;
      recvBuf[++j] = NULL;
    }

    prevspace = ch == ' ';
  }

  return recvBuf;
}

const char* CDate::Format(const char *fmt) {
  if (_formatStr != NULL)
    delete [] _formatStr;

  int len = strlen(fmt);
  _formatStr = new char[len + 1];
  return Format(fmt, _formatStr);  
}

void CDate::CalcMDY() {
  _y = static_cast<int>((_daycount * 100) / ((DPY + 0.25) * 100));

  bool isleapyear = (_y % 4) == 0;

  int daysintoyear = _daycount - static_cast<int>((_y * ((DPY + 0.25) * 100)) / 100);
  
  if (isleapyear)
    ++daysintoyear;

  _m = 1;

  int days = 0;

  if (daysintoyear <= 31) {
    _d = daysintoyear;
  }
  else {
    for (unsigned i = 1; i <= 12; ++i) {
      int next;

      if (i == 2) {
        next = isleapyear ? 29 : 28;
      }
      else if ((i % 2) == 1) {
        if (i <= 7)
          next = 31;
        else
          next = 30;
      }
      else {
        if (i < 7)
          next = 30;
        else
          next = 31;
      }

      _m = i;
      
      if ((days + next) >= daysintoyear)
        break;

      days += next;
    }

    _d = daysintoyear - days;
  }
  
  if (_d == 0)
    _d = 1;
}

void CDate::CalcDayCount() {
  if (_y > 0)
    _daycount = (DPY * _y) + static_cast<int>((_y - 1) / 4);
  else
    _daycount = 0;

  bool isleapyear = (_y % 4) == 0;

  for (unsigned i = 1; i < _m; ++i) {
    if (i == 2) {
      _daycount += isleapyear ? 29 : 28;
    }
    else if ((i % 2) == 1) {
      if (i <= 7)
        _daycount += 31;
      else
        _daycount += 30;
    }
    else {
      if (i < 7)
        _daycount += 30;
      else
        _daycount += 31;
    }
  }
  
  _daycount += _d;
}

int CDate::GetDayOfWeek() const {
  int y, m, d;

  if (_m < 3) {
    y = _y - 1;
    m = _m + 12;
    d = _d;
  }
  else {
    y = _y;
    m = _m;
    d = _d;
  }

  return (((13 * m + 3) / 5) + d + y + (y / 4) - (y / 100) + (y / 400) + 1) % 7;
}

int CDate::operator-(const CDate &rhs) const {
  return _daycount - rhs._daycount;
}

const CDate CDate::operator+(int count) const {
  CDate cd = *this;
  cd._daycount += count;
  cd.CalcMDY();
  return cd;
}

const CDate CDate::operator-(int count) const {
  CDate cd = *this;
  cd._daycount -= count;
  cd.CalcMDY();
  return cd;
}

CDate& CDate::operator+=(int count) {
  _daycount += count;
  CalcMDY();
  return *this;
}

CDate& CDate::operator-=(int count) {
  _daycount -= count;
  CalcMDY();
  return *this;
}

const CDate CDate::operator++(int) {
  CDate cd = *this;
  operator++();
  return cd;
}

const CDate CDate::operator--(int) {
  CDate cd = *this;
  operator--();
  return cd;
}

CDate& CDate::operator++() {
  return operator+=(1);
}

CDate& CDate::operator--() {
  return operator-=(1);
}

CDate::operator const char*() const {
  static char buf[9];// 9 because DEF_FORMAT is 8 chars long
  return Format(cdate::DEF_FORMAT, buf);
}
