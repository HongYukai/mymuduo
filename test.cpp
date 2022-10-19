//
// Created by 12096 on 2022/10/16.
//

#include <iostream>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include "HeapTimer.h"


using namespace std;

void callback(std::shared_ptr<client_data> cd) {
    if (!cd) return;
    cout << cd->sock_fd_ <<  " is time up!" << endl;
}

int main() {
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = inet_addr("192.168.5.141");
    TimerContainer *tc = new HeapTimer(101);
    for (int i = 0; i < 100; i++) {
        client_data *cli = new client_data(addr, sock_fd);
        shared_ptr<Timer> timer(new Timer(rand() % 100));
        timer->cb_ = callback;
        cli->timer_ = timer;
        timer->user_data_ = shared_ptr<client_data>(cli);
        tc->addTimer(timer);
    }
    while (tc->cur_) {
        tc->tick();
    }
}

