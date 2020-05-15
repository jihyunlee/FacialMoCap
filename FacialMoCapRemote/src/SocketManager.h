//
//  SocketManager.hpp
//  FacialMoCapRemote
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
    
    void setup(asio::io_service &io);
        
private:
    void accept();
    void onAccept( UdpSessionRef session );
    void connect();
    void onConnect( UdpSessionRef session );
    void onError( std::string err, size_t bytesTransferred );
    void onRead( ci::BufferRef buffer );
    void onReadComplete();
    void onWrite( size_t bytesTransferred );
    void write();

    std::string mHost;
    int32_t mPort;
    
    UdpServerRef mServer;
    UdpClientRef mClient;
    UdpSessionRef mReadSession;
    UdpSessionRef mWriteSession;

    std::vector<std::string> mText;
};
