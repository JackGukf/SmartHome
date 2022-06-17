#include <stdio.h>
#include <stdlib.h>

#ifndef _WEATHER_H
#define _WEATHER_H
#endif


extern pthread_cond_t weatherReady;
extern pthread_mutex_t weatherUpdateLock;

void * WEATHER_fnUpdate( void *ptr );

void *WEATHER_fnIncreaseTemperature( void *ptr );

void *WEATHER_fnSensorIsRead( void *ptr );
