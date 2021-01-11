// NSHeaderItem
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSHeaderItem.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

NSHeaderItem::NSHeaderItem( )
{

}

NSHeaderItem::~NSHeaderItem()
{

}

string NSHeaderItem::GetHeaderItem( string header, const char * item )
{

	basic_string <char>::size_type iItemLength = strlen(item);
	if( iItemLength == 0 || iItemLength > 30 || header.length() == 0 ){
		return "";
	}
	else if( iItemLength > header.length() ){
		return "";
	}
	
	//------------------------------------------------------------------
	const char Space[2] = { 32, 0 } ;
	//------------------------------------------------------------------
	const char *iso_8859_1 = "=?iso-8859-1"; //?q?";
	//--
	const char *iso_b = "?b?";
	//------------------------------------------------------------------
	const char *iso_q = "?q?";
	//------------------------------------------------------------------

	basic_string <char>::size_type intPosA;
	string tempVal;
	string final_header_val; // The return value will be stored here
	bool bWhiteSpace = false;
	bool bGoodLoop = false;
	bool bFound = false;
    bool bFunctionPass = false;

    int  result = 0;
	int  result2 = 0;
	int  result3 = 0;

    char * chItem = 0;
    char * chHead = 0; 
	char * chHeadValue = 0;

	try{

		chItem = new char[ strlen(item) + 1 ];
		chItem[ strlen(item) ] = 0;
		strcpy( chItem, item );  // Copy the contents of the input here
		ToLower(chItem);   // Lowercase this value for comparison reasons

		string::size_type lastPos = header.find_first_not_of("\n", 0);
		// Find first "non-delimiter".
		string::size_type pos = header.find_first_of("\n", lastPos);
		
		while (string::npos != pos || string::npos != lastPos)
		{
			// Found a token
			tempVal = header.substr(lastPos, pos - lastPos - 1);
			
			chHead = new char[ tempVal.length() + 1 ];	
			chHead[ tempVal.length() ] = 0;
			strcpy( chHead, tempVal.c_str() );
			if( isspace( chHead[0] ) ){
				LTrim( chHead );
				bWhiteSpace = true;
			}
			else{
				bWhiteSpace = false;
			}
			
			intPosA = tempVal.find_first_of( ":", 0 );	
			if( intPosA > 0 && intPosA < 27 ){
				bGoodLoop = true;
			}
			else{
				intPosA = tempVal.find_first_of( "=", 0 );
				if( intPosA > 0 && intPosA < 27 ){ 
					bGoodLoop = true;
				}
				else{
					bGoodLoop = false;
				}
			}	
			
			if( bGoodLoop && !( bWhiteSpace ) ){ 
				
				chHeadValue = substrR( chHead, static_cast<int>( (intPosA + 1) ) );
				LTrim( chHeadValue );
				RTrimCrLfSpace( chHeadValue );
				ToLower( chHead );
				
				result = InstrChar( chHead, iso_8859_1 );
				result2 = InstrChar( chHead, iso_b );
				result3 = InstrChar( chHead, iso_q );
				if ( result != -1 || result2 != -1 || result3 != -1 ){
					if( result != -1 && result3 != -1 ){
						DecodeIS088591QP( chHeadValue );
					}
					else{
						chHeadValue = MimeDecodeMailHeaderField( chHeadValue );
					}
				}

				if( InstrChar( chHead, chItem ) == 0 ){
					final_header_val = chHeadValue;
					bFound = true;
					bFunctionPass = true;
				}
				else{
					bFound = false;
				}

				delete [] chHeadValue;

			}
			else if( bFound && bWhiteSpace  )
			{	
				final_header_val.append( Space );
				final_header_val.append( chHead );
			}

			delete [] chHead;

			// Skip delimiters.  Note the "not_of"
			lastPos = header.find_first_not_of("\n", pos);
			// Find next "non-delimiter"
			pos = header.find_first_of("\n", lastPos);
		}

		delete [] chItem;

		if( bFunctionPass == true ){
			return final_header_val;
		}
		else{
			return "";
		}
	
	}
	catch( MiscError ){
		delete [] chItem;
		delete [] chHead;
		delete [] chHeadValue;
		throw;
	}
	catch(...){
		delete [] chItem;
		delete [] chHead;
		delete [] chHeadValue;
        throw(MiscError("GetHeaderItem has caught an unknown error", 49687));
	}
}
