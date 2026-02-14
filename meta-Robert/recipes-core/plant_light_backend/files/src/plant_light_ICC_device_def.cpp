#include "plant_light_ICC_device_def.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <cstdint>
#include <linux/i2c.h>


int ICC_Bus::single_write(uint8_t dev_addr, uint8_t set_val) {
    struct i2c_msg msgs = {
        dev_addr, 0, 1, &set_val
    };

    struct i2c_rdwr_ioctl_data rdwr = {&msgs, 1}; 

    int retval = ioctl(_icc_dev_fd, I2C_RDWR, &rdwr); 

    return retval; 
}

int ICC_Bus::set_dev_register(uint8_t dev_addr, uint8_t reg_addr, uint8_t reg_data) {
    struct i2c_msg msgs[2] = {
        {dev_addr, 0, 1, &reg_addr}, 
        {dev_addr, 0, 1, &reg_data}
    };
    
    struct i2c_rdwr_ioctl_data rdwr = {msgs, sizeof(msgs)}; 
    int retval = ioctl(_icc_dev_fd, I2C_RDWR, &rdwr); 

    return retval; 
}


int ICC_Bus::get_dev_register(uint8_t dev_addr, uint8_t reg_addr) {
    uint8_t reg_data = 0; 
    struct i2c_msg msgs[2] = {
        {
            .addr  = dev_addr,
            .flags = 0,              // write
            .len   = 1,
            .buf   = &reg_addr,
        },
        {
            .addr  = dev_addr,
            .flags = I2C_M_RD,       // read
            .len   = 1,
            .buf   = &reg_data,
        }
    };

    struct i2c_rdwr_ioctl_data rdwr = {
        .msgs  = msgs,
        .nmsgs = sizeof(msgs),
    };

    int retval = ioctl(_icc_dev_fd, I2C_RDWR, &rdwr); 

    printf("From dev %02X in register %d data: %d \n", 
        dev_addr, reg_addr, reg_data); 

    return retval; 

};


int ICC_Bus::batch_write(uint8_t dev_addr, uint8_t * tx_buffer, uint16_t len) {
    struct i2c_msg msgs = {
        dev_addr, 0, len, tx_buffer
    };

    struct i2c_rdwr_ioctl_data rdwr = {&msgs, 1}; 

    int retval = ioctl(_icc_dev_fd, I2C_RDWR, &rdwr); 

    return retval; 
}

int ICC_Bus::batch_read(uint8_t dev_addr, uint8_t * const rx_buffer, uint16_t len) {
    struct i2c_msg msgs = {
        dev_addr, I2C_M_RD, len, rx_buffer
    };

    struct i2c_rdwr_ioctl_data rdwr = {&msgs, 1}; 

    int retval = ioctl(_icc_dev_fd, I2C_RDWR, &rdwr); 

    return retval; 
}