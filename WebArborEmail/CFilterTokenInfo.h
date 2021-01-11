// EmailFilter
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "NSStringFunctions.h"
#include "NSErrorClasses.h"
#include <vector>

#ifndef FILTER_TOKEN_INFO
#define FILTER_TOKEN_INFO

namespace WEBARBOR_EMAIL_COMPONENTS{ 

class TokenDetail{
public:
	TokenDetail( string s, int iMessageArea, int iStartPos, int iLength )
		: _word(s), _type(iMessageArea), start_pos(iStartPos), exp_length(iLength){}
		string Token() const { return _word; }
		int Type() const { return _type; }
		int StartPos() const { return start_pos; }
		int Length() const { return exp_length; }
private:
	string _word;
	int _type;
	int start_pos;
	int exp_length;	
};

enum {	FROM = 1, 
		SUBJECT, 
		SENDER, 
		TO, 
		ENTIRE_HEADER, 
		ENTIRE_BODY, 
		KILL_LIST, 
		CONTAINS, 
		DOES_NOT_CONTAIN, 
		EQUALS, 
		DOES_NOT_EQUAL, 
		NONE,
		BEGIN_VERTICAL_OR,
		VERTICAL_OR,
		END_VERTICAL_OR,
		BASIC_TEXT_PATTERN,
		ANY_CHAR,
		ANY_WORD_CHAR,
		ANY_NON_WORD_CHAR,
		ANY_DIGIT,
		ANY_NON_DIGIT,
		ANY_WHITE_SPACE,
		ANY_NON_WHITE_SPACE,
		ANY_PUNC_CHAR,
		ANY_NON_PUNCT_CHAR,
		QUANTIFIER_ZERO_OR_MORE,
		QUANTIFIER_ONE_OR_MORE,
		QUANTIFIER_EXACT_NUM,
		QUANTIFIER_BETWEEN_NUM,
		EXIT_GOOD,
		EXIT_BAD,
		EXIT_NOT_FOUND
};

// Helper class of EmailFilter
//-----------------------------------------------||
class CFilterTokenInfo : private NSStringFunctions{
	
public:

	CFilterTokenInfo();
	~CFilterTokenInfo();
		
	void ParseLine( string sToken, int token_type, int condition_expression, int line_number, bool bLower );
	int getArea(){ return iMessageArea; };
	int getType(){ return iConditionExpression; };
	string getMailBox(){ return mailbox; };
	TokenDetail getDetails( int index );
	int getTokenCount(){ return iExpressionCount; };
	int getLineNumber(){ return iLineNumber; };

private:

int iMessageArea;
int iConditionExpression;
int iExpressionCount;
int iLineNumber;
string mailbox;
vector<TokenDetail>token_details;
inline void ProcessExpressions( string & sLine, bool & bLowerCase );

};

} // End namespace definition

#endif
