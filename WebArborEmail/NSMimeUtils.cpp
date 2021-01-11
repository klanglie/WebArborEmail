// NSMimeUtils.cpp
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSMimeUtils.h"

const char Space[2] = { 32, 0 } ;
const char double_crLf[5] = { 13, 10, 13, 10, 0 };

using namespace WEBARBOR_EMAIL_COMPONENTS;

NSMimeUtils::NSMimeUtils(){}

NSMimeUtils::~NSMimeUtils(){}

inline void NSMimeUtils::InitializeVariables()
{
	content_header = ""; 
	mime_version = "";
	content_type = "";
	content_encoding = "";
	content_id = "";
	content_description = "";
	content_disposition = "";
	name = "";
	file_name = "";
	charset = "";
	content_location = "";
}

string NSMimeUtils::getHeader(){
	return content_header;
}

string NSMimeUtils::getMimeVersion(){
	return mime_version;
}

string NSMimeUtils::getContentType(){
	return content_type;
}

string NSMimeUtils::getContentLocation(){
	return content_location;
}

string NSMimeUtils::getContentTransferEncoding(){
	return content_encoding;
}

string NSMimeUtils::getContentID(){
	return content_id;
}

string NSMimeUtils::getContentDescription(){
	return content_description;
}

string NSMimeUtils::getContentDisposition(){
	return content_disposition;
}

string NSMimeUtils::getName(){
	return name;
}

void NSMimeUtils::setFileName(string sName){
	file_name = sName;
}

string NSMimeUtils::getFileName(){
	return file_name;
}

string NSMimeUtils::getCharset(){
	return charset;
}

void NSMimeUtils::Decipher( string &in_content )
{

	try{

		InitializeVariables();
	    LTrim( in_content );

		if( in_content.length() > 0 ){
		
			size_t iPos = in_content.find( double_crLf );
			
			if( iPos != string::npos ){				
				content_header = in_content.substr( 0, iPos + 2 );			   
				in_content.erase( 0, iPos + 4 );
			}
			
			DecipherHelper();

		}
	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError("NSMimeUtils::Decipher has encountered an unknown error", 9352);
	}

}

inline void NSMimeUtils::DecipherHelper()
{

	register basic_string <char>::size_type intPosA;
	string smallHeaderName;
	string tempVal;	
	mime_header Status = None;
	bool bWhiteSpace;
	bool bHeaderItem = false;

	try{

		string::size_type lastPos = content_header.find_first_not_of("\n", 0);
		// Find first "non-delimiter".
		string::size_type pos = content_header.find_first_of("\n", lastPos);
		
		while (string::npos != pos || string::npos != lastPos)
		{
			// Found a token
			tempVal = content_header.substr(lastPos, pos - lastPos - 1);
			
			if( isspace( tempVal[0] ) ){
				bWhiteSpace = true;
				LTrim( tempVal );
			}
			else{
				bWhiteSpace = false;
			}

			intPosA = tempVal.find_first_of( ":", 0 );	
			if( intPosA > 0 && intPosA < 27 ){
				bHeaderItem = true;
			}
			else{
				intPosA = tempVal.find_first_of( "=", 0 );
				if( intPosA > 0 && intPosA < 27 ){ 
					bHeaderItem = true;
				}
				else{
					bHeaderItem = false;
				}
			}	
			
			RTrimCrLfSpace(tempVal);

			if( bHeaderItem && !( bWhiteSpace ) ){ 
				
				smallHeaderName = tempVal.substr( 0, (intPosA + 1) );
				ToLower( smallHeaderName );
				tempVal = tempVal.substr( smallHeaderName.length() );
				LTrim(tempVal);

				if( smallHeaderName == "mime-version:" ){
					mime_version = tempVal;
					Status = MimeVersion;
				}
				else if( smallHeaderName == "content-type:" ){
					content_type = tempVal;
					Status = ContentType;
				}
				else if( smallHeaderName == "content-transfer-encoding:" ){
					content_encoding = tempVal;
					Status = ContentEncoding;
				}
				else if( smallHeaderName == "content-id:" ){
					content_id = tempVal;
					Status = ContentID;
				}	
				else if( smallHeaderName == "content-location:" ){
					content_location = tempVal;
					Status = ContentLocation;
				}
				else if( smallHeaderName == "content-description:" ){
					content_description = tempVal;
					Status = ContentDescription;
				}
				else if( smallHeaderName == "content-disposition:" ){
					content_disposition = tempVal;
					Status = ContentDisposition;
				}
				else if( smallHeaderName == "name=" ){
					name = tempVal;
					Status = Name;
				}
				else if( smallHeaderName == "filename=" ){
					file_name = tempVal;
					Status = FileName;
				}
				else if( smallHeaderName == "charset=" ){
					charset = tempVal;
					Status = Charset;
				}
				else{
					Status = None;
				}

			}
			else if( ( Status != None ) )
			{	

				switch( Status ){
							
					case MimeVersion:
						//mime_version.append(Space);
						mime_version.append(tempVal);	
						break;
					
					case ContentType:
						//content_type.append(Space);
						content_type.append(tempVal);	
						break;
					
					case ContentEncoding:
						//content_encoding.append(Space);
						content_encoding.append(tempVal);	
						break;

					case ContentID:
						//content_id.append(Space);
						content_id.append(tempVal);	
						break;

					case ContentLocation:
						//content_location.append(Space);
						content_location.append(tempVal);	
						break;

					case ContentDescription:
						//content_description.append(Space);
						content_description.append(tempVal);	
						break;

					case ContentDisposition:
						//content_disposition.append(Space);
						content_disposition.append(tempVal);	
						break;

					case Name:
						name.append(Space);
						name.append(tempVal);	
						break;

					case FileName:
						file_name.append(Space);
						file_name.append(tempVal);	
						break;

					case Charset:
						//charset.append(Space);
						charset.append(tempVal);	
						break;

				}

			}

			// Skip delimiters.  Note the "not_of"
			lastPos = content_header.find_first_not_of("\n", pos);
			// Find next "non-delimiter"
			pos = content_header.find_first_of("\n", lastPos);
		}
	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError("NSMimeUtilities::DecipherHelper has caught an unexpected error.", 6247);
	}
}
