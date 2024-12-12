# DS1307 Driver for Raspberry Pi

This project provides a driver to work with the DS1307 RTC module on a Raspberry Pi. Below are the installation instructions and usage details.

---

## Installation

Follow these steps to install the driver on your Raspberry Pi:

1. Clone the repository using the command:
   ```bash
   git clone https://github.com/Shuzukochan/DS1307-Driver-For-Raspberry-Pi
   ```

2. Navigate to the downloaded directory:
   ```bash
   cd DS1307-Driver-For-Raspberry-Pi
   ```

3. Copy the `ds1307_module.dtbo` file to the `/boot/overlay` directory:
   ```bash
   sudo cp ds1307_module.dtbo /boot/overlay
   ```

4. Edit the `config.txt` file in the `/boot` directory to add the following line:
   ```text
   dtoverlay=ds1307_module
   ```
   Save and reboot the Raspberry Pi.

5. Load the driver using:
   ```bash
   sudo insmod ds1307.ko
   ```
   Use the `dmesg` command to check if the driver has been installed. Look for a message like:
   ```
   DS1307 Driver installed
   ```

   If you want to unload the driver later, use:
   ```bash
   sudo rmmod ds1307_module.dtbo
   ```

---

## Driver ioctl Functions

The driver provides several ioctl functions to interact with the DS1307 module:

### Reading Values

1. **Read Seconds**
   ```c
   ioctl(fd, ds1307_ioctl_Read_Second, &second);
   printf("Second: %d", second);
   ```

2. **Read Minutes**
   ```c
   ioctl(fd, ds1307_ioctl_Read_Minute, &minute);
   printf("Minute: %d", minute);
   ```

3. **Read Hours**
   ```c
   ioctl(fd, ds1307_ioctl_Read_Hour, &hour);
   printf("Hour: %d", hour);
   ```

4. **Read Day of the Week**
   ```c
   ioctl(fd, ds1307_ioctl_Read_Dayofweek, &dayofweek);
   printf("Day of Week: %d", dayofweek + 1); // Sunday = 1
   ```

5. **Read Day**
   ```c
   ioctl(fd, ds1307_ioctl_Read_Day, &day);
   printf("Day: %d", day);
   ```

6. **Read Month**
   ```c
   ioctl(fd, ds1307_ioctl_Read_Month, &month);
   printf("Month: %d", month);
   ```

7. **Read Year**
   ```c
   ioctl(fd, ds1307_ioctl_Read_Year, &year);
   printf("Year: %d", year);
   ```

### Writing Values

- **Write Custom Date and Time**
   ```c
   buffer[8] = {0, 10, 20, 04, 03, 06, 06, 24};
   ioctl(fd, ds1307_ioctl_Write_Data, buffer);
   ```

### Reading System Time

- **Get Current System Time**
   ```c
   ioctl(fd, ds1307_ioctl_Get_Time, &get);
   ```

---

## License

This project is licensed under the MIT License. Feel free to use and modify it.
