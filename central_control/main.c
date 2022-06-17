#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <hiredis/hiredis.h>


#include "sysvar.h"
#include "weather.h"
#include "irrigation.h"

#ifdef _MSC_VER
#include <winsock2.h> /* For struct timeval */
#endif

#define PIPE_ENABLE

int main(int argc, char **argv) {

    char value[100] = {};
    pthread_t periodicUpdateThread;
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pid_t pid;
    int pipefd[2];

    int ret;
    char descT1[] = {"Thread 1"};
    char descT2[] = {"Thread 2"};
    char descT3[] = {"Thread W Sensor"};
    char descT4[] = {"Thread irrigation"};
    int i = 0;
    int j = 0;
    char tx[30];
    char rx[30];

    /*redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (conn == NULL || conn->err) {
        if (conn) {
            printf("Error: %s\n", conn->errstr);
        } else {
            printf("Can't allocate redis context\n");
        }
    }

    redisReply *reply = redisCommand(conn, "set foo 123");
    freeReplyObject(reply);

    reply = redisCommand(conn, "get foo");
    printf("foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(conn, "set %s %s", "weather/humidity","95 %RH");
    freeReplyObject(reply);

    reply = redisCommand(conn, "get weather/humidity");
    printf("weather.temperature: %s\n", reply->str);
    freeReplyObject(reply);

    redisFree(conn);*/

    // create pipe
    if(pipe(pipefd) == -1 )
    {
        printf("Failed to create pipe.\n");
        exit(EXIT_FAILURE);
    }

    // create child process
    printf("Create child process.\n");
    pid = fork();

    if( pid == -1 )
    {
        printf("Failed to create child process.\n");
        exit(EXIT_FAILURE);
    }
    else if( pid == 0 )
    {
        //child process
        printf( "child process, pid = %u\n", getpid() );
        printf( "parent of child process, pid = %u\n", getppid() );

        #ifdef PIPE_ENABLE

        strcpy(tx, "Data from child process!");
        // close read fd
        close(pipefd[0]);

        write(pipefd[1], tx, strlen(tx));

        printf("Child process write data to parent: %s\n", tx);
        #endif


        ret = pthread_create( &t4, NULL, IRRIGATION_fnUpdate, descT4 );
        if( ret != 0 )
        {
            printf("Failed to create thread.\n");
        }
        else
        {
            printf("Succeed to create thread in child process.\n");
        }

        // to run process for short period
        while(j<10)
        {
            sleep(1);
            j++;
            printf("Child process delay cnt: %d.\n", j);
        }
    }
    else
    {
        // parent process
        SYSVAR_fnConnectServer();

        #ifdef PIPE_ENABLE
        //wait for child process to finish
        //wait(NULL);
        // close write fd
        close(pipefd[1]);

        read(pipefd[0], rx, 24);

        close(pipefd[0]);

        printf("Parent received data: %s \n", rx);
        #endif

        printf("Create threads \n");

        ret = pthread_create( &periodicUpdateThread, NULL, WEATHER_fnUpdate, NULL );

        ret = pthread_create( &t1, NULL, WEATHER_fnIncreaseTemperature, descT1 );

        ret = pthread_create( &t2, NULL, WEATHER_fnIncreaseTemperature, descT2 );

        ret = pthread_create( &t3, NULL, WEATHER_fnSensorIsRead, descT3 );

        printf("Thread info: %d %d %d \n", t1, t2, t3);



        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);


        printf("Query sysvar \n");

        SYSVAR_fnGet( value, "get weather/humidity");

        printf("weather.humidity: %s\n", value);

        // to run process for short period
        while(i<10)
        {
            sleep(1);
            i++;
            printf("Parent process delay cnt: %d.\n", i);
        }

        SYSVAR_fnDisconnectServer();
    }


    return 0;
}
