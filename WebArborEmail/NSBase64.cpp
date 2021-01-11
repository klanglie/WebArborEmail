// NSBase64.cpp
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSBase64.h"

//#include<iostream>
//using namespace std;

#define BASE64_TEST

using namespace WEBARBOR_EMAIL_COMPONENTS;

// NSStringFunctions constructor
NSBase64::NSBase64()
{ 
	cInputLocation = 0;
	cOutputLocation = 0;
}

// NSStringFunctions destructor
// Make sure to delete all global pointers that have been allocated
NSBase64::~NSBase64()
{
	delete [] cInputLocation;
	delete [] cOutputLocation;
}

// This is a helper function that wraps the dirty work 
// of mime-decoding into a simple easy to use iterface.
// Simply pass in the content to be decoded and the name
// and location of where the file is to be stored. 
// This is represented by the save_location variable that 
// is passed in. 
void NSBase64::writeMimeFile( string & content, const char *save_location, const char * file_name )
{
	
	delete [] cInputLocation;
    delete [] cOutputLocation;

	try{ 

		cInputLocation = new char[strlen(save_location) + 19];
		cInputLocation[strlen(save_location) + 18] = 0;

		strcpy( cInputLocation, save_location );
		strcat( cInputLocation, "mime_temp_file.txt" );
		
		ofstream outClientFile( cInputLocation, ios::out );
		if ( !outClientFile ){
			delete [] cInputLocation;
			throw MiscError( "writeMimeFile was unable to open a file for output", 103 );
		}	
		outClientFile << content << endl;
		outClientFile.close();

		content = "";

		size_t iLength = strlen(save_location) + strlen(file_name);
		
		cOutputLocation = new char[iLength + 1];
		cOutputLocation[iLength] = 0;

		strcpy( cOutputLocation, save_location );
		strcat( cOutputLocation, file_name );

		FILE *outfile;
		FILE *infile;
		
		infile = fopen( cInputLocation, "rb" );
		if( infile == NULL ){
			throw MiscError("writeMimeFile was unable to open an input file.", 98);
		}

		outfile = fopen( cOutputLocation, "wb" );
		if( outfile == NULL ){
			if( infile )
				fclose( infile );
			throw MiscError("writeMimeFile was unable to open an output file.", 99);
		}		

		DecodeBase64File( infile, outfile );

		fclose(infile);
		NSFileFunctions oFile;
		oFile.ShredFile(cInputLocation);	
		fclose(outfile);

	}
	catch( MiscError ){
		delete [] cInputLocation;
		delete [] cOutputLocation;
		throw;
	}
	catch( ... ){
		delete [] cInputLocation;
		delete [] cOutputLocation;
		throw MiscError("An unknown error has occurred in writeMimeFile.", 70);
	}
}

/*
** decode
**
** decode a base64 encoded stream discarding padding, line breaks and noise
*/
inline void NSBase64::DecodeBase64File( FILE *infile, FILE *outfile )
{
    register unsigned char in[4], out[3], v;
    register int i, len;

    while( !feof( infile ) ) {
        for( len = 0, i = 0; i < 4 && !feof( infile ); i++ ) {
            v = 0;
            while( !feof( infile ) && v == 0 ) {
                v = (unsigned char) getc( infile );
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if( !feof( infile ) ) {
                len++;
                if( v ) {
                    in[ i ] = (unsigned char) (v - 1);
                }
            }
            else {
                in[i] = 0;
            }
        }
        if( len ) {
            decodeblock( in, out );
            for( i = 0; i < len - 1; i++ ) {
                putc( out[i], outfile );
            }
        }
    }
}

// Base64 helper function.
inline void NSBase64::decodeblock( unsigned char in[4], unsigned char out[3] )
{   
    out[0] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[1] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[2] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

// This is a helper function that wraps the dirty work 
// of mime-decoding into a simple easy to use iterface.
// Simply pass in the content to be decoded and the name
// and location of where the file is to be stored. 
// This is represented by the save_location variable that 
// is passed in. 
void NSBase64::writeMimeFile2( string & content, const char *save_location, const char * file_name )
{
	
	try{

		//kilobyte (KB)
	    //1024 characters (or bytes) of data

		//megabyte (MB)
	    //1,048,576 bytes or 1,024 kilobytes of data.

		// If this attachment is greater than 10 MEG
		if( content.length() > 10485760 ){
			writeMimeFile( content, save_location, file_name );
			content = "";
			return;
		}

		char file_path[ MAX_FILE_LEN ];
		strcpy( file_path, save_location );
		strcat( file_path, file_name );

		//////////////////////////////////////////////////////////
		
		//string output;
		//decode( content, output );

		//ofstream outClientFile( sPath.c_str(), ios::out | ios::binary );
		//if ( !outClientFile ){
		//	throw MiscError( "writeMimeFile2 was unable to open a file for output", 106 );
		//}	

		//outClientFile << output << endl;
		//outClientFile.close();

		/////////////////////////////////////////////////////////
		DecodeBase64Buffer( content.c_str(), file_path );
		
		content = "";

	}
	catch( MiscError ){
		throw;
	}
	catch(...){
		throw MiscError("writeMimeFile2 has caught an unexpected error.", 1028);
	}

	return;
}
/*
** decode
**
** decode a base64 encoded stream discarding padding, line breaks and noise
*/
inline void NSBase64::DecodeBase64Buffer( const char *input, const char *output_location )
{
    register unsigned char in[4], out[3], v;
    register int i, len;

	register int iPos = 0;

	FILE *outfile;
	outfile = fopen( output_location, "wb" );
	if( outfile == NULL ){
		throw MiscError("base64Decode was unable to open an output file.", 992);
	}

    while( input[iPos] ) {
        for( len = 0, i = 0; i < 4 && input[iPos]; i++ ) {
            v = 0;
            while( input[iPos] && v == 0 ) {
                v = (unsigned char) input[iPos++];
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if( input[iPos] ) {
                len++;
                if( v ) {
                    in[ i ] = (unsigned char) (v - 1);
                }
            }
            else {
                in[i] = 0;
            }
        }
        if( len ) {
			out[0] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
			out[1] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
			out[2] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
            for( i = 0; i < len - 1; i++ ) {
                putc( out[i], outfile );
            }
        }
    }

	fclose(outfile);
}
