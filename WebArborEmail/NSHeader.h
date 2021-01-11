// NSHeader
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include<string>
#include"NSErrorClasses.h"
#include"NSStringFunctions.h"
#include"AMMimeUtils.h" // This is used for iso... header deciphering.
#include"NSQuotedPrintable.h"

#ifndef NS_HEADER
#define NS_HEADER

const int MAX_PERSON_DETAILS_LENGTH = 128;
const int MAX_NUMBER_EMAILS = 512;
namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

	class NSHeader : protected NSStringFunctions, protected AMMimeUtils {
	
	public:

		NSHeader();
		~NSHeader();

		int   getReceivedCount(){ return received_count; }
		const char *getReceived( int ); 
		const char *getReceivedFrom( int ); 
		const char *getReceivedBy( int );
		const char *getReceivedId( int ); 
		const char *getReceivedDomain( int );
		const char *getReceivedIp( int ); 
		const char *getReceivedDate( int );
		const char *getReceivedWith( int );
		const char *getReceivedFor( int );
		const char *getDate();
		const char *getFrom();
		const char *getFromName();
		const char *getFromEmail();
		const char *getFromAt();
		const char *getSubject();
		
		int   getToCount(){return to_count;}
		int   getCCCount(){return cc_count;}
		int   getBCCCount(){return bcc_count;}
		const char *getTo();
		const char *getCC();
		const char *getBCC();
		const char *getToName( int );
		const char *getCCName( int );
		const char *getBCCName( int );
		const char *getToEmail( int );
		const char *getCCEmail( int );
		const char *getBCCEmail( int );

		const char *getMessageId();
		const char *getSender();
		const char *getContentTransferEncoding();
		const char *getContentType();
		const char *getContentDisposition();
		const char *getMimeVersion();
		const char *getCharset();
		const char *getReplyTo();

		const char *getInReplyTo();
		const char *getReferences();
		const char *getReturnPath();
		const char *getComments();
		const char *getKeywords();
		const char *getEncrypted();
		const char *getContentId();
		const char *getContentDescription();
		const char *getContentMD5();
		const char *getContentLanguage();

	protected:
		
		void GetHeaders( string & );
		
		string content_transfer_encoding;
		string content_type;
		string content_disposition;

private:

		struct person_details{
			char name[ MAX_PERSON_DETAILS_LENGTH ];
			char email[ MAX_PERSON_DETAILS_LENGTH ];
		};

		enum append_status{ None, Receive, Date, From, 
							Subject, To, CC, BCC, MessageId, 
							Sender, ContentEncoding,
							ContentType, ContentDisposition, 
							MimeVersion, Charset, ReplyTo, 
							InReplyTo, References, ReturnPath,
							Comments, Keywords, Encrypted,
							ContentID, ContentDescription,
							ContentMD5, ContentLanguage};

		inline void DecipherReceived( char*, int );
		inline void GetNameEmail( person_details ** pd, string sNameEmail, int & iNameEmailCount );
		inline void GetNameEmailHelper( const char* );
		inline void shorten(string &input, int max);

		int				received_count;
		int				to_count;
		int				cc_count;
		int				bcc_count;
		person_details  *pToEmail;
		person_details  *pCcEmail;
		person_details  *pBCcEmail;

		//string header; 
		string received[6];
		string received_from[6];
		string received_by[6];
		string received_id[6];
		string received_domain[6];
		string received_ip[6];
		string received_date[6];
		string received_with[6];
		string received_for[6];
		string date;
		string from;
		string subject;
		string to;
		string cc;
		string bcc;
		string message_id;
		string sender;
		string mime_version;
		string message_charset;
		string from_name;
		string from_email;
		string from_at;
		string reply_to;

		string in_reply_to;
		string references;
		string return_path;
		string comments;
		string keywords;
		string encrypted;
		string content_id;
		string content_description;
		string content_md5;
		string content_language;

		inline void InitializeVariables();
		inline void ReplaceExtraEmailCommas( char * );
		inline void ReplaceHappyFaceWithComma( string & );
		inline bool IsIPv4Address( string & );

	};

} // End namespace definition

#endif