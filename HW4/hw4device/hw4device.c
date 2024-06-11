#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "hw4device"
#define BUFFER_SIZE 50

// file operation 함수들을 미리 선언
static int hw4device_open(struct inode *, struct file *);
static int hw4device_release(struct inode *, struct file *);
static ssize_t hw4device_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t hw4device_read(struct file *, char __user *, size_t, loff_t *);

// file operation을 등록
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = hw4device_open,
    .release = hw4device_release,
    .write = hw4device_write,
    .read = hw4device_read,
};

static int hw4device_open(struct inode *inode, struct file *file_ptr)
{
    printk(KERN_INFO "Device(%s) is opened!\n", DEVICE_NAME);
    return 0;
}

static int hw4device_release(struct inode *inode, struct file *file_ptr)
{
    printk(KERN_INFO "Device(%s) is closed!\n", DEVICE_NAME);
    return 0;
}

// user로부터 넘어온 문자열을 저장하는 버퍼
static char kernel_buffer[BUFFER_SIZE];
static int kernel_buffer_len = 0;

static ssize_t hw4device_write(struct file *file_ptr, const char __user *buffer, size_t length, loff_t *offset)
{
    // buffer -> kernel_buffer로 데이터 복사
    if (copy_from_user(kernel_buffer, buffer, length)) {
        return -EFAULT;
    }

    // kernel_buffer의 대소문자를 뒤집기
    for (int idx = 0; idx < length; ++idx) {
        if ('a' <= kernel_buffer[idx] && kernel_buffer[idx] <= 'z') {
            kernel_buffer[idx] -= 'a';
            kernel_buffer[idx] += 'A';
        }
        else if ('A' <= kernel_buffer[idx] && kernel_buffer[idx] <= 'Z') {
            kernel_buffer[idx] -= 'A';
            kernel_buffer[idx] += 'a';
        }
    }

    // kernel에 저장된 길이를 kernel_buffer_len에 저장
    kernel_buffer_len = length;

    return length;
}

static ssize_t hw4device_read(struct file *file_ptr, char __user *buffer, size_t length, loff_t *offset)
{
    // 현재 kernel에 저장된 값보다 더 많은 값을 읽으려고 할 때
    if (length > kernel_buffer_len) {
        length = kernel_buffer_len;
    }

    // 현재 kernel에 아무 값도 저장되지 않은 경우
    if (kernel_buffer_len == 0) {
        return 0;
    }

    // kernel_buffer -> buffer로 데이터 복사
    if (copy_to_user(buffer, kernel_buffer, length)) {
        return -EFAULT;
    }

    // 현재 kernel의 버퍼를 초기화
    kernel_buffer_len = 0;

    return length;
}

// 등록된 Major Number를 저장
static int major;

static int __init hw4device_init(void)
{
    // Character Device를 등록하고 Major Number를 받아옴
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        // 등록을 실패한 경우, 실패 메시지를 출력
        printk(KERN_ALERT "Failed to Register Device...\n");
        return major;
    }

    // 성공적으로 등록된 경우, 메시지로 출력
    printk(KERN_INFO "%s module is loaded. (major number is %d)\n", DEVICE_NAME, major);
    return 0;
}

static void __exit hw4device_cleanup(void)
{
    // 등록된 디바이스를 해제하고, 메시지로 출력
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "%s module is unloaded.\n", DEVICE_NAME);
}

// init과 cleanup 함수를 kernel에 등록
module_init(hw4device_init);
module_exit(hw4device_cleanup);
MODULE_LICENSE("GPL");