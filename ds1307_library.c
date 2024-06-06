#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ds1307_library.h"

char buffer[8];

void ds1307Open(void)
{
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device");
    }
}

void setTime(void)
{
    int get;
    ioctl(fd, ds1307_ioctl_Get_Time, &get);
}

int *getTime(int *date)
{   
    int second, minute, hour, year, month, day, dayofweek;
    ioctl(fd, ds1307_ioctl_Read_Second, &second);
    ioctl(fd, ds1307_ioctl_Read_Minute, &minute);
    ioctl(fd, ds1307_ioctl_Read_Hour, &hour);
    ioctl(fd, ds1307_ioctl_Read_Dayofweek, &dayofweek);
    ioctl(fd, ds1307_ioctl_Read_Year, &year);
    ioctl(fd, ds1307_ioctl_Read_Month, &month);
    ioctl(fd, ds1307_ioctl_Read_Day, &day);

    date[0] = day;
    date[1] = month;
    date[2] = year;
    date[3] = hour;
    date[4] = minute;
    date[5] = second;
    date[6] = dayofweek + 1;
    return date;
}

void setCustom(int dayOfWeek, int day, int month, int year, int hour, int min, int sec)
{
    buffer[0] = 0x00;
    buffer[1] = sec;
    buffer[2] = min;
    buffer[3] = hour;
    buffer[4] = dayOfWeek - 1;
    buffer[5] = day;
    buffer[6] = month;
    buffer[7] = year % 100;
    ioctl(fd, ds1307_ioctl_Write_Data, buffer);
}


void ds1307Close(void)
{
    close(fd);
}
