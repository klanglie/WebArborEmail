// DecoderEngine.cpp : Implementation of CDecoderEngine

#include "stdafx.h"
#include "DecoderEngine.h"

// CDecoderEngine
// 2005-12-02
#define TRIAL_DLL
const char * LANGLIE_SYSTEMS = "Langlie Systems, Inc.";

STDMETHODIMP CDecoderEngine::get_AppVersion(BSTR* pVal)
{
	const char * app_version = "1.0.2.5";

	CComBSTR sOut( app_version );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}
/*
void CDecoder::CopyComBSTR( BSTR &sOut, CComBSTR sIn )
{
	int len = SysStringByteLen(sIn);
	
	char *lp_s = new char[len + 1];

	lp_s[len] = 0;

	memcpy(lp_s, sIn, len);

	BSTR s = SysAllocStringByteLen(lp_s, len);

	delete[] lp_s;
	
	sOut = s;

}
*/

////////// Length of a BSTR String //////////
int BSTRLen(BSTR String)
{
	int Len = 0;
	int Pos = 0;
	bool Finish = false;
	if (String == NULL)
		return(0);
	while (!Finish)
	{
		unsigned int part = String[Pos];
		if (part == 0)
		{
			return(Len);
		}
		else
		{
			Len++;
		}
		Pos++;
	}
	return(0);
}

////////// Convert BSTR String to a Character Array //////////
char* BSTRtoChar(BSTR String)
{
	int i,Len = BSTRLen(String);
	char* FinalStr = new char[Len + 1];
	strcpy(FinalStr, "");
	for (i = 0;i < Len;i++)
	{
		unsigned int Part = String[i];
		FinalStr[i] = Part;
		FinalStr[i + 1] = '\0';
	}
	return(FinalStr);
}


////////// Convert a Character Array to a BSTR String //////////
//BSTR ChartoBSTR(char* String)
//{
//	int i,Len = strlen(String);
//	unsigned short* FinalStr = new unsigned short[Len + 1];
//	for (i = 0; i < Len;i++)
//	{
//		char Part = String[i];
//		FinalStr[i] = Part;
//		FinalStr[i + 1] = '\0';
//	}
//	return(FinalStr);
//}

STDMETHODIMP CDecoderEngine::get_AttachmentFileName(SHORT iType, SHORT iFile, BSTR* pVal)
{
	CComBSTR sOut(getAttachmentFileName(iType, iFile).c_str());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Bcc(BSTR* pVal)
{
	CComBSTR sOut(getBCC());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_BccCount(SHORT* pVal)
{
	*pVal = getBCCCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_BccEmail(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getBCCEmail(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_BccName(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getBCCName(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Cc(BSTR* pVal)
{
	CComBSTR sOut(getCC());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_CcCount(SHORT* pVal)
{
	*pVal = getCCCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_CcEmail(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getCCEmail(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_CcName(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getCCName(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Charset(BSTR* pVal)
{
	CComBSTR sOut(getCharset());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ContentDisposition(BSTR* pVal)
{
	CComBSTR sOut(getContentDisposition());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ContentTransferEncoding(BSTR* pVal)
{
	CComBSTR sOut(getContentTransferEncoding());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ContentType(BSTR* pVal)
{
	CComBSTR sOut(getContentType());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Date(BSTR* pVal)
{
	CComBSTR sOut(getDate());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_DecipherFile(BSTR FilePath, BSTR SaveLocation, SHORT* pVal)
{
	USES_CONVERSION;

	if( bIsRegistered == true )
	{
		setFilePath( W2A( FilePath ) );

		Decipher( W2A(SaveLocation) );
		
		*pVal = EmailRipper::getReturnStatus(); 

		////setFilePath( BSTRtoChar(filePath) );
		////Decipher( BSTRtoChar(saveLocation), HtmlFlag );	
	}
	else{
		*pVal = -1;
	}

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_DecipherString(BSTR RawMessage, BSTR SaveLocation, SHORT* pVal)
{
	USES_CONVERSION;

	if( bIsRegistered == true )
	{

		Decipher( W2A( RawMessage ), W2A(SaveLocation) );

		*pVal = EmailRipper::getReturnStatus();

		////Decipher( BSTRtoChar(message), BSTRtoChar(saveLocation), HtmlFlag );
	}
	else{
		*pVal = -1;
	}

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_From(BSTR* pVal)
{
	CComBSTR sOut(getFrom());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_FromAt(BSTR* pVal)
{
	CComBSTR sOut(getFromAt());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_FromEmail(BSTR* pVal)
{
	CComBSTR sOut(getFromEmail());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_FromName(BSTR* pVal)
{
	CComBSTR sOut(getFromName());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_GetHeaderItem(BSTR HeaderItem, BSTR* pVal)
{
	USES_CONVERSION;

	char s[10240];
	s[10239] = '\0';
	
	getHeaderItem( W2A( HeaderItem ), s );

	CComBSTR sOut(s);	
	sOut.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_HasInlineContent(SHORT* pVal)
{
	if( HasInlineContent() ){
		*pVal = 1;
	}
	else{
		*pVal = 0;
	}

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_HasInlineContentText(SHORT* pVal)
{
	if( HasInlineContentText() ){
		*pVal = 1;
	}
	else{
		*pVal = 0;
	}

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Header(BSTR* pVal)
{
	CComBSTR sOut(getHeader().c_str());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_HtmlMessageCount(SHORT* pVal)
{
	*pVal = getHtmlMessageCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_InlineAttachmentCount(SHORT* pVal)
{
	*pVal = getInlineAttachmentCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_InlineTextAttachmentCount(SHORT* pVal)
{
	*pVal = getInlineTextAttachmentCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_MessageBody(SHORT iType, SHORT Index, BSTR* pVal)
{
	CComBSTR sOut(getMessageBody(iType, Index).c_str());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_MessageId(BSTR* pVal)
{
	CComBSTR sOut(getMessageId());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_MessageSize(LONG* pVal)
{
	*pVal = getMessageSize();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_MimeVersion(BSTR* pVal)
{
	CComBSTR sOut(getMimeVersion());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_NormalAttachmentCount(SHORT* pVal)
{
	*pVal = getNormalAttachmentCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Received(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceived(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReplyTo(BSTR* pVal)
{
	CComBSTR sOut(getReplyTo());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReturnMessage(BSTR* pVal)
{
	CComBSTR sOut(EmailRipper::getReturnMessage());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReturnStatus(SHORT* pVal)
{
	*pVal = EmailRipper::getReturnStatus();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedBy(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedBy(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedCount(SHORT* pVal)
{
	*pVal = getReceivedCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedDate(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedDate(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedDomain(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedDomain(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedId(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedId(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedIp(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedIp(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedFor(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedFor(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedFrom(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedFrom(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReceivedWith(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getReceivedWith(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::SetRegisteredUser(BSTR Name, BSTR Key)
{
	USES_CONVERSION;

	char * client_name = W2A( Name );
	char * client_key = W2A( Key );

	if( strlen(client_name) > 2 && strlen(client_name) < 50 && strlen(client_key) == 128 ){
	
		bool bPass = verifyRegistrationCode( client_name, "Langlie Systems, Inc.", client_key, 1, 2, 30 );
		if( bPass == true ){
			#ifdef TRIAL_DLL
			if( EmailRipper::InstrChar( client_name, LANGLIE_SYSTEMS ) != 0 ){
					setIsTrialVersion( true );
				}
			#endif
			bIsRegistered = true;
			EmailRipper::setErrorMessage("", 0);
		}
		else{
			bIsRegistered = false;
			EmailRipper::setErrorMessage(NSRegistration::getReturnMessage(), NSRegistration::getReturnStatus());
		}
		
	}
	else{
		bIsRegistered = false;
		EmailRipper::setErrorMessage( WEBARBOR_EMAIL_COMPONENTS::standard_registration_error, 67029 );
	}

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Sender(BSTR* pVal)
{
	CComBSTR sOut(getSender());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Subject(BSTR* pVal)
{
   CComBSTR sOut(getSubject());
   sOut.CopyTo(pVal);
   
   return S_OK;
}

STDMETHODIMP CDecoderEngine::get_TextMessageCount(SHORT* pVal)
{
	*pVal = getTextMessageCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_EnrichedMessageCount(SHORT* pVal)
{
	*pVal = getEnrichedMessageCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_DeliveryStatusMessageCount(SHORT* pVal)
{
	*pVal = getDeliveryStatusMessageCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_To(BSTR* pVal)
{
	CComBSTR sOut(getTo());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ToCount(SHORT* pVal)
{
	*pVal = getToCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ToEmail(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getToEmail(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ToName(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getToName(Index) );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_TotalAttachmentCount(SHORT* pVal)
{
	*pVal = getTotalAttachmentCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_TotalMessageCount(SHORT* pVal)
{
	*pVal = getTotalMessageCount();

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_InReplyTo(BSTR* pVal)
{
	CComBSTR sOut(getReplyTo());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_References(BSTR* pVal)
{
	CComBSTR sOut(getReferences());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ReturnPath(BSTR* pVal)
{
	CComBSTR sOut(getReturnPath());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Comments(BSTR* pVal)
{
	CComBSTR sOut(getComments());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Keywords(BSTR* pVal)
{
	CComBSTR sOut(getKeywords());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_Encrypted(BSTR* pVal)
{
	CComBSTR sOut(getEncrypted());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ContentId(BSTR* pVal)
{
	CComBSTR sOut(getContentId());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ContentDescription(BSTR* pVal)
{
	CComBSTR sOut(getContentDescription());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ContentMD5(BSTR* pVal)
{
	CComBSTR sOut(getContentMD5());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ContentLanguage(BSTR* pVal)
{
	CComBSTR sOut(getContentLanguage());	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::EnableTestLogging(SHORT Flag)
{
	if( Flag == 1 ){
		setIsTestLogging( true );
	}
	else{
		setIsTestLogging( false );
	}

	return S_OK;
}

STDMETHODIMP CDecoderEngine::SetLogPath(BSTR FilePath)
{
	USES_CONVERSION;

	setLogPath( W2A( FilePath ) );

	return S_OK;
}

STDMETHODIMP CDecoderEngine::SetInlineText(BSTR InlineText)
{
	USES_CONVERSION;

	setInlineAddItem( W2A( InlineText ) );

	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaBodyCount(SHORT* pVal)
{
	*pVal = getImaBodyCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::ImaClear(void)
{
	ResetIma();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaFromCount(SHORT* pVal)
{
	*pVal = getImaFromCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaHeaderCount(SHORT* pVal)
{
	*pVal = getImaHeaderCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaIsLoaded(SHORT* pVal)
{
	if(getIsImaFileLoaded() == true){
		*pVal = 1;
	}
	else{
		*pVal = 0;
	}
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaItemCount(SHORT* pVal)
{
	*pVal = getImaItemCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaEmailOnlyCount(SHORT* pVal)
{
	*pVal = getImaKillListCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaLoadFile(BSTR FileName, SHORT MaxSize, SHORT iLower, SHORT* pVal)
{
	USES_CONVERSION;

	if( bIsRegistered == true )
	{
		if( iLower == 1 ){
			setIsLowerCaseIma( true );
		}
		else{
			setIsLowerCaseIma( false );
		}

		*pVal = LoadImaFile( W2A(FileName), MaxSize );
	}
	else{
		*pVal = -1;
	}
			
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaSenderCount(SHORT* pVal)
{
	*pVal = getImaSenderCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaSubjectCount(SHORT* pVal)
{
	*pVal = getImaSubjectCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaToCount(SHORT* pVal)
{
	*pVal = getImaToCount();
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaTokenLine(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getImaTokenLine( Index ).c_str() );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaTokenMailBox(SHORT Index, BSTR* pVal)
{
	CComBSTR sOut( getImaTokenMailBox( Index ).c_str() );	
	sOut.CopyTo(pVal);
    
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaMessageArea(SHORT Index, SHORT* pVal)
{
	*pVal = getImaMessageArea( Index );
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaConditionExpression(SHORT Index, SHORT* pVal)
{
	*pVal = getImaConditionExpression( Index );
	return S_OK;
}

STDMETHODIMP CDecoderEngine::get_ImaCompareResult(SHORT* pVal)
{
	*pVal = getImaCompareResult();
	return S_OK;
}


