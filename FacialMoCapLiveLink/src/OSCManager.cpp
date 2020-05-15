//
//  OSCManager.cpp
//  FacialMoCapLinkLiveApp
//
//  Created by Jihyun Lee on 5/14/20.
//

#include "OSCManager.h"

#include "cinder/app/App.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void OSCManager::createReceiver(uint16_t port, bool useUDP)
{
	mUseUDP = useUDP;
	mPort = port;
	
	if (mUseUDP) {
		createUDPReceiver();
	}
	else {
		createTCPReceiver();
	}
}

void OSCManager::createUDPReceiver()
{
	mUDPReceiver = std::shared_ptr<osc::ReceiverUdp>(new osc::ReceiverUdp(mPort));

	try {
		// Bind the receiver to the endpoint. This function may throw.
		mUDPReceiver->bind();
	}
	catch (const osc::Exception &ex) {
		CI_LOG_E("Error binding: " << ex.what() << " val: " << ex.value());
		//quit();
	}

	mUDPReceiver->listen(
		[](asio::error_code error, asio::ip::udp::endpoint endpoint) -> bool {
		if (error) {
			CI_LOG_E("Error Listening: " << error.message() << " val: " << error.value() << " endpoint: " << endpoint);
			return false;
		}
		else {
			CI_LOG_I("do i see you?");
			return true;
		}
	});
}

void OSCManager::createTCPReceiver()
{

}