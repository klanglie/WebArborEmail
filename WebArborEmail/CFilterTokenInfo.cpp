// CFilterToken
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "CFilterTokenInfo.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

CFilterTokenInfo::CFilterTokenInfo(){}

CFilterTokenInfo::~CFilterTokenInfo(){}

TokenDetail CFilterTokenInfo::getDetails(int index)
{
	if( iExpressionCount > 0 && ( index >= 0 && index < iExpressionCount ) ){
		return token_details[index];
	}
	else{
		return TokenDetail("", 0, 0, 0);
	}
}

void CFilterTokenInfo::ParseLine( string sToken, int token_type, int condition_expression, int line_number, bool bLower )
{

	iMessageArea = 0;
	iConditionExpression = 0;
	iExpressionCount = 0;
	iLineNumber = 0;
	mailbox = "";
	if(token_details.size() > 0) token_details.clear();

	try{

		switch( token_type ){			
			case FROM:
			case SUBJECT:
			case SENDER:
			case TO:
			case ENTIRE_HEADER:
			case ENTIRE_BODY:			
			case KILL_LIST:
				break;
			default:
				throw MiscError("A wrong token type has been passed into DecodeToken.", 131);
		}	
			
		switch( condition_expression ){
			case CONTAINS:
			case DOES_NOT_CONTAIN:
			case EQUALS:
			case DOES_NOT_EQUAL:
			case NONE:
				break;
			default:
				throw MiscError("A wrong condition expression has been passed into DecodeToken.", 483);
		}

		iMessageArea = token_type;
		iConditionExpression = condition_expression;
		iLineNumber = line_number;
		ProcessExpressions( sToken, bLower );
		iExpressionCount = static_cast<int>(token_details.size());

	}
	catch(MiscError){
		throw;
	}
	catch(...){
		throw MiscError("An error has occured while in DecodeToken.", 132);
	}
	
}

inline void CFilterTokenInfo::ProcessExpressions( string & sLine, bool & bLowerCase )
{

	size_t iPos = sLine.find_last_of(":");
	if( iPos != string::npos ){
		mailbox = sLine.substr(iPos + 1);
		sLine = sLine.substr(0, iPos);	
	} 

	bool bSlash = false;
	bool bCurly = false;
	bool bComma = false;
	string sLeftQuant = "";
	string sRightQuant = "";

	string sWord = "";
	bool bWord = false;
	int iType = NONE;
	int iLeft = 0;
	int iRight = 0;

	size_t iLen = sLine.length();
	for( size_t i = 0; i < sLine.length(); i++ ){

		if( sLine.at(i)== '\\' ){
			if( !bSlash ){
				bSlash = true;
			}
			else if( bSlash ){
				sWord += '\\';
				bWord = true;
				bSlash = false;
			}
		}
		else if( bSlash ){

			iType = NONE;
			if( sLine.at(i)== 'w' ){
				iType = ANY_WORD_CHAR;
			}
			else if( sLine.at(i)== 'W' ){
				iType = ANY_NON_WORD_CHAR;
			}
			else if( sLine.at(i)== 'd' ){
				iType = ANY_DIGIT;
			}
			else if( sLine.at(i)== 'D' ){
				iType = ANY_NON_DIGIT;
			}
			else if( sLine.at(i)== 's' ){
				iType = ANY_WHITE_SPACE;	
			}
			else if( sLine.at(i)== 'S' ){
				iType = ANY_NON_WHITE_SPACE;	
			}
			else if( sLine.at(i)== 'p' ){
				iType = ANY_PUNC_CHAR;	
			}
			else if( sLine.at(i)== 'P' ){
				iType = ANY_NON_PUNCT_CHAR;	
			}
			else{
				sWord += sLine.at(i);
				bWord = true;			
			}

			if( iType != NONE ){
				if( bWord ){
					if( bLowerCase ){
						ToLower(sWord);
					}
					TokenDetail oToken( sWord, BASIC_TEXT_PATTERN, 0, 0 );
					token_details.push_back(oToken);	
					sWord.clear();
					bWord = false;			
				}
				TokenDetail oToken( "", iType, 0, 0 );
				token_details.push_back(oToken);			
			}

			bSlash = false;
		}
		else if( !bSlash ){
				
			iType = NONE;
			if( sLine.at(i)== '(' ){
				iType = BEGIN_VERTICAL_OR;
			}
			else if( sLine.at(i)== '|' ){
				iType = VERTICAL_OR;
			}
			else if( sLine.at(i)== ')' ){
				iType = END_VERTICAL_OR;
			}
			else if( sLine.at(i)== '.' ){
				iType = ANY_CHAR;
			}
			else if( sLine.at(i)== '*' ){
				iType = QUANTIFIER_ZERO_OR_MORE;
			}
			else if( sLine.at(i)== '+' ){
				iType = QUANTIFIER_ONE_OR_MORE;	
			}
			else if( sLine.at(i)== '{' ){
				bCurly = true;		
			}
			else if( sLine.at(i)== ',' ){
				bComma = true;
			}
			else if( sLine.at(i)== '}' ){
				//
				iLeft = 0;
				iRight = 0;
				iLeft = ConvertStringToInt(sLeftQuant);
				if( bCurly && bComma ){					
					iRight = ConvertStringToInt(sRightQuant);
					iType = QUANTIFIER_BETWEEN_NUM;	
				}
				else if( bCurly ){
					iType = QUANTIFIER_EXACT_NUM;	
				}
				bComma = false;
				bCurly = false;				
			}
			else if( bCurly && !bComma ){
				sLeftQuant += sLine.at(i);
			}
			else if( bComma ){
				sRightQuant += sLine.at(i);
			}
			else{
				sWord += sLine.at(i);
				bWord = true;	
			}

			if( iType != NONE ){
			
				if( bWord ){
					if( bLowerCase ){
						ToLower(sWord);
					}
					TokenDetail oToken( sWord, BASIC_TEXT_PATTERN, 0, 0 );
					token_details.push_back(oToken);	
					sWord.clear();
					bWord = false;			
				}

				if( iType != QUANTIFIER_BETWEEN_NUM && iType != QUANTIFIER_EXACT_NUM )
				{
					TokenDetail oToken( "", iType, 0, 0 );
					token_details.push_back(oToken);						
				}
				else{
					sLeftQuant.clear();
					sRightQuant.clear();
					TokenDetail oToken( "", iType, iLeft, iRight );	
					token_details.push_back(oToken);				
				}
			}
		}	
	}

	if( bWord ){
		if( bLowerCase ){
			ToLower(sWord);
		}
		TokenDetail oToken( sWord, BASIC_TEXT_PATTERN, 0, 0 );
		token_details.push_back(oToken);				
	}

}


