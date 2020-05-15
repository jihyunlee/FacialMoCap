//
//  UDPSocket.c
//  FacialMotionCapture
//
//  Created by Jihyun Lee on 5/5/20.
//  Copyright © 2020 Jihyun Lee. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

int udpsocket_open()
{
    int socketId = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketId < 0) {
        printf("udpsocket_broadcast_client Error: Could not open socket.");
        return -1;
    }
    
    int reuseon   = 1;
    setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &reuseon, sizeof(reuseon));

    return socketId;
}

int udpsocket_close(int socketId)
{
    return close(socketId);
}

void udpsocket_enable_broadcast(int socketId)
{
    // set socket options enable broadcast
    int broadcastEnable = 1;
    int ret = setsockopt(socketId, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret < 0) {
        printf("udpsocket_broadcast_client Error: Could not open set socket to broadcast mode");
    }
}

int udpsocket_send(int socketId, char *msg, char *ip, int port)
{
    struct sockaddr_in addr;
    memset(&addr, 0x0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    int ret = (int) sendto(socketId, msg, strlen(msg), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < -1) {
        printf("Error: Could not send message.");
    }
    else {
//        printf("sent !!!!! %d\n", ret);
    }
    return ret;
}

int udpsocket_broadcast(int socketId, char *msg, int port)
{
//    printf("udpsocket_broadcast: (%lu bytes)\n%s\n", strlen(msg), msg);

    struct sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(port);
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    
    int ret = (int) sendto(socketId, msg, strlen(msg), 0, (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    if (ret < -1) {
        printf("Error: Could not open send broadcast.");
    }
    else {
//        printf("broadcasted !!!!! %d\n", ret);
    }

    return ret;
}
