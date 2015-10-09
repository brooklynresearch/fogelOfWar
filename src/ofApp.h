#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

//#define HOST "192.168.2.2"
//#define PORT 9998

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
    
        void drawTimeline(int x, int y);
        void drawTimecode(int x, int y);
    
        void sendMessage(ofxOscMessage msg);
    
        ofXml XML;
    
        string getFileName(string path);
    
        // display vars
        ofVideoPlayer ledSource;
        ofVideoPlayer ledSource2;
        ofVideoPlayer screenSource1;
        ofVideoPlayer screenSource2;
        int direction_increment;
        int arrow_increment;
    
        // xml vars
        int numPis;
        string HOST;
        string HOST2;
        int PORT;
        int PORT2;
    
        // timeline vars
        int timelineX;
        int timelineY;
        int timelineWidth;
        int timelineHeight;
    
        // OSC Sender
        ofxOscSender sender;
        ofxOscSender sender2;
    
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
    
        int prevR2;
        int prevG2;
        int prevB2;
};
