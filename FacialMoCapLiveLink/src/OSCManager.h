//
//  OSCManager.h
//  FacialMoCapLinkLiveApp
//
//  Created by Jihyun Lee on 5/14/20.
//

#pragma once
#include <memory>

#include "cinder/osc/Osc.h"

typedef std::shared_ptr<class OSCManager> OSCManagerRef;

class OSCManager {
public:
	static OSCManagerRef getInstance() {
		static OSCManagerRef instance = nullptr;
		if (instance == nullptr) {
			instance = OSCManagerRef(new OSCManager());
		}
		return instance;
	}

	OSCManager() {}
	~OSCManager() {}

	void createReceiver(uint16_t port, bool useUDP = true);

	std::shared_ptr<ci::osc::ReceiverUdp> getUDPReceiver() { return mUDPReceiver;  }

private:
	void createUDPReceiver();
	void createTCPReceiver();

	bool mUseUDP;
	uint16_t mPort;

	std::shared_ptr<ci::osc::ReceiverUdp> mUDPReceiver;
	std::shared_ptr<ci::osc::ReceiverTcp> mTCPReceiver;

	std::map<uint64_t, asio::ip::udp::endpoint> mUDPConnections;
	std::map<uint64_t, asio::ip::tcp::endpoint> mTCPConnections;
};