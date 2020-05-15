#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "SocketManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FacialMoCapRemoteApp : public App
{
public:
	void setup() override;
	void update() override;
	void draw() override;

private:
    SocketManagerRef mSocketManager;
};

void FacialMoCapRemoteApp::setup()
{
    mSocketManager = SocketManager::getInstance();
    mSocketManager->setup( io_service() );
}

void FacialMoCapRemoteApp::update()
{
}

void FacialMoCapRemoteApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
}

CINDER_APP( FacialMoCapRemoteApp, RendererGl )
