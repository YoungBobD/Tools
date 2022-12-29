/* 
 * Hex2Bin
 *
 * Utility to transform a hexidecimal file into a binary file. 
 * 
 * Note that this utility goes hand-in-hand with the utility Bin2Hex
 * that transform the file the other way. It is noteworthy that this
 * utility expects... 
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

#include "binedit.h"

void show_usage(char*, int );
int tokenize( char * instr, char tokens[MAX_TOKENS][MAX_STR_LEN] );

int main( int argc, char* argv[] ) {
   int indx, status, debug=0x8000;
   int local_int;
   int rec_lim = 0;
   int record_cnt =0;
   int toks;
   char tokens[MAX_TOKENS][MAX_STR_LEN];
   struct stat stat_buff;
   char in_str[MAX_STR_LEN];
   char* str_ptr;
   FILE * infile;
   FILE * outfile;
   
   int colon_count, start_indx, stop_indx;
   
   for( indx=1; indx<argc; indx++ ) {
      if( 0 == strcmp( "-c", argv[indx] ) ) {
         rec_lim = atoi( argv[indx+1] );
         printf( "Will only process %d bytes \n", rec_lim );
      }
      if( 0 == strcmp( "-d", argv[indx] ) ) {
         sscanf( argv[indx+1], "%i", &local_int );
         debug |= local_int;
      }
      if( 0 == strcmp( "-nowarn", argv[indx] ) ) {
         /* Disable warnings */
         debug &= 0x7FFF;
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
         printf( "---> Processing %d records from file \"%s\" \n", rec_lim, in_str );
      else 
         printf( "---> Processing all records from file \"%s\" \n", in_str );
   }
   
   infile = fopen( in_str, "r" );
   if( infile == NULL ) {
      printf( "Error on file open of input file \"%s\" \n", in_str );
      exit(-1);
   }
   
   /* Open the Output file */
   strcat( in_str, ".bin" );
   outfile = fopen( in_str, "w" );
   if( NULL == outfile ) {
      printf( "Error on file open output file \"%s\" \n", in_str );
      fclose( infile );
      exit(-1);
   }
   
   do { 
      str_ptr = fgets( in_str, MAX_STR_LEN, infile );
      if( str_ptr == NULL ) {
         if(debug & DEBUG_SIGNPOST )
            printf( "---> End of input File\n" );
         break;
      }
      
      if((rec_lim > 0 ) && 
         (++record_cnt > rec_lim ) ) {
         printf( "Record indx (%d) Exceeds record limit( %d) \n",
                 record_cnt, rec_lim );
         break;
      }
      
      toks = tokenize( in_str, tokens );
      
      if(debug & DEBUG_DATA_READER) printf( "[%d] %s", toks, in_str );
      
      colon_count = 0;
      start_indx=0; 
      stop_indx=toks;
      
      for( indx=0; indx<toks; indx++ ) {
         if( 0 == strcmp( ":", tokens[indx] ) ) {
            if( colon_count == 0 ) {
               start_indx=indx+1;
               colon_count++;
            } else {
               if( colon_count == 1 ) {
                  stop_indx=indx;
                  colon_count++;
               }
            }
         }
      }			 
      
      if(debug & DEBUG_DATA_READER )
         printf( "Found %d colons.  Start at [%d]  and stop at [%d] \n", colon_count, start_indx, stop_indx );
      
      for( indx=start_indx; indx<stop_indx; indx++ ) {
         if(debug & DEBUG_PEDANTIC) {
            printf( "'%s' ", tokens[indx] );
         }
         status = sscanf( tokens[indx], "%x", &local_int );
         
         if( status == 1 ) {
            fputc( local_int, outfile );
         } else {
            if(debug & DEBUG_WARN_ENABLE ) {
               printf( "Invalid Input Format! I found token \"%s\" \n", tokens[indx] );
            }
         }
      }
      if(debug & DEBUG_PEDANTIC) {
         printf( "\n" );
      }
      
      if( colon_count == 2 ) {
         /* Stuff the converted data into the output file */
      }
      
   } while(TRUE);
   
   fclose( outfile );
   fclose( infile );
   if( debug & DEBUG_SIGNPOST ) {
      printf( "%s, has completed the task \n", argv[0] );
   }
   exit(0);
}

void show_usage( char* invocation, int debug ) {
   
   printf( "Usage: %s <-c nnn> <-d xxx> <-h> filename \n", invocation );
   printf( " \n" );
   printf( " -c nnn     Maximum bytes to process (default is complete file) \n" );
   printf( " -d xxx     Enable Debugging Output with mask <xxx> in hex (0x0001 shows debug stategy)\n" );
   printf( " -h         Show usage infomration                             \n" );
   printf( " -nowarn    Disable warning messages  \n" );
   printf( " \n" );
   
   if( debug & 0x7FFF ) {
      printf( " Debug Flags: \n" );
      printf( " %c : 0x%04Xx : DEBUG_HELP \n", ((debug & DEBUG_HELP)?'X':' '), DEBUG_HELP );
      printf( " %c : 0x%04Xx : DEBUG_PEDANTIC \n", ((debug & DEBUG_PEDANTIC)?'X':' '), DEBUG_PEDANTIC );
      printf( " %c : 0x%04Xx : DEBUG_SIGNPOST \n", ((debug & DEBUG_SIGNPOST)?'X':' '), DEBUG_SIGNPOST );
      printf( " %c : 0x%04Xx : DEBUG_DATA_READER \n", ((debug & DEBUG_DATA_READER)?'X':' '), DEBUG_DATA_READER );
      printf( " %c : 0x%04Xx : DEBUG_DATA_READER \n", ((debug & DEBUG_DATA_READER)?'X':' '), DEBUG_DATA_READER );
      printf( " %c : 0x%04Xx : DEBUG_WARN_ENABLE \n", ((debug & DEBUG_WARN_ENABLE)?'X':' '), DEBUG_WARN_ENABLE );
      printf( "\nDebug value: 0x%04Xx \n", debug );
   }
   printf( " \n" );
   return;
}

int tokenize( char * instr, char tokens[MAX_TOKENS][MAX_STR_LEN] ) {
   char * cptr;
   int indx;
   char local_str[MAX_STR_LEN];
   
   /* initially, wipe the token array clean */
   for( indx=0; indx<MAX_TOKENS; indx++ ) {
      tokens[indx][0] = '\0';
   }
   
   if( strlen( instr ) < 2 ) {
      return(0);
   }
   
   strcpy( local_str, instr );
   cptr = strtok( local_str, " ,)(\n\t" );
   strcpy( tokens[0], cptr );
   
   for( indx=1; indx<MAX_TOKENS; indx++ ) {
      cptr = strtok( NULL, " ,)(\n\t" );
      if( (char*) NULL ==  cptr ) {
         break;
      } else {
         strcpy( tokens[indx], cptr );
      }
   }
   return(indx);
}
