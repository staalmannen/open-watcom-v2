/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Executable Strip Utility output routines.
*
****************************************************************************/


#include <string.h>
#include <stdlib.h>
#ifdef __WATCOMC__
#include <process.h>
#endif
#include "wio.h"
#include "watcom.h"
#include "wstrip.h"
#include "banner.h"
#include "clibext.h"

#define RESOURCE_MAX_SIZE       128

#if defined( INCL_MSGTEXT )

static char *StringTable[] = {
    "",                             // message ID's start at 1
    #include "incltext.gh"
};

int Msg_Init( void )
{
    return( EXIT_SUCCESS );
}

static int Msg_Get( int resourceid, char *buffer )
{
    strcpy( buffer, StringTable[resourceid] );
    return( EXIT_SUCCESS );
}

int Msg_Fini( void )
{
    return( EXIT_SUCCESS );
}

#else

#include "wressetr.h"
#include "wresset2.h"
#include "wreslang.h"

#define NO_RES_MESSAGE "Error: could not open message resource file.\r\n"
#define NO_RES_SIZE (sizeof(NO_RES_MESSAGE)-1)

static  HANDLE_INFO     hInstance = { 0 };
static  int             Res_Flag;
static  unsigned        MsgShift;

static long res_seek( WResFileID handle, long position, int where )
/* fool the resource compiler into thinking that the resource information
 * starts at offset 0 */
{
    if( where == SEEK_SET ) {
        return( lseek( handle, position + FileShift, where ) - FileShift );
    } else {
        return( lseek( handle, position, where ) );
    }
}


/* declare struct WResRoutines WResRtns {...} */
WResSetRtns( open, close, read, write, res_seek, tell, malloc, free );

static int Msg_Get( int resourceid, char *buffer )
{
    if ( LoadString( &hInstance, resourceid + MsgShift, (LPSTR)buffer, RESOURCE_MAX_SIZE ) != 0 ) {
        buffer[0] = '\0';
        return( 0 );
    }
    return( 1 );
}


int Msg_Init( void )
{
    char        name[_MAX_PATH];

    hInstance.handle = NIL_HANDLE;
    if( _cmdname( name ) != NULL && !OpenResFile( &hInstance, name ) ) {
        if( !FindResources( &hInstance ) && !InitResources( &hInstance ) ) {
            MsgShift = _WResLanguage() * MSG_LANG_SPACING;
            if( Msg_Get( MSG_USAGE_FIRST, name ) ) {
                Res_Flag = EXIT_SUCCESS;
                return( Res_Flag );
            }
        }
        CloseResFile( &hInstance );
        hInstance.handle = NIL_HANDLE;
    }
    Res_Flag = EXIT_FAILURE;
    write( STDOUT_FILENO, NO_RES_MESSAGE, NO_RES_SIZE );
    return Res_Flag;
}


int Msg_Fini( void )
{
    int     retcode = EXIT_SUCCESS;

    if( Res_Flag == EXIT_SUCCESS ) {
        if ( CloseResFile( &hInstance ) ) {
            Res_Flag = EXIT_FAILURE;
            retcode = EXIT_FAILURE;
        }
        hInstance.handle = NIL_HANDLE;
    }
    return( retcode );
}

#endif

static void Outs( int nl, char *s )
{
    write( STDOUT_FILENO, s, strlen( s ) );
    if( nl ) write( STDOUT_FILENO, "\r\n", 2 );
}

static void Outc( char c )
{
    write( STDOUT_FILENO, &c, 1 );
}

void Banner( void )
{
    Outs( 1, banner1w( "Executable Strip Utility", _WSTRIP_VERSION_ ) );
    Outs( 1, banner2 );
    Outs( 1, banner2a( "1988" ) );
    Outs( 1, banner3 );
    Outs( 1, banner3a );
}

void Usage( void )
{
    char        msg_buffer[RESOURCE_MAX_SIZE];
    int         i;

    for( i = MSG_USAGE_FIRST; i <= MSG_USAGE_LAST; i++ ) {
        Msg_Get( i, msg_buffer );
        Outs( 1, msg_buffer );
    }
    Msg_Fini();
    exit( -1 );
}

void Fatal( int reason, char *insert )
/* the reason doesn't have to be good */
{
    char        msg_buffer[RESOURCE_MAX_SIZE];
    int         i = 0;

    Msg_Get( reason, msg_buffer );
    while( msg_buffer[i] != '\0' ) {
        if( msg_buffer[i] == '%' ) {
            if( msg_buffer[i+1] == 's' ) {
                Outs( 0, insert );
            } else {
                Outc( msg_buffer[i+1] );
            }
            i++;
        } else {
            Outc( msg_buffer[i] );
        }
        i++;
    }
    Msg_Get( MSG_WSTRIP_ABORT, msg_buffer );
    Outs( 1, msg_buffer );
    Msg_Fini();
    exit( -1 );
}
