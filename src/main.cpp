#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofAppGLFWWindow win;
    //win.setNumSamples(8);
    win.setMultiDisplayFullscreen(true); //this makes the fullscreen window span across all your monitors
    
    ofSetupOpenGL(&win, 1920, 1080, OF_FULLSCREEN);
    
//	ofSetupOpenGL(1920,1080,OF_FULLSCREEN);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
