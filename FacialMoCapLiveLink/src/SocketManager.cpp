//
//  SocketManager.cpp
//  FacialMoCapLinkLiveApp
//
//  Created by Jihyun Lee on 5/13/20.
//

#include "SocketManager.h"

#include "cinder/Log.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace std;

void SocketManager::createServer(asio::io_service &io, const string& ipAddress, int32_t port)
{
	mIpAddress = ipAddress;
	mPort = port;

	mServer = UdpServer::create(io);
	mServer->connectAcceptEventHandler(&SocketManager::onAccept, this);
	mServer->connectErrorEventHandler(&SocketManager::onError, this);

	// Start listening.
	accept();
}

void SocketManager::createClient(asio::io_service &io, const string& ipAddress, int32_t port)
{
	mIpAddress = ipAddress;
	mPort = port;
    mConnected = false;

	mClient = UdpClient::create(io);
	mClient->connectConnectEventHandler(&SocketManager::onConnect, this);
	mClient->connectErrorEventHandler(&SocketManager::onError, this);
	mClient->connectResolveEventHandler([&]()
	{
        mConnected = true;
		CI_LOG_I("Connected to: " << mIpAddress << " port: " << toString(mPort));
	});

	connect();
}

void SocketManager::accept()
{
    if ( mReadSession ) {
        mReadSession.reset();
    }
    if ( mServer ) {
        mServer->accept( (uint16_t)mPort );
        CI_LOG_I( "Listening on port: " + toString( mPort ) );
    }
}

void SocketManager::onAccept( UdpSessionRef session )
{
    // Get the session from the argument and set callbacks.
    mReadSession = session;
    mReadSession->connectErrorEventHandler( &SocketManager::onError, this );
    mReadSession->connectReadCompleteEventHandler( &SocketManager::onReadComplete, this );
    mReadSession->connectReadEventHandler( &SocketManager::onRead, this );
    mReadSession->connectWriteEventHandler( &SocketManager::onWrite, this );

    // Start reading data from the client.
    mReadSession->read();
}

void SocketManager::connect()
{
    CI_LOG_I( "Connecting to: " + mIpAddress + ":" + toString( mPort ) );
    mClient->connect( mIpAddress, (uint16_t)mPort );
}

void SocketManager::onConnect( UdpSessionRef session )
{
    mConnected = true;
    mWriteSession = session;
    mWriteSession->connectErrorEventHandler( &SocketManager::onError, this );
    mWriteSession->connectWriteEventHandler( &SocketManager::onWrite, this );
}

void SocketManager::onError( string err, size_t bytesTransferred )
{
	CI_LOG_E(err);
}

void SocketManager::onRead( ci::BufferRef buffer )
{
	CI_LOG_I(toString(buffer->getSize()) << " bytes read");

    string response    = UdpSession::bufferToString( buffer );
//    CI_LOG_I(response);

    mReadSession->read();
}

void SocketManager::onReadComplete()
{
	CI_LOG_I("Read complete");
    mReadSession->read();
}

void SocketManager::onWrite( size_t bytesTransferred )
{
	CI_LOG_I(toString(bytesTransferred) << " bytes written");
    
	if (mReadSession) {
		mReadSession->read();
	}
}

void SocketManager::write(string& message)
{
    if ( mWriteSession && mWriteSession->getSocket()->is_open() ) {
        mWriteSession->write( UdpSession::stringToBuffer(message) );
    } else {
        connect();
    }
}
