//
//  SocketManager.cpp
//  FacialMoCapRemote
//
//  Created by Jihyun Lee on 5/13/20.
//

#include "SocketManager.h"

#include "cinder/Log.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace std;

void SocketManager::setup(asio::io_service &io)
{
    mHost = "127.0.0.1";
    mPort = 1204;
    
    mClient = UdpClient::create( io );
    mClient->connectConnectEventHandler( &SocketManager::onConnect, this );
    mClient->connectErrorEventHandler( &SocketManager::onError, this );
    mClient->connectResolveEventHandler( [ & ]()
    {
        mText.push_back( "Endpoint resolved" );
    } );
    
    mServer = UdpServer::create( io );
    mServer->connectAcceptEventHandler( &SocketManager::onAccept, this );
    mServer->connectErrorEventHandler( &SocketManager::onError, this );
    
    // Start listening.
    accept();
}

void SocketManager::accept()
{
    if ( mReadSession ) {
        mReadSession.reset();
    }
    if ( mServer ) {
        // This is how you start listening for a connection. Once
        // a connection occurs, a session will be created and passed
        // in through the onAccept method.
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
    CI_LOG_I( "Connecting to: " + mHost + ":" + toString( mPort ) );
    mClient->connect( mHost, (uint16_t)mPort );
}

void SocketManager::onConnect( UdpSessionRef session )
{
    CI_LOG_I("Connected");

    // Get the session from the argument and set callbacks.
    // Note that you can use lambdas.
    mWriteSession = session;
    mWriteSession->connectErrorEventHandler( &SocketManager::onError, this );
    mWriteSession->connectWriteEventHandler( &SocketManager::onWrite, this );

    write();
}

void SocketManager::onError( string err, size_t bytesTransferred )
{
    string text = "Error";
    if ( !err.empty() ) {
        text += ": " + err;
    }
     mText.push_back( text );
}

void SocketManager::onRead( ci::BufferRef buffer )
{
    mText.push_back( toString( buffer->getSize() ) + " bytes read" );

    // Data is packaged as a ci::Buffer. This allows
    // you to send any kind of data. Because it's more common to
    // work with strings, the session object has static convenience
    // methods for converting between std::string and ci::Buffer.
    string response    = UdpSession::bufferToString( buffer );
    mText.push_back( response );

    // Continue reading.
    mReadSession->read();
}

void SocketManager::onReadComplete()
{
    mText.push_back( "Read complete" );

    // Continue reading new responses.
    mReadSession->read();
}

void SocketManager::onWrite( size_t bytesTransferred )
{
    mText.push_back( toString( bytesTransferred ) + " bytes written" );
    
    // Read after writing to look for a response.
    mReadSession->read();
}

void SocketManager::write()
{
    if ( mWriteSession && mWriteSession->getSocket()->is_open() ) {
        // Write data is packaged as a ci::Buffer. This allows
        // you to send any kind of data. Because it's more common to
        // work with strings, the session object has static convenience
        // methods for converting between std::string and ci::Buffer.
        std::string str = "hello jihyun!";
        mWriteSession->write( UdpSession::stringToBuffer( str ) );
    } else {
        connect();
    }
}
