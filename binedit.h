/* 
 * binedit.h
 *
 * Common header file for bin2hex, and hex2bin 
 *
 * R. Denise
 * December 2015
 */

#ifndef BINEDIT_H
#define BINEDIT_H

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define DEBUG_Level_0 0x00000001
#define DEBUG_Level_1 0x00000002
#define DEBUG_Level_2 0x00000004
#define DEBUG_Level_3 0x00000008
#define DEBUG_Level_4 0x00000010
#define DEBUG_Level_5 0x00000020
#define DEBUG_Level_6 0x00000040
#define DEBUG_Level_7 0x00000080
#define DEBUG_Level_8 0x00000100
#define DEBUG_Level_9 0x00000200
#define DEBUG_Level_A 0x00000400
#define DEBUG_Level_B 0x00000800
#define DEBUG_Level_C 0x00001000
#define DEBUG_Level_D 0x00002000
#define DEBUG_Level_E 0x00004000
#define DEBUG_Level_F 0x00008000

#define DEBUG_HELP        DEBUG_Level_0
#define DEBUG_SIGNPOST    DEBUG_Level_1
#define DEBUG_PEDANTIC    DEBUG_Level_2
#define DEBUG_DATA_READER DEBUG_Level_3
#define DEBUG_PROCESS     DEBUG_Level_4
#define DEBUG_OUTPUT      DEBUG_Level_5
#define DEBUG_FUNC_GUTS   DEBUG_Level_6
#define DEBUG_REPORT      DEBUG_Level_7
#define DEBUG_TREE_READER DEBUG_Level_8
#define DEBUG_TREE_COMPR  DEBUG_Level_9
#define DEBUG_RETROGRADE  DEBUG_Level_A
#define DEBUG_TIME_FUNC   DEBUG_Level_B
#define DEBUG_CURRENT     DEBUG_Level_C

#define DEBUG_HEX_ENABLE  DEBUG_Level_E
#define DEBUG_WARN_ENABLE DEBUG_Level_F

#define MAX_STR_LEN   256
#define MAX_TOKENS    100
#define DEFAULT_BYTES_PER_LINE  16

#endif /* BINEDIT_H */
