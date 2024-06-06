#define DRIVER_NAME "ds1307_module"
#define CLASS_NAME "ds1307"
#define DEVICE_NAME "ds1307"
#define DEVICE_PATH "/dev/ds1307"
// IOCTL commands
#define ds1307_ioctl_MAGIC 'm'
#define ds1307_ioctl_Read_Second _IOR(ds1307_ioctl_MAGIC, 1, int)
#define ds1307_ioctl_Read_Minute _IOR(ds1307_ioctl_MAGIC, 2, int)
#define ds1307_ioctl_Read_Hour _IOR(ds1307_ioctl_MAGIC, 3, int)
#define ds1307_ioctl_Read_Dayofweek _IOR(ds1307_ioctl_MAGIC, 4, int)
#define ds1307_ioctl_Read_Day _IOR(ds1307_ioctl_MAGIC, 5, int)
#define ds1307_ioctl_Read_Month _IOR(ds1307_ioctl_MAGIC, 6, int)
#define ds1307_ioctl_Read_Year _IOR(ds1307_ioctl_MAGIC, 7, int)
#define ds1307_ioctl_Write_Data _IOW(ds1307_ioctl_MAGIC, 8, char)
#define ds1307_ioctl_Get_Time _IOR(ds1307_ioctl_MAGIC, 9, int)


static int fd;
void ds1307Open(void);
void ds1307Close(void);
void setTime(void);
int *getTime(int *date);
void setCustom(int day, int month, int year, int hour, int min, int sec, int dayOfWeek);