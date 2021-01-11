// NSStringFunctions
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include"NSErrorClasses.h"
#include<vector>
#include<string>
using namespace std;

#ifndef NS_STRING_FUNCTIONS
#define NS_STRING_FUNCTIONS

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

	class NSStringFunctions{

public:

	NSStringFunctions();
	virtual ~NSStringFunctions();

	char * c_string(const string &s);
	void LTrim( string & );
	void LTrim( char *t );
	int strreplace(char* source, const char* replace, const char* with);
	int FindAllOccurences( char* source, char* replace );
	
	void ToLower(char *);
	void ToLower(string &sInput);
	
	void RTrimCrLfSpace(char *);
	void RTrimCrLfSpace(string &str);

	int InstrChar( const char * input, const char * search );
	int InstrChar( const char * input, char search );
	int bmsearch(char *text,char *pat,int *no,int *pos);
	unsigned long  DetermineBufferSize(unsigned long dwAmountOfChars);
	
	void RTrim(string &str);
	void RTrim(string *stString);
		
	char* LeftTrim (char* const szTrim);
	char* RightTrim (char* const szTrim);
	#define TRIM(x) LeftTrim (RightTrim (x))

	int ConvertCharToInt(const char * input);
	int ConvertStringToInt( string &input );

	char *strrev(char *str);
	string intToString(int num);
	void Tokenize(const string&, vector<string>&, const string&);
	std::string ToUpper(const std::string& s); 
	char * ToUpper( const char * chInput );

	int strclean(char *string, const char *substring);	
	int xtoi(const char *s);
	int xdtoi(const int d);
	
	char *substrL( char *chInput, int iLen );
	char *substrR( char *s1, const char *s2 );
	char *substrR( char *chInput, int iStartPos );
	int strcasecmp(char * l1, char * l2);
	string int2hex(unsigned int n);
	inline bool ifindstr( size_t startx, size_t body_len, size_t search_len, char* body, const char* search, size_t & pos );

private:
	
	inline void trimLeft(string &str, char c);
	inline void trimRight(string &str, char c);
	
	inline int leftcopy( char* input, char* output, size_t pos );
	inline int rightcopy( char* input, size_t input_length, char* output, size_t pos );
	
	template < class T >
	inline int ConvertToInt( T input, int iLength );
	inline int pow(int x, int y);
	
};

} // end namespace

#endif