#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include "irrigation.h"
#include "sysvar.h"


void * IRRIGATION_fnUpdate( void *ptr )
{
    //#ifdef log_enable
    int i = 0;
    #ifdef OPEN_FILE_SYSTEM_CALL
    // open is linux system call
    int fd = open("irrgLog.txt", O_RDWR | O_CREAT);
    printf("irrgLog fd = %d\n", fd);

    if(fd==-1)
    {
        printf("Failed to create file!");
    }
    #else
    FILE *fp = fopen("irrgLog.txt", "r+");
    #endif

    if( NULL == fp )
    {
        printf("Failed to create irrgLog.txt file!");
    }

    printf("File position: %d before fseek\n", ftell(fp));

    #ifndef OPEN_FILE_SYSTEM_CALL
    // move pointer to end of file with 0 offset
    //fseek(fp, 0, SEEK_END);
    // move pointer to begin of file with 0 offset
    fseek(fp, 0, SEEK_SET);
    // move pointer to current cursor with 0 offset
    //fseek(fp, 0, SEEK_CUR);
    #endif

    printf("File position: %d after fseek\n", ftell(fp));
    while(1)
    {
        printf("Irrigation schedule update. \n");

        //#ifdef log_enable
        if( i < 10 )
        {
            #ifdef OPEN_FILE_SYSTEM_CALL
            write(fd, "irrigation log\n", strlen("irrigation log\n"));
            #else
            fwrite("irrigation log 2\n", 1, strlen("irrigation log 2\n"), fp );
            #endif
            i++;
        }

        if( i == 10 )
        {
            #ifdef OPEN_FILE_SYSTEM_CALL
            close(fd);
            #else
            fclose(fp);
            #endif
        }
        //#endif

        // sleep for  1 sec
        sleep(5);
    }

    return NULL;
}

void * IRRIGATION_fnUpdate2( void *ptr )
{
    printf("Irrigation schedule update 2. \n");

    return NULL;
}
