// NSBase64.cpp
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include<string>
#include<fstream>
#include"NSFileFunctions.h"
#include"NSErrorClasses.h"
#include"NSMaxLimits.h"

#ifndef NS_BASE_64
#define NS_BASE_64

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

class NSBase64{

public:

	NSBase64();
	~NSBase64();
	
	void writeMimeFile( string &, const char *, const char * );
	void writeMimeFile2( string &, const char *, const char * );

private:

	inline void DecodeBase64File( FILE *in, FILE *out );
	inline void decodeblock( unsigned char in[4], unsigned char out[3] );
	char * cInputLocation;
	char * cOutputLocation;

	inline void DecodeBase64Buffer( const char *input, const char *output_location );

};

} // end namespace

#endif