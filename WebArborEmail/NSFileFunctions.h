// NSFileFunctions.h
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include<sys/stat.h>
#include<fstream>
#include<string>

// These headers are for file shreading 
#include<fcntl.h>
//#include <sys/types.h>
#include<io.h>  //for bug fix...
#include<direct.h> // for DirExists
#include"NSErrorClasses.h"
#include"NSMaxLimits.h"

//// #include <new>
//// using std::bad_alloc;

using namespace std;

#ifndef NS_FILE_FUNCTIONS
#define NS_FILE_FUNCTIONS

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

class NSFileFunctions{

public:

	NSFileFunctions();
	virtual ~NSFileFunctions();

	void writeFile( const char *output, const char *path );	
	void appendFile( string &output, const char *path );

	char * OpenFile( const char * );
	int FileExists(const char *szFilename);
	void ShredFile(const char *szFilename);
	long FileLength(FILE *pFile);
	void FGetLine(char *szBuffer, int iNumChars, FILE *pFile);
    bool DirExists(const char *dirname);
    bool CreateDirectory(const char * dir_name);
	bool IsGif( const char * dir_path, const char * file_name );
	bool IsJpg( const char * dir_path, const char * file_name );
	bool RenameFile( const char * dir_path, const char * old_name, const char * new_name, bool bDeleteNew );

private:

	char * chFileData;

};

} // end namespace

#endif
