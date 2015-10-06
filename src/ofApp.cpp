#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    _debug = true;
    
    // directory listing
    string ledPath = "ledVid/active/";
    string screenPath1 = "screenVid_1/active/";
    string screenPath2 = "screenVid_2/active/";
    
    // video sync
    ledSource.loadMovie(getFileName(ledPath));
    screenSource1.loadMovie(getFileName(screenPath1));
    screenSource2.loadMovie(getFileName(screenPath2));
    
    // video play
    ledSource.play();
    screenSource1.play();
    screenSource2.play();
    
    gui.setup();
    gui.add(sendSliderColor.setup("Color Slider", false));
    gui.add(color.setup("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    
    sender.setup(HOST, PORT);
    
    ofSetColor(255,255,255);
    ofBackground(0,0,0);
    ofSetFrameRate(30);
    
    mouseColor = ofColor(128,128,0);
    
    ledSource.setLoopState(OF_LOOP_NONE);
    screenSource1.setLoopState(OF_LOOP_NONE);
    screenSource2.setLoopState(OF_LOOP_NONE);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    // screenVid1 update
    screenSource1.update();
    
    if(screenSource1.getIsMovieDone()){
        ofLog() << "movie finished " << endl;
        
        screenSource1.play();
        screenSource2.firstFrame();
        ledSource.firstFrame();
    }
    
    screenSource2.update();
    ledSource.update();
    
    if(_debug && sendSliderColor){
        ofColor readSlider = color;
        int red     = readSlider.r;
        int green   = readSlider.g;
        int blue    = readSlider.b;
        
        if (red != prevR ||
            green != prevG ||
            blue != prevB){
            
            ofxOscMessage m;
            
            m.setAddress("/led");
            m.addIntArg(255);
            m.addIntArg(red);
            m.addIntArg(green);
            m.addIntArg(blue);
            
            sendMessage(m);
            
            prevR = red;
            prevG = green;
            prevB = blue;
        }
        
    }
    
    else{
        // get first pixel color
        unsigned char * pixels = ledSource.getPixels();
        int nChannels = ledSource.getPixelsRef().getNumChannels();
        int widthOfLine = ledSource.width;  // how long is a line of pixels
        int red     = pixels[(0 * widthOfLine) * nChannels    ];
        int green   = pixels[(0 * widthOfLine) * nChannels + 1];
        int blue    = pixels[(0 * widthOfLine) * nChannels + 2];
        
        if (red != prevR ||
            green != prevG ||
            blue != prevB){
            
            ofxOscMessage m;
            
            m.setAddress("/led");
            m.addIntArg(255);
            m.addIntArg(red);
            m.addIntArg(green);
            m.addIntArg(blue);
            
            sendMessage(m);
            
            prevR = red;
            prevG = green;
            prevB = blue;
        }
    }
    // debug
    if (_debug) {
//        ofLog() <<  ofToString(red) << ", " << ofToString(green) << ", " << ofToString(blue) << endl;
    }
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    screenSource1.draw(0,0);
    screenSource2.draw(1920,0);
    
    if(_debug){
        ledSource.draw(0,0,200,200);
        gui.draw();
        ofSetColor(mouseColor);
        ofRect(mouseX, mouseY, 10, 10);
        ofSetColor(255,255,255);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch(key) {
        case OF_KEY_LEFT:
            
            break;
        case OF_KEY_RIGHT:
            
            break;
        
        case ' ':
            if(screenSource1.isPaused()){
                ledSource.setPaused(false);
                screenSource1.setPaused(false);
                screenSource2.setPaused(false);
            }
            
            else {
                ledSource.setPaused(true);
                screenSource1.setPaused(true);
                screenSource2.setPaused(true);
            }
            
            break;
        
        case 'd':
        case 'D':
            
            _debug = !_debug;
            break;
            
        case 'r':
        case 'R':
            
            string ledPath = "ledVid/active/";
            string screenPath1 = "screenVid_1/active/";
            string screenPath2 = "screenVid_2/active/";
            
            // video sync
            ledSource.loadMovie(getFileName(ledPath));
            screenSource1.loadMovie(getFileName(screenPath1));
            screenSource2.loadMovie(getFileName(screenPath2));
            
            // video play
            ledSource.play();
            screenSource1.play();
            screenSource2.play();
            
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if(_debug){
        mouseX = x;
        mouseY = y;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    ofLog() << dragInfo.files[0] << endl;
    string fileExt = dragInfo.files[0].substr(dragInfo.files[0].find_last_of(".") + 1);
    
    
    if( fileExt == "mov" || fileExt == "MOV" || fileExt == "mpeg" || fileExt == "mpg" || fileExt == "mp4")
    {
        ledSource.loadMovie(dragInfo.files[0]);
    }
    else {
        ofLog() << "not a usable file" << endl;
    }
}

void ofApp::sendMessage(ofxOscMessage msg) {
//    ofLog() << "sending message" << endl;
    sender.sendMessage(msg);
}

string ofApp::getFileName(string path) {
    
    if( ofDirectory::doesDirectoryExist(path)){
        ofDirectory dir(path);
        dir.allowExt("mov");
        dir.allowExt("mp4");
        dir.listDir();
        
        for(int i = 0; i < dir.numFiles(); i++){
            ofLogNotice(dir.getPath(i));
        }
        
        return dir.getPath(0);
    }
    
    return "";
}