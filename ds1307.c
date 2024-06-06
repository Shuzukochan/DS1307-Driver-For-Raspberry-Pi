#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/timekeeping.h>

#define DRIVER_NAME "ds1307_module"
#define CLASS_NAME "ds1307"
#define DEVICE_NAME "ds1307"
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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Uy, Vu, Hieu, Huy");
MODULE_DESCRIPTION("Project Cuoi Ki - He Thong Nhung");

static struct i2c_client *adc_client;
static struct class* ds1307_class = NULL;
static struct device* ds1307_device = NULL;
static int major_number;

static int my_adc_probe(struct i2c_client *client, const struct i2c_device_id *id);
static void my_adc_remove(struct i2c_client *client);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "maxim,ds1307_module",
	}, { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct i2c_driver my_driver = {
	.probe = my_adc_probe,
	.remove = my_adc_remove,
	.driver = {
		.name = "ds1307_module",
		.of_match_table = my_driver_ids,
	},
};


static u8 dec_to_bcd(u8 val) {
    return ((val / 10 * 16) + (val % 10));
}

static u8 bcd_to_dec(u8 bcd) {
    return ((bcd / 16 * 10) + (bcd % 16));
}

static int ds1307_get_time(struct i2c_client *client)
{
    struct timespec64 ts;
    struct tm tm;
    char buffer[8];
    int ret;
    ktime_get_real_ts64(&ts); // Lấy thời gian hệ thống
    time64_to_tm(ts.tv_sec, 7*3600, &tm); // Chuyển đổi thời gian thành cấu trúc struct tm, thêm 7 giờ cho múi giờ +7
    buffer[0] = 0x00; // Địa chỉ thanh ghi cần ghi
    buffer[7] = dec_to_bcd(((tm.tm_year + 1900) % 100)); // Năm (dưới dạng hai chữ số cuối)
    buffer[6] = dec_to_bcd(tm.tm_mon + 1); // Tháng (giá trị từ 1-12)
    buffer[5] = dec_to_bcd(tm.tm_mday); // Ngày
    buffer[4] = dec_to_bcd(tm.tm_wday); // Ngày trong tuần
    buffer[3] = dec_to_bcd(tm.tm_hour); // Giờ
    buffer[2] = dec_to_bcd(tm.tm_min); // Phút
    buffer[1] = dec_to_bcd(tm.tm_sec); // Giây

    ret = i2c_master_send(client, buffer, 8);
    if (ret < 0) {
        pr_err("Failed to write to the i2c bus.\n");
        return ret;
    }
    ret = i2c_master_send(client, buffer, 1);
    if (ret < 0) {
        pr_err("Failed to write to the i2c bus.\n");
        return ret;
    }
    return 0;
};

static int ds1307_write_data(struct i2c_client *client, char data[8])
{
    int ret;
    char buffer[8];
    for (int i = 0; i < 8; i++) {
        buffer[i] = dec_to_bcd(data[i]); // Địa chỉ thanh ghi cần ghi
    }

    ret = i2c_master_send(client, buffer, 8);
    if (ret < 0) {
        pr_err("Failed to write to the i2c bus.\n");
        return ret;
    }
    ret = i2c_master_send(client, buffer, 1);
    if (ret < 0) {
        pr_err("Failed to write to the i2c bus.\n");
        return ret;
    }
    return 0;
};

static int ds1307_read_data(struct i2c_client *client, int stt)
{
    int ret;
    int num[7];
    char buffer[7];
    // Send the register address
    buffer[0] = 0x00;
    ret = i2c_master_send(client, buffer, 1);
    if (ret < 0) {
        pr_err("Failed to write to the i2c bus.\n");
        return ret;
    }
    // Read the data
    ret = i2c_master_recv(client, buffer, 7);
    if (ret < 0) {
        pr_err("Failed to read from the i2c bus.\n");
        return ret;
    }
    // Chuyển đổi giá trị BCD sang thập phân
    num[6] = (bcd_to_dec(buffer[6]) + 2000) ; // Năm (2 chữ số cuối)
    num[5] = bcd_to_dec(buffer[5]); // Tháng
    num[4] = bcd_to_dec(buffer[4]); // Ngày
    num[3] = bcd_to_dec(buffer[3]); // Ngày trong tuần
    num[2] = bcd_to_dec(buffer[2]); // Giờ
    num[1] = bcd_to_dec(buffer[1]); // Phút
    num[0] = bcd_to_dec(buffer[0]); // Giây
    return num[stt];
}

static long ds1307_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int data;
    char data_char[8];

    if (copy_from_user(&data_char, (int __user *)arg, sizeof(data_char))) 
    {
        return -EFAULT;
    }

    switch (cmd) {
        case ds1307_ioctl_Read_Second:
            data = ds1307_read_data(adc_client, 0);
            break;
        case ds1307_ioctl_Read_Minute:
            data = ds1307_read_data(adc_client, 1);
            break;
        case ds1307_ioctl_Read_Hour:
            data = ds1307_read_data(adc_client, 2);
            break;
        case ds1307_ioctl_Read_Dayofweek:
            data = ds1307_read_data(adc_client, 3);
            break;
        case ds1307_ioctl_Read_Day:
            data = ds1307_read_data(adc_client, 4);
            break;
        case ds1307_ioctl_Read_Month:
            data = ds1307_read_data(adc_client, 5);
            break;
        case ds1307_ioctl_Read_Year:
            data = ds1307_read_data(adc_client, 6);
            break;
        case ds1307_ioctl_Get_Time:
            data = ds1307_get_time(adc_client);
            break;
        case ds1307_ioctl_Write_Data:
            ds1307_write_data(adc_client, data_char);
            break;
        default:
            return -EINVAL;
    }

    if (copy_to_user((int __user *)arg, &data, sizeof(data))) {
        return -EFAULT;
    }
    return 0;
} 

static int ds1307_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "DS1307 device opened\n");
    return 0;
}

static int ds1307_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "DS1307 device closed\n");
    return 0;
}

static struct file_operations fops = {
    .open = ds1307_open,
    .unlocked_ioctl = ds1307_ioctl, 
    .release = ds1307_release,
};


static int my_adc_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    adc_client = client;

    // Create a char device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ERR "Loi khi dang ki major number\n");
        return major_number;
    }

    ds1307_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ds1307_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Loi khi dang ki device class\n");
        return PTR_ERR(ds1307_class);
    }

    ds1307_device = device_create(ds1307_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ds1307_device)) {
        class_destroy(ds1307_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Loi khi dang ki the device\n");
        return PTR_ERR(ds1307_device);
    }

    printk(KERN_INFO "DS1307 driver installed\n");
    return 0;
}

static void my_adc_remove(struct i2c_client *client)
{
    device_destroy(ds1307_class, MKDEV(major_number, 0));
    class_unregister(ds1307_class);
    class_destroy(ds1307_class);
    unregister_chrdev(major_number, DEVICE_NAME);

    printk(KERN_INFO "DS1307 driver removed\n");
}

/* Hàm tạo init and exit tự động */
//module_i2c_driver(my_driver);

static int __init ds1307_init(void)
{
    printk(KERN_INFO "Initializing DS1307 driver\n");
    return i2c_add_driver(&my_driver);
}

static void __exit ds1307_exit(void)
{
    printk(KERN_INFO "Exiting DS1307 driver\n");
    i2c_del_driver(&my_driver);
}

module_init(ds1307_init);
module_exit(ds1307_exit);
