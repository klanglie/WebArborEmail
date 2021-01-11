// DecoderEngine.h : Declaration of the CDecoderEngine

#pragma once
#include "resource.h"       // main symbols
#include "EmailRipper.h"
#include "NSRegistration.h"
//#include "NSUidlCache.h"
#include "atlcom.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IDecoderEngine
[
	object,
	uuid("A4117D6B-7981-4D27-8148-8BDFDD3C7A5D"),
	dual,	helpstring("IDecoderEngine Interface"),
	pointer_default(unique)
]
__interface IDecoderEngine : IDispatch
{
	[propget, id(1), helpstring("property AppVersion")] HRESULT AppVersion([out, retval] BSTR* pVal);
	[propget, id(2), helpstring("property AttachmentFileName")] HRESULT AttachmentFileName([in] SHORT iType, [in] SHORT iFile, [out, retval] BSTR* pVal);
	[propget, id(3), helpstring("property Bcc")] HRESULT Bcc([out, retval] BSTR* pVal);
	[propget, id(4), helpstring("property BccCount")] HRESULT BccCount([out, retval] SHORT* pVal);
	[propget, id(5), helpstring("property BccEmail")] HRESULT BccEmail([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(6), helpstring("property BccName")] HRESULT BccName([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(7), helpstring("property Cc")] HRESULT Cc([out, retval] BSTR* pVal);
	[propget, id(8), helpstring("property CcCount")] HRESULT CcCount([out, retval] SHORT* pVal);
	[propget, id(9), helpstring("property CcEmail")] HRESULT CcEmail([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(10), helpstring("property CcName")] HRESULT CcName([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(11), helpstring("property Charset")] HRESULT Charset([out, retval] BSTR* pVal);
	[propget, id(12), helpstring("property ContentDisposition")] HRESULT ContentDisposition([out, retval] BSTR* pVal);
	[propget, id(13), helpstring("property ContentTransferEncoding")] HRESULT ContentTransferEncoding([out, retval] BSTR* pVal);
	[propget, id(14), helpstring("property ContentType")] HRESULT ContentType([out, retval] BSTR* pVal);
	[propget, id(15), helpstring("property Date")] HRESULT Date([out, retval] BSTR* pVal);
	[propget, id(16), helpstring("property DecipherFile")] HRESULT DecipherFile([in] BSTR FilePath, [in] BSTR SaveLocation, [out, retval] SHORT* pVal);
	[propget, id(17), helpstring("property DecipherString")] HRESULT DecipherString([in] BSTR RawMessage, [in] BSTR SaveLocation, [out, retval] SHORT* pVal);
	[propget, id(18), helpstring("property From")] HRESULT From([out, retval] BSTR* pVal);
	[propget, id(19), helpstring("property FromAt")] HRESULT FromAt([out, retval] BSTR* pVal);
	[propget, id(20), helpstring("property FromEmail")] HRESULT FromEmail([out, retval] BSTR* pVal);
	[propget, id(21), helpstring("property FromName")] HRESULT FromName([out, retval] BSTR* pVal);
	[propget, id(22), helpstring("property GetHeaderItem")] HRESULT GetHeaderItem([in] BSTR HeaderItem, [out, retval] BSTR* pVal);
	[propget, id(23), helpstring("property HasInlineContent")] HRESULT HasInlineContent([out, retval] SHORT* pVal);
	[propget, id(24), helpstring("property HasInlineContentText")] HRESULT HasInlineContentText([out, retval] SHORT* pVal);
	[propget, id(25), helpstring("property Header")] HRESULT Header([out, retval] BSTR* pVal);
	[propget, id(26), helpstring("property HtmlMessageCount")] HRESULT HtmlMessageCount([out, retval] SHORT* pVal);
	[propget, id(27), helpstring("property InlineAttachmentCount")] HRESULT InlineAttachmentCount([out, retval] SHORT* pVal);
	[propget, id(28), helpstring("property InlineTextAttachmentCount")] HRESULT InlineTextAttachmentCount([out, retval] SHORT* pVal);
	[propget, id(29), helpstring("property MessageBody")] HRESULT MessageBody([in] SHORT iType, [in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(30), helpstring("property MessageId")] HRESULT MessageId([out, retval] BSTR* pVal);
	[propget, id(31), helpstring("property MessageSize")] HRESULT MessageSize([out, retval] LONG* pVal);
	[propget, id(32), helpstring("property MimeVersion")] HRESULT MimeVersion([out, retval] BSTR* pVal);
	[propget, id(33), helpstring("property NormalAttachmentCount")] HRESULT NormalAttachmentCount([out, retval] SHORT* pVal);
	[propget, id(34), helpstring("property Received")] HRESULT Received([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(35), helpstring("property ReplyTo")] HRESULT ReplyTo([out, retval] BSTR* pVal);
	[propget, id(36), helpstring("property ReturnMessage")] HRESULT ReturnMessage([out, retval] BSTR* pVal);
	[propget, id(37), helpstring("property ReturnStatus")] HRESULT ReturnStatus([out, retval] SHORT* pVal);
	[propget, id(38), helpstring("property ReceivedBy")] HRESULT ReceivedBy([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(39), helpstring("property ReceivedCount")] HRESULT ReceivedCount([out, retval] SHORT* pVal);
	[propget, id(40), helpstring("property ReceivedDate")] HRESULT ReceivedDate([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(41), helpstring("property ReceivedDomain")] HRESULT ReceivedDomain([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(42), helpstring("property ReceivedId")] HRESULT ReceivedId([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(43), helpstring("property ReceivedIp")] HRESULT ReceivedIp([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(44), helpstring("property ReceivedFor")] HRESULT ReceivedFor([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(45), helpstring("property ReceivedFrom")] HRESULT ReceivedFrom([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(46), helpstring("property ReceivedWith")] HRESULT ReceivedWith([in] SHORT Index, [out, retval] BSTR* pVal);
	[id(47), helpstring("method SetRegisteredUser")] HRESULT SetRegisteredUser([in] BSTR Name, [in] BSTR Key);
	[propget, id(48), helpstring("property Sender")] HRESULT Sender([out, retval] BSTR* pVal);
	[propget, id(49), helpstring("property Subject")] HRESULT Subject([out, retval] BSTR* pVal);
	[propget, id(50), helpstring("property TextMessageCount")] HRESULT TextMessageCount([out, retval] SHORT* pVal);
	[propget, id(51), helpstring("property To")] HRESULT To([out, retval] BSTR* pVal);
	[propget, id(52), helpstring("property ToCount")] HRESULT ToCount([out, retval] SHORT* pVal);
	[propget, id(53), helpstring("property ToEmail")] HRESULT ToEmail([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(54), helpstring("property ToName")] HRESULT ToName([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(55), helpstring("property TotalAttachmentCount")] HRESULT TotalAttachmentCount([out, retval] SHORT* pVal);
	[propget, id(56), helpstring("property TotalMessageCount")] HRESULT TotalMessageCount([out, retval] SHORT* pVal);
	[propget, id(57), helpstring("property InReplyTo")] HRESULT InReplyTo([out, retval] BSTR* pVal);
	[propget, id(58), helpstring("property References")] HRESULT References([out, retval] BSTR* pVal);
	[propget, id(59), helpstring("property ReturnPath")] HRESULT ReturnPath([out, retval] BSTR* pVal);
	[propget, id(60), helpstring("property Comments")] HRESULT Comments([out, retval] BSTR* pVal);
	[propget, id(61), helpstring("property Keywords")] HRESULT Keywords([out, retval] BSTR* pVal);
	[propget, id(62), helpstring("property Encrypted")] HRESULT Encrypted([out, retval] BSTR* pVal);
	[propget, id(63), helpstring("property ContentId")] HRESULT ContentId([out, retval] BSTR* pVal);
	[propget, id(64), helpstring("property ContentDescription")] HRESULT ContentDescription([out, retval] BSTR* pVal);
	[propget, id(65), helpstring("property ContentMD5")] HRESULT ContentMD5([out, retval] BSTR* pVal);
	[propget, id(66), helpstring("property ContentLanguage")] HRESULT ContentLanguage([out, retval] BSTR* pVal);
	[id(67), helpstring("method EnableTestLogging")] HRESULT EnableTestLogging([in] SHORT Flag);
	[id(68), helpstring("method SetLogPath")] HRESULT SetLogPath([in] BSTR FilePath);
	[propget, id(69), helpstring("property EnrichedMessageCount")] HRESULT EnrichedMessageCount([out, retval] SHORT* pVal);
	[propget, id(70), helpstring("property DeliveryStatusMessageCount")] HRESULT DeliveryStatusMessageCount([out, retval] SHORT* pVal);	
	[id(71), helpstring("method SetInlineText")] HRESULT SetInlineText([in] BSTR InlineText);
	[id(72), helpstring("method ImaClear")] HRESULT ImaClear(void);
	[propget, id(73), helpstring("property ImaLoadFile")] HRESULT ImaLoadFile([in] BSTR FileName, [in] SHORT MaxSize, [in] SHORT iLower, [out, retval] SHORT* pVal);
	[propget, id(74), helpstring("property ImaIsLoaded")] HRESULT ImaIsLoaded([out, retval] SHORT* pVal);
	[propget, id(75), helpstring("property ImaTokenLine")] HRESULT ImaTokenLine([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(76), helpstring("property ImaTokenMailBox")] HRESULT ImaTokenMailBox([in] SHORT Index, [out, retval] BSTR* pVal);
	[propget, id(77), helpstring("property ImaMessageArea")] HRESULT ImaMessageArea([in] SHORT Index, [out, retval] SHORT* pVal);
	[propget, id(78), helpstring("property ImaConditionExpression")] HRESULT ImaConditionExpression([in] SHORT Index, [out, retval] SHORT* pVal);
	[propget, id(79), helpstring("property ImaBodyCount")] HRESULT ImaBodyCount([out, retval] SHORT* pVal);	
	[propget, id(80), helpstring("property ImaFromCount")] HRESULT ImaFromCount([out, retval] SHORT* pVal);
	[propget, id(81), helpstring("property ImaHeaderCount")] HRESULT ImaHeaderCount([out, retval] SHORT* pVal);	
	[propget, id(82), helpstring("property ImaItemCount")] HRESULT ImaItemCount([out, retval] SHORT* pVal);
	[propget, id(83), helpstring("property ImaEmailOnlyCount")] HRESULT ImaEmailOnlyCount([out, retval] SHORT* pVal);
	[propget, id(84), helpstring("property ImaSenderCount")] HRESULT ImaSenderCount([out, retval] SHORT* pVal);
	[propget, id(85), helpstring("property ImaSubjectCount")] HRESULT ImaSubjectCount([out, retval] SHORT* pVal);
	[propget, id(86), helpstring("property ImaToCount")] HRESULT ImaToCount([out, retval] SHORT* pVal);
	[propget, id(87), helpstring("property ImaCompareResult")] HRESULT ImaCompareResult([out, retval] SHORT* pVal);

};



// CDecoderEngine

[
	coclass,
	default(IDecoderEngine),
	threading(apartment),
	aggregatable(never),
	vi_progid("WebArborEmail.DecoderEngine"),
	progid("WebArborEmail.DecoderEngine.1"),
	version(1.0),
	uuid("22040AEC-1E12-41CF-AF92-7E1F81520374"),
	helpstring("DecoderEngine Class")
]
class ATL_NO_VTABLE CDecoderEngine :
	public IDecoderEngine, private WEBARBOR_EMAIL_COMPONENTS::EmailRipper, 
	private WEBARBOR_EMAIL_COMPONENTS::NSRegistration
{
public:
	CDecoderEngine()
	{
		bIsRegistered = false;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:

	bool bIsRegistered;

public:

	STDMETHOD(get_AppVersion)(BSTR* pVal);
	STDMETHOD(get_AttachmentFileName)(SHORT iType, SHORT iFile, BSTR* pVal);
	STDMETHOD(get_Bcc)(BSTR* pVal);
	STDMETHOD(get_BccCount)(SHORT* pVal);
	STDMETHOD(get_BccEmail)(SHORT Num, BSTR* pVal);
	STDMETHOD(get_BccName)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_Cc)(BSTR* pVal);
	STDMETHOD(get_CcCount)(SHORT* pVal);
	STDMETHOD(get_CcName)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_CcEmail)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_Charset)(BSTR* pVal);
	STDMETHOD(get_ContentDisposition)(BSTR* pVal);
	STDMETHOD(get_ContentTransferEncoding)(BSTR* pVal);
	STDMETHOD(get_ContentType)(BSTR* pVal);
	STDMETHOD(get_Date)(BSTR* pVal);
	STDMETHOD(get_DecipherFile)(BSTR FilePath, BSTR SaveLocation, SHORT* pVal);
	STDMETHOD(get_DecipherString)(BSTR RawMessage, BSTR SaveLocation, SHORT* pVal);
	STDMETHOD(get_From)(BSTR* pVal);
	STDMETHOD(get_FromAt)(BSTR* pVal);
	STDMETHOD(get_FromEmail)(BSTR* pVal);
	STDMETHOD(get_FromName)(BSTR* pVal);
	STDMETHOD(get_GetHeaderItem)(BSTR HeaderItem, BSTR* pVal);
	STDMETHOD(get_HasInlineContent)(SHORT* pVal);
	STDMETHOD(get_HasInlineContentText)(SHORT* pVal);
	STDMETHOD(get_Header)(BSTR* pVal);
	STDMETHOD(get_HtmlMessageCount)(SHORT* pVal);
	STDMETHOD(get_InlineAttachmentCount)(SHORT* pVal);
	STDMETHOD(get_InlineTextAttachmentCount)(SHORT* pVal);
	STDMETHOD(get_MessageBody)(SHORT iType, SHORT Index, BSTR* pVal);
	STDMETHOD(get_MessageId)(BSTR* pVal);
	STDMETHOD(get_MessageSize)(LONG* pVal);
	STDMETHOD(get_MimeVersion)(BSTR* pVal);
	STDMETHOD(get_NormalAttachmentCount)(SHORT* pVal);
	STDMETHOD(get_Received)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReplyTo)(BSTR* pVal);
	STDMETHOD(get_ReturnMessage)(BSTR* pVal);
	STDMETHOD(get_ReturnStatus)(SHORT* pVal);
	STDMETHOD(get_ReceivedBy)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReceivedCount)(SHORT* pVal);
	STDMETHOD(get_ReceivedDate)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReceivedDomain)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReceivedId)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReceivedIp)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReceivedFor)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReceivedFrom)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ReceivedWith)(SHORT Index, BSTR* pVal);
	STDMETHOD(SetRegisteredUser)(BSTR Name, BSTR Key);
	STDMETHOD(get_Sender)(BSTR* pVal);
	STDMETHOD(get_Subject)(BSTR* pVal);
	STDMETHOD(get_TextMessageCount)(SHORT* pVal);
	STDMETHOD(get_To)(BSTR* pVal);
	STDMETHOD(get_ToCount)(SHORT* pVal);
	STDMETHOD(get_ToEmail)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ToName)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_TotalAttachmentCount)(SHORT* pVal);
	STDMETHOD(get_TotalMessageCount)(SHORT* pVal);
	STDMETHOD(get_InReplyTo)(BSTR* pVal);
	STDMETHOD(get_References)(BSTR* pVal);
	STDMETHOD(get_ReturnPath)(BSTR* pVal);
	STDMETHOD(get_Comments)(BSTR* pVal);
	STDMETHOD(get_Keywords)(BSTR* pVal);
	STDMETHOD(get_Encrypted)(BSTR* pVal);
	STDMETHOD(get_ContentId)(BSTR* pVal);
	STDMETHOD(get_ContentDescription)(BSTR* pVal);
	STDMETHOD(get_ContentMD5)(BSTR* pVal);
	STDMETHOD(get_ContentLanguage)(BSTR* pVal);
	STDMETHOD(EnableTestLogging)(SHORT Flag);
	STDMETHOD(SetLogPath)(BSTR FilePath);
	STDMETHOD(get_EnrichedMessageCount)(SHORT* pVal);
	STDMETHOD(get_DeliveryStatusMessageCount)(SHORT* pVal);
	STDMETHOD(SetInlineText)(BSTR InlineText);
	STDMETHOD(ImaClear)(void);
	STDMETHOD(get_ImaLoadFile)(BSTR FileName, SHORT MaxSize, SHORT iLower, SHORT* pVal);
	STDMETHOD(get_ImaIsLoaded)(SHORT* pVal);
	STDMETHOD(get_ImaTokenLine)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ImaTokenMailBox)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_ImaMessageArea)(SHORT Index, SHORT* pVal);
	STDMETHOD(get_ImaConditionExpression)(SHORT Index, SHORT* pVal);
	STDMETHOD(get_ImaBodyCount)(SHORT* pVal);	
	STDMETHOD(get_ImaFromCount)(SHORT* pVal);
	STDMETHOD(get_ImaHeaderCount)(SHORT* pVal);
	STDMETHOD(get_ImaItemCount)(SHORT* pVal);
	STDMETHOD(get_ImaEmailOnlyCount)(SHORT* pVal);	
	STDMETHOD(get_ImaSenderCount)(SHORT* pVal);
	STDMETHOD(get_ImaSubjectCount)(SHORT* pVal);
	STDMETHOD(get_ImaToCount)(SHORT* pVal);
	STDMETHOD(get_ImaCompareResult)(SHORT* pVal);

};