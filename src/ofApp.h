#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxMediaType.h"

#define HOST "192.168.0.130"
#define PORT 9998

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void sendMessage(ofxOscMessage msg);
    
        string getFileName(string path);
    
        // display vars
        ofVideoPlayer ledSource;
        ofVideoPlayer screenSource1;
        ofVideoPlayer screenSource2;
    
        // gui vars
        ofxPanel gui;
        ofxToggle sendSliderColor;
        ofxColorSlider color;
    
        // OSC Sender
        ofxOscSender sender;
    
        // OSC Receiver
        ofxOscReceiver receiver;
    
        // mouse var
        ofColor mouseColor;
        int mouseX;
        int mouseY;
    
        // other vars
        bool _debug;
        int prevR;
		int prevG;
        int prevB;
};
