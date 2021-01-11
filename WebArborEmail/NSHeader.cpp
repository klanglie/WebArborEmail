// NSHeader
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSHeader.h"

////#include<iostream>
////using namespace std;

using namespace WEBARBOR_EMAIL_COMPONENTS;

// NSHeader constructor
NSHeader::NSHeader()
{ 
	pToEmail = 0;			// pointer variable
	pCcEmail = 0;			// pointer variable	
	pBCcEmail = 0;			// pointer variable	
}

inline void NSHeader::InitializeVariables()
{
	received_count = 0;
	to_count = 0;
	cc_count = 0;  
	bcc_count = 0;

	delete [] pToEmail;
	delete [] pCcEmail;
	delete [] pBCcEmail;

    pToEmail = 0;			// pointer variable
	pCcEmail = 0;			// pointer variable	
    pBCcEmail = 0;			// pointer variable	

	for( int i = 0; i < 6; i++ )
	{
		received[i].clear();
		received_from[i].clear();
		received_by[i].clear();
		received_id[i].clear();
		received_domain[i].clear();
		received_ip[i].clear();
		received_date[i].clear();
		received_with[i].clear();
		received_for[i].clear();
	}
	date.clear();
	from.clear();
	subject.clear();
	to.clear();
	cc.clear();
	bcc.clear();
	message_id.clear();
	sender.clear();
	content_transfer_encoding.clear();
	content_type.clear();
	content_disposition.clear();
	mime_version.clear();
	message_charset.clear();
	reply_to.clear();

	// Now initialize body-related variables
    content_type.clear();
    content_transfer_encoding.clear();

	// 2004-10-20 - New properties
	in_reply_to.clear();
	references.clear();
	return_path.clear();
	comments.clear();
	keywords.clear();
	encrypted.clear();
	content_id.clear();
	content_description.clear();
	content_md5.clear();
	content_language.clear();
}

// NSHeader destructor
// Make sure to delete all global pointers that have been allocated
NSHeader::~NSHeader()
{
	delete [] pToEmail;
	delete [] pCcEmail;
	delete [] pBCcEmail;
}

const char * NSHeader::getDate(){
	return date.c_str();
}

const char * NSHeader::getFrom(){
	return from.c_str();
}

const char * NSHeader::getFromName(){
	return from_name.c_str();
}

const char * NSHeader::getFromEmail(){
	return from_email.c_str();
}

const char * NSHeader::getFromAt(){
	return from_at.c_str();
}

const char * NSHeader::getSubject(){
	return subject.c_str();
}

const char * NSHeader::getTo(){
	return to.c_str();
}

const char * NSHeader::getCC(){
	return cc.c_str();
}
const char * NSHeader::getBCC(){
	return bcc.c_str();
}

const char * NSHeader::getMessageId(){
	return message_id.c_str();
}

const char * NSHeader::getSender(){
	return sender.c_str();
}

const char * NSHeader::getContentTransferEncoding(){
	return content_transfer_encoding.c_str();
}

const char * NSHeader::getContentType(){
	return content_type.c_str();
}

const char * NSHeader::getContentDisposition(){
	return content_disposition.c_str();
}

const char * NSHeader::getMimeVersion(){
	return mime_version.c_str();
}

const char * NSHeader::getCharset(){
	return message_charset.c_str();
}

const char * NSHeader::getReplyTo(){
	return reply_to.c_str();
}

const char * NSHeader::getInReplyTo(){
	return in_reply_to.c_str();
}

const char * NSHeader::getReferences(){
	return references.c_str();
}

const char * NSHeader::getReturnPath(){
	return return_path.c_str();
}

const char * NSHeader::getComments(){
	return comments.c_str();
}

const char * NSHeader::getKeywords(){
	return keywords.c_str();
}

const char * NSHeader::getEncrypted(){
	return encrypted.c_str();
}

const char * NSHeader::getContentId(){
	return content_id.c_str();
}

const char * NSHeader::getContentDescription(){
	return content_description.c_str();
}

const char * NSHeader::getContentMD5(){
	return content_md5.c_str();
}

const char * NSHeader::getContentLanguage(){
	return content_language.c_str();
}

const char * NSHeader::getReceived( int num )
{
	if( num >= 0 && num <= 5){
		return received[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedFrom(int num )
{
	if( num >= 0 && num <= 5){
		return received_from[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedBy( int num )
{
	if( num >= 0 && num <= 5){
		return received_by[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedId( int num )
{
	if( num >= 0 && num <= 5){
		return received_id[ num ].c_str();
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedDomain( int num )
{
	if( num >= 0 && num <= 5){
		return received_domain[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedIp( int num )
{
	if( num >= 0 && num <= 5){
		return received_ip[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedDate( int num )
{
	if( num >= 0 && num <= 5){
		return received_date[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedWith( int num )
{
	if( num >= 0 && num <= 5){
		return received_with[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getReceivedFor( int num )
{
	if( num >= 0 && num <= 5){
		return received_for[ num ].c_str(); 
	}
	else{
		return "";
	}
}

const char * NSHeader::getToName( int num ){
	if( to_count > 0 && num < to_count ){
		return pToEmail[ num ].name;
	}
	else{
		return "";
	}
}

const char * NSHeader::getToEmail( int num ){
	if( to_count > 0 && num < to_count ){
		return pToEmail[ num ].email;
	}
	else{
		return "";
	}
}

const char * NSHeader::getCCName( int num ){
	if( cc_count > 0 && num < cc_count ){
		return pCcEmail[ num ].name;
	}
	else{
		return "";
	}
}

const char * NSHeader::getCCEmail( int num ){
	if( cc_count > 0 && num < cc_count ){
		return pCcEmail[ num ].email;
	}
	else{
		return "";
	}
}

const char * NSHeader::getBCCName( int num ){
	if( bcc_count > 0 && num < bcc_count ){
		return pBCcEmail[ num ].name;
	}
	else{
		return "";
	}
}

const char * NSHeader::getBCCEmail( int num ){
	if( bcc_count > 0 && num < bcc_count ){
		return pBCcEmail[ num ].email;
	}
	else{
		return "";
	}
}

// DecipherHelper does most of the actual header deciphering.
void NSHeader::GetHeaders( string &header )
{
	if ( header.length() < 100 )
		return;

	InitializeVariables();

	const char Space[2] = { 32, 0 } ;
	//------------------------------------------------------------------
	const char *iso_8859_1 = "=?iso-8859-1"; //?q?"   ?B?;
	//--
	const char *iso_b = "?b?";
	//------------------------------------------------------------------
	const char *iso_q = "?q?";
	//------------------------------------------------------------------
	const char *misc_windows = "?windows-1252?Q?";
	//------------------------------------------------------------------

	register basic_string <char>::size_type intPosA;
	string tempVal;	
	append_status Status = None;
	bool bWhiteSpace;
	bool bGoodLoop = false;

    register int  result = 0;
	register int  result2 = 0;
	register int  result3 = 0;
	bool bIso = false;

	// set default pointer values
    char * chHead = 0;
    char * chHeadValue = 0;
    char * r = 0;

	received_count = 0;
	
	try{

		register string::size_type lastPos = header.find_first_not_of("\n", 0);
		// Find first "non-delimiter".
		register string::size_type pos = header.find_first_of("\n", lastPos);
		
		while (string::npos != pos || string::npos != lastPos)
		{
			// Found a token
			tempVal = header.substr(lastPos, pos - lastPos - 1);

			chHead = new char[ tempVal.length() + 1 ];	
			chHead[ tempVal.length() ] = 0;
			strcpy( chHead, tempVal.c_str());
			
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
				if( result != -1 || result2 != -1 || result3 != -1 ){
					if( result != -1 && result3 != -1 ){
						DecodeIS088591QP( chHeadValue );
					}
					else{
						chHeadValue = MimeDecodeMailHeaderField( chHeadValue );
					}
				}

				if( InstrChar(chHead, "received:") == 0  ){
					if( received_count < 6 ){
						received[ received_count++ ] = chHeadValue;
						Status = Receive;
					}
					else{
						Status = None;
					}
				}
				else if( InstrChar(chHead, "date:") == 0 ){
					date = chHeadValue;
					Status = Date;
				}
				else if( InstrChar(chHead, "from:") == 0 ){
					strreplace( chHeadValue, "\\.", "." );
					from = chHeadValue;
					Status = From;
				}
				else if( InstrChar(chHead, "subject:") == 0 ){
					//------------------------------------------------------------------------
					result = InstrChar( chHeadValue, misc_windows );
					if ( result != -1 ){
						string NonWinSubject = chHeadValue;
						NonWinSubject = NonWinSubject.substr(result + strlen(misc_windows));
						strcpy(chHeadValue, NonWinSubject.c_str());
						result = InstrChar( chHeadValue, "?=" );
						if ( result != -1 ){
							if( result = static_cast<int>( NonWinSubject.length() ) - 3){ // The " was found 
								NonWinSubject = NonWinSubject.substr(0, NonWinSubject.length() - 2);
								char * chTemp = c_string( NonWinSubject );
								NSQuotedPrintable oQP;
								char * chQPOut = new char[ strlen(chTemp) + 1 ];
								chQPOut[ strlen(chTemp) ] = '\0';
								oQP.DecodeQuotedPrintable( chTemp, chQPOut );
								strcpy( chHeadValue, chQPOut );
								delete [] chQPOut;
								delete [] chTemp;
								RTrimCrLfSpace(chHeadValue);
								strclean( chHeadValue, "?windows-1252?Q?" );								
								strclean( chHeadValue, "?=" );
								strreplace(chHeadValue, "_", " ");
							}
						}
					}
					// Original
					subject = chHeadValue;
					Status = Subject;
				}
				else if( InstrChar(chHead, "to:") == 0 )
				{
					ReplaceExtraEmailCommas(chHeadValue);
					if( to.length() == 0 ){						
					    to = chHeadValue;
					}
					else{
						RTrim(&to);
						to.append(",");
						to.append(Space);
						to.append(chHeadValue);
					}
					Status = To;
				}	
				else if( InstrChar(chHead, "cc:") == 0 )
				{
					ReplaceExtraEmailCommas(chHeadValue);
					if( cc.length() == 0 ){
						cc = chHeadValue;
					}
					else{
						RTrim(&cc);
						cc.append(",");
						cc.append(Space);
						cc.append(chHeadValue);
					}
					Status = CC;
				}
				else if( InstrChar(chHead, "bcc:") == 0 )
				{
					ReplaceExtraEmailCommas(chHeadValue);
					if( cc.length() == 0 ){
						bcc = chHeadValue;
					}
					else{
						RTrim(&bcc);
						bcc.append(",");
						bcc.append(Space);						
						bcc.append(chHeadValue);
					}
					Status = BCC;
				}
				else if( InstrChar(chHead, "message-id:") == 0 ){
					message_id = chHeadValue;
					Status = MessageId;
				}
				else if( InstrChar(chHead, "sender:") == 0 ){
					sender = chHeadValue;
					Status = Sender;
				}
				else if( InstrChar(chHead, "content-transfer-encoding:") == 0 ){
					content_transfer_encoding = chHeadValue;
					Status = ContentEncoding;
				}
				else if( InstrChar(chHead, "content-type:") == 0 ){
					content_type = chHeadValue;
					Status = ContentType;
				}
				else if( InstrChar(chHead, "content-disposition:") == 0 ){
					content_disposition = chHeadValue;
					Status = ContentDisposition;
				}
				else if( InstrChar(chHead, "mime-version:") == 0 ){
					mime_version = chHeadValue;
					Status = MimeVersion;
				}
				else if( InstrChar(chHead, "charset=") == 0 ){
					message_charset = chHeadValue;
					Status = Charset;
				}
				else if( InstrChar(chHead, "reply-to:") == 0 ){
					reply_to = chHeadValue;
					Status = ReplyTo;
				}
				else if( InstrChar(chHead, "return-path:") == 0 ){
					return_path = chHeadValue;
					Status = ReturnPath;
				}
				else if( InstrChar(chHead, "in-reply-to:") == 0 ){
					in_reply_to = chHeadValue;
					Status = InReplyTo;
				}
				else if( InstrChar(chHead, "references:") == 0 ){
					references = chHeadValue;
					Status = References;
				}
				else if( InstrChar(chHead, "keywords:") == 0 ){
					keywords = chHeadValue;
					Status = Keywords;
				}
				else if( InstrChar(chHead, "comments:") == 0 ){
					comments = chHeadValue;
					Status = Comments;
				}
				else if( InstrChar(chHead, "encrypted:") == 0 ){
					encrypted = chHeadValue;
					Status = Encrypted;
				}
				else if( InstrChar(chHead, "content-id:") == 0 ){
					content_id = chHeadValue;
					Status = ContentID;
				}
				else if( InstrChar(chHead, "content-description:") == 0 ){
					content_description = chHeadValue;
					Status = ContentDescription;
				}
				else if( InstrChar(chHead, "content-md5:") == 0 ){
					content_md5 = chHeadValue;
					Status = ContentMD5;
				}
				else if( InstrChar(chHead, "content-language:") == 0 ){
					content_language = chHeadValue;
					Status = ContentLanguage;
				}
				//else if( InstrChar(chHead, "x-mailer:") == 0 ){
				//	Status = None;
				//}
				//else if( InstrChar(chHead, "x-rcpt-to:") == 0 ){
				//	Status = None;
				//}
				//else if( InstrChar(chHead, "status:") == 0 ){
				//	Status = None;
				//}
				//else if( InstrChar(chHead, "x-uidl:") == 0 ){
				//	Status = None;
				//}
				else{
					Status = None;
				}
				
				delete [] chHeadValue;

			}
			else if( ( Status != None ) ) //&& bWhiteSpace  )
			{	
				chHeadValue = substrL( chHead, static_cast<int>( strlen(chHead) ) );
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
					bIso = true;
				}
				else{
					bIso = false;
				}

				switch( Status ){
				
					case Receive:
						if( received_count > 0 && received_count <= 6 ){
							received[ received_count - 1 ].append(" ");
							received[ received_count - 1 ].append(chHeadValue);					
						}
						break;
					
					case Date:
						date.append(Space);
						date.append(chHeadValue);	
						break;
					
					case From:
						from.append(Space);
						strreplace( chHeadValue, "\\.", "." );
						from.append(chHeadValue);	
						break;
					
					case Subject:
						if( bIso == false ){
							subject.append(Space);
						}
						subject.append(chHeadValue);	
						break;

					case To:
						to.append(Space);
						ReplaceExtraEmailCommas(chHeadValue);
						to.append(chHeadValue);	
						break;

					case CC:
						cc.append(Space);
						ReplaceExtraEmailCommas(chHeadValue);
						cc.append(chHeadValue);	
						break;

					case BCC:
						bcc.append(Space);
						ReplaceExtraEmailCommas(chHeadValue);
						bcc.append(chHeadValue);	
						break;

					case MessageId:
						message_id.append(Space);
						message_id.append(chHeadValue);	
						break;

					case ContentEncoding:
						content_transfer_encoding.append(Space);
						content_transfer_encoding.append(chHeadValue);	
						break;

					case ContentType:
						content_type.append(Space);
						content_type.append(chHeadValue);	
						break;

					case ContentDisposition:
						content_disposition.append(Space);
						content_disposition.append(chHeadValue);	
						break;

					case MimeVersion:
						mime_version.append(Space);
						mime_version.append(chHeadValue);	
						break;

					case Charset:
						message_charset.append(Space);
						message_charset.append(chHeadValue);	
						break;

					case ReplyTo:
						reply_to.append(Space);
						reply_to.append(chHeadValue);	
						break;
					
					case InReplyTo:
						in_reply_to.append(Space);
						in_reply_to.append(chHeadValue);	
						break;

					case References:
						references.append(Space);
						references.append(chHeadValue);	
						break;

					case ReturnPath:
						return_path.append(Space);
						return_path.append(chHeadValue);	
						break;

					case Comments:
						comments.append(Space);
						comments.append(chHeadValue);	
						break;

					case Keywords:
						keywords.append(Space);
						keywords.append(chHeadValue);	
						break;

					case Encrypted:
						encrypted.append(Space);
						encrypted.append(chHeadValue);	
						break;

					case ContentID:
						content_id.append(Space);
						content_id.append(chHeadValue);	
						break;

					case ContentDescription:
						content_description.append(Space);
						content_description.append(chHeadValue);	
						break;

					case ContentMD5:
						content_md5.append(Space);
						content_md5.append(chHeadValue);	
						break;

					case ContentLanguage:
						content_language.append(Space);
						content_language.append(chHeadValue);	
						break;
				}

				delete [] chHeadValue;

			}

			delete [] chHead;

			// Skip delimiters.  Note the "not_of"
			lastPos = header.find_first_not_of("\n", pos);
			// Find next "non-delimiter"
			pos = header.find_first_of("\n", lastPos);
		}

		for ( int i = 0; i < received_count; i++ ) 
		{
			r = new char[received[ i ].length() + 1];
			r[ received[ i ].length() ] = 0;
			strcpy(r, received[ i ].c_str() );
			
			DecipherReceived( r, i );
			delete [] r;
		}
	
		GetNameEmail( &pToEmail, to, to_count );
		GetNameEmail( &pCcEmail, cc, cc_count );
		GetNameEmail( &pBCcEmail, bcc, bcc_count );

		if( from.length() > 0 ){
			GetNameEmailHelper( from.c_str() );
		}
		else{
			from_name.clear();
			from_email.clear();
			from_at.clear();
		}

	}
	catch( MiscError ){
		delete [] chHead; 
		delete [] chHeadValue; 
		delete [] r; 
		throw;
	}
	catch(...){
		delete [] chHead; 
		delete [] chHeadValue; 
		delete [] r; 	
		throw MiscError("DecipherHelper has caught an unknown error", 2001);
	}

} // end of function DecipherHelper

inline void NSHeader::ReplaceExtraEmailCommas( char * input )
{
	if( InstrChar( input, "," ) == -1 ) return;

	int iDoubleQuote = InstrChar( input, "\"" );
	if( iDoubleQuote != - 1 ){
		
		int iNextQuote = InstrChar( (input + (iDoubleQuote + 1)), "\"" );
		//input - iDoubleQuote - 1; // Wind this poihter back to it's original position.

		if( iNextQuote != - 1 ){
			iNextQuote += 1;

			int iComma = InstrChar( input, "," );			
			if( iComma != -1 && iComma > iDoubleQuote && iComma < iNextQuote ){
				input[iComma] = 1;
			}
		}
	}
}

inline void NSHeader::ReplaceHappyFaceWithComma( string & input )
{

	char * chTemp = 0;
	try{

		chTemp = c_string(input);
		int iHappyFace = InstrChar( chTemp, 1 );
		if( iHappyFace != -1 ){
			chTemp[iHappyFace] = 44;
			input = chTemp;
		}
		delete [] chTemp;

	}
	catch(...){
		delete [] chTemp; 
		throw MiscError("ReplaceHappyFaceWithComma has caught an unknown error", 6583);
	}

}

inline void NSHeader::shorten(string &input, int max)
{
	if( static_cast<int>( input.length() ) > max )
		input = input.substr( 0, max );
}

// This is a helper function of DecipherHelper. It builds an array of 
// objects that hold all of the to: name and e-mails of the message.
// The struct person_details defined in NSHeader.h is used to store the data.
// When the class is destroyed or when this function is called again the pointer
// is deleted and possibly re-built.
inline void NSHeader::GetNameEmail( person_details ** pd, string sNameEmail, int & iNameEmailCount )
{

	if( *pd ){
		delete [] *pd;
		*pd = 0;
		iNameEmailCount = 0;
	}

	try{

		if( sNameEmail.length() > 0 ){

			if( sNameEmail.find(",") == string::npos ){
				
				iNameEmailCount = 1;
				*pd = new person_details[ iNameEmailCount ]; // allocate storage

				GetNameEmailHelper( sNameEmail.c_str() );
				// prevent the struct from overflow
				shorten(from_name, MAX_PERSON_DETAILS_LENGTH - 1 );
				shorten(from_email, MAX_PERSON_DETAILS_LENGTH - 1 );

				strcpy( (*pd)->name, from_name.c_str() );					
				strcpy( (*pd)->email, from_email.c_str() );	

			}
			else{

				vector<string> tokens;
				Tokenize( sNameEmail, tokens, "," );			
				iNameEmailCount = static_cast<int>( tokens.size() );
				*pd = new person_details[ iNameEmailCount ]; // allocate storage

				string sTemp;
				int i = 0;
				typedef vector<string>::const_iterator VI;
				for(VI p=tokens.begin(); p!=tokens.end(); ++p){			
					sTemp = *p;

					GetNameEmailHelper( sTemp.c_str() );
					// prevent the struct from overflow

					shorten(from_name, MAX_PERSON_DETAILS_LENGTH - 1 );
					shorten(from_email, MAX_PERSON_DETAILS_LENGTH - 1 );
					ReplaceHappyFaceWithComma(from_name);

					strcpy( (*pd + i)->name, from_name.c_str() );					
					strcpy( (*pd + i)->email, from_email.c_str() );
					i += 1;
				}			
			}
		}
	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError("GetNameEmail2 has caught an unknown error", 4001);
	}
}

// This function is used to set the global variables
// sFrom, sFromName, sFromEmail to variables that 
// are created from searching *in.
inline void NSHeader::GetNameEmailHelper( const char *in )
{

	from_name.clear();
	from_email.clear();
	from_at.clear();

	string sFrom = "";
	string sFromName = "";
	string sFromEmail = "";

	int result = -1;
	int next_result = -1;
	basic_string <char>::size_type iFind;
	char *str = 0;

	try{

		if( strlen(in) == 0 ){		
			return;
		}

		str = new char[ strlen(in) + 1 ];
		str[ strlen(in) ] = 0;
		strcpy( str, in );
		strclean( str, "\"" );

		sFrom = str; // Assign the local variable sFrom equal to str created above
		
		// Now we are going to look for an opening bracket
		result = InstrChar( str, "<" );
		if ( result != -1 )
		{
			if( result > 1 )
			{
				sFromName = sFrom.substr( 0, result );
			}
			// Now we are going to look for a closing bracket
			next_result = InstrChar( str, ">" );
			if ( next_result != -1 )	
			{
				sFromEmail = sFrom.substr( result + 1, ( next_result - result - 1) );
			}
		}
		else
		{
			// Else we look for an @ symbol
			result = InstrChar( str, "@" );	
			if ( result != -1 )
			{
				if( result > 1 )
				{
					sFromEmail = sFrom;
					sFromName.clear();		
				}
				else
				{
					sFromName = sFrom;
					sFromEmail.clear();		
				}

			}
		}

		if( sFromName.length() == 0 && sFromEmail.length() > 0 ){		
			// gjelinek@imcingular.com (GREG JELINEK)
			size_t iLeftParen = sFromEmail.find(" (");
			if( iLeftParen != string::npos ){
				size_t iRightParen = sFromEmail.find(")");
				if( (iRightParen != string::npos) && (iRightParen > iLeftParen) ){
					sFromName = sFromEmail.substr( iLeftParen + 2, iRightParen - (iLeftParen + 2) );
					sFromEmail = sFromEmail.substr(0, iLeftParen);
				}
			}			
		}

		from_name = sFromName;
		from_email = sFromEmail;
		if( from_name.length() == 0 && from_email.length() == 0 && strlen(str) > 0 )
		{
			from_name.append(str);
		}

		delete [] str;
		
		if( from_name.length() > 0 )
		{
			LTrim(from_name);
			RTrim(&from_name);
			/////////////////////////////////////////////////////////////////
			if( from_name.find("\\.") != string::npos ){
				char name[1024];
				memset ((void *) &name, '\0', 1024);
				strcpy(name, from_name.c_str());
				strreplace(name, "\\.", ".");
				from_name = name;
			}
		    /////////////////////////////////////////////////////////////////
			iFind = from_name.find_first_of("'");
			if( iFind == 0 ){
				iFind = from_name.find_last_of("'");
				if( iFind == (from_name.length() - 1) ){
					from_name = from_name.substr(1, from_name.length() - 2);
				}
			}
			/////////////////////////////////////////////////////////////////
		} 

		if( from_email.length() > 0 )
		{
			LTrim(from_email);
			RTrim(&from_email);	
			/////////////////////////////////////////////////////////////////
			iFind = from_email.find_first_of( "@", 0 );
			if( iFind != string::npos && iFind > 0 ){
				from_at = from_email.substr( iFind );
				RTrim(&from_at);
			}
			/////////////////////////////////////////////////////////////////
		}

	}
	catch( MiscError ){	
		delete [] str; 
		throw;
	}
	catch(...){
		delete [] str; 
		throw MiscError("GetNameEmailHelper has caught an unknown error", 5001);
	}
}

// This is a helper function for DecipherHelper
// The function rips apart a single received element into
// it's possible defined tokens.
inline void NSHeader::DecipherReceived( char *received, int num )
{
	char *tokenPtr;
	string tempVal;
	string from, by, _with, id, _for;
	register basic_string <char>::size_type intPosA;
	register basic_string <char>::size_type intPosB;
	register basic_string <char>::size_type intPosC;
	register int step = 0;

	//from = by = _with = id = _for = "";

	if(InstrChar(received,"from") == -1)
	{
		intPosC = strlen(received);
		// 7/13/2006 What a mess...
		// we are dealing with something like this:
		// (qmail 24323 invoked by uid 60001); 13 Jul 2006 19:37:05 -0000
		if(InstrChar(received, "(") == 0 && intPosC > 1){
			++received;
			intPosA = InstrChar(received, ");");
			if(intPosA != -1)
			{
				intPosB = InstrChar(received, "invoked by");
				if(intPosB != -1){
					int i;
					for(i = 0; i < intPosB - 1; i++){
						tempVal = received[i];
						received_from[ num ].append(tempVal);
					}	
					i += 12;
					if(i < intPosC && i < intPosA){
						for(; i < intPosA; i++){
							tempVal = received[i];
							received_by[ num ].append(tempVal);						
						}
						i += 3;
						if(i < intPosC){
							received += i;
							received_date[ num ].append(received);
							return;							
						}				
					}
				}				
			}
		}
	}

	tokenPtr = strtok( received, " " );

	try{

		while( tokenPtr != NULL ){
			
			tempVal = tokenPtr;
			if( tempVal == "from" ){ 
				step = 1;
			}
			else if( tempVal == "by" ){ 	
				step = 2;
			}
			else if( tempVal == "with" ){ 	
				step = 3;
			}
			else if( tempVal == "id" ){		
				step = 4;
			}
			else if( tempVal == "for" ){		
				step = 5;
			}
			//Received: from uucore5pumper2 (uuout1relay1.uu.flonetwork.com [172.20.61.10])
			//	by uuout1smtp6.uu.flonetwork.com (Postfix) with SMTP id 28A5D10CBA9
			//	for <kai@langlie.com>; Tue, 30 Jul 2002 12:46:32 -0400 (EDT)

			switch( step ){
				
				case 1:
					if( tempVal != "from" ){
						from.append( tempVal );
						from.append( " " );
					} 	
				break;

				case 2:
					if( tempVal != "by" ){
						by.append( tempVal );
						by.append( " " );
					}
				break;

				case 3:
					if( tempVal != "with" ){
						_with.append( tempVal );
						_with.append( " " );
					}
				break;

				case 4:
					if( tempVal != "id" ){
						id.append( tempVal );
						id.append( " " );
					}
				break;

				case 5:
					if( tempVal != "for" ){
						_for.append( tempVal );
						_for.append( " " );
					}
				break;
			}

			tokenPtr = strtok( NULL, " " );
		}
		
		if( step == 1 ){
			if( from.length() > 0 ){
				intPosA = from.find_last_of( ";" );
				if( intPosA > 0 ){
					received_date[ num ] = from.substr(intPosA + 1, from.length() - (intPosA + 1));
				}
			}
		}
		else if( step == 2 ){
			if( by.length() > 0 ){
				intPosA = by.find_last_of( ";" );
				if( intPosA > 0 ){
					received_date[ num ] = by.substr(intPosA + 1, by.length() - (intPosA + 1));
				}			
			}
		}
		else if( step == 3 ){
			if( _with.length() > 0 ){
				intPosA = _with.find_last_of( ";" );
				if( intPosA > 0 ){
					received_date[ num ] = _with.substr(intPosA + 1, _with.length() - (intPosA + 1));
					_with = _with.substr( 0, intPosA );
				}			
			}
		}
		else if( step == 4 ){
			if( id.length() > 0 ){
				intPosA = id.find_last_of( ";" );
				if( intPosA > 0 ){
					received_date[ num ] = id.substr(intPosA + 1, id.length() - (intPosA + 1));
				}			
			}
		}
		else if( step == 5 ){
			if( _for.length() > 0 ){
				intPosA = _for.find_last_of( ";" );
				if( intPosA > 0 ){
					received_date[ num ] = _for.substr(intPosA + 1, _for.length() - (intPosA + 1));
					_for = _for.substr( 0, intPosA );
				}
			}
		}

		if( _with.length() > 0 ){
			intPosA = _with.find_last_of( ";" );
			if( intPosA > 0 ){
				_with = _with.substr( 0, intPosA );
			}
			LTrim(_with);
			RTrim(&_with);
			// add received_with here
			received_with[ num ] = _with;
		}

		if( _for.length() > 0 ){
			intPosA = _for.find_last_of( ";" );
			if( intPosA > 0 ){
				_for = _for.substr( 0, intPosA );
			}
			LTrim(_for);
			RTrim(&_for);	
			// add received_for here
			received_for[ num ] = _for;
		}

		received_from[ num ] = from; // entire from contents

		// This next piece of code rips out the domain and ip address from from
		intPosA = from.find_first_of( "(", 0 );
		if( intPosA > 0 ){
			intPosB = from.find_first_of( ")", intPosA );
			if( intPosB > 0 ){
				intPosC = from.find_first_of( "[", intPosA );
				if( intPosC == ( intPosA + 1 )){			
					received_domain[ num ] = from.substr(0, intPosA - 1);
					intPosB = from.find_first_of( "]", intPosA + 1 );	
					intPosA+=2;
					//received_ip[ num ] = from.substr( intPosA, ( intPosB ) - intPosA);
					string sIP = from.substr( intPosA, ( intPosB ) - intPosA);
					if( sIP.length() > 0 ){
						RTrim(&sIP);
					}
					if( IsIPv4Address(sIP) == true ){
						received_ip[ num ] = sIP;
					}
				}
				else{
					++intPosA;
					from = from.substr( intPosA, ( intPosB ) - intPosA);
					intPosA = from.find_first_of( "[", 0 );
					if( intPosA > 0 ){
						intPosB = from.find_first_of( "]", intPosA );
						if( intPosB > 0 ){
							received_domain[ num ] = from.substr(0, intPosA - 1);
							++intPosA;
							string sIP = from.substr( intPosA, ( intPosB ) - intPosA);
							if( sIP.length() > 0 ){
								RTrim(&sIP);
							}
							if( IsIPv4Address(sIP) == true ){
								received_ip[ num ] = sIP;
							}								
						}	
					}
				}
			}	
		}
		else{
			intPosA = from.find_first_of( "[", 0 );
			if( intPosA > 0 ){
				intPosB = from.find_first_of( "]", intPosA );
				if( intPosB > 0 ){
					received_domain[ num ] = from.substr(0, intPosA - 1);
					++intPosA;
					//received_ip[ num ] = from.substr( intPosA, ( intPosB ) - intPosA);
					string sIP = from.substr( intPosA, ( intPosB ) - intPosA);
					if( sIP.length() > 0 ){
						RTrim(&sIP);
					}
					if( IsIPv4Address(sIP) == true ){
						received_ip[ num ] = sIP;
					}
				}	
			}
		}

		received_by[ num ] = by; // by is ok by itself

		if( id.length() > 0 )
		{
			// remove the semi-colon and everything after 
			// it from the received id if there is one.
			intPosA = id.find_first_of( ";", 0 );
			if( intPosA > 0 ){
				id = id.substr( 0, intPosA );	
			}

			// remove parenthesis from the id
			intPosA = id.find_first_of( "<", 0 );
			if( intPosA >= 0 ){
				intPosB = id.find_first_of( ">", intPosA );
				if( intPosB > 0 ){
					++intPosA;
					id = id.substr( intPosA, ( intPosB ) - intPosA);
				}	
			}

			received_id[ num ] = id;
		}
		
		if( received_from[ num ].length() > 0 ){
			RTrim(&received_from[ num ]);
		}
		
		if( received_by[ num ].length() > 0 ){			
			intPosA = received_by[ num ].find_last_of( ";" );
			if( intPosA > 0 ){
				received_by[ num ] = received_by[ num ].substr(0, intPosA);
			}
			RTrim(&received_by[ num ]);
			if( received_ip[ num ].length() == 0 && IsIPv4Address(received_by[ num ]) == true ){
				received_ip[ num ] = received_by[ num ];
			}
			else if( received_ip[ num ].length() == 0 && received_from[ num ].length() > 0 ){
				intPosA = received_from[ num ].find_first_of("[");
				if( intPosA == 0 ){
					string sIp = received_from[ num ].substr( intPosA + 1 );
					RTrim(&sIp);
					intPosA = sIp.find_last_of("]");
					if( intPosA = (sIp.length() - 1) ){
						sIp = sIp.substr(0, intPosA);
						if( IsIPv4Address(sIp) == true ){
							received_ip[ num ] = sIp;	
						}	
					}
				}
			}
		}
		
		//RTrim(&received_id[ num ]);

		if( received_domain[ num ].length() > 0 ){
			RTrim(&received_domain[ num ]);
		}
				
		if( received_date[ num ].length() > 0 ){
			LTrim(received_date[ num ]);
			RTrim(&received_date[ num ]);		
		}	

	}
	catch( MiscError ){	
		throw;
	}
	catch(...){
		throw MiscError("DecipherReceived has caught an unknown error", 4001);
	}

}

inline bool NSHeader::IsIPv4Address( string &ip )
{
	if( ip.length() > 15 || ip.length() < 7 ){
        //Host is longer or shorter than possible IP		
		return false;
	}

	register char *ipChar = 0;
	register int iCount = 0;
	register bool bPass = true;
	register int i = 0;

	try{
			
		string sNumSection = "";
					
		ipChar = new char[ip.length() + 1];
		ipChar[ ip.length() ] = 0;
		strcpy(ipChar, ip.c_str() );

		int no, count, pos[64];
		count=bmsearch(ipChar,".",&no,pos);

		if(no>0){
			iCount = no;		
		}

		if( iCount == 3 ){
		
			string sChar(ipChar);
			if(no > 0)
			{
				int iPosition = 0;
				for(i=0;i<=no;i++){

					iPosition = pos[i];
					if(i==0){
						sNumSection = sChar.substr(0, iPosition);
					}
					else{
						sNumSection = sChar.substr( pos[i-1] + 1, iPosition - pos[i-1] - 1 );				
					}
					
					int iNum = ConvertStringToInt(sNumSection);
					if( iNum < 0 || iNum > 255 ){
						bPass = false;
						break;
					}

				}

			}

		}
		else{
			bPass = false;
		}

		delete [] ipChar;

		return bPass;

	}
	catch( MiscError ){
		delete [] ipChar; 
		throw;
	}
	catch(...){
		delete [] ipChar; 
		throw MiscError("IsIPv4Address has caught an unknown error", 3826);
	}
}

//inline void NSHeader::GetNameEmail( person_details ** pd, string sNameEmail, int & iNameEmailCount )
//{
//	if( *pd ){
//		delete [] *pd;
//		*pd = 0;
//		iNameEmailCount = 0;
//	}
//	
//	char *toChar = 0;
//
//	try{
//
//		if( sNameEmail.length() > 0 ){
//			
//			string sEmail;
//			int i = 0;
//			
//			toChar = new char[sNameEmail.length() + 1];
//			toChar[ sNameEmail.length() ] = 0;
//			strcpy(toChar, sNameEmail.c_str() );
//
//			int no, count, pos[ MAX_NUMBER_EMAILS ];
//			count=bmsearch(toChar,",",&no,pos);
//            				
//			iNameEmailCount = 1;
//			if(no>0){
//				iNameEmailCount += no;		
//			}
//
//			*pd = new person_details[ iNameEmailCount ]; // allocate storage
//			string sChar(toChar);
//
//			if(no > 0)
//			{
//				int iPosition = 0;
//
//				for(i=0;i<no;i++){
//					
//					iPosition = pos[i];
//					if(i==0){
//						sEmail = sChar.substr(0, iPosition);
//					}
//					else{
//						sEmail = sChar.substr( pos[i-1] + 2, iPosition - pos[i-1] - 1 );				
//					}
//
//					GetNameEmailHelper(sEmail.c_str());					
//					// prevent the struct from overflow
//					shorten(from_name, MAX_PERSON_DETAILS_LENGTH - 1 );
//					shorten(from_email, MAX_PERSON_DETAILS_LENGTH - 1 );
//					ReplaceHappyFaceWithComma(from_name);
//
//					strcpy( (*pd + i)->name, from_name.c_str() );					
//					strcpy( (*pd + i)->email, from_email.c_str() );
//				}
//
//				sEmail = sChar.substr( iPosition + 1 );
//				GetNameEmailHelper(sEmail.c_str());
//				// prevent the struct from overflow
//
//				shorten(from_name, MAX_PERSON_DETAILS_LENGTH - 1 );
//				shorten(from_email, MAX_PERSON_DETAILS_LENGTH - 1 );
//				ReplaceHappyFaceWithComma(from_name);
//
//				strcpy( (*pd + (iNameEmailCount - 1))->name, from_name.c_str() );					
//				strcpy( (*pd + (iNameEmailCount - 1))->email, from_email.c_str() );
//
//			}
//			else{
//
//				GetNameEmailHelper( sChar.c_str() );
//				// prevent the struct from overflow
//				shorten(from_name, MAX_PERSON_DETAILS_LENGTH - 1 );
//				shorten(from_email, MAX_PERSON_DETAILS_LENGTH - 1 );
//
//				strcpy( (*pd)->name, from_name.c_str() );					
//				strcpy( (*pd)->email, from_email.c_str() );		
//			}
//			delete [] toChar;
//		}
//	}
//	catch( MiscError ){
//		delete [] toChar; 
//		throw;
//	}
//	catch(...){
//		delete [] toChar; 
//		throw MiscError("GetNameEmail has caught an unknown error", 3001);
//	}
//}