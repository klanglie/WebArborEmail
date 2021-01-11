// EmailRipper
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission

#include "stdafx.h"
#include "EmailRipper.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

int EmailRipper::getImaMessageArea( int iIndex ){

	int iRetVal = -1;

	if( b_IsLoaded && line_count > 0 && ( iIndex >= 0 && iIndex < line_count ) ){
		CFilterTokenInfo oToken = all_tokens[iIndex];
		switch( oToken.getArea() ){
		
			case FROM:
				iRetVal = 1;
				break;

			case SUBJECT:
				iRetVal = 2;
				break;

			case SENDER:
				iRetVal = 3;
				break;

			case TO:
				iRetVal = 4;
				break;

			case ENTIRE_HEADER:
				iRetVal = 5;
				break;

			case ENTIRE_BODY:
				iRetVal = 6;
				break;

			case KILL_LIST:
				iRetVal = 7;
				break;

			default:
				break;
		
		}		
	}
	
	return iRetVal;

}

int EmailRipper::getImaConditionExpression( int iIndex ){

	int iRetVal = -1;

	if( b_IsLoaded && line_count > 0 && ( iIndex >= 0 && iIndex < line_count ) ){
		CFilterTokenInfo oToken = all_tokens[iIndex];
		switch( oToken.getType() ){
		
			case CONTAINS:
				iRetVal = 1;
				break;

			case DOES_NOT_CONTAIN:
				iRetVal = 2;
				break;

			case EQUALS:
				iRetVal = 3;
				break;

			case DOES_NOT_EQUAL:
				iRetVal = 4;
				break;

			case NONE:
				break;

			default:
				break;
		
		}		
	}
	
	return iRetVal;

}

string EmailRipper::getImaTokenMailBox( int Idx ){

if( b_IsLoaded && line_count > 0 && ( Idx >= 0 && Idx < line_count ) ){
	CFilterTokenInfo oToken = all_tokens[Idx];
	return oToken.getMailBox();
}

return "";

}

string EmailRipper::getImaTokenLine( int Idx ){
	
	string sTokenLine = "";
	int iCount, i;
	
	CFilterTokenInfo oToken; 
	if( b_IsLoaded && line_count > 0 && ( Idx >= 0 && Idx < line_count ) ){
		oToken = all_tokens[Idx];			
	}
	else{
		return "";
	}

	switch( oToken.getArea() ){
	
		case FROM:
			sTokenLine.append("F");
			break;

		case SUBJECT:
			sTokenLine.append("S");
			break;

		case SENDER:
			sTokenLine.append("N");
			break;

		case TO:	
			sTokenLine.append("T");
			break;

		case ENTIRE_HEADER:
			sTokenLine.append("H");
			break;

		case ENTIRE_BODY:
			sTokenLine.append("B");
			break;

		case KILL_LIST:
			break;

		default:
			return "";
			break;
	}

	switch( oToken.getType() ){

		case CONTAINS:
			sTokenLine.append("~");
			break;

		case DOES_NOT_CONTAIN:
			sTokenLine.append("!~");
			break;

		case EQUALS:
			sTokenLine.append("=");
			break;

		case DOES_NOT_EQUAL:
			sTokenLine.append("!=");
			break;

		case NONE:
			break;

		default:
			return "";
			break;
	}
	
	iCount = oToken.getTokenCount();
	for( i = 0; i < iCount; i++ ){
	
		TokenDetail oExpression = oToken.getDetails(i);	

		switch( oExpression.Type() ){
			
			case BEGIN_VERTICAL_OR:
				sTokenLine.append("(");
				break;

			case VERTICAL_OR:
				sTokenLine.append("|");
				break;

			case END_VERTICAL_OR:
				sTokenLine.append(")");
				break;

			case BASIC_TEXT_PATTERN:
				if( oToken.getArea() != KILL_LIST ){
					sTokenLine.append( EscapeBadChars(oExpression.Token()) );
				}				
				break;

			case ANY_CHAR:
				sTokenLine.append(".");
				break;
			
			case ANY_WORD_CHAR:
				sTokenLine.append("\\w");
				break;

			case ANY_NON_WORD_CHAR:
				sTokenLine.append("\\W");
				break;

			case ANY_DIGIT:
				sTokenLine.append("\\d");
				break;

			case ANY_NON_DIGIT:
				sTokenLine.append("\\D");
				break;

			case ANY_WHITE_SPACE:
				sTokenLine.append("\\s");
				break;

			case ANY_NON_WHITE_SPACE:
				sTokenLine.append("\\S");
				break;

			case ANY_PUNC_CHAR:
				sTokenLine.append("\\p");
				break;

			case ANY_NON_PUNCT_CHAR:
				sTokenLine.append("\\P");
				break;

			case QUANTIFIER_ZERO_OR_MORE:
				sTokenLine.append("*");
				break;

			case QUANTIFIER_ONE_OR_MORE:
				sTokenLine.append("+");
				break;

			case QUANTIFIER_EXACT_NUM:
				sTokenLine.append("{");
				char buffer [33];
				itoa ( oExpression.StartPos(), buffer, 10 );
				sTokenLine.append( buffer );
				sTokenLine.append("}");
				break;

			case QUANTIFIER_BETWEEN_NUM:
				sTokenLine.append("{");
				char buffer2 [33];
				itoa ( oExpression.StartPos(), buffer2, 10 );
				sTokenLine.append( buffer2 );
				sTokenLine.append(",");
				itoa ( oExpression.Length(), buffer2, 10 );
				sTokenLine.append( buffer2 );
				sTokenLine.append("}");
				break;				

			default:
				return "";
				break;
		}

	}

	if( oToken.getArea() != KILL_LIST ){
		string sMailBox = oToken.getMailBox();
		if( sMailBox.length() > 0 ){
			sTokenLine.append(":");
			sTokenLine.append( sMailBox );		
		}
	}

	return sTokenLine;

}

inline void EmailRipper::getMinMaxTokenLength( CFilterTokenInfo & oToken, size_t & iMinLen, size_t & iMaxLen ){

	iMinLen = iMaxLen = 0;
	int iCount = oToken.getTokenCount();
	for( int i = 0; i < iCount; i++ ){
	
		TokenDetail oExpression = oToken.getDetails(i);	
		switch( oExpression.Type() ){
			
			case BEGIN_VERTICAL_OR:
				break;

			case VERTICAL_OR:
				break;

			case END_VERTICAL_OR:
				break;

			case BASIC_TEXT_PATTERN:
				if( oToken.getArea() != KILL_LIST ){
					iMinLen += oExpression.Token().length();
					iMaxLen += oExpression.Token().length();
				}				
				break;

			case ANY_CHAR:
				++iMinLen;
				++iMaxLen;
				break;
			
			case ANY_WORD_CHAR:
				++iMinLen;
				++iMaxLen;
				break;

			case ANY_NON_WORD_CHAR:
				++iMinLen;
				++iMaxLen;
				break;

			case ANY_DIGIT:
				++iMinLen;
				++iMaxLen;
				break;

			case ANY_NON_DIGIT:
				++iMinLen;
				++iMaxLen;
				break;

			case ANY_WHITE_SPACE:
				++iMinLen;
				++iMaxLen;
				break;

			case ANY_NON_WHITE_SPACE:
				++iMinLen;
				++iMaxLen;
				break;

			case ANY_PUNC_CHAR:
				++iMinLen;
				++iMaxLen;
				break;

			case ANY_NON_PUNCT_CHAR:
				++iMinLen;
				++iMaxLen;
				break;

			case QUANTIFIER_ZERO_OR_MORE:
				++iMinLen;
				++iMaxLen;
				break;

			case QUANTIFIER_ONE_OR_MORE:
				++iMinLen;
				++iMaxLen;
				break;

			case QUANTIFIER_EXACT_NUM:
				iMinLen += oExpression.StartPos();
				iMaxLen += oExpression.StartPos();
				break;

			case QUANTIFIER_BETWEEN_NUM:
				iMinLen += oExpression.StartPos();
				iMaxLen += oExpression.Length();
				break;				

			default:
				iMinLen = iMaxLen = 0;
				break;
		}
	}
}

inline void EmailRipper::AddToCollections(TokenPositions &token_positions,
												 DoubleIntMultiMap &positions,
												  size_t &iStartPos, 
												  size_t &iEndPos,
													 int &iInternalCollectionNumber)
{					
	if( iInternalCollectionNumber > 0 && token_positions.size() > 0 ){
		for (vector<TokenPosition>::const_reverse_iterator j (token_positions.end ());
			j != vector<TokenPosition>::const_reverse_iterator (token_positions.begin ()); ++j){							
			if( j->ExpressionNumber() == iInternalCollectionNumber - 1 &&
				j->EndPos() == iStartPos - 1 ){
					TokenPosition oPos( iInternalCollectionNumber, static_cast<int>(iEndPos) );
					token_positions.push_back(oPos);
					positions.insert( std::make_pair( iStartPos, iInternalCollectionNumber ) );
					break;
			}
			else if( j->ExpressionNumber() < iInternalCollectionNumber - 1 ) break;
		}
	}
	else{						
		TokenPosition oPos( iInternalCollectionNumber, static_cast<int>(iEndPos) );
		token_positions.push_back(oPos);
		positions.insert( std::make_pair( iStartPos, iInternalCollectionNumber ) );	
	}
}

//inline bool EmailRipper::DoesCollectionContain( DoubleIntMap & oAvoid, 
//													 int & iExpressionNumber, 
//													 int & iEndPos ){
//
//    bool bContains = false;
//	if( oAvoid.size() > 0 ){
//	
//		DoubleIntMap::iterator ser;
//		ser = find_if( oAvoid.begin(), oAvoid.end(), value_equals<int,int>(iEndPos) );
//		if(ser != oAvoid.end()){
//			if( ser->first == iExpressionNumber ) bContains = true;
//		}
//
//	}
//	return bContains;
//}

inline bool EmailRipper::DoCollectionsAddUp( TokenPositions & token_positions,
											 DoubleIntMultiMap & positions,
											 int & iEndExpressionNumber ){

	bool bPass = false;
	if( token_positions.size() > 0 ){
			
		if( iEndExpressionNumber == 1 ) return true;

		DoubleIntMap my_finds;
		for( int k = 0; k < iEndExpressionNumber - 1; k++ ){
			my_finds[k] = 0;
		}

		int iExpressionNumber = 0;
		int iEndPos = 0;

		for (vector<TokenPosition>::const_iterator j (token_positions.begin ());
			j != vector<TokenPosition>::const_iterator (token_positions.end ()); ++j){

				iExpressionNumber = j->ExpressionNumber();
				iEndPos = j->EndPos();

				for ( DoubleIntMultiMap::iterator pos = positions.lower_bound( iEndPos + 1 );
					pos != positions.upper_bound( iEndPos + 1 ); ++pos ) {

						if( pos->second == iExpressionNumber + 1 ){
						
								DoubleIntMap::iterator iFind = my_finds.find( iExpressionNumber );
								if( iFind != my_finds.end() ){
									my_finds[ iExpressionNumber  ] = iFind->second + 1;
									break;
								}

						}

				}

		}

		bPass = true;
		// loop through all of my_finds and make sure no x->second is 
		// equal to 0. If it is bPass = false.
		for( DoubleIntMap::iterator  pos2 = my_finds.begin(); pos2 != my_finds.end(); ++pos2 ){
			if( pos2->second == 0 ){
				bPass = false;
				break;
			}
		}
	}

	return bPass;
}

inline void EmailRipper::GetNextPositions( IntMap & oNext, TokenPositions & token_positions )
{
	// get the last end positions...
	int iStart = 0;
	int iCount = 0;
	if( oNext.size() > 0 ) oNext.clear();

	if( token_positions.size() > 0 ){
	
		int iExpressionNumber = 0;
		int iEndPos = 0;
		for (vector<TokenPosition>::const_reverse_iterator j (token_positions.end ());
			j != vector<TokenPosition>::const_reverse_iterator (token_positions.begin ()); ++j){
			
			iExpressionNumber = j->ExpressionNumber();
			iEndPos = j->EndPos();

			if( iCount == 0 ){
				iStart = iExpressionNumber;
			}
			else if( iExpressionNumber != iStart ){
				break;
			}

			oNext.push_back( iEndPos + 1 );			
			++iCount;
		}

	}
	else{
		oNext.push_back(0);
	}

}

inline int EmailRipper::ParseTokens(CFilterTokenInfo &oToken, char *chFind)
{

	register int iRetVal = -1;
	int iFunctionStatus = EXIT_BAD;

	int iPatternExpressionCount = oToken.getTokenCount();		
	int iExpressionType = NONE;

	TokenPositions token_positions;
	DoubleIntMultiMap positions;
	IntMap oNext;

	int iBeginVerticalOrCount = 0;
	int iEndVerticalOrCount = 0;
	bool bEncounteredOr = false;
	int iOrCount = 0;
	size_t iPreviousStartPos = 0;

	int iEndExpressionNumber = 0;
	int iInternalCollectionNumber = 0;		
	size_t len_find = strlen(chFind);
	int iTokenCount = 0;
	bool bContinue;
	size_t pos = 0;
	size_t end_pos = 0;
	bool bAdd = false;

	for( int i = 0; i < iPatternExpressionCount; i++ ){	

		TokenDetail oExpression = oToken.getDetails(i);		
		iExpressionType = oExpression.Type();
		++iTokenCount;

		////if( iExpressionType != END_VERTICAL_OR && iExpressionType != VERTICAL_OR ) bAdd = false;
		bContinue = true;
		if( ( iExpressionType != BEGIN_VERTICAL_OR && 
			  iExpressionType != END_VERTICAL_OR &&
			  iExpressionType != VERTICAL_OR &&
			  iExpressionType != QUANTIFIER_ZERO_OR_MORE &&
			  iExpressionType != QUANTIFIER_ONE_OR_MORE &&
			  iExpressionType != QUANTIFIER_EXACT_NUM &&
			  iExpressionType != QUANTIFIER_BETWEEN_NUM ) && 
			  (i + 1 < iPatternExpressionCount) ){
              // look for quantifiers...
				TokenDetail oNextExpression = oToken.getDetails( i + 1 );

				int iType;
				iType = oNextExpression.Type();

				if( oNextExpression.Type() == QUANTIFIER_ZERO_OR_MORE ){
					bContinue = false;
				}
				else if( oNextExpression.Type() == QUANTIFIER_ONE_OR_MORE ){
					bContinue = false;
				}
				else if( oNextExpression.Type() == QUANTIFIER_EXACT_NUM ){
					bContinue = false;
				}
				else if( oNextExpression.Type() == QUANTIFIER_BETWEEN_NUM ){
					bContinue = false;
				}
		}

		if( bContinue ){			

			if( iExpressionType == BASIC_TEXT_PATTERN ){
				//
				string sTemp = oExpression.Token();
				const char * chToken = sTemp.c_str();			
				size_t len_search = strlen(chToken);
				size_t iTempPos = 0;
				//				
				if( !bEncounteredOr ){
					++iEndExpressionNumber;
					if( token_positions.size() > 0 ) ++iInternalCollectionNumber;
				}

				if( token_positions.size() == 0  ){
					while( ifindstr( pos, len_find, len_search, chFind, chToken, pos ) ){
						end_pos = (pos + len_search) - 1;
						bAdd = true;
						if( bEncounteredOr ){
							++iOrCount;
							if( iOrCount == 1 ) iPreviousStartPos = pos;
						}
						else{
							AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );						
						}
						pos += len_search;
					}
				}
				else{
					if( bEncounteredOr && iOrCount > 0 ){
						if( oNext.size() > 0 ) oNext.clear();
						oNext.push_back( end_pos + 1 );
					}
					else{
						GetNextPositions( oNext, token_positions );	
					}
					for ( vector<int>::const_iterator j (oNext.begin () );
						j != vector<int>::const_iterator (oNext.end ()); ++j ){
						//
						pos = *j;
						iTempPos = *j;
						if( ifindstr( pos, len_find, len_search, chFind, chToken, pos ) ){
							if( pos == iTempPos ){
								end_pos = (pos + len_search) - 1;
								bAdd = true;
								if( bEncounteredOr ){
									++iOrCount;
									if( iOrCount == 1 ) iPreviousStartPos = pos;
								}
								else{
									AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );						
								}
							}
						}
						else if( bEncounteredOr && iOrCount > 0 ){
							++iEndExpressionNumber;
							bAdd = false;
						}
					}				
				}
			}
			else if( iExpressionType == BEGIN_VERTICAL_OR ){
				++iBeginVerticalOrCount;
				if( bEncounteredOr && bAdd ){
					AddToCollections( token_positions, positions, iPreviousStartPos, end_pos, iInternalCollectionNumber );
				}
				bEncounteredOr = false;
				iOrCount = 0;
				iPreviousStartPos  = 0;
			}
			else if( iExpressionType == END_VERTICAL_OR ){
				++iEndVerticalOrCount;
				if( bEncounteredOr && bAdd ){
					AddToCollections( token_positions, positions, iPreviousStartPos, end_pos, iInternalCollectionNumber );				
				}
				bEncounteredOr = false;
				bAdd = false;
				iOrCount = 0;
				iPreviousStartPos  = 0;
			}
			else if( iExpressionType == VERTICAL_OR ){

				if( bEncounteredOr && bAdd ){
					AddToCollections( token_positions, positions, iPreviousStartPos, end_pos, iInternalCollectionNumber );				
				}
				bEncounteredOr = true;
				bAdd = false;
				iOrCount = 0;
				iPreviousStartPos  = 0;
				
				if( (!IsEvenNumber( iTokenCount ) && (iBeginVerticalOrCount == iEndVerticalOrCount + 1)) ||
					(IsEvenNumber( iTokenCount ) && (iBeginVerticalOrCount == iEndVerticalOrCount)) ){							

					bool bPass = DoCollectionsAddUp( token_positions, positions, iEndExpressionNumber );
					if( !bPass ){
						// reset all internal items.
						token_positions.clear();
						positions.clear();
						//
						iEndExpressionNumber = 0;
						iInternalCollectionNumber = 0;		
						//
						iBeginVerticalOrCount = 0;
						iEndVerticalOrCount = 0;
						bEncounteredOr = false;
						iOrCount = 0;
						iPreviousStartPos  = 0;
						iTokenCount = 0;
					}
					else{
						iFunctionStatus = EXIT_GOOD;
						break;
					}
				}
			}
			else if( iExpressionType == QUANTIFIER_ZERO_OR_MORE ||
					 iExpressionType == QUANTIFIER_ONE_OR_MORE || 
					 iExpressionType == QUANTIFIER_EXACT_NUM ||
					 iExpressionType == QUANTIFIER_BETWEEN_NUM ){

				if( i > 0 ){

					if( !bEncounteredOr ){
						if( iExpressionType != QUANTIFIER_ZERO_OR_MORE ){
							++iEndExpressionNumber;						
						}
						if( token_positions.size() > 0 ) ++iInternalCollectionNumber;
					}

					int iStartPos = oExpression.StartPos();
					int iLength = oExpression.Length();										

					int iCount = 0;
					int iBadCount = 0;

					TokenDetail oPreviousExpression = oToken.getDetails( i - 1 );
					if( bEncounteredOr && iOrCount > 0 ){
						if( oNext.size() > 0 ) oNext.clear();
						oNext.push_back( end_pos + 1 );
					}
					else{
						GetNextPositions( oNext, token_positions );	
					}
					if( oPreviousExpression.Type() == BASIC_TEXT_PATTERN ){
					
						string sPreviousToken = oPreviousExpression.Token();
						char chToken = sPreviousToken.at( sPreviousToken.length() - 1 );
						size_t len_search = 1;								
						//
						if( token_positions.size() == 0  ){

							int iFoundCount = 0;
							for( size_t k = 0; k < strlen(chFind); k++ ){
								if( chFind[k] == chToken ){
											
									pos = k;
									iCount = FindStrHelper( iExpressionType, iStartPos, iLength, 
															pos, len_find, len_search, chFind, &chToken);							
										
									if( QuantifierHelper( iExpressionType, iCount, iStartPos, iLength, iBadCount) ){											
										pos = k;
										end_pos = pos + iCount - 1;
										bAdd = true;
										if( bEncounteredOr ){
											++iOrCount;
											if( iOrCount == 1 ) iPreviousStartPos = pos;
										}
										else{
											AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );						
										}																										
										++iFoundCount;
									}
								}
							}
							if( iFoundCount > 0 ){
								iBadCount = 0;
							}
							else if( iExpressionType != QUANTIFIER_ZERO_OR_MORE ){
								iBadCount = 1;
							}
							else if( iFoundCount == 0 && iExpressionType == QUANTIFIER_ZERO_OR_MORE ){
								iBadCount = 1;
							}
						}
						else{
													
							for ( vector<int>::const_iterator j (oNext.begin () );
								j != vector<int>::const_iterator (oNext.end ()); ++j ){
		
								pos = *j;
								iCount = FindStrHelper( iExpressionType, iStartPos, iLength, 
														pos, len_find, len_search, chFind, &chToken);							
									
								if( QuantifierHelper( iExpressionType, iCount, iStartPos, iLength, iBadCount) ){
									pos = *j;
									end_pos = pos + iCount - 1;
									bAdd = true;
									if( bEncounteredOr ){
										++iOrCount;
										if( iOrCount == 1 ) iPreviousStartPos = pos;
									}
									else{
										AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );						
									}																
								}
								else if( bEncounteredOr && iOrCount > 0 && iExpressionType != QUANTIFIER_ZERO_OR_MORE ){
									++iEndExpressionNumber;
									bAdd = false;
								}
							}
						}
					}
					else{

						int iTempPos = 0;
						int iPreviousType = oPreviousExpression.Type();

						if( token_positions.size() == 0  ){

							int iFoundCount = 0;
							for( size_t k = 0; k < strlen(chFind); k++ ){
								if( EvaluateTextPatternExpression( chFind[ k ], iPreviousType ) ){

									iTempPos = static_cast<int>(k);
									iCount = FindTextPattern( chFind, iTempPos, iStartPos, iLength,
																iExpressionType, iPreviousType );	

									if( QuantifierHelper( iExpressionType, iCount, iStartPos, iLength, iBadCount) ){
										pos = k;
										end_pos = pos + iCount - 1;
										bAdd = true;
										if( bEncounteredOr ){
											++iOrCount;
											if( iOrCount == 1 ) iPreviousStartPos = pos;
										}
										else{
											AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );						
										}																
										++iFoundCount;
									}
								}
							}
							if( iFoundCount > 0 ){
								iBadCount = 0;
							}
							else if( iExpressionType != QUANTIFIER_ZERO_OR_MORE ){
								iBadCount = 1;
							}
							else if( iFoundCount == 0 && iExpressionType == QUANTIFIER_ZERO_OR_MORE ){
								iBadCount = 1;
							}

						}
						else{

							for( vector<int>::const_iterator j (oNext.begin () );
								j != vector<int>::const_iterator (oNext.end ()); ++j ){

								iTempPos = *j;
								iCount = FindTextPattern( chFind, iTempPos, iStartPos, iLength,
															iExpressionType, iPreviousType );

								if( QuantifierHelper( iExpressionType, iCount, iStartPos, iLength, iBadCount) ){
									pos = *j;
									end_pos = pos + iCount - 1;
									bAdd = true;
									if( bEncounteredOr ){
										++iOrCount;
										if( iOrCount == 1 ) iPreviousStartPos = pos;
									}
									else{
										AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );						
									}																
								}
								else if( bEncounteredOr && iOrCount > 0 && iExpressionType != QUANTIFIER_ZERO_OR_MORE ){
									++iEndExpressionNumber;
									bAdd = false;
								}
							}
						}
					}
					// use this for both loops. If nothing was found, make sure this token fails.
					if( !bEncounteredOr && iBadCount == oNext.size() ){

						if( iExpressionType == QUANTIFIER_ZERO_OR_MORE ){
							if( token_positions.size() > 0 ) --iInternalCollectionNumber;
						}
						else{
							++iEndExpressionNumber;
						}						
					}
					//
				}
			}
			else{

				if( !bEncounteredOr ){
					++iEndExpressionNumber;
					if( token_positions.size() > 0 ) ++iInternalCollectionNumber;
				}
				
				if( token_positions.size() == 0  ){
					for( size_t k = 0; k < strlen(chFind); k++ ){
						if( EvaluateTextPatternExpression( chFind[ k ], iExpressionType ) ){
							pos = k;
							end_pos = k;
							bAdd = true;
							if( bEncounteredOr ){
								++iOrCount;
								if( iOrCount == 1 ) iPreviousStartPos = pos;
							}
							else{
								AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );														
							}	
						}
					}
				}
				else{
									
					if( bEncounteredOr && iOrCount > 0 ){
						if( oNext.size() > 0 ) oNext.clear();
						oNext.push_back( end_pos + 1 );
					}
					else{
						GetNextPositions( oNext, token_positions );	
					}
					for ( vector<int>::const_iterator j (oNext.begin () );
						j != vector<int>::const_iterator (oNext.end ()); ++j ){

						pos = *j;
						if( EvaluateTextPatternExpression( chFind[ pos ], iExpressionType ) ){								
							end_pos = pos;
							bAdd = true;
							if( bEncounteredOr ){
								++iOrCount;
								if( iOrCount == 1 ) iPreviousStartPos = pos;
							}
							else{
								AddToCollections( token_positions, positions, pos, end_pos,	iInternalCollectionNumber );						
							}							
						}
						else if( bEncounteredOr && iOrCount > 0 ){
							++iEndExpressionNumber;
							bAdd = false;
						}
					}
				}
			}
		}
		if( iEndExpressionNumber > iInternalCollectionNumber + 1 ){
			iFunctionStatus = EXIT_NOT_FOUND;
			break;
		}
	}

	if( iFunctionStatus == EXIT_BAD ){
		bool bPass = DoCollectionsAddUp( token_positions, positions, iEndExpressionNumber );
		if( bPass ) iFunctionStatus = EXIT_GOOD;
	}

	if( iFunctionStatus == EXIT_GOOD ){

		switch( oToken.getType() ){
			case CONTAINS:
				iRetVal = oToken.getLineNumber();	
				break;
			case DOES_NOT_CONTAIN:
				// This is already -1.
				break;
			case EQUALS:
				size_t iMin, iMax;
				getMinMaxTokenLength( oToken, iMin, iMax );
				if( iMin == iMax ){
					if( len_find == iMin ) iRetVal = oToken.getLineNumber(); 
				}
				else if( len_find >= iMin && len_find <= iMax ){
					iRetVal = oToken.getLineNumber();
				}
				break;
			case DOES_NOT_EQUAL:
				size_t iMin2, iMax2;
				getMinMaxTokenLength( oToken, iMin2, iMax2 );
				if( iMin2 == iMax2 ){
					if( len_find != iMin2 ) iRetVal = oToken.getLineNumber(); 
				}
				else if( len_find < iMin2 || len_find > iMax2 ){
					iRetVal = oToken.getLineNumber();
				}
				break;
		}
	}
	else{		
		switch( oToken.getType() ){

			case CONTAINS:
				// This is already -1.	
				break;
			case DOES_NOT_CONTAIN:
				iRetVal = oToken.getLineNumber();
				break;
			case EQUALS:
				// This item was not found 
				break;
			case DOES_NOT_EQUAL:
				iRetVal = oToken.getLineNumber();
				break;

		}
	}
	return iRetVal;
}

inline int EmailRipper::FindTextPattern( const char * chFind, int & start_pos, int & iStartPos, int & iLength,
										 int & iExpressionType, int & iPreviousExpressionType )
{

	int iCount = 0;
	while( EvaluateTextPatternExpression( chFind[ start_pos ], iPreviousExpressionType ) ){
											
		if( iExpressionType == QUANTIFIER_EXACT_NUM && iCount == iStartPos ){
			break;
		}
		else if( iLength > 0 && iCount == iLength ){									
			break;
		}			
		++iCount; 
		++start_pos;
	}
	return iCount;
}

inline int EmailRipper::FindStrHelper( int & iExpressionType,
									   int & iStartPos,
									   int & iLength,
									   size_t & start_pos, 
									   size_t & len_find, 
						               size_t & len_search, 
						               char * chFind, 
						               const char * chToken ){

	int iCount = 0;
	size_t pos = 0;
	while( ifindstr( start_pos, len_find, len_search, chFind, chToken, pos ) ){

		if( pos != start_pos ) break;

		if( iExpressionType == QUANTIFIER_EXACT_NUM && iCount == iStartPos ){
			break;
		}
		else if( iLength > 0 && iCount == iLength ){									
			break;
		}
		++iCount; 
		++start_pos;
	}
	return iCount;
}

inline bool EmailRipper::QuantifierHelper( int & iExpressionType, int & iCount, 
       						               int & iStartPos, int & iLength,
							               int & iBadCount ){

	bool bRetVal = false;
	if( iExpressionType == QUANTIFIER_EXACT_NUM && iCount != iStartPos ){
		++iBadCount;
	}
	else if( iLength > 0 && (iCount < iStartPos || iCount > iLength) ){
		++iBadCount;
	}	
	else if( iCount > 0 ){					
		bRetVal = true;
	}
	else{
		++iBadCount;
	}
	return bRetVal;

}

inline bool EmailRipper::IsEvenNumber( int iNum ){

	bool bRetVal = false;
	if( (iNum > 1) && ( (iNum % 2) == 0 ) ){
		bRetVal = true;
	}
	else if( iNum == 0 ){
		bRetVal = true;
	}
    return bRetVal;
}

inline bool EmailRipper::EvaluateTextPatternExpression( char ch, int iExpressionType ){

	// Any character .
	// Any word character (a-z, A-Z, 0-9) \w
	// Any non-word character \W
	// Any digit (0-9) \d
	// Any non-digit \D
	// Any white space (spaces and/or tabs and/or carriage returns) \s)
	// Any non-white space \S
	// Any punctuation character (any character other than \w or \s) \p
	// Any non-punctuation character \P

	bool bAdd = false;

	switch( iExpressionType ){

		case ANY_CHAR:
			if( IsAnyChar(&ch) ) bAdd = true;
			break;

		case ANY_WORD_CHAR:
			if( IsAnyWordChar(&ch) ) bAdd = true;
			break;

		case ANY_NON_WORD_CHAR:
			if( !IsAnyWordChar(&ch) ) bAdd = true;
			break;

		case ANY_DIGIT:
			if( IsAnyDigit(&ch) ) bAdd = true;
			break;

		case ANY_NON_DIGIT:
			if( !IsAnyDigit(&ch) ) bAdd = true;
			break;

		case ANY_WHITE_SPACE:
			if( IsAnyWhiteSpace(&ch) ) bAdd = true;
			break;

		case ANY_NON_WHITE_SPACE:
			if( !IsAnyWhiteSpace(&ch) ) bAdd = true;
			break;

		case ANY_PUNC_CHAR:
			if( ( !IsAnyWordChar(&ch) ||
				  !IsAnyWhiteSpace(&ch) ) &&
				   IsAnyChar(&ch) ) bAdd = true;
			break;

		case ANY_NON_PUNCT_CHAR:
			if( IsAnyWordChar(&ch) ||
				IsAnyWhiteSpace(&ch) ) bAdd = true;
			break;
	}

	return bAdd;

}

inline bool EmailRipper::IsAnyChar(char *c){

	if( *c >= 0 && *c <= 127 ){
		return true;
	}
	else{
		return false;
	}
}

inline bool EmailRipper::IsAnyWordChar(char *c){
	// a-z, A-Z, 0-9
	if( (*c >= 97 && *c <= 122) ||
		(*c >= 65 && *c <= 90) ||
		(*c >= 48 && *c <= 57) ){
		return true;
	}
	else{
		return false;
	}
}

inline bool EmailRipper::IsAnyDigit(char *c){

	if( *c >= 48 && *c <= 57 ){
		return true;
	}
	else{
		return false;
	}
}

inline bool EmailRipper::IsAnyWhiteSpace(char *c){

	if( *c == 9 || *c == 10 || *c == 13 || *c == 32){
		return true;
	}
	else{
		return false;
	}
}

int EmailRipper::SearchImaTokens( const char *fn, const char *fe, const char *su, 
								   const char *se, const char *t, 
								   const char *h,  const char *b ){

	 int iRetVal = -1;
	
	 char *from = 0;
	 char *from_email = 0;
	 char *subject = 0;
	 char *sender = 0;
	 char *to = 0;
	 char *header = 0;
	 char *body = 0;

	 try{

		if( b_IsLoaded && line_count > 0 ){
		 
			from = new char[strlen(fn) + 1];
			from[strlen(fn)] = 0;
			strcpy(from, fn);

			from_email = new char[strlen(fe) + 1];
			from_email[strlen(fe)] = 0;
			strcpy(from_email, fe);

			subject = new char[strlen(su) + 1];
			subject[strlen(su)] = 0;
			strcpy(subject, su);

			sender = new char[strlen(se) + 1];
			sender[strlen(se)] = 0;
			strcpy(sender, se);

			to = new char[strlen(t) + 1];
			to[strlen(t)] = 0;
			strcpy(to, t);

			header = new char[strlen(h) + 1];
			header[strlen(h)] = 0;
			strcpy(header, h);

			body = new char[strlen(b) + 1];
			body[strlen(b)] = 0;
			strcpy(body, b);

			TRIM(from);
			TRIM(from_email);
			TRIM(subject);
			TRIM(sender);
			TRIM(to);
			TRIM(header);
			TRIM(body);
		
			if( b_Lowercase ){
				if( strlen(from) > 0 && ( f_count > 0 || k_count > 0 ) ){
					ToLower(from);
				}
				if( strlen(from_email) > 0 && ( f_count > 0 || k_count > 0 ) ){
					ToLower(from_email);
				}
				if( strlen(subject) > 0 && s_count > 0 ){
					ToLower(subject);
				}
				if( strlen(sender) > 0 && n_count > 0 ){
					ToLower(sender);
				}
				if( strlen(to) > 0 && t_count > 0 ){
					ToLower(to);
				}
				if( strlen(header) > 0 && h_count > 0 ){
					ToLower(header);
				}
				if( strlen(body) > 0 && b_count > 0 ){
					ToLower(body);
				}		
            }

			typedef vector<CFilterTokenInfo>::const_iterator CI;
			CFilterTokenInfo oToken;
			int iTokenType = 0;

			for(CI a=all_tokens.begin(); a!=all_tokens.end(); ++a){			
				oToken = *a;
				//
				iTokenType = oToken.getArea(); 
				if( (iTokenType == FROM && f_count > 0) || 
					(iTokenType == KILL_LIST && k_count > 0) )
				{
					iRetVal = ParseTokens(oToken, from);
					if( iRetVal == -1 ){
						iRetVal = ParseTokens(oToken, from_email);
					}
				}
				else if( iTokenType == SUBJECT && s_count > 0 )
				{
					iRetVal = ParseTokens(oToken, subject);					
				}
				else if( iTokenType == SENDER && n_count > 0 )
				{
					iRetVal = ParseTokens(oToken, sender);						
				}
				else if( iTokenType == TO && t_count > 0  )
				{
					iRetVal = ParseTokens(oToken, to);					
				}
				else if( iTokenType == ENTIRE_HEADER && h_count > 0 )
				{
					iRetVal = ParseTokens(oToken, header);						
				}
				else if( iTokenType == ENTIRE_BODY && b_count > 0 )
				{
					iRetVal = ParseTokens(oToken, body);						
				}
				//
				if( iRetVal != -1 ) break;
				//
			}
		}
		else{
			throw MiscError("CompareMessage::Error - File is not loaded or wrong line length.", 14);
		}

		delete [] from;
		delete [] from_email;
		delete [] subject;
		delete [] sender;
		delete [] to;
		delete [] header;
		delete [] body;
	 }
	 catch( MiscError ){

		delete [] from;
		delete [] from_email;
		delete [] subject;
		delete [] sender;
		delete [] to;
		delete [] header;
		delete [] body;

		throw;
	 }
	 catch(...){

		delete [] from;
		delete [] from_email;
		delete [] subject;
		delete [] sender;
		delete [] to;
		delete [] header;
		delete [] body;

		throw MiscError("SearchImaTokens has caught an unknown error.", 15);
	 }
	 return iRetVal;
}

int EmailRipper::LoadImaFile( const char *fileName, int maxRows ){

	bool bError = true;
	try{
		
		if( maxRows <=0 ){
			throw MiscError("maxRows must be greater than 0.", 3);
		}

		validateFileExtension( fileName );

		string line;
		line_count = 0;

		ifstream fin;
		fin.open( fileName, ios::in );

		if(!fin){
			throw FileWillNotOpen();
		}
		else{
			while(getline(fin,line) && line_count <= maxRows) {
				if( line.length() > 0 )
				{					
					parseLine(line, line_count);
					++line_count;
				}
			}		
			fin.close();
		}

		if( line_count > 0 ){
			b_IsLoaded = true;
			bError = false;
		}

	}
	catch( FileWillNotOpen ex ){
		setErrorMessage( ex.what(), 128 );
	}
	catch( MiscError ex ){
		setErrorMessage( ex.what(), ex.number() );
	}
	catch(...){
		setErrorMessage( "LoadFile has caught an unknown error.", 2 );
	}

	if( !bError ){
		return line_count;
	}
	else{
		b_IsLoaded = false;
		return -1;
	}
}

inline void EmailRipper::parseLine(string &line, int &line_num)
{
	register char rule_type[2];
	CFilterTokenInfo oToken;
	register int iType, iCondition;
	iType = 0;
	iCondition = 0;

	try{

		LTrim(line);
		RTrim(&line);
		rule_type[0] = line[0];
		rule_type[1] = '\0';

		if(InstrChar(rule_type, "F") == 0){
		// From
			++f_count;
			iType = FROM;
		}
		else if(InstrChar(rule_type, "S") == 0){
		// Subject
			++s_count;
			iType = SUBJECT;
		}
		else if(InstrChar(rule_type, "N") == 0){
		// Sender
			++n_count;
			iType = SENDER;
		}
		else if(InstrChar(rule_type, "T") == 0){
		// To
			++t_count;
			iType = TO;
		}
		else if(InstrChar(rule_type, "H") == 0){
		// Entire header
			++h_count;
			iType = ENTIRE_HEADER;
		}
		else if(InstrChar(rule_type, "B") == 0){
		// Entire body
			++b_count;
			iType = ENTIRE_BODY;

		}	
		else if(line.find("@") != string::npos){
		// Treat as a regular kill.lst entry. 
			++k_count;
			iType = KILL_LIST;
			iCondition = NONE;
			oToken.ParseLine(line, iType, iCondition, line_num, b_Lowercase);
			all_tokens.push_back(oToken);
		}
		else{
			throw MiscError("parseLine::Invalid Type", 562);
		}
	
		if( iType != KILL_LIST ){
		
			if( line[1] == '~' ){
				iCondition = CONTAINS;	
			}
			else if( line[1] == '!' ){
				if( line[2] == '~' ){
					iCondition = DOES_NOT_CONTAIN;
				}
				else if( line[2] == '=' ){
					iCondition = DOES_NOT_EQUAL;
				}
			}
			else if( line[1] ==  '=' ){
				iCondition = EQUALS;
			}
			
			if( iCondition == CONTAINS || iCondition == EQUALS ){
				oToken.ParseLine(line.substr(2), iType, iCondition, line_num, b_Lowercase);
			}
			else{
				oToken.ParseLine(line.substr(3), iType, iCondition, line_num, b_Lowercase);
			}
			
			all_tokens.push_back(oToken);

		}

	}
	catch(...){
		throw MiscError("An error has occured while in parseLine.", 11);
	}
}

void EmailRipper::validateFileExtension( const char *fileName )
{
	// Add validation here for varifying that the file is a rules.ima 
	// or kill.list (ie extension)
	int iFindIma = InstrChar( fileName, ".ima" );
	int iFindLst = InstrChar( fileName, ".lst" );
	
	if( iFindIma != -1 ){
		if( iFindIma != strlen(fileName) - 4 ){
			throw MiscError("Error - LoadFile. Wrong file extension.", 521);
		}
	}
	else if( iFindLst != -1 ){
		if( iFindLst != strlen(fileName) - 4 ){
			throw MiscError("Error - LoadFile. Wrong file extension.", 522);
		}		
	}
	else{
		throw MiscError("Error - LoadFile. Wrong file extension.", 523);
	}
}

string EmailRipper::EscapeBadChars( string sInput ){

	char * sTempChar = 0;
	string sTempString = "";
		
	try{

		const unsigned long dwAllocTempVar = DetermineNewTokenLineSize( sInput.c_str() );

		sTempChar = new char[dwAllocTempVar];
		strcpy(sTempChar, sInput.c_str());

		if(strlen(sTempChar) > 0)
		{
			RemoveBadChars(sTempChar);
			sTempString = sTempChar;
		}

		delete [] sTempChar;
		
		return sTempString;
	
	}
	catch( MiscError ){
		delete [] sTempChar;
		throw;	
	}
	catch(...){
		delete [] sTempChar;
		throw MiscError( "Function EscapeBadChars has encountered an unknow error.", 144 );
	}
}

unsigned long EmailRipper::DetermineNewTokenLineSize(const char* text)
{
	int count;
	unsigned long total = 0;
    char * sTempChar = 0;

	try{

		try{
			
			sTempChar = new char[strlen(text) + 1];
			sTempChar[strlen(text)] = 0;
			strcpy(sTempChar, text);

			count = FindAllOccurences( sTempChar, "\\" );
			if(count!=-1){
				total = 2 * count;
			}

			count = FindAllOccurences( sTempChar, "{" );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, "}" );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, "(" );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, ")" );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, "|" );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, "*" );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, "+" );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, "," );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, "." );
			if(count!=-1){
				total += 2 * count;
			}

			count = FindAllOccurences( sTempChar, ":" );
			if(count!=-1){
				total += 2 * count;
			}

			total += static_cast< unsigned long >( strlen(sTempChar) );
			delete [] sTempChar;
		}
		catch(...){
			delete [] sTempChar;
			throw MiscError( "Function DetermineNewTokenLineSize has encountered an unknow error.", 146 );
		}

		return DetermineBufferSize(total);

	}
	catch( MiscError ){ 
		delete [] sTempChar;
		throw;
	}
	catch(...){
		delete [] sTempChar;
		throw MiscError( "Function DetermineNewTokenLineSize has encountered an unknow error.", 145 );
	}
}

void EmailRipper::RemoveBadChars(char *sInput)
{
	strreplace( sInput, "\\", "\\\\" );
	strreplace( sInput, "{", "\\{" );
	strreplace( sInput, "}", "\\}" );
	strreplace( sInput, "(" , "\\(" );
	strreplace( sInput, ")" , "\\)" );
	strreplace( sInput, "|", "\\|" );
	strreplace( sInput, "*", "\\*" );
	strreplace( sInput, "+", "\\+" );
	strreplace( sInput, ",", "\\," );
	strreplace( sInput, "." , "\\." );
	strreplace( sInput, ":" , "\\:" );
}