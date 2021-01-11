// AMMimeUtils
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "NSStringFunctions.h"
#include "NSQuotedPrintable.h"
#include "NSErrorClasses.h"
#include<vector>

#ifndef NS_AMMimeUtils
#define NS_AMMimeUtils

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

// Originally from www.codeproject.com
// Latest version (google search AMMimeUtils.cpp) from:
// http://cvs.sourceforge.net/viewcvs.py/myserverweb/myserverweb/
class AMMimeUtils{

public:

//decodes a single line from a header-field in a mail...
char* MimeDecodeMailHeaderField(char *s);

//decodes a single line from a header-field in a mail...
string MimeDecodeGenericMailField(char *s);
void DecodeIS088591QP( char * chInput );

char* strustr(char *source, char *s);

private:

};

//class to handle all base64 stuff...
class CBase64Utils
{
private:
  int ErrorCode;
public:
  int GetLastError() {return ErrorCode;};
	CBase64Utils();
	~CBase64Utils();
  char* Decode(char *input, int *bufsize); //caller must free the result, bufsize holds the decoded length
  char* Encode(char *input, int bufsize); //caller must free the result, bufsize is the length of the input buffer
};

//class to handle quoted-printable stuff
class CQPUtils
{
private:
  char* ExpandBuffer(char *buffer, int UsedSize, int *BufSize, int SingleChar = 1);
  int ErrorCode;
public:
  int GetLastError() {return ErrorCode;};
	char* Decode(char *input); //caller must free the result
  char* Encode(char *input); //caller must free the result
	CQPUtils();
	~CQPUtils();
};

} // end namespace

#endif