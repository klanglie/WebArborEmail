// NSQuotedPrintable.cpp
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include<string>
#include"NSStringFunctions.h"
#include"NSErrorClasses.h"
#include<fstream>
#include<sstream>

#ifndef NS_QUOTED_PRINTABLE
#define NS_QUOTED_PRINTABLE

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

	class NSQuotedPrintable : private NSStringFunctions{

public:

	NSQuotedPrintable();
	NSQuotedPrintable( const string& );
	virtual ~NSQuotedPrintable();

	void DecodeQP(const char *input_file_name);
	string & DecodeQP(bool);

	void DecodeQuotedPrintable( char * chQPIn, char * chQPOut );

private:
	
	string sQPVar;
	inline void RemoveQPLineBreaks();
	inline void ChangeHttpVar( bool );
	inline int HexToChar( char *, char * );
	inline size_t FindQPCharsInString(string&, size_t, const char & , const char & );	
	//
	inline void DecodeBinaryAttachment( const char *file_name, const char *input );
	inline bool IsQPChar(char *c);
	
//char QPMAP[256] = { // 48 - 57, 65 - 70, 97 - 102 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
//   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
//   0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 
//   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 
//   1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
//   0, 0, 0, 0, 0, 0, 
//};

};

} // end namespace

#endif