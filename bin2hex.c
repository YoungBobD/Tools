/* 
 * Bin2Hex 
 *
 * Utility to transform a binary file into the hexidecimal
 * representation of the content that can be edited by any Text
 * Editor.
 * 
 * Note that this utility goes hand-in-hand with the utility Hex2Bin
 * that transform the file the other way. It is noteworthy that this
 * utility produces a text file with offset values at the beginning of
 * each line followed by a colon character, and that the Hex2Bin
 * utility summarily ignores the offset field. That is, the user is
 * free to add data to a file without worrying about mucking about
 * with the offset values on subsequent lines. The offset values are
 * provided strictly to help navigate the hex representation.
 * 
 * R. Denise
 * December 2015
 *
 * (Note also that this is the re-creation of a utility that existed
 * in a previous life. If I find the older version, this one may get
 * retired.).
 *
 */

#include <stdio.h>

#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "binedit.h"

void show_usage(char*, int );
int push_output_line( FILE* , int start, int buffer[], int bytes_on_line, int debug );

int main( int argc, char* argv[] ) {
   int indx, status, debug=(DEBUG_WARN_ENABLE | DEBUG_HEX_ENABLE );
   int local_int;
   int rec_lim = 0;
   struct stat stat_buff;
   char in_str[MAX_STR_LEN];
   int line_start, byte_count, bytes_on_line; 
   int line_limit = DEFAULT_BYTES_PER_LINE;
   int current_buffer[100];
   FILE* infile;
   FILE* outfile;
   
   for( indx=1; indx<argc; indx++ ) {
      if( 0 == strcmp( "-c", argv[indx] ) ) {
         rec_lim = atoi( argv[indx+1] );
         printf( "Will only process %d bytes \n", rec_lim );
      }
      if( 0 == strcmp( "-d", argv[indx] ) ) {
         sscanf( argv[indx+1], "%i", &local_int );
         debug |= local_int;
      }
      if( 0 == strncmp( "-dec", argv[indx], 4 ) ) {
         /* Disable HEX Address */
         debug &= 0xBFFF;
      }
      if( 0 == strcmp( "-bpl", argv[indx] ) ) {
         int local_int = atoi( argv[indx+1] );
         printf( "Will output %d bytes/line \n", local_int );
         line_limit = local_int;
      }
      if( 0 == strcmp( "-nowarn", argv[indx] ) ) {
         /* Disable warnings */
         debug &= 0x7FFF;
      }
      if( 0 == strcmp( "-a", argv[indx] ) ) {
        /* assert test */
        int XX = atoi( argv[indx+1] );
        assert( XX > 5 );
      }
   }
   
   strncpy( in_str, argv[argc-1], MAX_STR_LEN-1 );
   status = stat( in_str, &stat_buff );
   if( 0 > status) {
      show_usage( argv[0], debug );
      exit(-1);
   }
   
   if(debug & DEBUG_SIGNPOST) {
      if( rec_lim > 0 ) 
         printf( "---> Processing %d bytes from file \"%s\" \n", rec_lim, in_str );
      else 
         printf( "---> Processing all records from file \"%s\" \n", in_str );
   }
   
   infile = fopen( in_str, "r" );
   if( infile == NULL ) {
      printf( "Error on file open of input file \"%s\" \n", in_str );
      exit(-1);
   }
   
   /* Open the Output file */
   strcat( in_str, ".hex" );
   outfile = fopen( in_str, "w" );
   if( NULL == outfile ) {
      printf( "Error on file open output file \"%s\" \n", in_str );
      fclose( infile );
      exit(-1);
   }
   
   bytes_on_line = 0;
   byte_count = 0;
   line_start = 0;
   
   do {
      local_int = fgetc( infile );
      
      current_buffer[bytes_on_line] = local_int;
      
      if(( EOF == local_int ) ||
         (( rec_lim > 0 ) && ( rec_lim < byte_count ) ) ) {
         push_output_line( outfile, line_start, current_buffer, bytes_on_line, debug );
         fclose( outfile );
         fclose( infile );
         break;
      }
      
      bytes_on_line ++;
      byte_count++;
      
      if( line_limit <= bytes_on_line ) {
         push_output_line( outfile, line_start, current_buffer, bytes_on_line, debug );
         bytes_on_line = 0;
         line_start = byte_count;
      }
      
   } while( TRUE );
   
   if( debug & DEBUG_SIGNPOST ) {
      printf( "Bin2Hex, has completed its task\n" );
   }
   exit(0);
}

void show_usage( char* invocation, int debug ) {
   
   printf( "Usage: %s <-c nnn> <-d xxx> <-h> \n", invocation );
   printf( " \n" );
   printf( " -c nnn     Maximum EVR records to process (default is ALL records in file) \n" );
   printf( " -d xxx     Enable Debugging Output with mask <xxx> in hex (0x0001 shows debug stategy)\n" );
   printf( " -dec       Display address in Decimal (default is Hex)  \n" );
   printf( " -bpl nnn   Number of bytes per line of output (default is %d ) \n", DEFAULT_BYTES_PER_LINE );
   printf( " -h         Show usage infomration                             \n" );
   printf( " -nowarn    Disable warning messages  \n" );
   printf( " -v         Show version information \n" );
   
   if( debug & DEBUG_HELP ) {
      printf( " \n" );
      printf( " Debug Flags: \n" );
      printf( " %c : 0x%04Xx : DEBUG_HELP \n", ((debug & DEBUG_HELP)?'X':' '), DEBUG_HELP );
      printf( " %c : 0x%04Xx : DEBUG_SIGNPOST \n", ((debug & DEBUG_SIGNPOST)?'X':' '), DEBUG_SIGNPOST );
      printf( " %c : 0x%04Xx : DEBUG_OUTPUT \n", ((debug & DEBUG_OUTPUT)?'X':' '), DEBUG_OUTPUT );
      printf( " %c : 0x%04Xx : DEBUG_HEX_ENABLE \n", ((debug & DEBUG_HEX_ENABLE)?'X':' '), DEBUG_HEX_ENABLE );
      printf( " %c : 0x%04Xx : DEBUG_WARN_ENABLE \n", ((debug & DEBUG_WARN_ENABLE)?'X':' '), DEBUG_WARN_ENABLE );
      printf( "\nDebug value: 0x%04Xx \n", debug );
   }
   printf( " \n" );
   return;
}

int push_output_line( FILE* outfile, int start, int * data, int limit, int debug ) {
   
   int indx;
   char str1[MAX_STR_LEN];
   char str2[MAX_STR_LEN];
   char token[16];
   
   memset(str1, 0, MAX_STR_LEN );
   memset(str2, 0, MAX_STR_LEN );
   
   for( indx=0; indx<limit; indx++ ) {
      sprintf( token, "%02X ", data[ indx ] );
      strcat( str1, token );
      
      if(( 31 < data[indx] ) &&
         ( 127 > data[indx] ) ) {
         sprintf( token, "%c ", data[indx] );
         strcat( str2, token );
      } else {
         strcat( str2, ". " );
      }
   }
   
   if( debug & DEBUG_HEX_ENABLE ) {
      if( debug & DEBUG_OUTPUT ) {
         printf( "%06X : %s : %s \n", start, str1, str2 );
      }
      fprintf( outfile, "%06X : %s : %s \n", start, str1, str2 );
   } else {
      if( debug & DEBUG_OUTPUT ) {
         printf( "%08u : %s : %s \n", start, str1, str2 );
      }
      fprintf( outfile, "%08u : %s : %s \n", start, str1, str2 );
   }
   return(0);
}
