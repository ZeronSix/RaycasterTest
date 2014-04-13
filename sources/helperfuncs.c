#include <stdio.h>
#include <stdlib.h>
#include "helperFuncs.h"

const char* LoadStringFromFile( const char* fileName ) {
	int size = 0;
	char* buffer;

	FILE* f = fopen( fileName, "rb" );
	if ( f == NULL ) { 
		fprintf( stderr, "Failed to open a file %s", fileName );
	} 
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	buffer = malloc( size + 1 );

	if ( size != fread( buffer, sizeof(char), size, f ) ) { 
		fprintf( stderr, "Failed to read a file %s", fileName );
		free( buffer );
		return NULL;
	} 
	fclose(f);
	
	buffer[size] = 0;
	return buffer;
}