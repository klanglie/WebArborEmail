// NSRegistration
// v1.0.0.6 3/20/2004
// (c) 2003 - 2004 Langlie Systems, Inc.
// All rights reserved.
// You may not re-use this code without permission.


#include "stdafx.h"
#include "NSRegistration.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

// NSRegistration constructor
NSRegistration::NSRegistration()
{
	pReturnMessage = 0;		// pointer variable
	return_status = 0;
	strcpy( registered_date, "\0\0\0\0\0\0\0\0\0\0" );
	days_registered = 0;
	what_version = 0;
	what_type = 0;
}

// NSRegistration destructor
// Make sure to delete all global pointers that have been allocated
NSRegistration::~NSRegistration()
{
	if( pReturnMessage ) delete [] pReturnMessage; 
}

// This function will check to see that a file path exists
void NSRegistration::setErrorMessage( const char *Message, int iNum )
{
	if( pReturnMessage ) delete [] pReturnMessage; // reclaim string

	pReturnMessage = new char[ strlen(Message) + 1 ]; // allocate storage
	//assert( pReturnMessage != 0 ); // terminate if memory not allocated
	strcpy( pReturnMessage, Message ); // copy literal to object

	return_status = iNum;
}

inline string NSRegistration::encryptString( string sKey, int iKeyLen, string sData, int iDataLen, int iEncType )
{

	CBlowFish oBlowFish((unsigned char*)sKey.c_str(), iKeyLen);

	char * szDataIn = 0;
	char * szDataOut = 0;
	char * szHex = 0;

	try{

		szDataIn = new char[ iDataLen + 1 ];
		szDataOut = new char[ iDataLen + 1 ];
		szHex  = new char[ iDataLen * 4 ];

		memset( szDataIn, 0, iDataLen + 1 );
		memset( szDataOut, 0, iDataLen + 1 );

		// Encrypt
		strcpy(szDataIn, sData.c_str());
		memset(szDataOut, 0, iDataLen + 1);
		if( iEncType == 1 ){
			oBlowFish.Encrypt((unsigned char*)szDataIn, (unsigned char*)szDataOut, iDataLen, CBlowFish::ECB);
		}
		else if( iEncType == 2 ){
			oBlowFish.Encrypt((unsigned char*)szDataIn, (unsigned char*)szDataOut, iDataLen, CBlowFish::CBC);
		}
		else if( iEncType == 3 ){
			oBlowFish.Encrypt((unsigned char*)szDataIn, (unsigned char*)szDataOut, iDataLen, CBlowFish::CFB);
		}

		CharStr2HexStr((unsigned char*)szDataOut, szHex, iDataLen);
	
		string sOutput;
		sOutput.append( szHex );

		delete [] szHex;
 		delete [] szDataOut;
		delete [] szDataIn;
		
		return sOutput;
	}
	catch(...){
		if( szDataIn )
			delete [] szDataIn;

		if( szDataOut )
			delete [] szDataOut;
		
		if( szHex )
			delete [] szHex;

		throw MiscError( standard_registration_error, 14883);
	}

}

inline string NSRegistration::getKey( const char * plain_text, const char * append_key, 
									  const char * hash_var, int iWhatProduct )
{

	string sKey = "";
	char * strNewKey = 0;
	int iInputLen = strlen(plain_text);

	try{

		strNewKey = new char[ iInputLen + 1 ];
		strNewKey[ iInputLen ] = 0;
		strcpy( strNewKey, plain_text );

		strNewKey = strrev(strNewKey);
		sKey.append( strNewKey );

		delete [] strNewKey;

		
		if( sKey.length() > 30 ){
			sKey = sKey.substr(0, 30);
		}
		
		//cout << "sKey 1:" << sKey << endl;

		string special = getSpecialSeedVar( iWhatProduct );
		if( special.length() > 6 ){
			special = special.substr( 0, 6 );
		}
		sKey.append( special );

		//cout << "sKey 2:" << sKey << endl;

		if( sKey.length() > 36 ){
			sKey = sKey.substr(0, 36);
		}

		string hash;
		hash.append( hash_var );
		if( hash.length() > 6 ){
			hash = hash.substr( 0, 6 );
		}
		sKey.append( hash );

		//cout << "sKey 3:" << sKey << endl;

		if( strlen(append_key) > 0 ){
			if( sKey.length() > 42 ){
				sKey = sKey.substr(0, 42);
			}
			string sAppend = append_key;
			if( sAppend.length() > 20 ){
				sAppend = sAppend.substr( 0, 20 );
			}
			sKey.append(sAppend);
			//cout << "append_key :" << append_key << endl;
		}	

		//cout << "sKey 4:" << sKey << endl;

		if( sKey.length() < 56 ){
			sKey.append( plain_text );		
		}

		//cout << "sKey.length() :" << sKey.length() << endl;
		//cout << "sKey 5:" << sKey << endl;
			

		if( sKey.length() < 56 ){
			while( sKey.length() < 56 ){
				sKey.append(" ");
			}
		}		
		else if( sKey.length() > 56 ){
			sKey = sKey.substr(0, 56);
		}

		//cout << "sKey 6:" << sKey << endl;

	}
	catch( MiscError ){
		if( strNewKey )	delete [] strNewKey;
		throw;
	}
	catch(...){
		if( strNewKey )	delete [] strNewKey;
		throw MiscError( standard_registration_error, 14886);
	}
	
	//cout << "sKey :" << sKey << endl;
	//cout << "sKey Length:" << sKey.length() << endl;

	return sKey;
}

// plain_text is something like the users name / e-mail.
// append_text is something added to the key used for encryption. If empty, not added.
// iWhatProduct is for product type. Ex: WAMessage.Decoder product type = 1
// bUseSeed - If between a certain range(1-10), then this guid string is used also for encryption.
// Is this user buying or trying?
string NSRegistration::createRegistrationCode( char *plain_text, const char *append_key, 
							   int iWhatProduct, bool bTrial, int iEncType ){

	if( pReturnMessage ) delete [] pReturnMessage;
	pReturnMessage = 0;
	return_status = 0;

	// Here is the format I am aiming for:
	// p=wamd;v=1;t=1;d=20040316;h=(md5);
	// p = product ( wamd = WAMessage.Decoder )
	// v = version
	// t = type ( 1 = trial, 2 = purchase )
	// d = date
	// h = hash
	
	strcpy( registered_date, "\0\0\0\0\0\0\0\0\0\0" );
	days_registered = 0;
	what_version = 0;
	what_type = 0;
	string sEnc;

	try{

		if( strlen(plain_text) < 3 || strlen(plain_text) >= 50 ){
			throw MiscError( standard_registration_error, 14901);
		}
		else if( (iEncType < 1) || (iEncType > 3) ){
			throw MiscError( standard_registration_error, 14881);
		}
		else if( iWhatProduct < 0 || iWhatProduct > 1000 ){
			throw MiscError( standard_registration_error, 14893);
		}	

		sEnc.append("p=");
		sEnc.append(intToString( iWhatProduct ));
		sEnc.append(";");
	
		sEnc.append( "v=1;" ); // This encoding algorithm is version 1.

		if( bTrial == true ){ // The only thing different is the type (trial or purchase).
			sEnc.append("t=1;d=");
		}
		else{
			sEnc.append("t=2;d=");
		}
		
		CDate date; // retrieves current date
		
		sEnc.append(date);
		sEnc.append(";h=");

		const char *hash_var = MD5String( plain_text );
		sEnc.append( hash_var );
		sEnc.append(";");

		while( ( sEnc.length() % 8 ) != 0  ){
			sEnc.append(" ");
		}

		string sKey = getKey(plain_text, append_key, hash_var, iWhatProduct);
		if( sKey.length() != 56 ){
			throw MiscError( standard_registration_error, 14882);
		}
		else{
			sEnc = encryptString(sKey, sKey.length(), sEnc, sEnc.length(), iEncType);
		}

	}
	catch( MiscError ex ){
		setErrorMessage( ex.what(), ex.number() );
		sEnc = "";
	}
	catch(...){
		setErrorMessage( standard_registration_error, 14877 );
		sEnc = "";
	}

	return sEnc;
}

bool NSRegistration::verifyRegistrationCode( char *plain_text, const char *append_key,
											 const char *enc_text, int iWhatProduct, 
											 int iEncType, int iTrialLength ){
	
	if( pReturnMessage ) delete [] pReturnMessage;
	pReturnMessage = 0;
	return_status = 0;

	bool bPass = false;	
	char szDataIn[ MAX_ENCRYPT_SIZE + 1 ];
	szDataIn[128] = 0;
	strcpy( registered_date, "\0\0\0\0\0\0\0\0\0\0" );
	days_registered = 0;
	what_version = 0;
	what_type = 0;

	try{

		if( strlen(plain_text) < 3 || strlen(plain_text) >= 50 ){
			throw MiscError( standard_registration_error, 14901);
		}
		else if( (iEncType < 1) || (iEncType > 3) ){
			throw MiscError( standard_registration_error, 14881);
		}
		else if( iWhatProduct < 0 || iWhatProduct > 1000 ){
			throw MiscError( standard_registration_error, 14893);
		}
		else if( strlen(enc_text) != MAX_ENCRYPT_SIZE ){
			throw MiscError( standard_registration_error, 14884);										  
		}

		HexStr2CharStr( enc_text, (unsigned char*)szDataIn, MAX_ENCRYPT_SIZE );

		const char *hash_var = MD5String( plain_text );

		string sKey = getKey(plain_text, append_key, hash_var, iWhatProduct);		
		CBlowFish oBlowFish((unsigned char*)sKey.c_str(), sKey.length());

		//Decryption
		if( iEncType == 1 ){
			oBlowFish.Decrypt((unsigned char*)szDataIn, MAX_ENCRYPT_SIZE, CBlowFish::ECB);
		}
		else if( iEncType == 2 ){
			oBlowFish.Decrypt((unsigned char*)szDataIn, MAX_ENCRYPT_SIZE, CBlowFish::CBC);
		}
		else if( iEncType == 3 ){
			oBlowFish.Decrypt((unsigned char*)szDataIn, MAX_ENCRYPT_SIZE, CBlowFish::CFB);
		}

		string sData;
		sData.append( szDataIn );

		int iFindPos = InstrChar( szDataIn, " " );
		if( iFindPos > 0 ){
			sData = sData.substr( 0, iFindPos );
		}
		else{
			iFindPos = sData.find_last_of(";");
			sData = sData.substr( 0, iFindPos + 1 );
		}

		string product;
		string version;
		string type;
		string date;
		string hash;
		
		int find_product, find_version, find_type, find_date, find_hash;

		find_product = sData.find("p=");
		find_version = sData.find("v=");
		find_type = sData.find("t=");
		find_date = sData.find("d=");
		find_hash = sData.find("h=");

		if( !(find_hash>find_date) || !(find_date>find_type) || 
			!(find_type>find_version) || !(find_version>find_product) ){
			throw MiscError( standard_registration_error, 14888);		
		}
		else if( find_product != 0 ){
			throw MiscError( standard_registration_error, 14889);
		}
		
	    int iLen = sData.length();
		for( int i = 0; i < iLen; i++ ){
			if( sData[i] == ';' ){
				if( i > 0 && i == ( find_version - 1 ) ){
					product = sData.substr( 2, i - 2);
				}
				else if( i > find_version && i == ( find_type - 1 ) ){
					version = sData.substr( find_version + 2, i - ( find_version + 2 ) );
				}
				else if( i > find_type && i == ( find_date - 1 ) ){
					type = sData.substr( find_type + 2, i - ( find_type + 2 ) );
				}
				else if( i > find_date && i == ( find_hash - 1 ) ){
					date = sData.substr( find_date + 2, i - ( find_date + 2 ) );
				}
				else if( i > find_hash ){
					hash = sData.substr( find_hash + 2, i - ( find_hash + 2 ) );
				}
			}
		}
		
		if( strcmp( hash_var, hash.c_str() ) != 0 ){
			throw MiscError( standard_registration_error, 14890);
		}


		int iProduct = ConvertStringToInt( product );
		if( iProduct != iWhatProduct ){
			throw MiscError( standard_registration_error, 14891);
		}
		
		try{
			int iVersion = ConvertStringToInt( version );
			int iType = ConvertStringToInt( type );
			
			CDate oDate( date.c_str() );	
			CDate oTodaysDate; // retrieves current date

			if( iType == 1 ){
				
				// Check this date object against a current date object
				// Add iTrialLength days to this date object, etc...
				if( oTodaysDate < oDate ){
					throw MiscError("System clock out of sync.", -1);
				}
				else if( ( oTodaysDate - oDate ) > iTrialLength ){
					throw MiscError("Trial offer no longer valid.", 1);
				}

			}
			
			// This date is built to return to the user
			strcpy( registered_date, date.c_str() );
			days_registered = oTodaysDate - oDate;
			what_version = iVersion;
			what_type = iType;			
		}
		catch( MiscError ){
			throw;
		}
		catch( ... ){
			throw MiscError( standard_registration_error, 14892);
		}

		bPass = true;

	}
	catch( MiscError ex ){
		setErrorMessage( ex.what(), ex.number() );
		bPass = false;
	}
	catch(...){
		setErrorMessage( standard_registration_error, 14885 );
		bPass = false;
	}
	return bPass;
}

inline string NSRegistration::getSpecialSeedVar( int iProduct )
{
	switch( iProduct ){

	case 1:
		return "344A47A8A842441581E78FBC7201E572";
		break;

	case 2:
		return "88C8FC4E5E54443dB50A62D2A68E21A5";
		break;

	case 3:
		return "50D6E9E2579D44feA312CB54AC1AB02F";
		break;

	case 4:
		return "2BBE2600878044849DC6CF624DAD9E54";	
		break;

	case 5:
		return "2C7DB2FB5FA943ef9B0E195C9F6DD924";	
		break;

	case 6:
		return "03F0F2F6794C4b02877AEF0373131E7F";	
		break;

	case 7:
		return "AA074ED191A146ef9C3AFD5E6DDD4C6F";	
		break;

	case 8:
		return "91582AC83F904296BB4822335B944596";	
		break;

	case 9:
		return "ED4461BF5F714527AE9A90FA16774C6C";	
		break;
	
	case 10:
		return "CFBE7948701847e5A17171B4B97FC39A";	
		break;

	default:
		//return "D2664EE3FD58443cB2D263B792BA839C";
		return intToString( iProduct * 9995 );		
		break;
	}

}

//Function to convert unsigned char to string of length 2
void NSRegistration::Char2Hex(const unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

//Function to convert string of length 2 to unsigned char
void NSRegistration::Hex2Char(const char* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
}    

//Function to convert string of unsigned chars to string of chars
void NSRegistration::CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//Function to convert string of chars to string of unsigned chars
void NSRegistration::HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}