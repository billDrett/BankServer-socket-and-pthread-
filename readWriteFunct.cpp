#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "readWriteFunct.h"

int read_data(int fd, char* buffer) //diavasei ta dedomena ths gramhs me morfh "size data..."
{
    char tmp[5];
    int len, nread = 0;
    const int sizeLenght = 4;

    for(int i = 0; i < sizeLenght; i += nread) //ta 4 prwta byte einai to megethos se byte twn data
    {
        if((nread = read(fd, &tmp[i], sizeLenght-i)) <= 0)
        {
            return -1;
        }
    }
    tmp[4] = '\0';
    len = atoi(tmp);

    read(fd, tmp, 1); //agnow to space

    for(int i = 0; i < len; i += nread) //diavasw ta dedomena tou buffer
    {
        if((nread = read(fd, &buffer[i], len-i)) <= 0)
        {
            return -1;
        }
    }

    return len;
}

int numberOfDigits(int num) //epistrefei to plithos twn psifion toy aritmou
{
    if(num == 0) return 1;

    int i = 1;
    while((num /= 10) != 0)
    {
        i++;
    }

    return i;
}

void write_data(int fd, const char* buffer)
{
    int len = strlen(buffer) + 1;
    char lenString[6];

    for(int i = 0; i < 4 - numberOfDigits(len); i++) //gemizw me midenika mexri na exw akrivos 4 byte gia to plithos tou buffer
    {
        lenString[i] = '0';
    }
    sprintf(&lenString[4-numberOfDigits(len)], "%d ", len);

    write(fd, lenString, 5);
    write(fd, buffer, len);
}
