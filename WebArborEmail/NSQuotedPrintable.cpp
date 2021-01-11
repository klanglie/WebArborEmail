// NSQuotedPrintable.cpp
// v2.0.1.72 2005/11/30
// Updated 10/5/2005
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSQuotedPrintable.h"
// http://rfc-2045.rfclist.net/rfc-2045-23.htm

using namespace WEBARBOR_EMAIL_COMPONENTS;

const char crLf[3] = { 13, 10, 0 };

////---------------------------------------------------------------------------
//#include<iostream> // for debugging purposes only!
//using std::cout;
//#include<fstream>
//// write to a file
//void writeFile( string &output, const char * file_path )
//{
//	ofstream outClientFile( file_path, ios::out );
//
//	if ( !outClientFile ){
////		cerr << "File could not be opened" << endl;
//		exit(1);
//	}
//	
//	outClientFile << output << endl;
//	
//	outClientFile.close();
//}
////---------------------------------------------------------------------------

// NSQuotedPrintable destructor
// Make sure to delete all global pointers that have been allocated
NSQuotedPrintable::NSQuotedPrintable()
{
}

// NSQuotedPrintable constructor. Note that this class must be 
// constructed in the following manner only:
NSQuotedPrintable::NSQuotedPrintable( const string& inVar ) : sQPVar( inVar )
{ 
	
}

// NSQuotedPrintable destructor
// Make sure to delete all global pointers that have been allocated
NSQuotedPrintable::~NSQuotedPrintable()
{
}

// This function starts the whole quoted-printable decoding engine.
// The input is passed into the class constructor and is copied
// The resulting output, passed in byref by the variable sQPVar 
// Hopefully now the message will now be fully qp decoded and looking
// like how the author intended it to look.
string & NSQuotedPrintable::DecodeQP(bool bHtml)
{

	char * chQP = NULL;
	char * chQPOut = NULL;

	try{

		if( sQPVar.length() > 0 )
		{
			
			RemoveQPLineBreaks();
			//writeFile(sQPVar, "RemoveQPLineBreaks.txt");
			
			ChangeHttpVar( true );	
			//writeFile(sQPVar, "_ChangeHttpVar_True.txt");
			
			chQP = c_string( sQPVar );				
			chQPOut = new char[ strlen(chQP) + 1 ];
			chQPOut[ strlen(chQP) ] = '\0';

			DecodeQuotedPrintable( chQP, chQPOut );			
			sQPVar = chQPOut;
			
			delete [] chQPOut;
			delete [] chQP;
			//writeFile(sQPVar, "_DecodeQuotedPrintable.txt");

			if( bHtml == true ){
				size_t iFind1 = sQPVar.find("<html");
				if( iFind1 != string::npos && iFind1 != 0 ){
					sQPVar = sQPVar.substr( iFind1 );
					RTrimCrLfSpace(sQPVar);
				}
				else{
					size_t iFind2 = sQPVar.find("<HTML");
					if( iFind2 != string::npos && iFind2 != 0 ){
						sQPVar = sQPVar.substr( iFind2 );
						RTrimCrLfSpace(sQPVar);
					}
				}			
			}

			ChangeHttpVar( false );
			//writeFile(sQPVar, "_ChangeHttpVar_False.txt");

			// 4/15/2005. In response to: The Rush Backstage Club message
			if( sQPVar.find("’") != string::npos ){
				chQP = c_string( sQPVar );
				strreplace(chQP, "’", "'");
				sQPVar = chQP;
				delete [] chQP;
			}

		}
		
		return sQPVar;

	}
	catch( MiscError ){
		delete [] chQP;
		delete [] chQPOut;
		throw;
	}
	catch(...){
		delete [] chQP;
		delete [] chQPOut;
		throw MiscError("NSQuotedPrintable::DecodeQP has encountered an unknown error", 9352);
	}
}

// RemoveQPLineBreaks is used to remove the equals sign from the
// end of a line of text and concatonate the following line to the first line. 
// The input is searched by line breaks and each line represents
// a token that is either appended to with a line break or has an equals
// sign which means that the following line is appended to it the previous line.
inline void NSQuotedPrintable::RemoveQPLineBreaks()
{

	string sLine;
	string sbTemp = "";
	register size_t iLen, iPos;
	
	try{

		register string::size_type lastPos = sQPVar.find_first_not_of("\n", 0);
		// Find first "non-delimiter".
		register string::size_type pos = sQPVar.find_first_of("\n", lastPos);

		while (string::npos != pos || string::npos != lastPos)
		{
			// Found a token
			sLine = sQPVar.substr(lastPos, pos - lastPos - 1);
			iLen = sLine.length();

			if( iLen == 0 ){
				sbTemp.append( crLf );
			}
			else{

				// Each line should be 78 chars long or less, but some implementations encode their messages improperly!
				if ( iLen > 2 && sLine.at(0) == '.' && sLine.at(1) == '.' )
				{
					sLine = sLine.substr(1);
					iLen -= 1;
				}
								
				if( sLine.at( (iLen - 1) ) == '=' ){										
					iPos = sLine.find("=0A=");
					if( iPos != string::npos && iPos == (iLen - 4) ){
						sLine.erase( (iLen - 4), 4 );						
						sbTemp.append( sLine );
						sbTemp.append( crLf );
					}
					else{
						sLine.erase( (iLen - 1), 1 );
						sbTemp.append( sLine );
					}
				}
				else{
					sbTemp.append( sLine );
					sbTemp.append( crLf );
				}

			}

			// Skip delimiters.  Note the "not_of"
			lastPos = sQPVar.find_first_not_of("\n", pos);
			// Find next "non-delimiter"
			pos = sQPVar.find_first_of("\n", lastPos);
		}
			
		sQPVar = sbTemp;

	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError("NSQuotedPrintable::RemoveQPLineBreaks has encountered an unknown error", 2646);
	}
}

// ChangeHttpVar will change the equals sign in an input string's http:// links
// in a message to the ascii character 1 or vice versa. This is done so that the QP engine
// does not decode a token that it finds in a link that it shouldn't be decoding. 
// There are however a few  commonly found QP symbols in a link that do need converting. 
// These special cases if found are removed using the strreplace function. Thses special 
// characters include but might not be limited to =3D(=), =2E(.), and =22(\).
inline void NSQuotedPrintable::ChangeHttpVar( bool bStart )
{

	if( sQPVar.find("http://") == string::npos ){
		return;
	}
	else if( bStart == false && sQPVar.find_first_of(5) == string::npos ){
		return;
	}

	string sbTemp = "";
	string sLine;
	char *str = 0;

	register basic_string <char>::size_type idxStart, idxFind, iLength;
	register basic_string <char>::size_type idxSpace, idxQuote, idxBracket, idxQP20;
	register bool bStartQuote;

	register bool bFind = false;	
	register const char * c4 = "=";	

	try{

		register string::size_type lastPos = sQPVar.find_first_not_of("\n", 0);
		// Find first "non-delimiter".
		register string::size_type pos = sQPVar.find_first_of("\n", lastPos);
		
		while ( string::npos != pos || string::npos != lastPos )
		{
			// Found a token
			sLine = sQPVar.substr(lastPos, pos - lastPos - 1);

			idxStart = 0;
			idxFind = sLine.find_first_of( 'h', idxStart );
			while( idxFind != string::npos ){
				
				
				iLength = 0;
				if( sLine.find_first_of( 't', idxFind + 1 ) == idxFind + 1  &&
					sLine.find_first_of( 't', idxFind + 2 ) == idxFind + 2  &&
					sLine.find_first_of( 'p', idxFind + 3 ) == idxFind + 3  && 
					sLine.find_first_of( ':', idxFind + 4 ) == idxFind + 4  &&
					sLine.find_first_of( '/', idxFind + 5 ) == idxFind + 5  &&
					sLine.find_first_of( '/', idxFind + 6 ) == idxFind + 6  &&
					sLine.find_first_of(  c4, idxFind + 7 ) != string::npos )
				{

					if( idxFind > 0 ){
						if( sLine.at(idxFind-1) == '"'){
							bStartQuote = true;
						}
						else{
							bStartQuote = false;
						}
					}	
					else{
						bStartQuote = false;
					}

					idxSpace = sLine.find_first_of( 32, idxFind );
					idxQuote = sLine.find_first_of( '"', idxFind );
					idxBracket = sLine.find_first_of( '>', idxFind );
					
					if( idxSpace != string::npos ){
						
						if( idxQuote != string::npos && idxQuote < idxSpace && bStartQuote ){
							iLength = idxQuote - idxFind;
						}
						else if( ( idxBracket != string::npos && idxBracket < idxSpace ) ){
							iLength = idxBracket - idxFind;
						}
						else{
							idxQP20 = FindQPCharsInString(sLine, idxFind, '2', '0');
							if( idxQP20 != string::npos && idxQP20 < idxSpace ){
								iLength = idxQP20 - idxFind;
							}
							else{
								iLength = idxSpace - idxFind; // This is the default case with a space!
							}	
						}
					}
					else{

						if( idxQuote != string::npos && bStartQuote ){
							iLength = idxQuote - idxFind;
						}
						else if( idxBracket != string::npos ){
							iLength = idxBracket - idxFind;
						}
						else{
							idxQP20 = FindQPCharsInString(sLine, idxFind, '2', '0');
							if( idxQP20 != string::npos  ){
								iLength = idxQP20 - idxFind;
							}
							else{
								iLength = sLine.length() - idxFind; // This is the default case with no space!
							}	
						}

					}

					if ( iLength > 0 ){
					
						// Change all equals signs to a smily face character.
						str = new char[ iLength + 1 ];
						str[ iLength ] = 0;
						strcpy( str, sLine.substr( idxFind, iLength ).c_str() );						

						if( bStart == true ){

							register basic_string <char>::size_type i, j, k, iLen;
							iLen = strlen(str) - 2;
							for( i = 0; i < iLen; i++ ){
								if( *( str + i ) == '=' ){
									j = i + 1;
									k = i + 2;	
									if( !isdigit(*( str + j )) && !isalpha(*( str + k )) ){
										if( (idxQuote == string::npos) && *( str + j ) == '2' && *( str + k ) == '2'  ){
											// do nothing
										}
										else{
											*( str + i ) = 5;
											bFind = true;
										}
									}
								}
							}						
						}
						else{
							char c3[2] = { 5, 0};
							if( strreplace( str, c3, c4 ) > 0 ) bFind = true;
						}

						if( bFind == true ){
							sLine.erase( idxFind, iLength );
							sLine.insert( idxFind, str, iLength );							
							bFind = false;
						}

						delete [] str;

					} 

				}
				
				if( iLength == 0 ){
					idxStart = idxFind + 1;
				}
				else{
					idxStart = idxFind + iLength; 
				}

				idxFind = sLine.find_first_of( 'h', idxStart );

			}
		
			sbTemp.append( sLine );
			sbTemp.append( crLf );

			// Skip delimiters.  Note the "not_of"
			lastPos = sQPVar.find_first_not_of("\n", pos);
			// Find next "non-delimiter"
			pos = sQPVar.find_first_of("\n", lastPos);
		}

		sQPVar = sbTemp;

	}
	catch( MiscError ){
		delete [] str;
		throw;
	}
	catch(...){
		delete [] str;
		throw MiscError("NSQuotedPrintable::ChangeHttpVar has encountered an unknown error", 3728);
	}
}

void NSQuotedPrintable::DecodeQuotedPrintable( char * chQPIn, char * chQPOut ){

	register basic_string <char>::size_type i, j, k, iLen, iSkip, iCount, iPos;

	iLen = strlen(chQPIn);
	iSkip = 2;
	iCount = 0;
	iPos = 0;
	
	try{

		for( i = 0; i < iLen; i++ ){
			
			if( *( chQPIn + i ) == '=' && (i < iLen - 2) ){
				j = i + 1;
				k = i + 2;
				if( ( *( chQPIn + j ) >= 48 && *( chQPIn + j ) <= 57  ||
					*( chQPIn + j ) >= 65 && *( chQPIn + j ) <= 70  ||
					*( chQPIn + j ) >= 97 && *( chQPIn + j ) <= 102 ) &&
					( *( chQPIn + k ) >= 48 && *( chQPIn + k ) <= 57 ||
					*( chQPIn + k ) >= 65 && *( chQPIn + k ) <= 70 ||
					*( chQPIn + k ) >= 97 && *( chQPIn + k ) <= 102 ) ){				

						//=B9 equals an apostrophe and can look funny in NSPOP.
						if( *( chQPIn + j ) == 66 && *( chQPIn + k ) == 57 ){
							chQPOut[iPos++] = '\'';
						}
						else{
							chQPOut[iPos++] = static_cast<char>(HexToChar( ( chQPIn + j ), ( chQPIn + k ) )); 
						}
						
						iCount += 2;
						iSkip = 0;
				
					}
				else if( iSkip < 2 ){
					++iSkip; 
				}
				else{
					chQPOut[iPos++] = *( chQPIn + i ); 
				}
			}
			else if( iSkip < 2 ){
				++iSkip; 
			}
			else{
				chQPOut[iPos++] = *( chQPIn + i ); 	
			}
		}
		
		if( iCount > 0 ){
			chQPOut[ iLen - iCount ] = '\0';				  
		}

	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError("NSQuotedPrintable::DecodeQuotedPrintable has encountered an unknown error", 39482);
	}
}

// This function returns an ascii integer representing 
// the decoded character from the two chars that are passed in.
// See RFC's on Quoted-Printable for detail.
inline int NSQuotedPrintable::HexToChar( char *chLeft, char *chRight) 
{
	register char chTemp;
	register int iResult, iTotal;

	for( short i = 1; i < 3; i++ )
	{
		if( i == 1 ){
			chTemp = *chRight;
		}
		else{
			chTemp = *chLeft;
		}	

		switch(chTemp){
		
			case '0':
			iResult = 0;
			break;
			
			case '1':
			iResult = 1;
			break;
		
			case '2':
			iResult = 2;
			break;
			
			case '3':
			iResult = 3;
			break;
		
			case '4':
			iResult = 4;
			break;
			
			case '5':
			iResult = 5;
			break;
		
			case '6':
			iResult = 6;
			break;
			
			case '7':
			iResult = 7;
			break;
		
			case '8':
			iResult = 8;
			break;
			
			case '9':
			iResult = 9;
			break;
		
			case 'a':
			case 'A':
			iResult = 10;
			break;
			
			case 'b':
			case 'B':
			iResult = 11;
			break;
		
			case 'c':
			case 'C':
			iResult = 12;
			break;
			
			case 'd':
			case 'D':
			iResult = 13;
			break;
		
			case 'e':
			case 'E':
			iResult = 14;
			break;

			case 'f': 
			case 'F':
			iResult = 15;
			break;
			
		}

        if( i == 1 ){
            iTotal = iResult;
		}
		else{
            iTotal += ( iResult * 16 );
		}

	}

	return iTotal;

}

inline size_t NSQuotedPrintable::FindQPCharsInString( string& sIn, size_t iBeginPos, const char & c1, const char &  c2)
{
	basic_string <char>::iterator str_Iter;
	str_Iter = sIn.begin();
	size_t i = 0;
	size_t idxEnd = string::npos;

	while( str_Iter != sIn.end()) {
	
		if( i > iBeginPos){
			if( *str_Iter == '=' && *(str_Iter + 1) == c1 && *(str_Iter + 2) == c2){

				idxEnd = i;
				break;
			}
		}
		str_Iter++;
		i++;
	}

	return idxEnd;
}


void NSQuotedPrintable::DecodeQP(const char *input_file_name)
{
	char * a = 0;
	try{
		// This code is for binary attachments only! Not text/html
		// They must be dealt with using the other way.
		a = c_string( sQPVar );
		DecodeBinaryAttachment( input_file_name, a );
		delete [] a;
	}
	catch( MiscError ){
		delete [] a;
		throw;
	}
	catch(...){
		delete [] a;
		throw MiscError("NSQuotedPrintable::DecodeQP(2) has encountered an unknown error", 26413);
	}
}

inline void NSQuotedPrintable::DecodeBinaryAttachment( const char *file_name, const char *input ){

	register enum state{
		NONE,
		EQUALS,
		QUOTED_PRINTABLE
	} Status;
	
	Status = NONE;
	register char c_qp1 = 0;
	register char c = 0;
	//register const char crLf[3] = { 13, 10, 0 } ;
	ofstream fout;

	try{

		fout.open( file_name, ios_base::binary );
		if( !fout.is_open() ){
			throw MiscError( "NSQuotedPrintable::DecodeBinaryAttachment was unable to open an output file", 20693 );
			////cerr << "Error: could not poen file" << endl;
			////exit(1);
		}

		while(*input){
			c = *input; 
			switch(c){
				
				case '=':
					Status = EQUALS;
					break;

				case 13: // carriage return
					if( Status != EQUALS ){
						fout << crLf;
					}
					break;
	            	
				case 10: // line feed
					Status = NONE;
					break;

				default:

					if( Status == EQUALS && IsQPChar(&c) == true ){
						c_qp1 = c;
						Status = QUOTED_PRINTABLE;
					}
					else if( Status == QUOTED_PRINTABLE && IsQPChar(&c) == true ){
						fout << static_cast<unsigned char>(HexToChar(&c_qp1, &c));				
						Status = NONE;
					}
					else if(Status == NONE){
						fout << c;			
					}
					break;
			}		
			input++;
		}

		fout.close();

	}
	catch( MiscError ){		
		throw;
	}
	catch(...){
		throw MiscError("NSQuotedPrintable::DecodeBinaryAttachment has encountered an unknown error", 20694);
	}

}

inline bool NSQuotedPrintable::IsQPChar(char *c){

	if( (*c >= 48 && *c <= 57) || (*c >= 65 && *c <= 70) ){
		return true;
	}
	else{
		return false;
	}
}
