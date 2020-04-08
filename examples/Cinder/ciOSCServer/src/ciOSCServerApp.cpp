#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/osc/Osc.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using Receiver = osc::ReceiverUdp;
using protocol = asio::ip::udp;

const uint16_t localPort = 1204;

class ciOSCServerApp : public App {
  public:
    ciOSCServerApp();
    void setup() override;
    void draw() override;
    
    Receiver mReceiver;
};

ciOSCServerApp::ciOSCServerApp()
: mReceiver( localPort )
{
}

void ciOSCServerApp::setup()
{
    mReceiver.setListener( "/mill",
    [&]( const osc::Message &msg ){
        CI_LOG_I("message received: " << msg[0].string());
    });
    
    try {
        // Bind the receiver to the endpoint. This function may throw.
        mReceiver.bind();
    }
    catch( const osc::Exception &ex ) {
        CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
        quit();
    }
    
    // UDP opens the socket and "listens" accepting any message from any endpoint. The listen
    // function takes an error handler for the underlying socket. Any errors that would
    // call this function are because of problems with the socket or with the remote message.
    mReceiver.listen(
    []( asio::error_code error, protocol::endpoint endpoint ) -> bool {
        if( error ) {
            CI_LOG_E( "Error Listening: " << error.message() << " val: " << error.value() << " endpoint: " << endpoint );
            return false;
        }
        else
            return true;
    });
}


void ciOSCServerApp::draw()
{
}

auto settingsFunc = []( App::Settings *settings ) {
#if defined( CINDER_MSW )
    settings->setConsoleWindowEnabled();
#endif
    settings->setMultiTouchEnabled( false );
};

CINDER_APP( ciOSCServerApp, RendererGl, settingsFunc )
