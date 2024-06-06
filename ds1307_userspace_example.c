#include <stdio.h>
#include <stdlib.h>
#include "ds1307_library.h"
int date[7];

int main(void)
{
    ds1307Open();
    //setCustom(25, 1, 2024, 8, 40, 40, 5);
    printf("Thời gian và ngày hiện tại:\n");
    setTime();
    getTime(date);
    printf("Ngày: %02d-%02d-%02d\n", date[0], date[1], date[2]);
    printf("Giờ: %02d:%02d:%02d\n", date[3], date[4], date[5]);
    printf("Thứ: %02d\n", date[6]);
    ds1307Close();
    while(1)
    {

    }
    return 0;
}