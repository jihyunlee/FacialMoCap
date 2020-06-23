#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "OSCManager.h"
#include "SocketManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const uint16_t INCOMING_PORT = 1204;    // from iOS via OSC
const uint16_t OUTGOING_PORT = 1205;    // to UE4 via UDP

class FacialMoCapLiveLinkApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;

private:
	void onOSCReceived(const osc::Message &msg);
    void onUpdateIpAddress();
    void broadcast(const string& message);
    bool validateIpAddress(const string& address);
    
	OSCManagerRef mOSCManager;
    vector<SocketManagerRef> mSockets;
    uint16_t mIncomingPort = INCOMING_PORT;
    uint16_t mOutgoingPort = OUTGOING_PORT;
    vector<string> mIpAddresses;
    string mNetworkDomain;
    params::InterfaceGlRef    mParams;
};

void FacialMoCapLiveLinkApp::setup()
{
	mOSCManager = OSCManager::getInstance();
	mOSCManager->createReceiver(INCOMING_PORT);
	mOSCManager->getUDPReceiver()->setListener("/HT", bind(&FacialMoCapLiveLinkApp::onOSCReceived, this, std::placeholders::_1));
	mOSCManager->getUDPReceiver()->setListener("/HR", bind(&FacialMoCapLiveLinkApp::onOSCReceived, this, std::placeholders::_1));
	mOSCManager->getUDPReceiver()->setListener("/HRQ", bind(&FacialMoCapLiveLinkApp::onOSCReceived, this, std::placeholders::_1));
	mOSCManager->getUDPReceiver()->setListener("/ELR", bind(&FacialMoCapLiveLinkApp::onOSCReceived, this, std::placeholders::_1));
	mOSCManager->getUDPReceiver()->setListener("/ERR", bind(&FacialMoCapLiveLinkApp::onOSCReceived, this, std::placeholders::_1));
	mOSCManager->getUDPReceiver()->setListener("/W", bind(&FacialMoCapLiveLinkApp::onOSCReceived, this, std::placeholders::_1));

    SocketManagerRef socket = make_shared<SocketManager>();
	socket->createClient(io_service(), "127.0.0.1", OUTGOING_PORT);
    mSockets.push_back(socket);
    
    mParams = params::InterfaceGl::create( getWindow(), "Facial MoCap LiveLink Configuration", toPixels( ivec2( getWindowWidth()-30, getWindowHeight()-30 ) ) );
    mParams->addText( "iOS", "label=`iOS config`" );
    mParams->addParam( "iOS Port", &mIncomingPort, true);
    
    mParams->addSeparator();
    mParams->addText( "LiveLink", "label=`LiveLink config`" );
    mParams->addParam( "LiveLink Port", &mOutgoingPort, true);
    mParams->addParam( "Network Domain (e.g. 192.168.1.)", &mNetworkDomain);
    
    mIpAddresses = { "", "", "", "", "", "", "", "", "", "" };
    mParams->addParam( "IP Address 1", &mIpAddresses.at(0) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 2", &mIpAddresses.at(1) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 3", &mIpAddresses.at(2) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 4", &mIpAddresses.at(3) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 5", &mIpAddresses.at(4) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 6", &mIpAddresses.at(5) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 7", &mIpAddresses.at(6) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 8", &mIpAddresses.at(7) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 9", &mIpAddresses.at(8) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    mParams->addParam( "IP Address 10", &mIpAddresses.at(9) ).updateFn(bind(&FacialMoCapLiveLinkApp::onUpdateIpAddress, this));
    
    mNetworkDomain = "192.168.1.";
}

void FacialMoCapLiveLinkApp::onOSCReceived(const osc::Message &message)
{
	const string address = message.getAddress();

	// Head position
	if (!address.compare("/HT")) {
        broadcast(address + "," + to_string(message.getArgFloat(0)) + "," + to_string(message.getArgFloat(1)) + "," + to_string(message.getArgFloat(2)));
	}
	// Head rotation in degrees
	else if (!address.compare("/HR")) {
		broadcast(address + "," + to_string(message.getArgFloat(0)) + "," + to_string(message.getArgFloat(1)) + "," + to_string(message.getArgFloat(2)));
	}
	// Head rotation as quaternion
	else if (!address.compare("/HRQ")) {
		broadcast(address + "," + to_string(message.getArgFloat(0)) + "," + to_string(message.getArgFloat(1)) + "," + to_string(message.getArgFloat(2)) + "," + to_string(message.getArgFloat(3)));
	}
	// Eye left rotation
	else if (!address.compare("/ELR")) {
		broadcast(address + "," + to_string(message.getArgFloat(0)) + "," + to_string(message.getArgFloat(1)));
	}
	// Eye right rotation
	else if (!address.compare("/ERR")) {
		broadcast(address + "," + to_string(message.getArgFloat(0)) + "," + to_string(message.getArgFloat(1)));
	}
	// Blendshape parameters
	else if (!address.compare("/W")) {
		broadcast(address + "," + to_string(message.getArgInt32(0)) + "," + to_string(message.getArgFloat(1)));
	}
	else {
		CI_LOG_E("Address listener not found: " << address);
	}
}

bool FacialMoCapLiveLinkApp::validateIpAddress(const string& address)
{
    if(address.empty()) {
        return false;
    }
    
    if(mNetworkDomain.compare(address.substr(0, address.find_last_of(".")+1))) {
        CI_LOG_E(address << " not on the same network");
        return false;
    }
    
    return true;
}

void FacialMoCapLiveLinkApp::onUpdateIpAddress()
{
    for (auto &address : mIpAddresses) {
        
        if(validateIpAddress(address)) {
            
            bool alreadyExist = false;
            for (auto &socket : mSockets) {
                if(!socket->getIpAddress().compare(address) && socket->isConnected()) {
                    alreadyExist = true;
                }
            }
            
            if(!alreadyExist) {
                CI_LOG_I(address << " added to the list");
                SocketManagerRef socket = make_shared<SocketManager>();
                socket->createClient(io_service(), address, OUTGOING_PORT);
                mSockets.push_back(socket);
            }
        }
        else {
            address = "";
        }
    }
}

void FacialMoCapLiveLinkApp::broadcast(const string& message)
{
    CI_LOG_I(message);
    string _message = message;
    for (auto &socket : mSockets) {
        socket->write(_message);
    }
}

void FacialMoCapLiveLinkApp::update()
{
}

void FacialMoCapLiveLinkApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    mParams->draw();
}

CINDER_APP( FacialMoCapLiveLinkApp, RendererGl )
