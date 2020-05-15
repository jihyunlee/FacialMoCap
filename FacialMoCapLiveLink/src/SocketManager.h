//
//  SocketManager.h
//  FacialMoCapLinkLiveApp
//
//  Created by Jihyun Lee on 5/13/20.
//

#pragma once
#include <memory>

#include "CinderAsio.h"
#include "UdpClient.h"
#include "UdpServer.h"

typedef std::shared_ptr<class SocketManager> SocketManagerRef;

class SocketManager {
public:
    static SocketManagerRef getInstance() {
        static SocketManagerRef instance = nullptr;
        if (instance == nullptr) {
            instance = SocketManagerRef(new SocketManager());
        }
        return instance;
    }
    
    SocketManager() {}
    ~SocketManager() {}
    
    void createServer(asio::io_service &io, const std::string& ipAddress, int32_t port);
	void createClient(asio::io_service &io, const std::string& ipAddress, int32_t port);
	void write(std::string& message);
    const std::string& getIpAddress() { return mIpAddress; }
    bool isConnected() { return mConnected; }

private:
    void accept();
    void onAccept( UdpSessionRef session );
    void connect();
    void onConnect( UdpSessionRef session );
    void onError( std::string err, size_t bytesTransferred );
    void onRead( ci::BufferRef buffer );
    void onReadComplete();
    void onWrite( size_t bytesTransferred );

    std::string mIpAddress;
    int32_t mPort;
    bool mConnected;
    
    UdpServerRef mServer;
    UdpClientRef mClient;
    UdpSessionRef mReadSession;
    UdpSessionRef mWriteSession;
};
