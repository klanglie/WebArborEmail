// AMMimeUtils
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "AMMimeUtils.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

string AMMimeUtils::MimeDecodeGenericMailField(char *s)
{
	//filename="=?iso-8859-1?Q?Italien_nov_2003_Lennart_h=E5ller_om_Fabio_och_Massimo_0?=
	//=?iso-8859-1?Q?06=5F3A_kopiera.jpg?="

	if (s == NULL || strlen(s) == 0 ) return s;

	const char *iso_8859_1 = "=?iso-8859-1";
	const char *iso_8859_1_end_token = "?=";
    const char *qp = "?q?";
	const char *b64 = "?b?";

	int iStartVal;
	int iEndVal;
	int iAdd;
	string sChar;
	string sWord;	
	vector<string>content;
	int iAdvance = 0;
	int j;

	char *s1 = s;
	char *chLower = 0;

	NSStringFunctions oString;

	try{

		//remove starting spaces...
		while (*s1 == ' ') s1++;

		//remove trailing spaces and cr + lf
		oString.RTrimCrLfSpace(s1);	

		chLower = new char[strlen(s1) + 1];
		strcpy(chLower, s1);
		chLower[strlen(s1)] = 0;
		oString.ToLower(chLower);

		iStartVal = oString.InstrChar(chLower, iso_8859_1);
		iEndVal = oString.InstrChar(chLower, iso_8859_1_end_token);
		
		// Add + 3 to make suret that the word that is encoded is at least 3 chars long.
		while( iStartVal >= 0 && iEndVal > (static_cast<int>(strlen(iso_8859_1)) + iStartVal + 3) ){

			if(iStartVal > 0){
				for( j = 0; j <= iStartVal; j++ ){
				// Advance these strings to the beginning of the iStartVal position.
				s1++;
				chLower++;
				++iAdvance;
				}
			}
			for( j=iStartVal; j < static_cast<int>(strlen(iso_8859_1)); j++ ){
				// Advance these strings to the string after iso-8859-1
				s1++;
				chLower++;
				++iAdvance;
			}
			
			// Re-calculate the end value. It is based on the beginning string position.
			int iEndVal = oString.InstrChar( chLower, iso_8859_1_end_token );
			for( j=0; j < iEndVal; j++ ){
				sChar = *(s1 + j);
				sWord.append( sChar );	
			}

			char * chTemp = new char[sWord.length() + 1];
			strcpy(chTemp, sWord.c_str());
			chTemp[sWord.length()] = 0;
			oString.strreplace(chTemp, "_", " ");
			content.push_back(chTemp);
			delete [] chTemp;

			sWord = "";
			
			iAdd = 0;
			if( (iEndVal + strlen(iso_8859_1_end_token) + 1 ) <  strlen(s1) ){
				iAdd = 1;
			}

			// Advance to the next / end position.
			for( j=0; j < (iEndVal + static_cast<int>(strlen(iso_8859_1_end_token)) + iAdd); j++ ){
				s1++;
				chLower++;
				++iAdvance;
			}

			iStartVal = oString.InstrChar(chLower,"=?iso-8859-1");
			iEndVal = oString.InstrChar(chLower, "?=");

		}
		
		string sLower, sOutput = "";
		typedef vector<string>::const_iterator VI;
		for(VI p=content.begin(); p!=content.end(); ++p){
			sWord = *p;
			sLower = sWord;
			oString.ToLower(sLower);
			iStartVal = static_cast<int>(sLower.find(qp));
			iAdd = static_cast<int>(sLower.find(b64));
			if( iStartVal == 0 || iAdd == 0){

				sWord = sWord.substr(strlen(qp));
				char *decodedText = 0;
				char * s3 = 0;
				if( iStartVal == 0 ){
					
					CQPUtils qp;
					
					s3 = oString.c_string(sWord);
					decodedText = qp.Decode(s3);

					sOutput += decodedText;
				}	
				else if( iAdd == 0 ){

					//string sB64temp;
					//decode( sWord, sB64temp );
					//sOutput += sB64temp;

					////////////////////////////////////
					CBase64Utils bu;
					
					s3 = oString.c_string(sWord);
					int iLen = static_cast<int>(strlen(s3));
					decodedText = bu.Decode(s3, &iLen);
					sOutput += decodedText;						
					
				}	
				delete [] s3;
				delete [] decodedText;
			}
		}
		content.clear();

		if(iAdvance > 0){
			// rewind the string positions.
			for(int h=0; h<iAdvance; h++){
			s1--;
			chLower--;
			}
		}

		delete [] chLower;

		// here is where the return value goes!
		return sOutput;

	}
	catch( MiscError ){
		if(iAdvance > 0){
			// rewind the string positions.
			for(int h=0; h<iAdvance; h++){
			chLower--;
			s1--;
			}
		}
		delete [] chLower;
		throw;
	}
	catch( ... ){
		if(iAdvance > 0){
			// rewind the string positions.
			for(int h=0; h<iAdvance; h++){
			chLower--;
			s1--;
			}
		}
		delete [] chLower;
		throw MiscError("An unknown error has occurred in AMMimeUtils.", 3294);
	}

}

void AMMimeUtils::DecodeIS088591QP( char * chInput ){

	// Example
	//=?ISO-8859-1?Q?ViewPoint_=96_News_from_ViewSonic?=

	const char *ISO_8859_1 = "=?ISO-8859-1"; //?q?"   ?B?;
	const char *ISO_Q = "?Q?";

	NSStringFunctions oString;
	char * chUpper = oString.ToUpper( chInput );

	int iPos1 = oString.InstrChar( chUpper, ISO_8859_1 );
	int iPos2 = oString.InstrChar( chUpper, ISO_Q );

	if( (iPos1 >= 0) && (iPos2>iPos1) ){
		char * chTemp = chInput + iPos2 + 3;
		NSQuotedPrintable oQP;		
		oString.strclean( chTemp, "?=" );	
		// Funky qp decoding char...
		oString.strreplace( chTemp, "=96", "-" );

		char * chQPOut = new char[ strlen(chTemp) + 1 ];
		chQPOut[ strlen(chTemp) ] = '\0';
		oQP.DecodeQuotedPrintable( chTemp, chQPOut );

		oString.strreplace( chQPOut, "_", " " );
		strcpy( chInput, chQPOut );

		delete [] chQPOut;
	}

	delete [] chUpper;

}

////////// The "real" decoding stuff //////////

#define strupos(x, y) (strustr(x, y) != NULL ? strustr(x, y) - x : -1) //char version
char* AMMimeUtils::strustr(char *source, char *s)
{

	char *csource=new char[strlen(source)+1];
  if(csource == 0)
    return 0;
	char *cs = new char[strlen(s)+1];
  if(cs==0)
  {
    delete [] csource;
    return 0;
  }
  strcpy(csource, source);
  strcpy(cs, s);
	strupr(csource);
	strupr(cs);
	char *result = strstr(csource, cs);
	if (result != NULL)
	{
		int pos =static_cast<int>(result - csource);
		result = source;
		result += pos;
	}
	delete [] csource;
	delete [] cs;
	return result;
}
/*!
 *Unique instance of this class
 */
CBase64Utils base64Utils;

const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#define SKIP '\202'
#define NOSKIP 'A'
#define MaxLineLength 76
const char base64map[] =
{
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,  SKIP,  SKIP,   SKIP,  SKIP,  SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,    62,   SKIP,   SKIP,   SKIP,    63,
	52,    53,    54,    55,    56,    57,    58,    59,
	60,    61,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,    0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,
	7 ,    8 ,    9 ,    10,    11,    12,    13,    14,
	15,    16,    17,    18,    19,    20,    21,    22,
	23,    24,    25,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,    26,    27,    28,    29,    30,    31,    32,
	33,    34,    35,    36,    37,    38,    39,    40,
	41,    42,    43,    44,    45,    46,    47,    48,
	49,    50,    51,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
};

const char hexmap[] = 
{
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,    7 ,
	8 ,    9 ,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,    10,    11,    12,    13,    14,    15,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
};

const char QpEncodeMap[] = 
{
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   NOSKIP,   SKIP,   SKIP,   NOSKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
	NOSKIP,   SKIP,   SKIP,   SKIP,   SKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   SKIP,   NOSKIP,   NOSKIP,
	SKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   SKIP,   SKIP,   SKIP,   SKIP,   NOSKIP,
	SKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
	NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
    NOSKIP,   NOSKIP,   NOSKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
};

char* AMMimeUtils::MimeDecodeMailHeaderField(char *s)
{

	if (s == NULL) return s;
	if (s[strlen(s) - 2] == '\r')
	{
		s[strlen(s) - 2] = '\0';
	}
	char *s1 = s;
	char *rest = NULL;
	char *start = NULL;
	while (*s1 == ' ') s1++;
	if (strupos(s1, "=?") > 0)
	{
		int startendpos =static_cast<int>(strupos(s1, "=?"));
		start = new char[startendpos + 1];
		strncpy(start, s, startendpos);
		start[startendpos] = '\0';
		s1 += startendpos;
	}
	if (strupos(s1, "=?") == 0)
	{
		int alloclen;
		char *decodedText=0;
		int plainpos =static_cast<int>(strupos(s1, "Q?="));
		if (plainpos > 0)
		{
			plainpos += 3;
			char *m = s1 + plainpos;
			plainpos +=static_cast<int>(strupos(m, "?="));
		}
		else
		{
			plainpos =static_cast<int>(strupos(s1, "?="));
		}
		if (plainpos > 1)
		{
			char *mid = s1 + plainpos + 2;
			s1[plainpos] = '\0';
			if (strlen(mid) > 0)
			{
				rest = new char[strlen(mid) + 1];
				strcpy(rest, mid);
			}
		}
		if (strupos(s1, "?Q?") > 0)
		{
			CQPUtils qp;
			int pos =static_cast<int>(strupos(s1, "?Q?"));
			s1 += pos;
			if (strlen(s1) < 4) return s;
			s1 += 3;
			decodedText = qp.Decode(s1);
		}
		if (strupos(s1, "?B?") > 0)
		{
			CBase64Utils bu;
			int sLen;
			int pos =static_cast<int>(strupos(s1, "?B?"));
			s1 += pos;
			if (strlen(s1) < 4) return s; 
			s1 += 3;
			sLen =static_cast<int>(strlen(s1));
			decodedText = bu.Decode(s1, &sLen);
		}
		alloclen =static_cast<int>(strlen(decodedText)) + 1;
		if (start != NULL) alloclen +=static_cast<int>(strlen(start));
		if (rest != NULL) alloclen +=static_cast<int>(strlen(rest));
		alloclen *= sizeof(char);
		s = (char*)realloc(s, alloclen);
		s[0] = '\0';
		if (start != NULL)
		{
			strcat(s, start);
		}
		strcat(s, decodedText);
  		if (rest != NULL)
		{
			strcat(s, rest);
		}
		delete [] decodedText;
	}
	delete [] rest;
	delete [] start;
	return s;
}

CBase64Utils::CBase64Utils()
{
	ErrorCode = 0;
}

CBase64Utils::~CBase64Utils()
{

}
/*!
 *Decode a string using the Base64 codification
 */
char* CBase64Utils::Encode(char *input, int bufsize)
{
	int alsize = ((bufsize * 4) / 3);
	char *finalresult = (char*)calloc(alsize + ((alsize / 76) * 2) + 
                                    (10 * sizeof(char)), sizeof(char));
	int count = 0;
	int LineLen = 0;
	char* fresult = finalresult;
	char *s = input;
	int tmp = 0;
	while (count <= bufsize)
	{
		if (count % 3 == 0 && count != 0)
		{
			tmp >>= 8;
			tmp &= 0xFFFFFF;
			int mid = tmp;
			mid >>= 18;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
			LineLen++;
			mid = tmp;
			mid >>= 12;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
			LineLen++;
			mid = tmp;
			mid >>= 6;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
			LineLen++;
			mid = tmp;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
			LineLen++;
			tmp = 0;

			if (LineLen >= MaxLineLength)
			{
				*(fresult++) = '\r';
				*(fresult++) = '\n';
				LineLen = 0;
			}
			if (bufsize - count < 3)
			break;
		}
		unsigned char mid = (256 - (0 - *s));
		tmp |= mid;
		tmp <<= 8;
		count++;
		s++;
	}
	int rest = (bufsize - count) % 3;
	if (rest != 0)
	{
		tmp = 0;
		int i;
		for (i = 0; i < 3; i++)
		{
			if (i < rest)
			{
				unsigned char mid = (256 - (0 - *s));
				tmp |= mid;
				tmp |= *s;
				tmp <<= 8;
				count++;
				s++;
			}
			else
			{
				tmp |= 0;
				tmp <<= 8;
			}
		}
		tmp >>= 8;
		tmp &= 0xFFFFFF;
		int mid = tmp;
		if (rest >= 1)
		{
			mid >>= 18;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
			mid = tmp;
			mid >>= 12;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
		}
		if (rest >= 2)
		{
			mid = tmp;
			mid >>= 6;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
		}
		if (rest >= 3)
		{
			mid = tmp;
			mid &= 0x3F;
			*(fresult++) = base64chars[mid];
		}
		for (int c = 3; c > rest; c--)
		{
			*(fresult++) = '=';
		}
	}
	return finalresult;
}
/*!
 *Decode a string from a Base64 codification
 */
char* CBase64Utils::Decode(char *input, int *bufsize)
{
	int std = 0, count = 1, resultlen = 0;
	char *finalresult = (char*)calloc(*bufsize + sizeof(char), sizeof(char));
	char *s = input, *result = finalresult;
	while (*s != '=' && count <= *bufsize)
	{
		while (base64map[static_cast<int>(static_cast<char>(*s))] == SKIP)
		{
			if (*s != '\r' && *s != '\n')
			{
				ErrorCode = 1;
			}
			s++;
			(*bufsize)--;
			if (count >= *bufsize)
			{
				break;
			}
		}
		std |= base64map[*(s++) & 0xFF];
		std <<= 6;
		if (count % 4 == 0)
		{
			int tmp;
			std >>= 6;
			tmp = std;
			tmp >>= 16;
			tmp &= 0xFF;
			*(result++) = static_cast<char>(tmp);
			tmp = std;
			tmp >>= 8;
			tmp &= 0xFF;
			*(result++) = static_cast<char>(tmp);
			tmp = std;
			tmp &= 0xFF;
			*(result++) = static_cast<char>(tmp);
			std = 0;
			resultlen += 3;
		}
		count++;
	}
	count--;
	if (count % 4 != 0)
	{
		int tmp;
		for (int i = 0; i < 4 - (count % 4); i++)
		{
			std <<= 6;
			resultlen++;
		}
		std >>= 6;
		tmp = std;
		tmp >>= 16;
		tmp &= 0xFF;
		*(result++) = static_cast<char>(tmp);
		tmp = std;
		tmp >>= 8;
		tmp &= 0xFF;
		*(result++) = static_cast<char>(tmp);
		tmp = std;
		tmp &= 0xFF;
		*(result++) = static_cast<char>(tmp);
	}
	*bufsize = resultlen;
	return finalresult;
}
CQPUtils::CQPUtils()
{
	ErrorCode = 0;
}

CQPUtils::~CQPUtils()
{

}
char* CQPUtils::Decode(char *input)
{
	char *s = input;
	char *finalresult = (char*)calloc(strlen(input) + sizeof(char), sizeof(char));
	char *result = finalresult;
	while (*s != '\0')
	{
		if (*s == '=') 
		{
			int i;
			for (i = 0; i < 3; i++)
			{
				if (s[i] == '\0')
				{
					ErrorCode = 1;
					return finalresult;
				}
			}
			char mid[3];
			int ok = 1;
			s++;
			for (i = 0; i < 2; i++)
			{
				if (hexmap[static_cast<int>(static_cast<char>(s[i])) ] == SKIP)
				{
					ok = 0;
					if (s[i] == '\r' && s[i + 1] == '\n')
					{
						s += 2;
						break;
					}
					else
					{
						ErrorCode = 1;
					}
				}
				mid[i] = s[i];
			}
			if (ok)
			{
        int m;
				s += 2;
				m = hexmap[static_cast<int>(static_cast<char>(mid[0]))];
				m <<= 4;
				m |= hexmap[static_cast<int>(static_cast<char>(mid[1]))];
				*(result++) = static_cast<char>(m);
			}
		}
		else
		{
			if (*s != '\0') *(result++) = *(s++);
		}
	}
	return finalresult;
}


#define BufAdd 10

char* CQPUtils::ExpandBuffer(char *buffer, int UsedSize, int *BufSize, 
                             int Singlechar)
{
	int AddVal;
	if (Singlechar) AddVal = 3;
	else AddVal = 5;
	if (UsedSize >= *BufSize - AddVal)
	{
		*BufSize += BufAdd;
		return (char*)realloc(buffer, *BufSize * sizeof(char));
	}
	return buffer;
}

char* CQPUtils::Encode(char *input)
{
	int BufSize = static_cast<int>(strlen(input) + BufAdd);
	int UsedSize = 0;
	int LineLen = 0; 
	char *finalresult = (char*)calloc(BufSize, sizeof(char));
	char *fresult = finalresult;
	char *s = input;
	while (*s != '\0')
	{
		unsigned char mid = (256 - (0 - *s));
		if (*s == '\n')
			LineLen = 0;
		if (QpEncodeMap[mid] == SKIP)
		{
			char mids[3];
			if (LineLen >= MaxLineLength - 4)
			{
				finalresult = ExpandBuffer(finalresult, UsedSize, &BufSize, 0);
				*(fresult++) = '=';
				*(fresult++) = '\r';
				*(fresult++) = '\n';
				UsedSize += 3;
				LineLen = 0;
			}
			finalresult = ExpandBuffer(finalresult, UsedSize, &BufSize, 0);
#ifdef WIN32
			itoa(mid, mids, 16);
#else
			snprintf(mids, 3, "%X", mid);
#endif
			strupr(mids);
			*(fresult++) = '=';
			*(fresult++) = mids[0];
			*(fresult++) = mids[1];
			UsedSize += 3;
			LineLen += 2;
			s++;
		}
		else
		{
			if (LineLen >= MaxLineLength - 4)
			{
				finalresult = ExpandBuffer(finalresult, UsedSize, &BufSize, 0);
				*(fresult++) = '=';
				*(fresult++) = '\r';
				*(fresult++) = '\n';
				UsedSize += 3;
				LineLen = 0;
			}
			finalresult = ExpandBuffer(finalresult, UsedSize, &BufSize);
			UsedSize++;
			LineLen++;
			*(fresult++) = *(s++);
		}
	}
	*(fresult++) = '\0';
	return finalresult;
}
