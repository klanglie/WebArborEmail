// NSMimeUtils.h
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include"NSStringFunctions.h"
#include"NSErrorClasses.h"

#ifndef DECIPHER_MIME
#define DECIPHER_MIME

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

class NSMimeUtils : private NSStringFunctions{

public:
	
	NSMimeUtils();
	virtual ~NSMimeUtils();

	void Decipher( string& );

	string  getHeader();
	string  getMimeVersion();
	string  getContentType();
	string  getContentTransferEncoding();
	string  getContentID();
	string  getContentLocation();
	string  getContentDescription();
	string  getContentDisposition();
	string  getName();
	string  getFileName();
	string  getCharset();
    void    setFileName( string );

private:

enum mime_header{ None, 
				  MimeVersion,
				  ContentType, 
				  ContentEncoding, 
				  ContentID,
				  ContentLocation,
				  ContentDescription, 
				  ContentDisposition,
				  Name, 
				  FileName, 
				  Charset };
	
	inline void InitializeVariables();
	inline void DecipherHelper();

	string content_header; 
	string mime_version;
	string content_type;
	string content_encoding;
	string content_id;
	string content_description;
	string content_disposition;
	string name;
	string file_name;
	string charset;
    string content_location;

};

} // End namespace definition

#endif
