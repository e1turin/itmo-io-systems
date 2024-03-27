#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/version.h>


static dev_t first;
static struct cdev c_dev;
static struct class *cl;

#define MAX_HISTORY 1024
static uint64_t history[MAX_HISTORY] = {0};
static size_t   history_sz = 0;

#define STR_BUF_SIZE 1024
static char str_buf[STR_BUF_SIZE] = {0};

static int my_open(struct inode *i, struct file *f) {
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}

static int my_close(struct inode *i, struct file *f) {
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

static int num_put_user(char * fmt, size_t idx, char __user *buf) {
    int size = sprintf(str_buf, fmt, history[idx]);

    int j;
    for (j = 0; j < size; ++j) {
      // TODO: check out of user buffer
      put_user(str_buf[j], buf++);
    }

    return size;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len,
                       loff_t *off) {
  int bytes_read = 0;
  size_t num_idx;

  printk(KERN_INFO "Driver: read()\n");
  
  if (history_sz == 0) {
    pr_info("Empty history");
    return 0;
  }

  pr_info("History size: %lu\n", history_sz);
  
  // if (*off != 0) {
  //   return 0;
  // }

  // handle first number
  for (num_idx = 0; num_idx < history_sz - 1; ++num_idx) {
    bytes_read += num_put_user("%lld, ", num_idx, buf);
  }

  if (num_idx < history_sz) {
    bytes_read += num_put_user("%lld;\n", num_idx, buf);
  }

  // *off += bytes_read;
  *off = 0;

  return bytes_read;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len,
                        loff_t *off) {

  char cur_char       = 0;
  uint64_t create_num = 0;
  char reading_num    = 0;
  uint64_t mul_nums   = 1;
  char have_num       = 0;
  size_t c_iter;

  pr_info("Driver: write()\n");

  for (c_iter = 0; c_iter < len; ++c_iter){
    get_user(cur_char, buf + c_iter);

    if ('0' <= cur_char && cur_char <= '9') {
      have_num = 1;
      reading_num = 1;
      create_num = create_num * 10 + (cur_char - '0');
    } else {
      // Do I need to check EOF in input?
      if (reading_num) {
        mul_nums *= create_num;
        create_num = 0;
      }
      reading_num = 0;
    }
  }

  if (reading_num) {
    mul_nums *= create_num;
  }

  pr_info("History size: %lu\n", history_sz);
  if (have_num) {
    if (history_sz == MAX_HISTORY) {
      pr_info("History is full. Clearing history.");
      history_sz = 0;
      memset(history, 0, MAX_HISTORY * sizeof(uint64_t));
    }
    history[history_sz++] = mul_nums;
  }

  // Do I nead to support partial write?
  *off = len;
  // *off = 0; //say that it's EOF

  return len;
}

static struct file_operations mychdev_fops = {.owner = THIS_MODULE,
                                              .open = my_open,
                                              .release = my_close,
                                              .read = my_read,
                                              .write = my_write};

static int __init ch_drv_init(void) {
  printk(KERN_INFO "Module Init\n");
  if (alloc_chrdev_region(&first, 0, 1, "lab-1") < 0) {
    return -1;
  }
  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
    unregister_chrdev_region(first, 1);
    return -1;
  }
  if (device_create(cl, NULL, first, NULL, "var8") == NULL) {
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  cdev_init(&c_dev, &mychdev_fops);
  if (cdev_add(&c_dev, first, 1) == -1) {
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  return 0;
}

static void __exit ch_drv_exit(void) {
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "Bye!!!\n");
}

module_init(ch_drv_init);
module_exit(ch_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Author");
MODULE_DESCRIPTION("The first kernel module");

