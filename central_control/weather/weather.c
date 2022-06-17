#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "weather.h"
#include "sysvar.h"

pthread_mutex_t lock;
int temperature = 25;

pthread_cond_t weatherReady;
pthread_mutex_t weatherUpdateLock;


void * WEATHER_fnUpdate( void *ptr )
{
    char temp[20];
    while(1)
    {
        pthread_mutex_lock(&weatherUpdateLock);
        SYSVAR_fnSet("set %s %s", "weather/humidity","95 %RH");
        //sprintf( temp, "%d", temperature );

        //printf("String is %s \n", temp);
        //SYSVAR_fnSet("set %s %s %s", "weather/temperature", temp, " degree");
        SYSVAR_fnSet("set %s %s", "weather/temperature", "25 degree");

        printf("Waiting signal to update weather info. \n");
        // sleep for  1 sec
        //sleep(1);

        pthread_cond_wait(&weatherReady, &weatherUpdateLock);

        pthread_mutex_unlock(&weatherUpdateLock);
    }

    return NULL;
}

void *WEATHER_fnIncreaseTemperature( void *ptr )
{
    int i = 0;
    while(i< 2)
    {

        pthread_mutex_lock(&lock);

        printf("%s Got the lock. \n", (char *)ptr);

        temperature++;

        printf("Temperature: %d.\n", temperature);

        pthread_mutex_unlock(&lock);

        printf("%s Released the lock. \n", (char *)ptr );
        i++;
    }

}

void *WEATHER_fnSensorIsRead( void *ptr )
{
    int i = 0;
    while(i<5)
    {
        pthread_mutex_lock(&weatherUpdateLock);

        printf("%s Send signal to update weather info. \n", (char *)ptr );

        pthread_cond_signal(&weatherReady);

        pthread_mutex_unlock(&weatherUpdateLock);

        sleep(5);
        i++;

    }
}
