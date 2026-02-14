#include "plant_light_udp.hpp"

#include <asm-generic/socket.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <sys/socket.h>
#include <vector>
#include <fcntl.h>

#define VERBOSE_MODE_ON     0


UdpHandler::UdpHandler(void) {
    memset(&_server_address, 0, sizeof(_server_address)); 
    memset(_raw_rx_data_buffer, 0, RX_BUFFER_SIZE); 

    _server_address.sin_family = AF_INET; 
    _server_address.sin_port = htons(SERVER_PORT); 
    _server_address.sin_addr.s_addr = htonl(INADDR_ANY); 

    _sock = socket(PF_INET, SOCK_DGRAM, 0); 

    if(_sock < 0) {
        std::cout << "Socket init error" << std::endl; 
        return; 
    }

    int retval = bind(
        _sock, 
        (sockaddr *) &_server_address, 
        (socklen_t ) sizeof(_server_address));

    if(retval < 0) {
        std::cout << "Socket bind error" << std::endl; 
        return; 
    }

    _read_timeout.tv_sec = 1; 
    _read_timeout.tv_usec = 0; 
    setsockopt(
        _sock, 
        SOL_SOCKET, 
        SO_RCVTIMEO, 
        &_read_timeout, 
        sizeof(_read_timeout));  

    _client_address_len = sizeof(_client_address); 

    //_udp_rx_buffer.reserve(RX_BUFFER_SIZE); 
#if VERBOSE_MODE_ON == 1
    std::cout << "UDP init succeeded" << std::endl; 
#endif

    // init polling FD: 
    _pfd.fd = _sock;
    _pfd.events = POLLIN;

}


int UdpHandler::wait_for_message(void) {

    int ret = poll(&_pfd, 1, 10);

    if(ret <= 0 || !(_pfd.revents & POLLIN)) {
        return 0; 
    }

    _client_address_len = sizeof(_client_address); 


    int len = recvfrom(
        _sock, 
        _raw_rx_data_buffer, 
        RX_BUFFER_SIZE, 
        0, 
        (sockaddr *) &_client_address, 
        (socklen_t *)&_client_address_len); 

    if (len < 0) {
        return len;
    }

    _udp_rx_buffer.assign(
        _raw_rx_data_buffer, 
        _raw_rx_data_buffer + len);

#if VERBOSE_MODE_ON == 1
    printf("Data len: %d, data first sample: %d\n", len, _udp_rx_buffer[0]); 
#endif

#if VERBOSE_MODE_ON == 1
    std::cout << "Received : " << std::endl; 
    this->print_data(_udp_rx_buffer); 
#endif

    return len; 
}

int UdpHandler::send_data(const std::vector<uint8_t>& f_tx_data) {
    if(f_tx_data.size() == 0) {
        std::cout << "Empty TX array" << std::endl; 
        return -1; 
    }
#if VERBOSE_MODE_ON == 1
    std::cout << "Sending : " << std::endl; 
    this->print_data(f_tx_data); 
#endif

    int retval = sendto(
        _sock, 
        f_tx_data.data(),
        f_tx_data.size(), 
        0, 
        (sockaddr *) &_client_address, 
        sizeof(_client_address) 
    ); 

    if(retval < 0) {
        std::cout << "Send failed" << std::endl; 
    }

    return retval; 
}




const std::vector<uint8_t> & UdpHandler::get_rx_data() const {
    return _udp_rx_buffer; 
}


void UdpHandler::print_data(const std::vector<uint8_t>& v) const {
    for (uint8_t b : v) {
        printf("%02X ", b); 
    }
    printf("\n"); 
}