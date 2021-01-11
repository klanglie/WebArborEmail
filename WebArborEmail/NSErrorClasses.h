// NSErrorClasses.h
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#ifndef NS_ERROR_CLASSES
#define NS_ERROR_CLASSES

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

class FileWillNotOpen{
public:
	FileWillNotOpen()
		: message( "File could not be opened" ){}
		const char *what() const { return message; }
private:
	const char *message;
};

class MiscError{
public:
	MiscError( const char *s, int i )
		: message(s), error_number(i){}
		const char *what() const { return message; }
		int number() const { return error_number; }
private:
	const char *message;
	int error_number;
};

} // end namespace

#endif