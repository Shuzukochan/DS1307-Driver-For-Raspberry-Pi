# DS1307 Driver for Raspberry Pi

This project provides a driver to work with the DS1307 RTC module on a Raspberry Pi. The driver is compatible with Raspberry Pi OS 32-bit (Legacy). Below are the library setup and usage details.

---

## Hardware Connection

To connect the DS1307 module to the Raspberry Pi 4, use the following wiring table:

| Raspberry Pi 4 | DS1307   |
|----------------|----------|
| 5V Power       | VCC      |
| GND            | GND      |
| GPIO 2 (SDA)   | SDA      |
| GPIO 3 (SCL)   | SCL      |

---

## Library Setup and Usage

### 1. Library Setup

#### 1.1 Install Dependencies and Setup

1. Install the Raspberry Pi kernel headers:
   ```bash
   sudo apt-get install raspberrypi-kernel-headers
   ```

2. Clone the driver repository from GitHub:
   ```bash
   git clone https://github.com/Shuzukochan/DS1307-Driver-For-Raspberry-Pi-4.git
   ```

3. Copy the overlay file to the appropriate directory:
   ```bash
   cd DS1307-Driver-For-Raspberry-Pi-4/
   sudo cp ds1307_module.dtbo /boot/overlays
   ```

4. Modify the `/boot/config.txt` file by adding the following lines:
   ```txt
   dtoverlay=ds1307_module
   arm_64bit=0
   dtparam=i2c_arm=on
   ```

5. Reboot the Raspberry Pi:
   ```bash
   sudo reboot
   ```

6. Insert the DS1307 kernel module:
   ```bash
   cd DS1307-Driver-For-Raspberry-Pi-4/
   make
   sudo insmod ds1307.ko
   ```

#### 1.2 Compile Your Program

1. Ensure the files `ds1307_library.c`, `ds1307_library.h`, and `libds1307_library.a` are in the same folder as your program.

2. Compile your program with the following command:
   ```bash
   gcc codecuaoban.c -L. -lds1307_library -o codecuaoban
   ```

3. Run your compiled program:
   ```bash
   sudo ./codecuaoban
   ```

### 2. Library Functions

#### 2.1 Open Module

```c
ds1307Open();
```
**Description:** Opens the module and initializes the I2C connection.

#### 2.2 Set System Time on the Raspberry Pi

```c
setTime();
```
**Description:** Reads the current system time from the Raspberry Pi and writes it to the module.

#### 2.3 Set Custom Time

```c
setCustom(int dayOfWeek, int day, int month, int year, int hour, int min, int sec);
```
**Description:** Sets a custom time in the module based on user-defined values.

**Note:** Parameters must be provided in the order of day of the week, day, month, year, hour, minute, second.

#### 2.4 Read Time

```c
getTime(date);
```
**Description:** Reads the time stored in the module and returns the values.

**Note:** Declare an array `int date[7]` before use. The returned values are as follows:
- `date[0]` = Day
- `date[1]` = Month
- `date[2]` = Year
- `date[3]` = Hour (24-hour format)
- `date[4]` = Minute
- `date[5]` = Second
- `date[6]` = Day of the week (Sunday = 1)

#### 2.5 Close Module

```c
ds1307Close();
```
**Description:** Closes the module and terminates the I2C connection.

**Note:** This function must be called after using the module.

---

## Example Program

You can test the driver with the following example program:

Run the example program with:
```bash
./ds1307_userspace_example
```

ds1307_userspace_example is an example program to write the time to the module (from user input or system time), read the time from the module, and display it on the terminal:

```c
#include <stdio.h>
#include <stdlib.h>
#include "ds1307_library.h"

int date[7];

int main(void) {
    ds1307Open();

    // Uncomment the following line to set custom time
    // setCustom(5, 25, 5, 2024, 8, 40, 40);
    
    setTime();
    getTime(date);

    printf("Current date and time from the module:\n");
    printf("Date: %02d-%02d-%02d\n", date[0], date[1], date[2]);
    printf("Time: %02d:%02d:%02d\n", date[3], date[4], date[5]);
    printf("Day of the week: %d\n", date[6]);

    ds1307Close();
    while (1);

    return 0;
}
```

---

### Notes:
- This driver has been tested on Raspberry Pi OS Legacy 32-bit (Bullseye).
- The kernel module `ds1307.ko` and the device overlay `ds1307_module.dtbo` are required for the driver to work properly.
- Ensure that you have configured I2C properly on your Raspberry Pi before using the module.

---
