// NSStringFunctions
// v2.0.1.72 2005/11/30
// Previous v1.0.0.2087 03/16/2004
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSStringFunctions.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

// NSStringFunctions constructor
NSStringFunctions::NSStringFunctions()
{ 

}

// NSStringFunctions destructor
// Make sure to delete all global pointers that have been allocated
NSStringFunctions::~NSStringFunctions()
{
}

char * NSStringFunctions::substrL( char *chInput, int iLen ){
	char * chOutput = new char[ iLen + 1 ];
	memcpy( chOutput, chInput, iLen );
	chOutput[ iLen ] = '\0';
	return chOutput;
}

char * NSStringFunctions::substrR( char *s1, const char *s2 ){
	int i = 0;
	while( s1[i] == s2[i] ) ++i;
	return s1 + i;
}

char * NSStringFunctions::substrR( char *chInput, int iStartPos ){
	char *s = chInput + iStartPos;
	size_t iLen = strlen(s);
	char * chOutput = new char[ iLen + 1 ];
	memcpy( chOutput, s, iLen );
	chOutput[ iLen ] = '\0';
	return chOutput;	
}

//**************************************
//     
// Name: strcasecmp() -- to do case in-s
//     ensitive string comparison
// Description:to do case in-sensitive s
//     tring comparison
// By: Donald Bono
//
// Inputs:2 char * pointers
//
// Returns::: { +1 }, { 0 } or { -1 }
//
// Assumes:~
//
// Side Effects:unknown
//
//************************************** 
int NSStringFunctions::strcasecmp(char * l1, char * l2){

    char * t1 = l1;
    char * t2 = l2;
    char c1;
    char c2;

    while ((t1[0] != 0x0) && (t2[0] != 0x0)){

        c1 = toupper (t1[0]); 
        c2 = toupper (t2[0]); 
        if (c1 < c2) { return -1; }
        if (c1 > c2) { return +1; }
        t1++; 
        t2++; 
    } 
    return 0;
}

unsigned long NSStringFunctions::DetermineBufferSize(unsigned long dwAmountOfChars)
{
    unsigned long dwBufferSizes[] = { 16,32,64,128,256,512,1024,2048,4096,8192,16384,
        32768,65536,131072,262144,524288,1048576,dwAmountOfChars + 131072 };
    unsigned long dwIter(0);
    while (dwAmountOfChars >= dwBufferSizes[dwIter])
    {
        ++dwIter;
    }
    return dwBufferSizes[dwIter];
}

int NSStringFunctions::bmsearch( char *text, char *pat, int *no, int *pos )
{
	*no=0;
	int i, table[256];
	register int iPatternLen = static_cast<int>( strlen(pat) );	
	register int iInputLen = static_cast<int>( strlen(text) );
	register int compcount = 0;

	for( i = 0; i < 256; i++ )
		table[i] = iPatternLen;
	
	for( i=0; i < iPatternLen; i++ )
		table[ pat[i] ] = iPatternLen - ( i + 1 );

	register int ptct = iPatternLen - 1;
	
	while( ptct < iInputLen )
	{
		int count=0;
		while( count < iPatternLen )
		{
			if( text[ ptct - count ] != pat[ iPatternLen - 1 - count ] ){
				compcount++;
				break;
			}
			else count++;
		}
		if( count == iPatternLen )
		{
			( *no )++;
			*( pos + ( *no ) -1 ) = ( ptct - count + 1 );
			ptct += iPatternLen;
		}
		else if( text[ ptct - count ] >= 0 && text[ ptct - count ] <= 255 ){
			ptct += table[ text[ ptct - count ] ];
		}
		else{
			ptct += iPatternLen;
		}
	}
	return compcount;
}

// This is a genral purpose function used to convert
// a string to a character style c string. It is
// very similar to c_string() but is used to avoid c_string()
char * NSStringFunctions::c_string( const string &s )
{
	char *p = new char[s.length() + 1];
	s.copy(p, string::npos);
	p[s.length()] = 0;
	return p;
}

int NSStringFunctions::FindAllOccurences( char* source, char* replace )
{
	register int iCount = 0;
	register size_t len_source = strlen(source);
	register size_t len_replace = strlen(replace);

	register size_t pos;
	bool bFound = ifindstr( 0, len_source, len_replace, source, replace, pos );
	if( bFound == false ){
		return -1;
	}
	else{		
		while( bFound != false )
		{
			++iCount;
			bFound = ifindstr( ++pos, len_source, len_replace, source, replace, pos );
		}
	}

	return iCount;
}

int NSStringFunctions::strreplace( char* source, const char* replace, const char* with )
{
	if( InstrChar( source, replace ) == -1 ) return 0; // The replace char[] is not found

	register size_t len_source = strlen(source);
	register size_t len_replace = strlen(replace);	
	register size_t len_with = strlen(with);
	register size_t pos;
	bool bFound = ifindstr( 0, len_source, len_replace, source, replace, pos );
	register int iCount = 0;

	char *left = 0;
	char *right =  0;
	
	try{

		while( bFound == true ) 
		{
			
			left = new char [ pos + 1 ];
			left[ pos ] = 0;

			right = new char [ (len_source - (pos + len_replace)) + 2 ];
			right[ (len_source - (pos + len_replace)) + 1 ] = 0;

			int iLeftLen = leftcopy( source, left, pos );
			int iRightLen = rightcopy( source, len_source, right, pos + len_replace );

			if(iLeftLen > 0)
			{
				strcpy(source,left);
				strcat(source,with);
			}
			else
			{
				strcpy(source,with);
			}
			
			if(iRightLen > 0)
			{
				strcat(source,right);
			}

			delete [] left;
			delete [] right;
			
			len_source = strlen(source);				
			if( len_with > len_replace ){
				pos += ( len_with - len_replace ) + 1;
			}
			else{
				pos += len_with;
			}

			iCount += 1;
			bFound = ifindstr( pos, len_source, len_replace, source, replace, pos );
		}
			
		return iCount;

	}
	catch( MiscError ){
		delete [] left;
		delete [] right;
		throw;
	}
	catch(...){
		delete [] left;
		delete [] right;
		throw MiscError("NSStringFunctions::strreplace has caught an unknow error", 9837);
	}
}

// find string, starting from 
inline bool NSStringFunctions::ifindstr( size_t startx, size_t len_body, size_t len_search, char* body, const char* search, size_t &pos )
{
	
	bool bRetVal = false;
	if( ( len_body <= 0 || len_search <= 0 ) || ( len_search > len_body ) )
	{
		return bRetVal; // failure
	}

	bool bFound;
	for( size_t i = startx; i < len_body; i++ )
	{ 
		if( body[i] == search[0] )
		{	
			if( len_search == 1 ){
				pos = i;
				bRetVal = true;
				break;
			}
			else if( ( i + len_search ) <= len_body ){
				
				bFound = true;
				for( size_t z = 0; z < len_search; z++ ){
					if( body[ i + z ] != search[z] )
					{
						bFound = false;
					}
				}
				if( bFound == true ){
					pos = i; // return the position found
					bRetVal = true;
					break;
				}

			}
			else{
				break;
			}			
		}
	}
	return bRetVal;
}

// left copy
inline int NSStringFunctions::leftcopy( char* input, char* output, size_t pos )
{
    register int index = 0;

	if(pos==0){ 
		return 0;
	}
	else
	{
		for(size_t i = 0; i < pos; i++){
			output[index++] = input[i];
		}
		
		output[index] = 0;	
	}

	return index;
}

// right copy
inline int NSStringFunctions::rightcopy( char* input, size_t input_length, char* output, size_t pos )
{
	register int index = 0;

	for(size_t i = pos; i < input_length; i++){
		output[index++] = input[i];
	}

	output[index] = 0;	

	return index;    
}

// LTrim is used to trim the left spaces off a string
void NSStringFunctions::LTrim(string &str) //string to trim
{
	try{
	
		int i = static_cast<int>(str.at(0));
		//empty strings are bad
		if( str.empty() || i < 0 || i > 255 || !isspace(str[0]) ) return;
		
		//remove left whitespace
		string::iterator it=str.begin();
		for(;it!=str.end() && isspace(*it);it++);
		str.erase(str.begin(),it);

	}
	catch(...){
	
	}
}

// LTrim is used to trim the left spaces off a character array 
void NSStringFunctions::LTrim(char *t)
{

	if ( !strlen(t) || !isspace(t[0]) || strlen(t) > 1023 ) return;

	char tmp[1024];				//Temp Char VAR
	char *buf;					//Pointer

	strcpy(tmp,t);
	tmp[1023] = 0;
	buf=tmp;

	////while (*buf == ' ' && strlen(buf))	*buf++;
	while ( isspace(*buf) && strlen(buf))	*buf++;

	strcpy(t,buf);
}

// This function makes the passed in character array all lowercase.
void NSStringFunctions::ToLower(char *chString)
{
	register int Chr;
	register int Diff = 'a' - 'A'; //this is the integer difference between the two cases.

	for( int j = 0; chString[j] != '\0'; j++ ){
		if( chString[j] >= 65 && chString[j] <= 90 ){
			Chr = chString[j] + Diff;
			chString[j] = Chr;				
		}
	}
}

// This function makes the passed in character array all lowercase.
void NSStringFunctions::ToLower(string &sInput)
{
	register int Chr;
	register int Diff = 'a' - 'A'; //this is the integer difference between the two cases.

	basic_string <char>::iterator str_Iter;
	str_Iter = sInput.begin();

	while( str_Iter != sInput.end()) {
	
		if( *str_Iter >= 65 && *str_Iter <= 90 ){
			Chr = *str_Iter + Diff;
			*str_Iter = Chr;	
		}
		str_Iter++;
	}
}

std::string NSStringFunctions::ToUpper(const std::string& s) 
{
  std::string upper(s);
  for(size_t i=0; i<s.length(); i++)
    upper[i] = toupper(s.at(i));
  return upper;
}

char * NSStringFunctions::ToUpper( const char * chInput ){
	size_t iLen = strlen(chInput);
	char * chOutput = new char[ iLen + 1 ];
	chOutput[ iLen ] = '\0';
	for( size_t i = 0; i < iLen; i++ )
		chOutput[ i ] = toupper( chInput[i] );
	return chOutput;
}

char * NSStringFunctions::strrev(char *str)
{
    char *p1, *p2;
    if (! str || ! *str)
    return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2){
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

// This function is used to search a char array for another char array.
// It will return the index or position of the first search token if found
// If no search string is found, the function will return -1
int NSStringFunctions::InstrChar( const char * input, const char * search )
{
	const char * pdest = strstr( input, search );
	if ( pdest != NULL ){
		return (int)(pdest - input);
	}
	else{
		return -1;
	}
}

// This function is used to search a char array for another char array.
// It will return the index or position of the first search token if found
// If no search string is found, the function will return -1
int NSStringFunctions::InstrChar( const char * input, char search )
{
	const char * pdest = strchr( input, search );
	if ( pdest != NULL ){
		return (int)(pdest - input);
	}
	else{
		return -1;
	}
}

// This function will trim the ascii characters 32, 10 and 13
// from the end of an inputed character string. This includes
void NSStringFunctions::RTrimCrLfSpace(char *t)
{
	char *buf;

	if (!strlen(t)) return;

	buf = t;
	buf += strlen(t) -1;

	while ( (*buf == 32 || *buf == 10 || *buf == 13  ) && strlen(t) ){
		*buf-- = '\0';
	}
}

void NSStringFunctions::RTrimCrLfSpace(string &str){
		
	size_t pos = str.length();
	if( pos == 0 ) return;

	char chSpace = 32;
	char chCr = 13;
	char chLf = 10;

	for(--pos; str[pos] == chSpace || str[pos] == chCr || str[pos] == chLf; --pos)
		;
	str.erase(++pos, str.length() - pos);
}

//void NSStringFunctions::LTrim(string &str) //string to trim
//{
//	trimLeft( str, 32 );
//}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//| Function:	RTrim()
//|					Overload Function
//| In:			string stString
//|					In String
//| Return:		None
//| Notes:		This Function Trims Spaces From The End Of A String
//| Example:	string p = "TTTT    ";
//|				RTrim(p);			//Should Chang p To "TTTT"
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void NSStringFunctions::RTrim(string *stString)
{
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
	register size_t i;							//Count VAR
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	if (!stString->length())					//If There Is No Length
		return;

	i = stString->length()-1;					//Set The Counter To The String Length -1

	while (stString->substr(i,1) == " ")		//While There Is Still " "
	{
		if( i == 0 && stString->substr(i,1) == " " ){
			*stString = "";
			break;
		}
		*stString = stString->substr(0, i);		//Remove Off The End Of The String
		i--;									//Advance The Counter
	}

}

void NSStringFunctions::RTrim(string &str) //string to trim
{
	trimRight( str, 32 );
}

// trim the left character of a given string
inline void NSStringFunctions::trimLeft(string &str, char c) {
	int pos;
	for(pos = 0; str[pos] == c; ++pos)
		;
	str.erase( 0, pos );
}

// trim the right character of a given string
inline void NSStringFunctions::trimRight(string &str, char c) {
	size_t pos = str.length();
	if( pos == 0 ) return;
	for(--pos; str[pos] == c; --pos)
		;
	str.erase(++pos, str.length() - pos);
}

char* NSStringFunctions::LeftTrim (char* const szTrim)
{
    	long lSpaceCount;
    	for (lSpaceCount = 0; *(szTrim + lSpaceCount) == ' '; ++lSpaceCount);
    	long lNextChar;
    	for (lNextChar = 0; *(szTrim + lSpaceCount + lNextChar - 1) != '\0';
    		*(szTrim + lNextChar) = *(szTrim + lNextChar + lSpaceCount), ++lNextChar);
    	return szTrim;
}

char* NSStringFunctions::RightTrim (char* const szTrim)
{
    long lSpaceCount;    	
    for (lSpaceCount = static_cast<long>(strlen (szTrim)) - 1; *(szTrim + lSpaceCount) == ' '; --lSpaceCount);
    *(szTrim + lSpaceCount + 1) = '\0';
    return szTrim;
}

int NSStringFunctions::ConvertCharToInt(const char * input)
{
	return ConvertToInt( input, static_cast<int>(strlen(input)) );
}

int NSStringFunctions::ConvertStringToInt( string &input )
{
	return ConvertToInt( input, static_cast<int>(input.length()) );
}

template < class T >
inline int NSStringFunctions::ConvertToInt( T input, int iLength ){

	register int iPos = iLength - 1;
	register int iLoop, iValue = 0;
	for( iLoop=0; iLoop < iLength; iLoop++ ){

		switch( input[iLoop] ){
		
			case '0':
			iValue += 0 * pow( 10, iPos ); 
			break;
		
			case '1':
            iValue += 1 * pow( 10, iPos );
			break;

			case '2':
            iValue += 2 * pow( 10, iPos );
			break;

			case '3':
            iValue += 3 * pow( 10, iPos );
			break;

			case '4':
            iValue += 4 * pow( 10, iPos );
			break;

			case '5':
            iValue += 5 * pow( 10, iPos );
			break;

			case '6':
            iValue += 6 * pow( 10, iPos );
			break;

			case '7':
            iValue += 7 * pow( 10, iPos );
			break;

			case '8':
            iValue += 8 * pow( 10, iPos );
			break;

			case '9':
            iValue += 9 * pow( 10, iPos );
			break;

			default:
				return - 1;
		}
		--iPos;
	}
	return iValue;
}

inline int NSStringFunctions::pow(int x, int y){
	int iLoop;
	int iValue = x;
	if(y==0){
		iValue = 1;
	}
	else if(y==1){
		iValue = x;
	}
	else if(y>1){
		for(iLoop=1;iLoop<y;iLoop++)
			iValue *= x;
	}
	return iValue;
}

// Description: returns an integer as a 
//     string (e.g. int 56 -> string "56")
// Pre: none
// Post: none
string NSStringFunctions::intToString(int num){
    const int ASCII_INT_OFFSET = 48; // the offset for converting int values to
    // chars numbers using the corresponding
    //     
    // ascii values
    // (e.g. int 1 -> char '1', int 4 -&g
    //     t; char '4')
    const int NUMBER_BASE = 10; // used for conversion of integers to strings
    string sNumber; // number as a string
    bool negative = false; // number will be treated as positive, so it must
    // be remembered that it was negative
    if (num < 0){

        negative = true;
        num *= -1; // hard -1 is for negation
    }
    // store right-most digit of num in stri
    //     ng (units digit)
    sNumber = char((num % NUMBER_BASE) + ASCII_INT_OFFSET);
    while (num >= NUMBER_BASE){

        // get rid of right most digit (subtract
        //     it, then divide by number base)
        num = (num - (num % NUMBER_BASE)) / NUMBER_BASE;
        // insert next digit in string
        sNumber = char((num % NUMBER_BASE) + ASCII_INT_OFFSET) + sNumber;
    }
    if (negative){

        sNumber = "-" + sNumber;
    }
    return sNumber;
}

// This function will turn a string into a vector based on 
// the passed in delimiters. By default a space is a delimiter.
void NSStringFunctions::Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ")
{
	// http://linuxselfhelp.com/HOWTO/C++Programming-HOWTO-7.html
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

/*********************************************************************
 *
 * Function    :  strclean
 *
 * Description :  In-Situ-Eliminate all occurances of substring in
 *                string
 *
 * Parameters  :
 *          1  :  string = string to clean
 *          2  :  substring = substring to eliminate
 *
 * Returns     :  Number of eliminations
 *
 *********************************************************************/
int NSStringFunctions::strclean(char *string, const char *substring)
{
   int hits = 0;
   size_t len = strlen(substring);
   char *pos, *p;

   while((pos = strstr(string, substring)) != NULL)
   {
      p = pos + len;
      do
      {
         *(p - len) = *p;
      }
      while (*p++ != '\0');

      hits++;
   }

   return(hits);
}

/*********************************************************************
 *
 * Function    :  xtoi
 *
 * Description :  Hex string to integer conversion.
 *
 * Parameters  :
 *          1  :  s = a 2 digit hex string (e.g. "1f").  Only the
 *                    first two characters will be looked at.
 *
 * Returns     :  The integer value, or 0 for non-hex strings.
 *
 *********************************************************************/
int NSStringFunctions::xtoi(const char *s)
{
   int d1, d2;

   d1 = xdtoi(*s);
   if(d1 >= 0)
   {
      d2 = xdtoi(*(s+1));
      if(d2 >= 0)
      {
         return (d1 << 4) + d2;
      }
   }

   return 0;
}

/*********************************************************************
 *
 * Function    :  xdtoi
 *
 * Description :  Converts a single hex digit to an integer.
 *
 * Parameters  :
 *          1  :  d = in the range of ['0'..'9', 'A'..'F', 'a'..'f']
 *
 * Returns     :  The integer value, or -1 for non-hex characters.
 *
 *********************************************************************/
int NSStringFunctions::xdtoi(const int d)
{
   if ((d >= '0') && (d <= '9'))
   {
      return(d - '0');
   }
   else if ((d >= 'a') && (d <= 'f')) 
   {
      return(d - 'a' + 10);
   }
   else if ((d >= 'A') && (d <= 'F'))
   {
      return(d - 'A' + 10);
   }
   else
   {
      return(-1);
   }
}

string NSStringFunctions::int2hex(unsigned int n)
{
    if(n == 0)
        return "0";
    static char tb[] = { 
        '0', '1', '2', '3', 
        '4', '5', '6', '7', 
        '8', '9', 'a', 'b', 
        'c', 'd', 'e', 'f'
    };
    size_t sz = sizeof(n), zeros = 0; 
    string r;
    char cp;
    for(size_t i = 0; i < sz*2; ++i)
    {
        cp = static_cast<char>( (n >> (i*4)) & 0xF );
        if(cp == 0)
            zeros++;
        else {
            if(zeros)
                r.insert((string::size_type)0, zeros, '0');
            zeros = 0;
            r.insert((string::size_type)0, 1, tb[cp]);
        }
    }
    return r;
}