.func begin vprintf_s vwprintf_s
.funcw vwprintf_s
.func end
.synop begin
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdarg.h>
#include <stdio.h>
int vprintf_s( const char * restrict format, va_list arg );
.ixfunc2 '&StrIo' &func
.if &'length(&wfunc.) ne 0 .do begin
#include <stdarg.h>
#include <wchar.h>
int vwprintf_s( const wchar_t * restrict format, va_list prg );
.ixfunc2 '&StrIo' &wfunc
.ixfunc2 '&Wide' &wfunc
.do end
.synop end
.*
.rtconst begin
The
.arg format
argument shall not be a null pointer. The
.mono %n
specifier (modified or not by flags, field width, or precision) shall not
appear in the string pointed to by
.arg format
.ct .li .
Any argument to
.id &func.
corresponding to a
.mono %s
specifier shall not be a null pointer.
.np
If there is a runtime-constraint violation, the
.id &func.
function does not
attempt to produce further output, and it is unspecified to what extent
.id &func.
produced output before discovering the runtime-constraint violation.
.rtconst end
.*
.desc begin
The
.id &func.
function is equivalent to the
.kw vprintf
function except for the explicit runtime-constraints listed above.
.if &'length(&wfunc.) ne 0 .do begin
.np
The
.id &wfunc.
function is identical to
.id &func.
except that it accepts a
wide-character string argument for
.arg format
.ct .li .
.do end
.desc end
.*
.return begin
The
.id &func.
function returns the number of characters written, or a
negative value if an output error or runtime-constraint violation occurred.
.if &'length(&wfunc.) ne 0 .do begin
.np
The
.id &wfunc.
function returns the number of wide characters written, or a
negative value if an output error or runtime-constraint violation occurred.
.do end
.return end
.*
.see begin
.im seeprtf vprintf_s
.see end
.*
.exmp begin
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdarg.h>
.exmp break
void errmsg( char *format, ... )
{
    va_list arglist;

    printf_s( "Error: " );
    va_start( arglist, format );
    vprintf_s( format, arglist );
    va_end( arglist );
}
.exmp break
void main( void )
{
    errmsg( "%s %d %s", "Failed", 100, "times" );
}
.exmp output
Error: Failed 100 times
.exmp end
.*
.class TR 24731
.system
