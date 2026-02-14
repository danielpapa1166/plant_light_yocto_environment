#pragma once

#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <cstdint>
#include <mutex>
#include <vector>
#include <sys/poll.h>

#define RX_BUFFER_SIZE      1024

struct UdpHandler {
    UdpHandler(); 
    int wait_for_message(void); 
    int send_data(const std::vector<uint8_t>& f_tx_data); 
    const std::vector<uint8_t> & get_rx_data() const;
    void print_data(const std::vector<uint8_t>& v) const;
private: 

    pollfd _pfd{};


    uint8_t _raw_rx_data_buffer[RX_BUFFER_SIZE]; 
    std::mutex udp_tx_data_mutex; 
    std::vector<uint8_t> _udp_tx_buffer; 
    std::vector<uint8_t> _udp_rx_buffer; 


    int _sock = 0; 
    const int SERVER_PORT = 8888; 
    sockaddr_in _server_address; 
    sockaddr_in _client_address; 
    int _client_address_len; 

    struct timeval _read_timeout; 
}; 