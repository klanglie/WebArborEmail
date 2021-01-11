// NSFileFunctions.cpp
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"

#include "NSFileFunctions.h"
using namespace WEBARBOR_EMAIL_COMPONENTS;

// NSFileFunctions constructor
NSFileFunctions::NSFileFunctions()
{ 
	chFileData = 0;
}

// NSFileFunctions destructor
// Make sure to delete all global pointers that have been allocated
NSFileFunctions::~NSFileFunctions()
{
	delete [] chFileData;
}

// write to a file
// This is a general purpose multi-functioning function used to output
// file contents to the specified path. All data is overridden.
// There is no use of binary output. I don't see a need for it here.
void NSFileFunctions::writeFile( const char *output, const char * pFilePath )
{
	ofstream outClientFile( pFilePath, ios::out );

	try{

		if ( !outClientFile ){
			//throw FileWillNotOpen();
			throw MiscError( "writeFile was unable to open a file", 103 );
			//cerr << "File could not be opened" << endl;
			//exit(1);
		}
		
		outClientFile << output << endl;		
		outClientFile.close();
	}
	catch( MiscError ){
		if ( outClientFile ) outClientFile.close();
		throw;
	}
	catch(...){
		if ( outClientFile ) outClientFile.close();
		throw MiscError( "writeFile has caught an unknown exception.", 104 );
	}
}

// append to a file
// This is a general purpose multi-functioning function used 
// to append output to the specified path. All data is overridden.
// There is no use of binary output. I don't see a need for it here.
void NSFileFunctions::appendFile( string &output, const char * pFilePath )
{
	ofstream outClientFile( pFilePath, ios::app );

	try{

		if ( !outClientFile ){
			//throw FileWillNotOpen();
			throw MiscError( "appendFile was unable to open a file", 203 );
			//cerr << "File could not be opened" << endl;
			//exit(1);
		}		
		outClientFile << output << endl;		
		outClientFile.close();
	}
	catch(...){
		if ( outClientFile ) outClientFile.close();
		throw MiscError( "appendFile has caught an unknown exception.", 204 );
	}
}

// This method is used to read in the contents of a file
// once setFilePath has been called. If a failure occurs
// because the file path does not exist, an error is 
// thrown and the next higher function in the stack is passed
// the error object and read from there.
char * NSFileFunctions::OpenFile(const char * file_path)
{
	
	if( chFileData ) delete [] chFileData;
	struct stat results;

	try{

		if ( stat(file_path, &results) == 0 ){
	        
			// The size of the file in bytes is in
			// results.st_size
			ifstream fin; // ("data.bin", ios::in | ios::binary);
			fin.open( file_path, ios::in | ios::binary );
			if(!fin){
				throw FileWillNotOpen();
		/*		cerr << "File could not be opened." << endl;
				exit(1);*/		
			}		

			chFileData = new char[ results.st_size + 1 ];
			if( chFileData == 0 ){
				throw MiscError( "NSFileFunctions::Allocation failed for chFileData.", 8978 );			
			}
			chFileData[ results.st_size ] = 0;
					
			fin.read ( chFileData, results.st_size );
			fin.close();

			return chFileData;
		}
		else{
			throw FileWillNotOpen();
		}

		return 0;

	}
	catch( FileWillNotOpen ){
		delete [] chFileData;
		throw;
	}
	#if _WIN32_WINNT >= 0x0500
	catch( bad_alloc exception ){
		delete [] chFileData;
		throw MiscError( exception.what(), 8976 );
	}
	#endif
	catch(...){
		delete [] chFileData;
		throw MiscError("NSFileFunctions::OpenFile has encountered an unknown error.", 1425);	
	}
}

bool NSFileFunctions::CreateDirectory(const char * dir_name){

bool bRetVal = false;
int stat;

    stat = mkdir(dir_name);
    if (!stat)
    {
       bRetVal = true;
    }
     
    return bRetVal;     
     
}

bool NSFileFunctions::DirExists(const char *dirname){
   // This uses direct.h
   if( _chdir( dirname )   )
      return false;
   else
      return true;
}

int NSFileFunctions::FileExists(const char *szFilename)
/* does the file exist? */
{
  FILE *pFile = fopen(szFilename, "r");

  if (pFile != NULL)
  /* if the file exists, "be kind, rewind"--err..close the file then return true/1 */
  {
    fclose(pFile);
    return 1;
  }
  else
  {
    return 0;
  }
}

void NSFileFunctions::ShredFile(const char *szFilename)
{
  //FILE *pFile;
  int fileHandle;
  /* char *pstrNullBytes; */
  long lFilePos, lLength;
  
  //pFile = fopen(szFilename, "r+b");

  fileHandle = _open(szFilename, _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);

  lLength = _filelength(fileHandle);//FileLength(pFile);  /* get the file length */
  lFilePos = 0;  /* set our position counter to 0 */

  /*
     originally, I allocated memory for this...but for _huge_ files, this won't do...
     I mean, imagine grabbing 5 MB of space just to write nulls? Crazy, right ? :D
  */
  /*
  pstrNullBytes = calloc(lLength, sizeof(char));

  
  if (!pstrNullBytes)
  {
    fprintf(stderr, "\aError, not enough memory for buffer! File left intact..");
    return;
  }
  */

  /* fwrite(pstrNullBytes, lLength, 1, f); */ 
  /* 
     ^- then I'd write everything to the file at once...
  */

  /* a safer way: write byte by byte */
  /*
     increments by the result of fwrite (bytes written)
     in case it doesn't perform the write for some reason....
  */
 
  while (lFilePos < lLength)
   // lFilePos += fwrite("", 1, 1, pFile);  /* print a null (0 byte char) in place of the original char */
   lFilePos += _write(fileHandle, "", 1);

  //fclose(pFile); 
   //_close(fileHandle);

  while(_commit(fileHandle) != 0) //ensures that the file is flushed to disk
    ;

  _close(fileHandle);

  /* free(pstrNullBytes); */

  remove(szFilename);  /* delete the file! */
}

long NSFileFunctions::FileLength(FILE *pFile)
/* 
  pinched from borland's help files, thank you very much :) 
  pFile must be a valid pointer (i.e. the file exists) for
  it to work...no other preconditions are needed
*/
{
  
  long lFileSize;
  long lCurrentPos = ftell(pFile);
  
  fseek(pFile, 0L, SEEK_END);         /* go to the end... */
  lFileSize = ftell(pFile);           /* this'll give us either the size or -1 due to an error */
  fseek(pFile, lCurrentPos, SEEK_SET); /* reset file pointer */

  return lFileSize;  
}

void NSFileFunctions::FGetLine(char *szBuffer, int iNumChars, FILE *pFile)
/* 
  reads up to iNumChars - 1, the next newline, or until the end-of-file
  and it does NOT include the newline in the buffer
  added from last upload: checks for bad numchars values (felt it was necessary)
  and loop checks for equality, rather than > (bug fix)
*/
{
  int c;
  int i = 0;

  if (iNumChars <= 0)  /* whaddaya kidding me?! get outta here! :) */
    return;
  
  for (;;)
  {
    if (i == iNumChars - 1 || (c = fgetc(pFile)) == EOF || c == '\n')
    {
      szBuffer[i] = '\0';
      break;
    }
    szBuffer[i++] = c;
  }
}

bool NSFileFunctions::IsGif( const char * dir_path, const char * file_name ){

	//A GIF file must be identified by the signature GIF87a or GIF89a in its first 6 bytes.
	register bool bRetVal = false;
    if( strlen(dir_path) == 0 || strlen(file_name) == 0 ) return bRetVal;	

    char file_path[ MAX_FILE_LEN ];
    strcpy( file_path, dir_path );
    strcat( file_path, file_name );

	const char * GIF87a = "GIF87a";
	const char * GIF89a = "GIF89a";

	struct gif_header
	{
		char ch[7];
	}header;
	header.ch[6] = '\0';

	ifstream fin( file_path, ios_base::binary );
	if( !fin.is_open() ){
		throw MiscError( "NSFileFunctions::IsGifFile unable to open file.", 97824 );	
		//cerr << "Error: could not open file." << endl;
		//exit(1);
	}

	fin.read(reinterpret_cast<char *> (&header), sizeof(header) - 1);
	fin.close();

	if( strstr( header.ch, GIF87a ) != NULL || 
		strstr( header.ch, GIF89a ) != NULL ){
		bRetVal = true;	
	}

	return bRetVal;

}

bool NSFileFunctions::IsJpg( const char * dir_path, const char * file_name ){

	//A JPG files first 8 bits must begin with 0xFF. 2nd 8 bits begin with 0xD8
	register bool bRetVal = false;
    if( strlen(dir_path) == 0 || strlen(file_name) == 0 ) return bRetVal;

    char file_path[ MAX_FILE_LEN ];
    strcpy( file_path, dir_path );
    strcat( file_path, file_name );

	struct bitc{
		unsigned b1 : 8;
		unsigned b2 : 8;
	}in;

	ifstream fin( file_path, ios_base::binary );
	if( !fin.is_open() ){
		throw MiscError( "NSFileFunctions::IsJpg unable to open file.", 97825 );	
		//cerr << "Error: could not open file." << endl;
		//exit(1);
	}

	fin.read( reinterpret_cast<char *> (&in), sizeof(in));
	fin.close();

	if( in.b1 == 0xFF && in.b2 == 0xD8 ) bRetVal = true;

	return bRetVal;

}

bool NSFileFunctions::RenameFile( const char * dir_path, const char * old_name, 
								  const char * new_name, bool bDeleteNew ){

   register bool bRetVal = false;

   if( strlen(dir_path) == 0 || strlen(old_name) == 0 || strlen(new_name) == 0 )
	   return bRetVal;

   char old_path[ MAX_FILE_LEN ];
   char new_path[ MAX_FILE_LEN ];
   
   strcpy( old_path, dir_path );
   strcat( old_path, old_name );

   strcpy( new_path, dir_path );
   strcat( new_path, new_name );

   if( FileExists(new_path) && bDeleteNew == true ){
	   ShredFile(new_path);
   }

   /* Attempt to rename file: */
   register int result = rename( old_path, new_path );
   if( result == 0 ) bRetVal = true;

   return bRetVal;

}