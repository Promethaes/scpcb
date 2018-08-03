
#include "std.h"
#include "bbfilesystem.h"
#include "bbstream.h"
#include <fstream>

gxFileSystem *gx_filesys;

struct bbFile : public bbStream{
	filebuf *buf;
	bbFile( filebuf *f ):buf(f){
	}
	~bbFile(){
		delete buf;
	}
	int read( char *buff,int size ){
		return buf->sgetn( (char*)buff,size );
	}
	int write( const char *buff,int size ){
		return buf->sputn( (char*)buff,size );
	}
	int avail(){
		return buf->in_avail();
	}
	int eof(){
		return buf->sgetc()==EOF;
	}
};

static set<bbFile*> file_set;

static inline void debugFile( bbFile *f ){
	if( debug ){
		if( !file_set.count( f ) ) RTEX( "File does not exist" );
	}
}

static inline void debugDir( gxDir *d ){
	if( debug ){
		if( !gx_filesys->verifyDir( d ) ) RTEX( "Directory does not exist" );
	}
}

static bbFile *open( String t,int n ){
	filebuf *buf=d_new filebuf();
	if( buf->open( t.cstr(),n|ios_base::binary ) ){
		bbFile *f=d_new bbFile( buf );
		file_set.insert( f );
		return f;
	}
	delete buf;
	return 0;
}

bbFile *bbReadFile( String f ){
	return open( f,ios_base::in );
}

bbFile *bbWriteFile( String f ){
	return open( f,ios_base::out|ios_base::trunc );
}

bbFile *bbOpenFile( String f ){
	return open( f,ios_base::in|ios_base::out );
}

void bbCloseFile( bbFile *f ){
	debugFile( f );
	file_set.erase( f );
	delete f;
}

int bbFilePos( bbFile *f ){
	return f->buf->pubseekoff( 0,ios_base::cur );
}

int bbSeekFile( bbFile *f,int pos ){
	return f->buf->pubseekoff( pos,ios_base::beg );
}

gxDir *bbReadDir( String d ){
	return gx_filesys->openDir( d,0 );
}

void bbCloseDir( gxDir *d ){
	gx_filesys->closeDir( d );
}

String bbNextFile( gxDir *d ){
	debugDir( d );
	return d->getNextFile();
}

String bbCurrentDir(){
	return gx_filesys->getCurrentDir();
}

void bbChangeDir( String d ){
	gx_filesys->setCurrentDir( d );
}

void bbCreateDir( String d ){
	gx_filesys->createDir( d );
}

void bbDeleteDir( String d ){
	gx_filesys->deleteDir( d );
}

int bbFileType( String f ){
	int n=gx_filesys->getFileType( f );
	return n==gxFileSystem::FILE_TYPE_FILE ? 1 : (n==gxFileSystem::FILE_TYPE_DIR ? 2 : 0);
}

int	bbFileSize( String f ){
	return gx_filesys->getFileSize( f );
}

void bbCopyFile( String src,String dest ){
	gx_filesys->copyFile( src,dest );
}

void bbDeleteFile( String f ){
	gx_filesys->deleteFile( f );
}

bool filesystem_create(){
	if( gx_filesys=gx_runtime->openFileSystem( 0 ) ){
		return true;
	}
	return false;
}

bool filesystem_destroy(){
	while( file_set.size() ) bbCloseFile( *file_set.begin() );
	gx_runtime->closeFileSystem( gx_filesys );
	return true;
}