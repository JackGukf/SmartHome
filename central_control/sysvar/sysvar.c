#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#define REDIS_SERVER_HOST_IP    ( "127.0.0.1" )
#define REDIS_SERVER_PORT       ( 6379 )

static redisContext *redisServerconn = NULL;

int SYSVAR_fnConnectServer( void )
{
    redisServerconn = redisConnect( REDIS_SERVER_HOST_IP, REDIS_SERVER_PORT );
    if (redisServerconn == NULL || redisServerconn->err)
    {
        if (redisServerconn)
        {
            printf("Error: %s\n", redisServerconn->errstr);
        }
        else
        {
            printf("Can't allocate redis context\n");
            return -1;
        }
    }

    /*redisReply *reply = redisCommand(conn, "set foo 123");
    freeReplyObject(reply);

    reply = redisCommand(conn, "get foo");
    printf("foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(conn, "set %s %s", "weather/humidity","95 %RH");
    freeReplyObject(reply);

    reply = redisCommand(conn, "get weather/humidity");
    printf("weather.temperature: %s\n", reply->str);
    freeReplyObject(reply);*/

    printf("Connected to Redis server successfully!\n");

    return 0;
}

int SYSVAR_fnSet( const char *format, ...)
{
    redisReply *reply;

    if( redisServerconn == NULL )
    {
        return -1;
    }
    va_list ap;
    va_start(ap,format);
    reply = redisCommand(redisServerconn, format, ap );
    va_end(ap);

    freeReplyObject(reply);

    return 0;
}

int SYSVAR_fnGet( const char *value, const char *format, ... )
{
    redisReply *reply;

    if( redisServerconn == NULL )
    {
        return -1;
    }

    va_list ap;
    va_start(ap,format);
    reply = redisCommand(redisServerconn, format, ap );
    va_end(ap);

    strcpy( value, reply->str );

    //string copy
    freeReplyObject(reply);

    return 0;
}


int SYSVAR_fnDisconnectServer( void )
{
    if( redisServerconn == NULL )
    {
        return -1;
    }


    redisFree(redisServerconn);

    return 0;
}
