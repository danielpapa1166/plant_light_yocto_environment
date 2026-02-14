#pragma once 


#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

struct ICC_Bus {
public:
    static ICC_Bus& instance() {
        static ICC_Bus instance;   // thread-safe since C++11
        return instance;
    }

    ICC_Bus(const ICC_Bus&) = delete;
    ICC_Bus& operator=(const ICC_Bus&) = delete;

    int single_write(uint8_t dev_addr, uint8_t set_val); 
    int set_dev_register(uint8_t dev_addr, uint8_t reg_addr, uint8_t reg_data); 
    int get_dev_register(uint8_t dev_addr, uint8_t reg_addr); 
    int batch_write(uint8_t dev_addr, uint8_t * tx_buffer, uint16_t len); 
    int batch_read(uint8_t dev_addr, uint8_t * const rx_buffer, uint16_t len); 

private:
    const char * _icc_dev_name = "/dev/i2c-1";  
    int _icc_dev_fd;

    ICC_Bus() {
        _icc_dev_fd = open(
            _icc_dev_name, 
            O_RDWR); 


        if (_icc_dev_fd < 0) {
            perror("i2c open"); 
            return; 
        }
    }
    ~ICC_Bus() {
        close(_icc_dev_fd); 
    }
};
