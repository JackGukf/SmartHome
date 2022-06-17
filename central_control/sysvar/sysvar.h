#include <stdio.h>
#include <stdlib.h>

#ifndef _SYSVAR_H
#define _SYSVAR_H
#endif


int SYSVAR_fnConnectServer( void );

int SYSVAR_fnSet( const char *format, ...);

int SYSVAR_fnGet( const char *value, const char *format, ... );

int SYSVAR_fnDisconnectServer( void );
