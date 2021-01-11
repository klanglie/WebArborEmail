// NSUUCode.h
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include<string>
#include<io.h>  //for bug fix...

using namespace std;

#ifndef NS_UU_CODE
#define NS_UU_CODE

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

#define DEC(c)  (((c) - ' ') & 077) // This is for uudecoding

class NSUUCode{

public:

	NSUUCode();
	virtual ~NSUUCode();

	char *uudecode ( FILE *, const char * );	

private:

	inline int ReadDataLine( FILE *, char * );

};

} // end namespace

#endif