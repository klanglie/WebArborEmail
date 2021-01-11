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

#ifndef _CDATE_H_
#define _CDATE_H_

#include <iostream>

namespace cdate {
  static const char *MONTH_NAMES[] = { 
    "January", "February", "March", "April", "May", "June", "July", 
    "August", "September", "October", "November", "December" 
  };
  
  static const char *DAY_NAMES[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
    "Friday", "Saturday"
  };

  static const char *DEF_FORMAT = "YYYYMMDD";
}; // namespace cdate

class CDate {
  enum { DPY = 365 };

  unsigned _m, _d, _y;
  unsigned _daycount;
  
  char *_formatStr;

  void StringToDate(const char *date, const char *fmt);
  
  void CalcDayCount(); // calc _daycount based on _m, _d, _y
  void CalcMDY();      // calc _m, _d, _y based on _daycount
  
public:
  CDate();// retrieves current system date  
  CDate(const CDate &rhs);
  CDate(int m, int d, int y);
  explicit CDate(int totalDays);
  explicit CDate(const char *date, const char *fmt=cdate::DEF_FORMAT);
  ~CDate();

  // form a date string in the format of fmt (which contains M's, D's, and Y's)
  // note: a sequence of 3 or more M's will cause the name of the month to be
  // used; when using the name, capital M's and lower case m's change the output;
  // if you want the entire month name, use 9 M's to accomodate the longest name
  const char* Format(const char *fmt);
  // it is the programmer's responsibility to insure recvBuf has room for fmt
  const char* Format(const char *fmt, char *recvBuf) const;

  // get the difference between two dates  
  int operator-(const CDate &rhs) const;

  const CDate operator+(int count) const;
  const CDate operator-(int count) const;
  CDate& operator+=(int count);
  CDate& operator-=(int count);
  const CDate operator++(int);
  const CDate operator--(int);
  CDate& operator++();
  CDate& operator--();
  
  operator const char*()                    { return Format(cdate::DEF_FORMAT); }
  operator const char*() const;

  int GetDayOfWeek() const;// where 0=SUN, 1=MON, ...

  int GetMonth() const                      { return _m; }
  int GetDay() const                        { return _d; }
  int GetYear() const                       { return _y; }
  int GetDayCount() const                   { return _daycount; }  
  const char* GetMonthName() const          { return cdate::MONTH_NAMES[_m]; }
  const char* GetDayName() const            { return cdate::DAY_NAMES[GetDayOfWeek()]; }
  
  void SetMonth(int m)                      { _m = m; CalcDayCount(); }
  void SetDay(int d)                        { _d = d; CalcDayCount(); }
  void SetYear(int y)                       { _y = y; CalcDayCount(); }
  void SetDayCount(int dc)                  { _daycount = dc; CalcMDY(); }
  
  bool operator==(const CDate &rhs) const   { return _daycount == rhs._daycount; }
  bool operator!=(const CDate &rhs) const   { return !operator==(rhs); }
  bool operator<(const CDate &rhs) const    { return _daycount < rhs._daycount; }
  bool operator>(const CDate &rhs) const    { return _daycount > rhs._daycount; }
  bool operator<=(const CDate &rhs) const   { return operator<(rhs) && operator==(rhs); }
  bool operator>=(const CDate &rhs) const   { return operator>(rhs) && operator==(rhs); }
  
  friend std::ostream& operator<<(std::ostream &out, const CDate &cd) { return out << (const char*)cd; }
  // we'll leave it up to the user to decide how to read a date then properly format it into a CDate
};

#endif //_CDATE_H_
