// EmailRipper
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission

#include "stdafx.h"
#include "EmailRipper.h"

//#include<iostream>
//using namespace std;

const char *text_plain = "text/plain";
//------------------------------------------------------------------
const char *text_html = "text/html";
//------------------------------------------------------------------
const char *text_enriched = "text/enriched"; 
//------------------------------------------------------------------
const char *base64 = "base64";
//------------------------------------------------------------------
const char *quoted_printable = "quoted-printable";
//------------------------------------------------------------------
const char *mixed = "mixed";
//------------------------------------------------------------------
const char *digest = "digest";
//------------------------------------------------------------------
const char *application = "application";
//------------------------------------------------------------------
const char *multipart = "multipart";
//------------------------------------------------------------------
const char *alternative = "alternative";
//------------------------------------------------------------------
const char *multipart_alternative = "multipart/alternative";
//------------------------------------------------------------------
const char *boundary = "boundary=";
//------------------------------------------------------------------
const char *sevenBit = "7bit"; 
//------------------------------------------------------------------
const char *attachment = "attachment";
//------------------------------------------------------------------
const char *message_rfc = "message/rfc822";
//------------------------------------------------------------------
const char Lf[2] = { 10, 0 };
//------------------------------------------------------------------
const char crLf[3] = { 13, 10, 0 };
//------------------------------------------------------------------
const char double_crLf[5] = { 13, 10, 13, 10, 0 };
//------------------------------------------------------------------
const char uu_begin_marker[9] = { 13, 10, 'b', 'e', 'g', 'i', 'n', 32, 0 } ;
//------------------------------------------------------------------
const char msg_begin[7] = { 'b', 'e', 'g', 'i', 'n', 32, 0 };
//------------------------------------------------------------------
const char uu_end_marker[6] = { 'e', 'n', 'd', 13, 10, 0 } ;
//------------------------------------------------------------------
const char Space[2] = { 32, 0 } ;
//------------------------------------------------------------------
const char *iso_8859_1 = "=?iso-8859-1"; //?q?";
//--
const char *iso_b = "?b?";
//------------------------------------------------------------------
const char *iso_q = "?q?";
//------------------------------------------------------------------
const char *misc_windows = "?windows-1252?Q?";
//------------------------------------------------------------------
//const char *iso_b = "=?iso-8859-1?"; //B?";
//------------------------------------------------------------------
const char *name_equals = "name=";
//------------------------------------------------------------------
const char *filename_equals = "filename=";
//------------------------------------------------------------------
const char *application_octet_stream = "application/octet-stream";
//------------------------------------------------------------------
const char double_period[4] = { 10, '.', '.', 0 } ;
//------------------------------------------------------------------
const char *message_delivery_status = "message/delivery-status";
//------------------------------------------------------------------

using namespace WEBARBOR_EMAIL_COMPONENTS;

// EmailRipper constructor
EmailRipper::EmailRipper()
{ 
	InitializeConstructor();	
}

// EmailRipper constructor
EmailRipper::EmailRipper( const char *path )  
{	
	InitializeConstructor();
	setFilePath( path );    
}

inline void EmailRipper::InitializeConstructor()
{
	pFilePath = 0;              // pointer variable
	pLogPath = 0;			    // pointer varliable
	pSaveLocation = 0;		    // pointer variable
	pReturnMessage = 0;		    // pointer variable
	pInlineAddItem = 0;         // pointer variable
	//
	setErrorMessage("OK");
	bIsTrialVersion = false;
	bTestLogging = false;
	bInlineAddItem = false;
	bSave = false;
	bLog = false;

	ResetIma();
}

void EmailRipper::ResetIma()
{
	if(all_tokens.size() > 0) all_tokens.clear();
	
	b_IsLoaded = false;
	b_Lowercase = false;

	line_count = 0;
	f_count = 0;
	s_count = 0;
	n_count = 0;
	t_count = 0;
	h_count = 0;
	b_count = 0;
	k_count = 0;

	compare_result = -1;
}

// EmailRipper destructor
// Make sure to delete all global pointers that have been allocated
EmailRipper::~EmailRipper()
{
	delete [] pFilePath; 
	delete [] pLogPath;
	delete [] pSaveLocation; 
	delete [] pReturnMessage;
	delete [] pInlineAddItem;
}

string EmailRipper::getHeader(){
	return header;
}

string EmailRipper::getMessageBody( int iMessageArea, int iPart ){
	
	if( iMessageArea < 1 || iMessageArea > 4 ){
		return "";
	}
	else if( iPart < 0 || ( (iPart + 1) > static_cast<int>( vContent.size() ) ) ){
		return "";
	}
	string sRetVal = "";
	NSMessageContent oContent;
	int iCurrent = 0;
	typedef vector<NSMessageContent>::const_iterator VI;
	for(VI p=vContent.begin(); p!=vContent.end(); ++p){			
		oContent = *p;
		if( oContent.GetEncoding() == iMessageArea ){
			if( iCurrent == iPart ){
				sRetVal = oContent.GetBody();
				break;
			}
			else{
				iCurrent += 1;
			}		
		}
	}

	if( bIsTrialVersion == false ){
		return sRetVal;
	}
	else if( iMessageArea == 1 ){
		return "WebArborEmail Trial\r\n-------------------\r\n" + sRetVal;			       
	}
	else{
		return sRetVal;
	}
}

// This function will check to see that a file path exists
void EmailRipper::setFilePath( const char *Path )
{
	if( pFilePath )	delete [] pFilePath; // reclaim string
	pFilePath = new char[ strlen(Path) + 1 ]; // allocate storage
	//assert( pFilePath != 0 ); // terminate if memory not allocated
	strcpy( pFilePath, Path ); // copy literal to object
}	

// This function will check to see that a file path exists
void EmailRipper::setLogPath( const char *Path )
{
	if( pLogPath ) delete [] pLogPath; // reclaim string
	register size_t iLen = strlen(Path);	
	if( iLen > 0 && DirExists(Path) ){
		register bool bCat92 = false;
		if( Path[ iLen - 1 ] != 92 ){
			bCat92 = true;
			iLen += 1;
		}		
		pLogPath = new char[ iLen + 1 ]; // allocate storage
		strcpy( pLogPath, Path ); // copy literal to object		
		if( bCat92 == true ){
			strcat( pLogPath, "\\" );
		}
		//assert( pLogPath != 0 );
		bLog = true;
	}
	else{
		bLog = false;
	}
}

// This function will check to see that a file path exists
void EmailRipper::setSaveLocation( const char *Path )
{
	if( pSaveLocation ) delete [] pSaveLocation; // reclaim string
	register size_t iLen = strlen(Path);	
	if( iLen > 0 && DirExists(Path) ){
		register bool bCat92 = false;
		if( Path[ iLen - 1 ] != 92 ){
			bCat92 = true;
			iLen += 1;
		}		
		pSaveLocation = new char[ iLen + 1 ]; // allocate storage
		strcpy( pSaveLocation, Path ); // copy literal to object		
		if( bCat92 == true ){
			strcat( pSaveLocation, "\\" );
		}
		//assert( pSaveLocation != 0 );
		bSave = true;
	}
	else{
		bSave = false;
	}
}

void EmailRipper::setInlineAddItem( const char *AddItem )
{
	if( pInlineAddItem ) delete [] pInlineAddItem; // reclaim string
	register size_t iLen = strlen(AddItem);	
	if( iLen > 0 ){
		register bool bCat92 = false;
		register bool bDirExists = DirExists(AddItem); 
		if( bDirExists && AddItem[ iLen - 1 ] != 92 ){
			bCat92 = true;
			iLen += 1;
		}
		if( bDirExists ){
			const char * chFile = "file:///";
			iLen += strlen(chFile);
			pInlineAddItem = new char[ iLen + 1 ]; // allocate storage
			strcpy( pInlineAddItem, chFile );
			strcat( pInlineAddItem, AddItem ); // copy literal to object		
		}
		else{
			pInlineAddItem = new char[ iLen + 1 ]; // allocate storage
			strcpy( pInlineAddItem, AddItem ); // copy literal to object		
		}		
		if( bCat92 == true ){
			strcat( pInlineAddItem, "\\" );
		}
		//assert( pInlineAddItem != 0 );
		bInlineAddItem = true;
	}
	else{
		bInlineAddItem = false;
	}
}

// This function will check to see that a file path exists
void EmailRipper::setErrorMessage( const char *Message )
{
	if( pReturnMessage ) delete [] pReturnMessage; // reclaim string
	pReturnMessage = new char[ strlen(Message) + 1 ]; // allocate storage
	//assert( pReturnMessage != 0 ); // terminate if memory not allocated
	strcpy( pReturnMessage, Message ); // copy literal to object
}

// This function will check to see that a file path exists
void EmailRipper::setErrorMessage( const char *Message, int iNum )
{
	if( pReturnMessage ) delete [] pReturnMessage; // reclaim string
	pReturnMessage = new char[ strlen(Message) + 1 ]; // allocate storage
	//assert( pReturnMessage != 0 ); // terminate if memory not allocated
	strcpy( pReturnMessage, Message ); // copy literal to object
	return_status = iNum; // 2004-03-18a
}

// This method is called regardless of 
// which Decipher method was called
inline void EmailRipper::InitializeEmailRipperVariables()
{
	header.clear(); 
	m_Body.clear();
    m_strBoundary.clear();

	bInlineText = false;
	bHasContentId = false;	
	bMulti = false;
	
	contentid.clear();	
	vContent.clear();
	i_text_message_count = 0;
	i_html_message_count = 0;
	i_enriched_message_count = 0;
	i_message_delivery_status_count = 0;
	message_size = 0;
	return_status = 0;

	i_non_inline_attachment_count = 0;
	i_inline_attchment_count = 0;
	i_inline_attachment_text_count = 0;
	vAttachment.clear();
	//i_mime_level = 0;	
}

// Method: Decipher
// Purpose: This method is overloaded for fils
void EmailRipper::Decipher( const char *save_location )
{
	try{
		if( FileExists(pFilePath) == 0 ){
			throw MiscError("Decipher::FileExists - File does not exist.", 4168);
		}
		else{
			//
			Decipher( OpenFile(pFilePath), save_location );
			//
		}
	}
	catch( MiscError ex ){
		setErrorMessage(ex.what());
		return_status = ex.number();	
	}
	catch( FileWillNotOpen ex ){
		setErrorMessage(ex.what());
		return_status = 1;
	}
}

// Method: Decipher
// Purpose: This method is used to start the decoding process
void EmailRipper::Decipher( const char *in_header, const char*save_location )
{

	try{
	
		InitializeEmailRipperVariables();
		setSaveLocation( save_location );

		if( strlen( in_header ) ){
			
			header = in_header;			
			int iPos = InstrChar( in_header, double_crLf );	
			
			if( iPos != -1 ){
				m_Body = header.substr( iPos + 4 );
				header = header.substr( 0, iPos + 2 );			   
			}

			DecipherHelper();
				
		}
		else{
			setErrorMessage("Message length is too short");
			return_status = 4;
			string s( in_header );
			LogMessage( s );
		}

	}
	catch( MiscError ex ){
		setErrorMessage(ex.what());
		return_status = ex.number();	
		LogMessage( header + crLf + m_Body );	
	}
	catch(...){
		setErrorMessage("EmailRipper::Decipher has caught an unknown error");
		return_status = 75;
		LogMessage( header + crLf + m_Body );	
	}

}

void EmailRipper::DecipherHelper(){

	try{

		message_size = static_cast<long>(sizeof(char) * ( header.length() + m_Body.length() ));

		GetHeaders(header);
		//
		if( b_IsLoaded ){
			//Delivery rules search the first 32,000 bytes of a message.
			if( m_Body.length() > 32000 ){
				compare_result = SearchImaTokens(getFromName(), getFromEmail(), getSubject(), getSender(), getTo(), header.c_str(), m_Body.substr(0,32000).c_str() );
			}
			else{
				compare_result = SearchImaTokens(getFromName(), getFromEmail(), getSubject(), getSender(), getTo(), header.c_str(), m_Body.c_str() );
			}					
		}
		else{
			compare_result = -1;
		}
		//
		if( bIsTrialVersion == true && message_size > 500000 ){
			// 500000 is .5 MEG
			NSMessageContent oContent;
			oContent.SetEncoding(1);
			i_text_message_count += 1;
			string sMessage("THIS IS TRIAL SOFTWARE. CONTENT MUST BE LESS THAN .5 MEG IN ORDER TO BE DECODED.");
			sMessage.append(crLf);
			sMessage.append("(c) 2007 Langlie Systems, Inc.");
			oContent.SetBody(sMessage);
			vContent.push_back(oContent);						
		}
		else if( m_Body.length() > 0 ){
			DecipherBody(); // now decipher the body
		}

		if( bTestLogging == true ){
			LogMessage( header + crLf + m_Body );
		}

	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError( "EmailRipper::DecipherHelper has caught an unknown error", 2 );
	}
}

inline void EmailRipper::RemoveInlineContent( string &inVar )
{

	char * s1 = 0;
    char * s2 = 0;
	char * s3 = 0;

	try{

		size_t iFind;
		typedef vector<string>::const_iterator VI;
		for(VI p=contentid.begin(); p!=contentid.end(); ++p){
			string sTemp = *p;
			iFind = sTemp.find( "no_cid=", 0 );
			if( iFind!=string::npos ){
					
				sTemp = sTemp.substr( iFind + 9 );	
				string sFind = sTemp.substr(0, sTemp.find("|",0));
				string sReplace = sTemp.substr(sTemp.find("|",0) + 1);
				/* Call a helper function to remove the content. */
				s1 = c_string(inVar);
				s2 = c_string(sFind);
				if( bInlineAddItem == true ){
					string sTemp = sReplace;
					sReplace.clear();				
					sReplace.append( pInlineAddItem );
					sReplace.append( sTemp ) ;
				}
				s3 = c_string(sReplace);
				inVar = ReplaceInlineContentHelper( s1,
													s2,
													s3 );
				delete [] s1;
				delete [] s2;
				delete [] s3;
				/* Make sure to clean up all used resources. */
			}
		}
	}
	catch( MiscError ){
		delete [] s1;
		delete [] s2;
		delete [] s3;
		throw;
	}
	catch(...){		
		delete [] s1;
		delete [] s2;
		delete [] s3;
		throw MiscError( "RemoveInlineContent has run into an unexpected problem", 6492 );
	}
}

inline string EmailRipper::ReplaceInlineContentHelper(char * input_string, char *find, char *replace)
{
    
	char * sTempChar = 0;

	try{

		string sTempString;
		register basic_string <char>::size_type iInputLen = strlen(input_string);
		register basic_string <char>::size_type iFindLen = strlen(find);
		register basic_string <char>::size_type iReplaceLen = strlen(replace);

		if(iInputLen > 0 && iFindLen > 0 && iReplaceLen > 0)
		{
			if( iFindLen > iReplaceLen ){
				strreplace( input_string, find, replace );
				sTempString = input_string;
			}
			else{
				unsigned long dwAllocTempVar = DetermineNewMessageBodySize(input_string, iInputLen, find, iReplaceLen - iFindLen);
				if( dwAllocTempVar > 0 )
				{
					sTempChar = new char[dwAllocTempVar];
					strcpy(sTempChar, input_string);

					strreplace( sTempChar, find, replace );
					sTempString = sTempChar;

					delete [] sTempChar;
				}
				else{
					sTempString = input_string;
				}				
			}
			b_DidRemoveInlineContent = true;
		}
		else{
			sTempString = input_string;
		}
		
		return sTempString;

	}
	catch( MiscError ){
		delete [] sTempChar;
		throw;
	}
	catch(...){
		delete [] sTempChar;
		throw MiscError( "ReplaceInlineContentHelper has run into an unexpected problem", 5492 );
	}
}

inline unsigned long EmailRipper::DetermineNewMessageBodySize(char *sInput, size_t input_len, char *find, size_t replace_size)
{
	try{
		register int iCount = FindAllOccurences( sInput, find );
		if( iCount != -1 ){
			return DetermineBufferSize( static_cast<unsigned long>(input_len + (replace_size * iCount)) );
		}
		else{
			return 0;
		}
	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError("DetermineNewMessageBodySize has caught an unexpected error.", 4124);
	}
}

inline string EmailRipper::GetBoundary( string & sInput, int iStartPos )
{

	string sBoundary = sInput.substr( iStartPos + 9 );
	register string::size_type iPos = sBoundary.find( 34, 0 );

	if( iPos != string::npos ){
		sBoundary = sBoundary.substr( iPos + 1 );
		iPos = sBoundary.find( 34, 0 );
		if( iPos != string::npos ){
			sBoundary = sBoundary.substr( 0, iPos );
		}
	}
	else{
		iPos = sBoundary.find( 32, 0 );
		if( iPos != string::npos ){
			sBoundary = sBoundary.substr( 0, iPos );
		}
	}

	return sBoundary;
}

// DecipherBody is where the body deciphering fun begins.
// The function uses the global m_Body variable and passes it
// among a bunch of functions to eventually end up with a message
// and or attachments.
inline void EmailRipper::DecipherBody()
{

	register int iPos;

	char *smallContType = 0;
	char *smallContEncoding = 0;

   try{
	
		smallContType = new char[ content_type.length() + 1 ];
		smallContType[ content_type.length() ] = 0;
		strcpy( smallContType, content_type.c_str() );
		ToLower( smallContType );

		smallContEncoding = new char[ content_transfer_encoding.length() + 1 ];
		smallContEncoding[ content_transfer_encoding.length() ] = 0;
		strcpy( smallContEncoding, content_transfer_encoding.c_str() );
		ToLower( smallContEncoding );
	
		if( bSave == true )
			RemoveUUCode( m_Body );       

		if( strstr( smallContType, mixed ) != NULL ||
				strstr( smallContType, digest ) != NULL ||
				strstr( smallContType, application ) != NULL ||
				strstr( smallContType, multipart ) != NULL ||
				strstr( smallContType, alternative ) != NULL ){
		
			if( strstr( smallContType, boundary ) != NULL ){
				
				iPos = InstrChar( smallContType, boundary );
				if( iPos != -1 ){

					m_strBoundary = GetBoundary( content_type, iPos );
					if( DecipherBodyHelper() > 0 ){
					
						bMulti = true;
					
					}
				}
			}
		}
		
		if( bMulti == false ){
			
			string sBody = "";
			if( strstr( smallContEncoding, base64 ) != NULL && bSave == true  ){

				bool bFile = false;
				string sName = "";
				string sFileName = "";
				string sReturnFileName = ExtractFileName( sName, sFileName, content_type, content_disposition );
				if( sReturnFileName.length() > 0 ){			
					
					NSBase64 o64;
					sReturnFileName = AddAttachmentName( sReturnFileName, 1 );
					o64.writeMimeFile2( m_Body, pSaveLocation, sReturnFileName.c_str() );
					
					bFile = true;
				}	

				if( bFile == false && m_Body.length() > 0 ){
					sBody = MimeHelper( m_Body, pSaveLocation, "mime_file_output.txt" );
				}				
			}
			else if( strstr( smallContEncoding, quoted_printable ) != NULL ){
				NSQuotedPrintable oQP(m_Body);
				if( strstr( smallContType, text_html ) != NULL ){
					sBody = oQP.DecodeQP(true);
				}
				else{
					sBody = oQP.DecodeQP(false);
				}				
			}
			else{
				RemoveDoublePeriodsFromMessage( m_Body ); 
				sBody = m_Body;
			}				
			
			if( sBody.length() > 0 ){
				NSMessageContent oContent;
				if( strstr( smallContType, text_html ) != NULL ){
					oContent.SetEncoding(2);
					i_html_message_count += 1;
				}
				else if( strstr( smallContType, text_enriched ) != NULL ){
					oContent.SetEncoding(3);
					i_enriched_message_count += 1;					
				}
				else{
					oContent.SetEncoding(1);
					i_text_message_count += 1;
				}
				oContent.SetBody( sBody );
				vContent.push_back( oContent );
			}
		}

		delete [] smallContType;
		delete [] smallContEncoding;

		m_Body.clear();

	}
	catch( MiscError ){
		delete [] smallContType;
		delete [] smallContEncoding;		
		throw;
	}
	catch(...){
		delete [] smallContType;
		delete [] smallContEncoding;
		throw MiscError( "DecipherBody has caught an unknown error.", 6001 );

	}
}

inline int EmailRipper::DecipherBodyHelper(){
	

	int mCount = FindMimeContent( m_Body );
	if( mCount > 0 ){					
        
		string** pMimeContent = new string*[ mCount ]; 
		
		try{
			GetMimeContent( m_Body, pMimeContent, mCount );
			SortMimeContent( m_Body, pMimeContent, mCount );
		}
		catch( MiscError ){

			// Delete content from free store
			for( int i = 0; i < mCount; i++ ){
				if( pMimeContent[i] )
					delete pMimeContent[i];
			}

			delete [] pMimeContent;
			throw;
		}

		// Delete content from free store
		for( int i = 0; i < mCount; i++ )
			delete pMimeContent[i];

		// Now delete the array of pointers
		delete [] pMimeContent;

		// remove inline content
		if( bHasContentId==true && vContent.size() > 0 ){

			string sBody;
			NSMessageContent oContent;
			vector<NSMessageContent>vTempContent;

			int iRemoveCount = 0;
			typedef vector<NSMessageContent>::const_iterator VI;
			for(VI p=vContent.begin(); p!=vContent.end(); ++p){			
				oContent = *p;
				sBody = oContent.GetBody();
				if( oContent.GetEncoding() == 2 ){
					// If RemoveInlineContent replaces any content,
					// b_DidRemoveInlineContent is set to true.
					// Otherwise nothing has been changed, so there is
					// no need to increment iRemoveCount.
					b_DidRemoveInlineContent = false;
					RemoveInlineContent( sBody );
					if( b_DidRemoveInlineContent == true ){
						iRemoveCount += 1;
					}					
				}
				oContent.SetBody( sBody );
				vTempContent.push_back( oContent );								
			}
			
			if( iRemoveCount > 0 ){
				vContent.swap(vTempContent);
			}
			else if( iRemoveCount == 0 ){
				// Turn all non-text inline attachments back into normal ones.
				ChangeAllNonTextInlineAttachmentsToNormalOnes();
			}
			
		}
		else if( vContent.size() == 0 && vAttachment.size() == 1 && i_non_inline_attachment_count == 1 )
		{
			string sVal = "";
			NSMessageContent oContent;
			typedef vector<NSMessageContent>::const_iterator VI;
			for(VI p=vAttachment.begin(); p!=vAttachment.end(); ++p){			
				oContent = *p;
				if( oContent.GetEncoding() == 1 ){
					sVal = oContent.GetBody();
					if( sVal.find(".htm", 0) != string::npos )
					{
						char file_path[1024];
						file_path[1023] = 0;
						strcpy( file_path, pSaveLocation );
						strcat( file_path, sVal.c_str() );

						string sContent = OpenFile( file_path );
						NSMessageContent oContent;
						oContent.SetEncoding(2);
						oContent.SetBody( sContent );
						vContent.push_back( oContent );
						i_html_message_count += 1;
						RemoveAttachmentName( sVal, 1 );
						ShredFile( file_path );
						break;
					}
				}
			}						
		}
	}

	return mCount;
}

// Based on m_strBoundary found in DecipherBody above, FindMimeContent
// will count how many mime boundarys are in the message. It returns
// a count of how many pieces of mime content it finds. Note how it
// is used recursively in the function SortMimeContent below.
inline int EmailRipper::FindMimeContent( string &inVar )
{

	register string::size_type iStart, iMiddle, iEnd;
	register int iBorderCount = 0;
    string m_strEndBoundary = "";

	try{

		if( m_strBoundary.length() > 0 && inVar != "" ){
			
			iStart = inVar.find( "--" + m_strBoundary, 0 );
			if( iStart == string::npos){
				iStart = inVar.find( m_strBoundary, 0 );
			}
			else{
				m_strBoundary = "--" + m_strBoundary;
			}
			m_strEndBoundary = m_strBoundary + "--";

			if( iStart != string::npos ){
				
				iStart = 0;												
				iMiddle = inVar.find( m_strBoundary, iStart );
				iEnd = inVar.find( m_strEndBoundary, iStart ); 

				while( iMiddle != string::npos ){

					if( iMiddle != iEnd ){
						iBorderCount++;
					}
					iStart = iMiddle + m_strBoundary.length();
					iMiddle = inVar.find( m_strBoundary, iStart );
				
				}
				
				return iBorderCount;			
			}
				
		}

	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError( "FindMimeContent has caught an unknown error.", 7001 );
	}

	return 0;

} // End of FindMimeContent

// GetMimeContent uses similar algorithim as FindMimeContent
// but since this function already knows how many seperate mime entities exist,
// it uses the pointer variable pStr that is passed in byref to return the mime content.
inline void EmailRipper::GetMimeContent( string &inVar, string* pStr[], int border_count )
{

	register string::size_type iStart, iMiddle, iNext; //iEnd
	register int index = 0;
	register int iCount = 0;

	try{

		if( m_strBoundary.length() > 0 && inVar != "" ){
			
			iStart = 0;												
			iMiddle = inVar.find( m_strBoundary, iStart );

			while( iMiddle != string::npos ){
				
				iStart = iMiddle + m_strBoundary.length();
				iNext = inVar.find( m_strBoundary, iStart );

				if( iNext != string::npos ){
					pStr[index++] = new string( inVar.substr( iStart, iNext - iStart  ) );
					++iCount;
				}
				else if( iCount < border_count ){
					pStr[index++] = new string( inVar.substr( iStart  ) );
					++iCount;
				}
				iMiddle = inVar.find( m_strBoundary, iStart );
				
			}
			
		}
	
	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError( "GetMimeContent has caught an unknown error.", 8001 );
	}

} // End of GetMimeContent

// SortMimeContent does alot of the work and sometimes
// even works recursively. It is used after FindMimeContent
// and GetMimeContent to seperate each of the piecs of mime content
// into it's mime header and body using NSMimeUtils
void EmailRipper::SortMimeContent( string &inVar, string* pStr[], int mime_count )
{

	register string::size_type iContentLength, iText, iHtml, iBase64, iQuotedPrintable, iBoundary, i7bit, iTextEnriched; 
	register string::size_type iFind, iMessageRfc, iAttachment, iName, iFileName, iInline, iApplicationOctetStream;

    string sContentType; 
    string sLowerContentEncoding;
    string sContentDisposition;
    string sContentId;
	string sContentLocation;
	string sFileName;
	bool bSkip;

	NSMimeUtils oMime;
	
	//i_mime_level += 1;

	try{

		//cout << "mime_count :" << mime_count << endl;
		//cout << "-----------------------------------" << endl;
		bool bHasFileContentID;
		for( int i = 0; i < mime_count; i++ )
		{
			bSkip = false;
			NSMessageContent oContent;
			
			/* Decipher this mime object and obtain it's parts for parsing below. */
			oMime.Decipher( *pStr[i] );
			/* Create two variables */
			sContentType = oMime.getContentType();
			string sLowerContentType(sContentType);
			ToLower(sLowerContentType);
			/* Alter a variable */
			sLowerContentEncoding = oMime.getContentTransferEncoding();
			ToLower(sLowerContentEncoding);
			/* Create two variables */
			sContentDisposition = oMime.getContentDisposition();
			string sLowerContentDisposition(sContentDisposition);
			ToLower(sLowerContentDisposition);
			/* Leave these un-altered */
            sContentId = oMime.getContentID();
			sContentLocation = oMime.getContentLocation();

			iContentLength = pStr[i]->length();
			if(sContentType.length() == 0 && sLowerContentEncoding.length() == 0 && 
			   sContentDisposition.length() == 0 && iContentLength > 0 ){ 
					sLowerContentType = "text/plain"; 
			} 

			iAttachment = sLowerContentDisposition.find( attachment, 0, 10 ); 
			iMessageRfc = sLowerContentType.find( message_rfc, 0, 10 ); 
			iText = sLowerContentType.find( text_plain, 0, 10 ); 		
			iHtml = sLowerContentType.find( text_html, 0, 9 );			
			iBase64 = sLowerContentEncoding.find( base64, 0, 6 );
			iQuotedPrintable = sLowerContentEncoding.find( quoted_printable, 0, 16 );
			iBoundary = sLowerContentType.find( boundary, 0, 9 );
			i7bit = sLowerContentEncoding.find( sevenBit, 0, 4 );
			iName = sLowerContentType.find( name_equals, 0, 5 );
			iFileName = sLowerContentDisposition.find( filename_equals, 0, 9 );
			iInline = sLowerContentDisposition.find("inline", 0);
			iApplicationOctetStream = sLowerContentType.find( application_octet_stream, 0, 24 );	
			iTextEnriched = sLowerContentType.find( text_enriched, 0, 13 );

			//cout << "-----------------------------------" << endl;
			////cout << "i_mime_level: " << //i_mime_level << endl;
			//cout << "iInline: " << iInline << endl;
			//cout << "iText :" << iText << endl;
			//cout << "iHtml :" << iHtml << endl;
			//cout << "sContentType :" << sContentType << endl;
			//cout << "sLowerContentEncoding :" << sLowerContentEncoding <<endl;
			//cout << "sContentDisposition :" << sContentDisposition << endl;
			//cout << "sContentId :" << sContentId << endl;
			//cout << "*pStr[i]:" << endl;
			////cout << *pStr[i].substr(0, 50) << endl;
			//cout << "-----------------------------------" << endl;

			bHasFileContentID = false;
			if( iContentLength > 0 ){
			
				if(sContentId.length() > 0){
					sFileName = ExtractFileName( oMime.getName(), oMime.getFileName(), sContentType, sContentDisposition);	
					if( sFileName.length() == 0  ){ // bad121.txt 9/3/2005
						size_t iFindImage = sLowerContentType.find( "image", 0, 5 );
						if( iFindImage != string::npos ){
							if( sLowerContentType.find( "gif", 0, 3 ) > iFindImage ){
								sFileName = "inline.gif";
							}
							else if( (sLowerContentType.find( "jpg", 0 ) > iFindImage ) || (sLowerContentType.find( "jpeg", 0 ) > iFindImage ) ){
								sFileName = "inline.jpg";
							}
						}
					}
					if( sFileName.length() > 0 && bSave == true ){
						string sFile = "";
						if( handleInlineContentID( sFileName, sContentId, sContentLocation, sLowerContentType, sFile ) == true ){
							
							const char * chFile = sFile.c_str();
							if( iBase64 != string::npos ){
								NSBase64 o64;								
								o64.writeMimeFile2( *pStr[i], pSaveLocation, chFile );
								// If this file is labeled as being a .gif or .jpg
								// then verify that it actually is one.
								string sExt = getFileExtension( sFile );
								if( sExt.find( "gif", 0 ) != string::npos ){
									if( IsGif( pSaveLocation, chFile ) == false ){
										if( IsJpg( pSaveLocation, chFile ) == true ){
											RenameFalseContentID( sFile, ".jpg" );
										}
									}
								}
								else if( (sExt.find( "jpg", 0 ) != string::npos) || (sExt.find( "jpeg", 0 ) != string::npos) ){
									if( IsJpg( pSaveLocation, chFile ) == false ){
										if( IsGif( pSaveLocation, chFile ) == true ){
											RenameFalseContentID( sFile, ".gif" );
										}
									}
								}
							}
							else{
								char save_location[ MAX_FILE_LEN ];
								save_location[ MAX_FILE_LEN - 1 ] = 0;
								strcpy( save_location, pSaveLocation );
								strcat( save_location, chFile );
								if( iQuotedPrintable != string::npos ){
									NSQuotedPrintable oQP(*pStr[i]);
									oQP.DecodeQP( save_location );
								}
								else{
									writeFile( pStr[i]->c_str(), save_location );	
								}
							}
							AddAttachmentNameNotFancy( sFile.c_str(), 2 );
							bHasFileContentID = true;
						}
					}
				}

				if( bHasFileContentID == false ){
				
					if( iInline != string::npos && iBoundary == string::npos )
					{
						bool bWriteFile = false;
 						sFileName = ExtractFileName( oMime.getName(), oMime.getFileName(), sContentType, sContentDisposition);					
						if( sFileName.length() > 0  && bSave == true ){
							if( getTotalMessageCount() == 0 && 
							  ( iText != string::npos || iHtml != string::npos || iTextEnriched != string::npos ) ){
								  // do nothing!
							}
							else if( i7bit != string::npos || iMessageRfc != string::npos ){
								bWriteFile = SaveInlineContentToFile(*pStr[i]);
							}
							else{
								bWriteFile = decodeContentToFileIfNeedBe(*pStr[i], sFileName, iBase64, iQuotedPrintable, iApplicationOctetStream, iText, iHtml, i7bit );
							}							
						}

						if( bWriteFile == false && bSave == true ){
							
							if( iText != string::npos || i7bit != string::npos || iMessageRfc != string::npos || 
								iHtml != string::npos || iTextEnriched != string::npos ){
									
								string sBody = "";	
								if( iBase64 != string::npos && bSave == true ){							
									sBody = MimeHelper(*pStr[i], pSaveLocation, "mime_file_output.txt");
								}
								else if( iQuotedPrintable != string::npos ){
									NSQuotedPrintable oQP(*pStr[i]);
									if( iHtml != string::npos ){
										sBody = oQP.DecodeQP(true);
									}
									else{
										sBody = oQP.DecodeQP(false);
									}									
								}
								else{
									RemoveDoublePeriodsFromMessage( *pStr[i] ); 
									sBody = *pStr[i];
								}

								if( sBody.length() > 0 ){

									if( iHtml != string::npos ){
										oContent.SetEncoding(2);
										i_html_message_count += 1;
									}
									else if( iTextEnriched != string::npos ){
										oContent.SetEncoding(3);
										i_enriched_message_count += 1;									
									}
									else if( getTotalMessageCount() == 0 ){
										oContent.SetEncoding(1);
										i_text_message_count += 1;
									}
									else{
										bSkip = true;
									}

									if( bSkip == false ){
										oContent.SetBody( sBody );
									}
									else if( SaveInlineContentToFile(sBody) == false ){
										oContent.SetEncoding(1);
										i_text_message_count += 1;
										oContent.SetBody( sBody );
										bSkip = false;
									}																			
								}																	
							}
							else{
								
								SaveInlineContentToFile(*pStr[i]);
							}
						}
					}
					else if( iText != string::npos ){

						bool bAvoid = false;
						if( ( i7bit != string::npos ) || ( iBase64 != string::npos ) || 
							( iName != string::npos ) || ( iFileName != string::npos ) ||
							( iApplicationOctetStream != string::npos ) ||
							( sLowerContentDisposition.find(attachment, 0) != string::npos ) )
						{	
							sFileName = ExtractFileName( oMime.getName(), oMime.getFileName(), sContentType, sContentDisposition);	
							bAvoid = decodeContentToFileIfNeedBe( *pStr[i], sFileName, iBase64, iQuotedPrintable, iApplicationOctetStream, iText, iHtml, i7bit );
						}
						
						if( bAvoid == false ){

							string sBody = "";	
							if( iBase64 != string::npos && bSave == true ){							
								sBody = MimeHelper(*pStr[i], pSaveLocation, "mime_file_output.txt");
							}
							else if( iQuotedPrintable != string::npos ){
								NSQuotedPrintable oQP(*pStr[i]);
								sBody = oQP.DecodeQP(false);
							}
							else{
								RemoveDoublePeriodsFromMessage( *pStr[i] ); 
								sBody = *pStr[i];
							}

							if( sBody.length() > 0 ){
								if( (sBody.length() == 2 && sBody.find( crLf ) == 0) ||
									(sBody.length() == 4 && sBody.find( double_crLf ) == 0) ){
								  // This message is empty. do nothing!
								}
								else{
									oContent.SetEncoding(1);
									i_text_message_count += 1;
									oContent.SetBody( sBody );								
								}
							}
						}
					}
					else if( (iHtml != string::npos || iTextEnriched != string::npos) && (iBoundary == string::npos) ){
							
						string sBody = "";
						bool bWriteFile = false;
						if( iName != string::npos || iFileName != string::npos )
						{	
							sFileName = ExtractFileName( oMime.getName(), oMime.getFileName(), sContentType, sContentDisposition);	
							bWriteFile = decodeContentToFileIfNeedBe( *pStr[i], sFileName, iBase64, iQuotedPrintable, iApplicationOctetStream, iText, iHtml, i7bit  );
						}

						if( bWriteFile == false )
						{
							if( iBase64 != string::npos && bSave == true ){
								sBody = MimeHelper(*pStr[i], pSaveLocation, "mime_file_output.txt");
							}
							else if( iQuotedPrintable != string::npos ){			
								NSQuotedPrintable oQP(*pStr[i]);
								sBody = oQP.DecodeQP(true);
							}
							else{
								RemoveDoublePeriodsFromMessage( *pStr[i] ); 
								sBody = *pStr[i];
							}

							if( sBody.length() > 0 ){
								if( (sBody.length() == 2 && sBody.find( crLf ) == 0) ||
									(sBody.length() == 4 && sBody.find( double_crLf ) == 0) ){
								  // This message is empty. do nothing!
								}
								else{
									if( iHtml != string::npos ){
										oContent.SetEncoding(2);
										i_html_message_count += 1;								
									}
									else if( iTextEnriched != string::npos ){
										oContent.SetEncoding(3);
										i_enriched_message_count += 1;								
									}
									oContent.SetBody( sBody );
								}
							}	
						}
					}
					else if( iBoundary != string::npos ){ // && 
							//bMimeHtml != true && bMimeText != true ){
						
						m_strBoundary = sContentType.substr( iBoundary + 9 );	
						
						iFind = m_strBoundary.find(';', 0);
						if(iFind != string::npos){
							m_strBoundary = m_strBoundary.substr(0, iFind);						
						}

						iFind = m_strBoundary.find( 34, 0 );
						if( iFind != string::npos ){
							m_strBoundary = m_strBoundary.substr( iFind + 1 );
							iFind = m_strBoundary.find( 34, 0 );
							if( iFind != string::npos ){
								m_strBoundary = m_strBoundary.substr( 0, iFind );
							}
						}

						int mCount = FindMimeContent( *pStr[i] );
						if( mCount > 0 ){					

							string** pMimeContent = new string*[ mCount ]; 
							
							GetMimeContent( *pStr[i], pMimeContent, mCount );
							SortMimeContent( *pStr[i], pMimeContent, mCount );

							// Delete content from free store
							for( int i = 0; i < mCount; i++ )
								delete pMimeContent[i];

							// Now delete the array of pointers
							delete [] pMimeContent;
							
						}	

					}
					else{

						bool bHandleInline = false;
						if( iMessageRfc != string::npos )
						{ 		
							if( bSave == true && ( sLowerContentEncoding.length() == 0 && 
								sContentDisposition.length() == 0 ) || ( iAttachment != string::npos ) ){
								// maybe also try using content-description to get the name of the file?
								// look at ok01 2004-05-11
								bHandleInline = handleInlineText(*pStr[i]);
							}
						}

						if( bHandleInline == false ){
							
							if( sLowerContentType.find( message_delivery_status ) != string::npos ){
								oContent.SetEncoding(4);
								i_message_delivery_status_count += 1;
								oContent.SetBody( *pStr[i] );						
							}
							else{
							
								sFileName = ExtractFileName( oMime.getName(), oMime.getFileName(), sContentType, sContentDisposition );
								if( decodeContentToFileIfNeedBe( *pStr[i], sFileName, iBase64, 
									iQuotedPrintable, iApplicationOctetStream, iText, iHtml, i7bit  ) == false )
								{ 
									RemoveDoublePeriodsFromMessage( *pStr[i] );
									oContent.SetEncoding(1);
									i_text_message_count += 1;
									oContent.SetBody( *pStr[i] );							
								}
							}
						}
					}

					if( bSkip == false && oContent.GetEncoding() != 0 ){
						vContent.push_back(oContent);
					}				
					
				}

			}
			// Erase this content
			pStr[i]->clear();
		}
		//i_mime_level -= 1;
	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError( "SortMimeContent has caught an unknown error.", 9001 );
	}

}

inline bool EmailRipper::SaveInlineContentToFile( string & sInput ){

	bool bRetVal = false;
	NSHeaderItem oMessage;

	char file_name[1024];
	file_name[1023] = 0;
	
	try{
	
		LTrim( sInput );
	
		string sSubject = "";
		string sFrom = ""; // Used for verification
		string sDate = ""; // Used for verification

		string sHeader = GetHeader( sInput );
		if( sHeader.length() > 0 ){
			sSubject = oMessage.GetHeaderItem( sHeader, "Subject: " );
			sDate = oMessage.GetHeaderItem( sHeader, "Date: " );
			sFrom = oMessage.GetHeaderItem( sHeader, "From: " );
		}

		// Do comparison here to see if this is a genuine "inline" message.
		if( sSubject.length() > 0 && ( sDate.length() > 0 || sFrom.length() > 0 ) )
		{
			strcpy( file_name, sSubject.c_str() );
			strreplace( file_name, " ", "_" ); // 2005-06-23 Bug Fix. bad70.txt
			RemoveBadFileNameChars( file_name );
			TRIM( file_name );
			strcat( file_name, ".txt" );
			bRetVal = true;
		}
		else{
			strcpy( file_name, "inline_content.txt");								
		}

		string sFileName = file_name;
		sFileName = AddAttachmentName( sFileName, 3 );

		char file_path[ MAX_FILE_LEN ];
		file_path[ MAX_FILE_LEN - 1 ] = 0;

		strcpy( file_path, pSaveLocation );
		strcat( file_path, sFileName.c_str() );

		writeFileSpecial( sInput, file_path );

		contentid.push_back( sFileName.c_str() ); 
		bInlineText = true;	

		return bRetVal;

	}
	catch(...){
		throw MiscError( "SaveInlineContentToFile has caught an unknown error.", 9382 );
	}

}

inline bool EmailRipper::decodeContentToFileIfNeedBe( string &sContent, string &sFileName,
												string::size_type iBase64, 
												string::size_type iQuotedPrintable,
												string::size_type iApplicationOctetStream, 
												string::size_type iText,
												string::size_type iHtml,
												string::size_type i7bit)
{
	char * cLocation = 0;
	bool bWrite = false;

	try{

		if( sFileName.length() > 0 && bSave == true && sContent.length() > 0 ){

			if( iBase64 != string::npos || iQuotedPrintable != string::npos || 
				     iApplicationOctetStream != string::npos || iText != string::npos || 
					 iHtml != string::npos || i7bit != string::npos )
			{
				sFileName = AddAttachmentName( sFileName, 1 );
				{
					if(  iBase64 != string::npos )
					{
						NSBase64 o64;
						o64.writeMimeFile2( sContent, pSaveLocation, sFileName.c_str() );	
					}					
					else
					{
						cLocation = new char[strlen(pSaveLocation) + sFileName.length() + 1];
						strcpy( cLocation, pSaveLocation);
						strcat( cLocation, sFileName.c_str());
						cLocation[strlen(pSaveLocation) + sFileName.length()] = 0;
						
						if( iQuotedPrintable != string::npos )
						{
							NSQuotedPrintable oQP(sContent);				
							oQP.DecodeQP( cLocation );
						}
						else{
							writeFile( sContent.c_str(), cLocation );
						}						
						delete [] cLocation;					
					}
					bWrite = true;
				}					
			}
		}

		return bWrite;
	}
	catch( MiscError ){
		delete [] cLocation;
		throw;
	}
	catch(...){
		delete [] cLocation;
		throw MiscError( "decodeContentToFileIfNeedBe has caught an unknown error.", 8063 );
	}
}

inline bool EmailRipper::handleInlineContentID( string &sFileName, string &sContentId, string &sContentLocation,
											    string &sLowerContentType, string &sFile )
{

	char * sChar = 0;
	bool bContinue = false;
	bool bAppend = false;

	try{

		bool bAppendCid = false;
		if( sContentId.find("@") != string::npos ){
			bAppendCid = true;
		}
		else if( sContentLocation.length() > 0 ){
			sContentId = sContentLocation;
		}
		else{
			bAppendCid = true;
		}
		
		if( sContentId.find("<") != string::npos ){
			sChar = c_string( sContentId );
			strclean( sChar, "<" );
			strclean( sChar, ">" );
			sContentId = sChar;
			delete [] sChar;
		}						

		string sId;
		if( bAppendCid == true ){
			sId.append("no_cid=0|cid:");
		}
		else{
			sId.append("no_cid=1|");
		}

		sId.append(sContentId);
		sId.append("|");
		if( sFileName.length() > 0){
			
			if( getFileExtension(sFileName) == "" ){
				if( sLowerContentType.find( "gif", 0 ) != string::npos){
					sFileName.append(".gif");
				}
				else if( (sLowerContentType.find( "jpg", 0 ) != string::npos) || (sLowerContentType.find( "jpeg", 0 ) != string::npos) ){
					sFileName.append(".jpg");
				}			
			} 
			if( DoesFileNameExist( sFileName, 2 ) == true )
				sFileName = getNewAttachmentName( sFileName, 2 );

			sId.append(sFileName);
			sFile = sFileName;
			bContinue = true;
		}
		else{
			sFile = sContentId;
			if( sLowerContentType.find( "gif", 0 ) != string::npos){
				sFile.append(".gif");
				bContinue = true;
			}
			else if( (sLowerContentType.find( "jpg", 0 ) != string::npos) || (sLowerContentType.find( "jpeg", 0 ) != string::npos) ){
				sFile.append(".jpg");
				bContinue = true;
			}
			if( DoesFileNameExist( sFile, 2 ) == true )
				sFile = getNewAttachmentName( sFile, 2 );

			sId.append(sFile);			
		}

		if( bContinue==true ){ //&& iAttachment == string::npos){
			contentid.push_back(sId);					
			bHasContentId = true;
			bAppend = true;
		}
		return bAppend;
	}
	catch( MiscError ){
		delete [] sChar;
		throw;
	}
	catch(...){
		delete [] sChar;
		throw MiscError( "handleInlineContentID has caught an unknown error.", 9067 );
	}
}

inline bool EmailRipper::handleInlineText(string &sInput)
{

	string sName = "";
	char file_name[ MAX_FILE_LEN ];
	file_name[ MAX_FILE_LEN - 1 ] = 0;

	char * cFileName = 0;
	char * cFileLocation = 0;

	size_t iCharSize = 0;
    int iRandom;
    char buf[4];
	bool bPass = false;

	try{

		LTrim(sInput);
		NSRandomGeneration oGen;
		iRandom = rand()%250; 
		oGen.init_mm(); 
		iRandom += oGen.number_range( 251, 500 );
		_itoa( iRandom, buf, 10 );

		cFileName = new char[ strlen(buf) + 20 ];
		strcpy( cFileName, "inline_content");
		strcat( cFileName, buf);
		strcat( cFileName, ".txt");
		cFileName[ strlen(buf) + 19 ] = 0;

		sName = cFileName;
		sName = AddAttachmentName( sName, 3 );
		iCharSize = strlen(pSaveLocation) + sName.length() + 1;
		{
			cFileLocation = new char[iCharSize];
			strcpy( cFileLocation, pSaveLocation);						
			strcat( cFileLocation, sName.c_str() );
			cFileLocation[ iCharSize - 1 ] = 0;
			//
			writeFileSpecial( sInput, cFileLocation );

			contentid.push_back(sName.c_str());  					
			bInlineText = true; 
			bPass = true;
		}

		delete [] cFileLocation;
		delete [] cFileName;
		
		return bPass;
	}
	catch( MiscError ){
		delete [] cFileLocation;
		delete [] cFileName;
		throw;
	}
	catch(...){
		delete [] cFileLocation;
		delete [] cFileName;
		throw MiscError( "handleInlineMessageContent has caught an unknown error.", 8367 );
	}
}

// This is very similar to DecipherHelper except that it is used to 
// return a character array through the use of the output variable
// You pass in the item you want to search for and a character array
// buffer that is used for output. For example I might create a 
// char aray called s[500] and pass it in expecting a header item that 
// is 499 characters in length or less.
bool EmailRipper::getHeaderItem( const char *item, char * output  )
{
	
	try{
		
		bool bPass = false;
		register basic_string <char>::size_type iItemLength = strlen(item);
		if( iItemLength == 0 || iItemLength > 30 || header.length() == 0 ){
			setErrorMessage("Item or header length is zero or Item length is greater than thirty characters.");
			return_status = 6;
		}
		else if( iItemLength > header.length() ){
			setErrorMessage("Item length is greater than header length.");
			return_status = 7;
		}
		else{
			NSHeaderItem oHeader;
			string sItem = oHeader.GetHeaderItem( header, item );	
			if( sItem.length() > 0 ){
				strcpy( output, sItem.c_str() );
				bPass = true;
			}
			else{
				setErrorMessage("Item Not Found!");
				return_status = 8;
			}	
		}
		
		if( bPass == false ){
			memset(output, '\0', strlen(output));
		}

		return bPass;

	}
	catch( MiscError ex ){
		setErrorMessage(ex.what());
		return_status = ex.number();
		return false;
	}
	catch(...){
		setErrorMessage("GetHeaderItem has caught an unknown error");
		return_status = 9;
		return false;
	}
	
} // end of function getHeaderItem

inline int EmailRipper::RemoveBadFileNameChars(char *chInput)
{
	int iCount = 0;
	if( strlen(chInput) > 0 ){
		// Count the total number of replacements
		iCount += strreplace( chInput, "\\", "_" );
		iCount += strreplace( chInput, "/", "_" );
		iCount += strreplace( chInput, ":", "_" );
		iCount += strreplace( chInput, "*", "_" );
		iCount += strreplace( chInput, "?", "_" );
		iCount += strreplace( chInput, "\"", "_" );
		iCount += strreplace( chInput, "<", "_" );
		iCount += strreplace( chInput, ">" , "_" );
		iCount += strreplace( chInput, "|" , "_" );	
	}
	return iCount;
}

void EmailRipper::RemoveFileNameQuotes( string & file_name )
{
	register string::size_type iFind, iEnd;
	// search for a "
	iFind = file_name.find( 34, 0 );
	if( iFind != string::npos ){
		// search for another "
		iEnd = file_name.find( 34, (iFind + 1) );
		if( iEnd != string::npos ){
			file_name = file_name.substr( iFind + 1, iEnd - iFind - 1 );
		}
	}
}

// ExtractFileName is used to get the file name out of the 
// content-type: and content-disposition: headers.
string EmailRipper::ExtractFileName( string name, string file_name, 
									 string & sContentType, string & sContentDisposition)
{
	try{

		register string::size_type iFind, iEnd, iName, iFileName;
		string sFileName = "";
		
		string sLowerContentType(sContentType);
		string sLowerContentDisposition(sContentDisposition);
		ToLower(sLowerContentType);
		ToLower(sLowerContentDisposition);

		iName = sLowerContentType.find( name_equals, 0, 5 );
		iFileName = sLowerContentDisposition.find( filename_equals, 0, 9 );

		if( name.length() > 0 ){
			sFileName = name;
			RemoveFileNameQuotes(sFileName);
		}
		else{
			if( file_name.length() > 0 ){
				sFileName = file_name;
				RemoveFileNameQuotes(sFileName);
			}
			else{		
				if( iName != string::npos && iName > 0 ){
					// search for a "
					iFind = sContentType.find( 34, (iName + 5) );
					if( iFind != string::npos && iFind > 0 ){
						// search for another "
						iEnd = sContentType.find( 34, (iFind + 1) );
						if( iEnd != string::npos && iEnd > 0 ){
							sFileName = sContentType.substr( iFind + 1, iEnd - iFind - 1 );
						}
					}
					else{
						sFileName = sContentType.substr( iName + 5 ); 
					}
				}
				else{
					if( iFileName != string::npos && iFileName > 0 ){
						// search for a "
						iFind = sContentDisposition.find( 34, (iFileName + 9) );
						if( iFind != string::npos && iFind > 0 ){
							// search for another "
							iEnd = sContentDisposition.find( 34, (iFind + 1) );
							if( iEnd != string::npos && iEnd > 0 ){
								sFileName = sContentDisposition.substr( iFind + 1, iEnd - iFind - 1 );
							}
						}
						else{
							sFileName = sContentDisposition.substr( iFileName + 9 ); 
						}
					}
					else{
						sFileName = "";
					}
				}
			}				
		}

		string sLowerFileName(sFileName);
		ToLower(sLowerFileName);
		iFind = sLowerFileName.find( iso_8859_1 );
		iEnd = sLowerFileName.find( iso_b );
		iName = sLowerFileName.find( iso_q );
		if( iFind != string::npos || iEnd != string::npos || iName != string::npos ){
			char * input = c_string(sFileName);
			sFileName = MimeDecodeGenericMailField(input);
			delete [] input;
		}

		iFind = sFileName.find_last_of("/");
		if( iFind != string::npos ){
			sFileName = sFileName.substr(iFind + 1);
		}

		iFind = sFileName.find_last_of("\\");
		if( iFind != string::npos ){
			sFileName = sFileName.substr(iFind + 1);
		}

		return sFileName;

	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw(MiscError("ExtractFileName has caught an unknown error.", 6128));
	}
}

// Function: RemoveDoublePeriodsFromMessage
// Purpose: To remove a .. from the beginning of a line in a message
inline bool EmailRipper::RemoveDoublePeriodsFromMessage( string &Input )
{

	try{

		if( Input.find( double_period ) == string::npos ) return false;

		string sLine;
		string sbTemp;
		istringstream iss;
		char line_buff[ MAX_CHAR_BUFFER ];
		register int iLineCount = 0;

		iss.str (Input);
		while( iss.getline( line_buff, MAX_CHAR_BUFFER ) )
		{
			sLine = line_buff;
			
			if ( sLine.length() > 2 && sLine.at(0) == '.' && sLine.at(1) == '.' )
			{
				sLine = sLine.substr(1);
			}
			
			sLine.append( Lf );
			sbTemp.append(sLine);
			iLineCount += 1;
		}

		if( iLineCount > 0 ){
			Input = sbTemp;
			return true;
		}
		else{
			return false;
		}

	}
	catch( MiscError ){		
		throw;
	}
	catch(...){
		throw(MiscError("RemoveDoublePeriods has caught an unknown error.", 5138));
	}
}

// This function is called everytime a body is being deciphered
// to see if the body contains anyy uu-encoded messages. If so,
// it's helper functions decode the data and the filename is 
// added to a global varaible that is to be returned to the user.
inline void EmailRipper::RemoveUUCode( string &Input )
{

	register basic_string <char>::size_type intPosA, intPosB;
	char * cDir = 0;

	try{

		if( bSave == false ){
			return;
		}

		intPosA = Input.find( uu_begin_marker );
		if( intPosA == string::npos ){
			return;
		}
		
		intPosB = Input.find( uu_end_marker, intPosA + 15 );
		if( intPosB == string::npos ){
			return;
		}

		register basic_string <char>::size_type iEnd, iCharCount, iBeginPos;
		string sAttachment;
		string sLine;
		intPosA = intPosB = iEnd = iCharCount = iBeginPos = 0;

		istringstream iss;
		char line_buff[ MAX_CHAR_BUFFER ];
		iss.str (Input);
		while( iss.getline( line_buff, MAX_CHAR_BUFFER ) )
		{

			sLine = line_buff;
			sLine.append( Lf );
			
			iCharCount += sLine.length();
			iBeginPos = iCharCount - sLine.length();

			intPosA = sLine.find( msg_begin, 0, 6 );
			if( intPosA == 0 ){
				intPosB = sLine.find( crLf, intPosA + 10);
				if( intPosB > 10 ){
					sLine = sLine.substr( intPosA, intPosB - intPosA );
					if( sLine.find( 32, 6 ) == 9 ){	
						if( isdigit(sLine.at(6)) && isdigit(sLine.at(7)) && isdigit(sLine.at(8)) ){
							if( Input.find("M", iCharCount - 1) == iCharCount ){
							
								// Do a little bit of validation
								string sFileName = sLine.substr(10);
								RTrim( &sFileName );

								// We will assume that there has to be a file extension,
								// even though unix/linux does not soley rely on them.
								if( getFileExtension(sFileName) == "" ) return;							
								if( ( sFileName.length() - sFileName.find_last_of(".") ) > 7 ) return;
								
								// found begin ### 
								const char * cTempFileName = sFileName.c_str();	
								// Do a quasi-test to see if this is a file... 2004/10/21

								// End quasi-text
								iEnd = Input.find( "end", iBeginPos + 15 );
								if( iEnd > 0 ){
									
									// Assign to a class variable here to return!
									// Make sure to increment m_AttachmentCount above
									AddAttachmentNameNotFancy( cTempFileName, 1 );
									sAttachment = Input.substr( iBeginPos, ( ( iEnd + 3 ) - iBeginPos ) );

									cDir = new char[ strlen(pSaveLocation) + 17 ];
									strcpy(cDir, pSaveLocation);
									strcat(cDir, "uu_temp_file.txt");
									cDir[strlen(pSaveLocation) + 16] = 0;

									writeFile( sAttachment.c_str(), cDir );
									
									FILE *f;

									f = fopen( cDir, "rb" );
									if (f == NULL) {
										delete [] cDir;
										throw MiscError("RemoveUUCode was unable to open a file for output", 432);
										//return;
										//printf("I can't find %s.\n",argv[1]); 
										//return(1);
									}

									NSUUCode oUU;
									oUU.uudecode(f, pSaveLocation);

									fclose(f);						
									
									ShredFile( cDir );
									
									delete [] cDir;

									Input = Input.substr( 0, iBeginPos - 1 ) + Input.substr( iEnd + 3 );

									// Make a recursive call in case there are more uu-encoded
									// attachments in this particular message.
									RemoveUUCode( Input );
									break;

								}							
							}
						}						
					}
				}
			}			
		}
	}
	catch( MiscError ){
		delete [] cDir;
		throw;
	}
	catch(...){
		delete [] cDir;
		throw(MiscError("RemoveUUCode has caught an unknown error.", 5138));
	}
}

inline string EmailRipper::MimeHelper(string & inContent, const char * save_location, const char * temp_file)
{

	char * cDir = 0;
	string sOutput = "";
	char * replace_str = 0;

	try{

		NSBase64 o64;
		o64.writeMimeFile2( inContent, save_location, temp_file );
		
		size_t iSize = strlen(save_location) + strlen(temp_file);
		cDir = new char[ iSize + 1 ];
		strcpy(cDir, save_location);
		strcat(cDir, temp_file);
		cDir[ iSize ] = 0;

		sOutput = OpenFile( cDir );

		ShredFile( cDir );
		delete [] cDir;

		if( sOutput.find("â€™") != -1 ){
			replace_str = c_string( sOutput );
			strreplace(replace_str, "â€™", "'");
			sOutput = replace_str;
			delete [] replace_str;
		}

		return sOutput;	

	}
	catch( MiscError ){
		delete [] cDir;
		delete [] replace_str;
		throw;
	}
	catch(...){
		delete [] cDir;
		delete [] replace_str;
		throw(MiscError("MimeHelper has caught an unknown error.", 8436));
	}
}

string EmailRipper::getAttachmentFileName( int iMessageArea, int iPart ){
	
	if( iMessageArea < 1 || iMessageArea > 3 ){
		return "";
	}
	else if( iPart < 0 || ( (iPart + 1) > static_cast<int>( vAttachment.size() ) ) ){
		return "";
	}
	string sRetVal = "";
	NSMessageContent oContent;
	int iCurrent = 0;
	typedef vector<NSMessageContent>::const_iterator VI;
	for(VI p=vAttachment.begin(); p!=vAttachment.end(); ++p){			
		oContent = *p;
		if( oContent.GetEncoding() == iMessageArea ){
			if( iCurrent == iPart ){
				sRetVal = oContent.GetBody();
				break;
			}
			else{
				iCurrent += 1;
			}		
		}
	}

	return sRetVal;
}

inline string EmailRipper::getNewAttachmentName( string &sFileName, int iMessageArea )
{
	string sNewFileName = "";
	if( sFileName.length() > 0 ){
		int iVal = 1;
		string sExtension = getFileExtension( sFileName );
		size_t iLen = sExtension.length();
		do{
			if( iLen == 0 ){
				sNewFileName = sFileName + intToString( iVal ) + ".txt";
			}
			else{
				sNewFileName = sFileName.substr( 0, sFileName.length() - iLen ) + intToString( iVal ) + sExtension;
			}
			iVal += 1;
		}while( DoesFileNameExist( sNewFileName, iMessageArea )==true );	
	}
	return sNewFileName;
}

inline string EmailRipper::AddAttachmentName( string &sFileName, int iMessageArea )
{

	if( iMessageArea < 1 || iMessageArea > 3 ){
		return "";
	}

	switch( iMessageArea )
	{
	case 1:
		i_non_inline_attachment_count += 1;
		break;
	case 2:
		i_inline_attchment_count += 1;
		break;		
	case 3:
		i_inline_attachment_text_count += 1;
		break;
	}

	string sReturnString = "";
	NSMessageContent oAttachments;
	oAttachments.SetEncoding( iMessageArea );

	char * chFileName = c_string( sFileName );
	RemoveBadFileNameChars( chFileName );
	sFileName = chFileName;
	delete [] chFileName;

	if( DoesFileNameExist( sFileName, iMessageArea ) == true ){
		sReturnString = getNewAttachmentName( sFileName, iMessageArea );
		oAttachments.SetBody( sReturnString );
	}
	else{
		oAttachments.SetBody( sFileName );
		sReturnString = sFileName;
	}
	vAttachment.push_back(oAttachments);
	return sReturnString;
}

inline void EmailRipper::AddAttachmentNameNotFancy( const char * chFile, int iMessageArea )
{

	switch( iMessageArea )
	{
	case 1:
		i_non_inline_attachment_count += 1;
		break;
	case 2:
		i_inline_attchment_count += 1;
		break;		
	case 3:
		i_inline_attachment_text_count += 1;
		break;
	}

	NSMessageContent oAttachments;
	oAttachments.SetEncoding( iMessageArea );

	//////////////////////////////////////////////////////
	//////size_t iLen = strlen(chFile);
	//////char * chModFile = new char[ iLen + 1 ];
	//////memcpy( chModFile, chFile, iLen );
	//////chModFile[ iLen ] = '\0';
	//////RemoveBadFileNameChars( chModFile );
	//////string sFileName = chModFile;
	//////delete [] chModFile;
	//////////////////////////////////////////////////////

	string sFileName = chFile;
	oAttachments.SetBody( sFileName );
	vAttachment.push_back(oAttachments);
}

inline bool EmailRipper::DoesFileNameExist( string &sFileName, int iMessageArea )
{
	bool bExists = false;
	NSMessageContent oContent;

	typedef vector<NSMessageContent>::const_iterator VI;
	for(VI p=vAttachment.begin(); p!=vAttachment.end(); ++p){			
		oContent = *p;
		if( oContent.GetBody() == sFileName ){
			bExists = true;
			break;
		}
	}
	return bExists;
}

inline void EmailRipper::ChangeAllNonTextInlineAttachmentsToNormalOnes()
{
	if( vAttachment.size() > 0 ){
		int iCount = 0;
		for( size_t i = 1; i <= vAttachment.size(); i++ ){
			if( vAttachment[i-1].GetEncoding() == 2 ){
				vAttachment[i-1].SetEncoding(1);
				i_non_inline_attachment_count += 1;
				i_inline_attchment_count -= 1;
				iCount += 1;
			}
		}
		if( iCount > 0 ){
			bHasContentId = false;
		}			
	}
}

inline bool EmailRipper::RemoveAttachmentName( string &sFileName, int iMessageArea )
{
	bool bCopy = false;
	if( DoesFileNameExist(sFileName, iMessageArea) == true ){

		NSMessageContent oContent;
		vector<NSMessageContent>vTempContent;

		typedef vector<NSMessageContent>::const_iterator VI;
		for(VI p=vAttachment.begin(); p!=vAttachment.end(); ++p){			
			oContent = *p;
			if( oContent.GetEncoding() == iMessageArea && oContent.GetBody() == sFileName ){												
				bCopy = true;
			} 
			else{
				vTempContent.push_back( oContent );
			}
		}
		
		if( bCopy == true ){
			switch( iMessageArea )
			{
			case 1:
				i_non_inline_attachment_count -= 1;
				break;
			case 2:
				i_inline_attchment_count -= 1;
				break;		
			case 3:
				i_inline_attachment_text_count -= 1;
				break;
			}			
			vAttachment.swap(vTempContent);
		}
	}
	return bCopy;
}

inline string EmailRipper::getFileExtension(string sFile){
	try{
		register size_t iFind = sFile.find_last_of(".");
		string ext = "";
		if( iFind != string::npos ){
			ext = sFile.substr(iFind);
			sFile = sFile.substr(0, iFind);
			ToLower( ext );
		}
		return ext;
	}
	catch( MiscError ){
	    throw;
	}
	catch(...){
		throw MiscError("getFileExtension has caught an unknown error", 3241);
	}
}

void EmailRipper::LogMessage( string message )
{
	try{

		bool bLogLocationExists = false;
		bool bSaveLocationExists = false;

		if( bLog == true )
		{				
			int i = 1;
			string sLogPath( pLogPath );
			string sFileName( sLogPath + "_WebArborEmailError" + intToString(i) + ".txt" );
			while( FileExists(sFileName.c_str()) == 1 )
			{
				i += 1;
				sFileName = sLogPath + "_WebArborEmailError" + intToString(i) + ".txt";
			}			
			writeFile( message.c_str(), sFileName.c_str() );
			bLogLocationExists = true;
		}

		if( bSave == true )
		{	
			if( bLogLocationExists == true && strcasecmp( pSaveLocation, pLogPath ) == 0 ) return;

			int i = 1;
			string sLogPath( pSaveLocation );
			string sFileName( sLogPath + "_WebArborEmailError" + intToString(i) + ".txt" );
			while( FileExists(sFileName.c_str()) == 1 )
			{
				i += 1;
				sFileName = sLogPath + "_WebArborEmailError" + intToString(i) + ".txt";
			}			
			writeFile( message.c_str(), sFileName.c_str() );			
			bSaveLocationExists = true;
		}

		if( bLogLocationExists == false && bSaveLocationExists == false ) {
			setErrorMessage("Unable to log bad message. Neither log path nor save location exist.");
			return_status = 384;
		}	
	}
	catch(...){
		setErrorMessage("Unable to log bad message. Make sure file paths are valid and that proper permissions are in place.");
		return_status = 297;	
	}
}

void EmailRipper::writeFileSpecial( string &Input, const char * pFilePath )
{

	ofstream outClientFile( pFilePath, ios::out );

	try{

		if ( !outClientFile ){
			//throw FileWillNotOpen();
			throw MiscError( "writeFileSpecial was unable to open a file", 109 );
			//cerr << "File could not be opened" << endl;
			//exit(1);
		}

		istringstream iss;
		char line_buff[ MAX_CHAR_BUFFER ];

		iss.str (Input);
		while( iss.getline( line_buff, MAX_CHAR_BUFFER ) )
		{	
			RTrimCrLfSpace( line_buff );
			outClientFile << line_buff << Lf;
		}

		outClientFile.close();

	}
	catch( MiscError ){		
		throw;
	}
	catch(...){
		if ( outClientFile ) outClientFile.close();
		throw(MiscError("writeFileSpecial has caught an unknown error.", 6851));
	}
}

string EmailRipper::GetHeader( string &Input ){

	string sRetVal = "";
	size_t iPos = Input.find( double_crLf );
	if( iPos != string::npos ){
		sRetVal = Input.substr( 0, iPos + 2 );
	}
	return sRetVal;
}

inline void EmailRipper::RenameFalseContentID( string &sFile, const char * chExt ){

	// rename the file sFile
	// rename the physical file
	// rename the contentid vector entry
	bool bFound = false;
	string sTemp = "";
	string sFind = "|";
	sFind.append(sFile);
	size_t iPos = 0;

	vector<string>vTempContent;
	typedef vector<string>::const_iterator VI;
	for(VI p=contentid.begin(); p!=contentid.end(); ++p){			
		sTemp = *p;
		iPos = sTemp.find( sFind );
		if( iPos != string::npos ){
			string sLeft = sTemp.substr(0, iPos + 1);
			iPos = sFile.find_last_of(".");
			string sOldFile = sFile;
			sFile = sFile.substr( 0, iPos );
			// Rename file. This will be used in the calling function.
			sFile.append( chExt );
			if( DoesFileNameExist( sFile, 2 ) == true )
			{
				sFile = getNewAttachmentName( sFile, 2 );
			}
			sLeft.append( sFile );			
			if( RenameFile( pSaveLocation, sOldFile.c_str(), sFile.c_str(), true ) == true ){
				vTempContent.push_back( sLeft );
				bFound = true;
			}
			else{
				sFile = sOldFile;	
			}
		}
		else{
			vTempContent.push_back( sTemp );
		}
	}
	
	if( bFound == true ){
		contentid.swap(vTempContent);
	}
		
}