// NSRegistration
// v1.0.0.6 3/20/2004
// (c) 2003 - 2004 Langlie Systems, Inc.
// All rights reserved.
// You may not re-use this code without permission.

#include "md5.h"
#include "Blowfish.h"
#include "CDate.h"
#include "NSErrorClasses.h"
#include "NSStringFunctions.h"
using namespace cdate;

#ifndef NS_REGISTRATION_MODULE
#define NS_REGISTRATION_MODULE

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

const char standard_registration_error[] = "Invalid Registration Information.";

class NSRegistration : private NSStringFunctions{

public:

	NSRegistration();
	virtual ~NSRegistration();

	string createRegistrationCode( char *plain_text, const char *append_key_text, 
								   int iProduct, bool bTrial, int iEncType );

	bool verifyRegistrationCode( char *plain_text, const char *append_key_text, 
		        				 const char *enc_text, int iProduct,  
								 int iEncType, int iTrialLength );

	int   getReturnStatus() { return return_status; };
	const char *getReturnMessage() { return pReturnMessage; };
	
	const char *getRegistrationDate() { return registered_date; };
	int getDaysSinceRegistration() { return days_registered; };
	
	int   getType() { return what_type; };
	int   getVersion() { return what_version; };

protected:
	void setErrorMessage( const char *, int );

private:

	int				return_status;
	char			*pReturnMessage;
	
	#define MAX_ENCRYPT_SIZE 128

	void Char2Hex(const unsigned char ch, char* szHex);
	void Hex2Char(const char* szHex, unsigned char& rch);
	void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize);
	void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize);
	
	inline string getSpecialSeedVar( int iProduct );
	inline string encryptString( string sKey, int iKeyLen, string sData, int iDataLen, int iEncType );
	inline string getKey( const char * plain_text, const char * append_key, 
						  const char * hash_key, int iWhatProduct );
    
	
	char registered_date[10];
	int days_registered;
	int what_version;
	int what_type;
};

} // end namespace

#endif