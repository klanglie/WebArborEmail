// EmailRipper
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission

#include"NSQuotedPrintable.h"
#include"NSBase64.h"
#include"NSUUCode.h"
#include"NSFileFunctions.h"
#include"NSErrorClasses.h"
#include"NSRandomGenerator.h"
#include"AMMimeUtils.h" // This is used for iso... header deciphering.
#include"NSMimeUtils.h"
#include<vector>
#include"NSHeaderItem.h"
#include"NSHeader.h"
#include"NSMessageContent.h"
#include<sstream> // Used with RemoveDoublePeriodsFromMessage & RemoveUUCode
//#include<cassert>
#include"NSMaxLimits.h"
#include"CFilterTokenInfo.h"
#include<map>
//#include<fstream>
//#include<algorithm>

#ifndef EMAIL_RIPPER
#define EMAIL_RIPPER

const int MAX_CHAR_BUFFER = 2048;
//enum { 
//    CR = 0xD, 
//    LF = 0xA, 
//    NL = '\n' 
//};
namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace


class EmailRipper : protected NSFileFunctions, public NSHeader{

public:
	
	EmailRipper(); 
	EmailRipper( const char* );
	~EmailRipper(); 
	
	void  setFilePath( const char* );	
	void  Decipher( const char* );
	void  Decipher( const char*, const char* );
    bool  getHeaderItem( const char*, char* );

	int   getReturnStatus() { return return_status; };
	const char *getReturnMessage() { return pReturnMessage; };

	bool  HasInlineContent() { return bHasContentId; };
	bool  HasInlineContentText() { return bInlineText; };

	long  getMessageSize(){ return message_size; };	
	string getHeader();

	int  getTextMessageCount(){ return i_text_message_count; };
	int  getHtmlMessageCount(){ return i_html_message_count; };
	int  getEnrichedMessageCount(){ return i_enriched_message_count; };
	int  getDeliveryStatusMessageCount(){ return i_message_delivery_status_count; };
	int  getTotalMessageCount(){ return (i_text_message_count + i_html_message_count + i_enriched_message_count + i_message_delivery_status_count); };
	string getMessageBody( int iType, int iPart );

	int  getNormalAttachmentCount(){ return i_non_inline_attachment_count; };
	int  getInlineAttachmentCount(){ return i_inline_attchment_count; };
	int  getInlineTextAttachmentCount(){ return i_inline_attachment_text_count; };
	int  getTotalAttachmentCount(){ return (i_non_inline_attachment_count + i_inline_attchment_count + i_inline_attachment_text_count); };
	string getAttachmentFileName( int iType, int iFile );

	void setIsTrialVersion( bool bType ) { bIsTrialVersion = bType; };	
	void setLogPath( const char * );
	void setIsTestLogging( bool bTest ) { bTestLogging = bTest; };

	void setResetErrorResponse() { setErrorMessage("", 0); };
	void setInlineAddItem( const char *AddItem );

	// Filtering
	int LoadImaFile( const char*, int );
	bool getIsImaFileLoaded() { return b_IsLoaded; };

	void ResetIma();
	bool getIsLowerCaseIma() { return b_Lowercase; };
	void setIsLowerCaseIma( bool bLower ) { b_Lowercase = bLower; };
		
	/* Begin item count */
	int getImaItemCount() { return line_count; };
	int getImaFromCount() { return f_count; };
	int getImaSubjectCount() { return s_count; };
	int getImaSenderCount() { return n_count; };
	int getImaToCount() { return t_count; };
	int getImaHeaderCount() { return h_count; };
    int getImaBodyCount() { return b_count; };
	int getImaKillListCount() { return k_count; };
	/* End item count */

	int SearchImaTokens(  const char *from, const char*from_email, const char *subject, const char *sender,
									const char *to, const char *header, const char *body );

	int getImaMessageArea( int Idx );
	int getImaConditionExpression( int Idx );
	string getImaTokenMailBox( int Idx );
	string getImaTokenLine( int Idx );
	int getImaCompareResult() { return compare_result; };
	// End Filtering

protected:

	void DecipherHelper();
	void setErrorMessage( const char *, int );
	void setErrorMessage( const char * );
	int	 return_status;
	char *pReturnMessage;
	void setSaveLocation( const char *Path );
	inline void InitializeEmailRipperVariables();

	string header; 
	string m_Body;

private:
	
	inline void InitializeConstructor();

	char			*pFilePath;
	long			message_size;
	char			*pLogPath;
	char			*pInlineAddItem;

	// Body variables:
	char	*pSaveLocation;

	string	m_strBoundary;

	vector<NSMessageContent>vAttachment;
	int i_non_inline_attachment_count;
	int i_inline_attchment_count;
	int i_inline_attachment_text_count;

	bool bIsTrialVersion;
	bool bTestLogging;
	bool bInlineAddItem;
	bool bSave;
	bool bLog;
	bool bInlineText;
	bool bHasContentId;
	bool bMulti;	

	vector<string>contentid;
	vector<NSMessageContent>vContent;
	int i_text_message_count;
	int i_html_message_count;
	int i_enriched_message_count;
	int i_message_delivery_status_count;
	//int i_mime_level;

	bool b_DidRemoveInlineContent;

	inline void DecipherBody();
	inline int FindMimeContent( string & );
	inline void GetMimeContent( string &, string* [], int );
	void SortMimeContent ( string &, string* [], int );
	
	string ExtractFileName( string name, string file_name,
					   string & sContentType, string & sContentDisposition );
	
	inline void RemoveUUCode( string & );
	inline bool RemoveDoublePeriodsFromMessage( string & ); 
	inline string MimeHelper(string & inContent, const char * save_location, const char * temp_file);

	inline void RemoveInlineContent( string & );
	inline string ReplaceInlineContentHelper( char *, char *, char * );
	
	inline unsigned long DetermineNewMessageBodySize( char *, size_t, char *, size_t );
	inline bool handleInlineText( string & );

	inline bool handleInlineContentID(string &sFileName, string &sContentId, string &sContentLocation, string &sLowerContentType, string &sFile);
	inline bool decodeContentToFileIfNeedBe( string &sContent, string &sFileName, 
									   string::size_type iEncode, 
									   string::size_type iQuotedPrintable, 
									   string::size_type iApplicationOctetStream, 
									   string::size_type iText, 
									   string::size_type iHtml, 
									   string::size_type i7bit );
	
	inline string AddAttachmentName( string &sFileName, int iType );
	inline bool DoesFileNameExist( string &sFileName, int iType );
	inline bool RemoveAttachmentName( string &sFileName, int iType );
	inline string getFileExtension(string sFile);
	inline int RemoveBadFileNameChars( char * );
	inline string getNewAttachmentName( string &sFileName, int iType );
	inline void AddAttachmentNameNotFancy( const char * chFile, int iType );
	void LogMessage( string message );
	inline string GetBoundary( string & sInput, int iStartPos );
	inline int DecipherBodyHelper();
	inline void ChangeAllNonTextInlineAttachmentsToNormalOnes();
	inline bool SaveInlineContentToFile( string & sInput );
	void writeFileSpecial( string &Input, const char *path );
	string GetHeader( string &Input );
	inline void RenameFalseContentID( string &sFile, const char * chExt );
	void RemoveFileNameQuotes( string & file_name );

	//Filtering
	class TokenPosition{	
	public: 
		TokenPosition(int expression_number, int end_pos):iExpressionNumber(expression_number), iEndPos(end_pos){}
		int ExpressionNumber() const { return iExpressionNumber; }
		int EndPos() const { return iEndPos; }
	private:
		int iExpressionNumber;
		int iEndPos;
	};

	int compare_result;

	bool b_IsLoaded;
	bool b_Lowercase;

	int line_count;
	int f_count;
	int s_count;
	int n_count;
	int t_count;
	int h_count;
	int b_count;
	int k_count;
	vector<CFilterTokenInfo>all_tokens;

	void validateFileExtension( const char * );
	inline void parseLine(string &, int &);
	
	string EscapeBadChars( string );
	unsigned long DetermineNewTokenLineSize(const char *);
	void RemoveBadChars(char *);
	inline int ParseTokens(CFilterTokenInfo &oToken, char * chFind);
	//
	typedef vector<TokenPosition> TokenPositions;
	typedef multimap<int,int> DoubleIntMultiMap;		
	typedef map<int,int> DoubleIntMap;
	typedef vector<int> IntMap;
	//
	inline void AddToCollections(vector<TokenPosition> &token_positions,
								DoubleIntMultiMap &positions,
								size_t &iStartPos, 
								size_t &iEndPos,
								int &iInternalCollectionNumber);

	inline bool DoCollectionsAddUp( TokenPositions & token_positions,
				            		DoubleIntMultiMap & positions,
							        int & iEndExpressionNumber );

	inline bool DoesCollectionContain( DoubleIntMap & oAvoid, 
											int & iExpressionNumber, 
											int & iEndPos );

	inline int EmailRipper::FindStrHelper( int & iExpressionType,
										int & iStartPos,
										int & iLength,
										size_t & start_pos, 
										size_t & len_find, 
										size_t & len_search, 
										char * chFind, 
										const char * chToken );

	inline bool EmailRipper::QuantifierHelper( int & iExpressionType, int & iCount, 
       										int & iStartPos, int & iLength,
											int & iBadCount );

	inline int FindTextPattern( const char * chFind, int & start_pos, int & iStartPos, int & iLength,
										 int & iExpressionType, int & iPreviousExpressionType );

	inline void GetNextPositions( IntMap &, TokenPositions & );
	inline bool EvaluateTextPatternExpression( char ch, int iExpressionType );
	inline void getMinMaxTokenLength( CFilterTokenInfo & oToken, size_t & iMinLen, size_t & iMaxLen );

	inline bool IsAnyChar(char *c);
	inline bool IsAnyWordChar(char *c);
	inline bool IsAnyDigit(char *c);
	inline bool IsAnyWhiteSpace(char *c);
	inline bool IsEvenNumber( int iNum );

	//// p. 211
	//template <class K, class V>
	//class value_equals{
	//private:
	//	V value;
	//public:
	//	value_equals(const V& v) : value(v){}
	//	// comparison
	//	bool operator() (pair<const K, V> elem){
	//		return elem.second == value;
	//	}
	//};

	//End Filtering

};

} // End namespace definition

#endif
