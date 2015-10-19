#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    if (XML.load("settings.xml") ){
        ofLog() << "XML loaded" << endl;
    }
    
    else {
        // if nonexistent, create
        XML.addChild("SETTINGS");
        XML.setTo("SETTINGS");
    }
    
    _debug = false;
    
    if(XML.exists("//NUMPIS")) {
        numPis = XML.getValue<int>("//NUMPIS");
        ofLog() << "loaded NUMPIS: " << ofToString(numPis) << endl;
    } else {
        numPis = 1;
    }
    
    if(XML.exists("//HOST")) {
        HOST = XML.getValue<string>("//HOST");
        ofLog() << "loaded HOST: " << ofToString(HOST) << endl;
    } else {
        HOST = "192.168.2.3";
    }
    
    if(XML.exists("//PORT")) {
        PORT = XML.getValue<int>("PORT");
        ofLog() << "loaded PORT: " << ofToString(PORT) << endl;
    } else {
        PORT = 9998;
    }
    
    if (numPis > 1) {
        if(XML.exists("//HOST2")) {
            HOST2 = XML.getValue<string>("//HOST2");
            ofLog() << "loaded HOST2: " << ofToString(HOST2) << endl;
        } else {
            HOST2 = "192.168.2.3";
        }
        
        if(XML.exists("//PORT2")) {
            PORT2 = XML.getValue<int>("PORT2");
            ofLog() << "loaded PORT2: " << ofToString(PORT2) << endl;
        } else {
            PORT2 = 9998;
        }
    }
    
    // directory listing
    string ledPath = "ledVid/active/";
    string ledPath2 = "ledVid2/active/";
    string screenPath1 = "screenVid_1/active/";
    string screenPath2 = "screenVid_2/active/";
    
    sender.setup(HOST, PORT);
    
    // video sync
    ledSource.loadMovie(getFileName(ledPath));
    screenSource1.loadMovie(getFileName(screenPath1));
    screenSource2.loadMovie(getFileName(screenPath2));
    
    direction_increment = 1;
    arrow_increment = 60;
    
    ofSetColor(255,255,255);
    ofBackground(0,0,0);
    ofSetFrameRate(30);
    
    mouseColor = ofColor(128,128,0);
    
    ledSource.setLoopState(OF_LOOP_NONE);
    screenSource1.setLoopState(OF_LOOP_NONE);
    screenSource2.setLoopState(OF_LOOP_NONE);
    
    if (numPis > 1) {
        
        sender2.setup(HOST2, PORT2);
        
        ledSource2.loadMovie(getFileName(ledPath));
        ledSource2.setLoopState(OF_LOOP_NONE);
    }
    
    // timeline vars
    timelineX = 600;
    timelineY = 600;
    timelineWidth = 600;
    timelineHeight = 120;
    
    // video play
    ledSource.play();
    screenSource1.play();
    screenSource2.play();
    
    if (numPis > 1){
        ledSource2.play();
    }
    
    screenSource1.firstFrame();
    screenSource2.firstFrame();
    ledSource.firstFrame();
    
    if (numPis > 1) {
        ledSource2.firstFrame();
    }
    
    prevR = -1;
    prevG = -1;
    prevB = -1;
    
    prevR2 = -1;
    prevG2 = -1;
    prevB2 = -1;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    // screenVid1 update
    screenSource1.update();
    screenSource2.update();
    ledSource.update();
    
    if (numPis > 1) {
        ledSource2.update();
    }
    
    if(screenSource1.getIsMovieDone()){
        ofLog() << "movie finished " << endl;
        
        screenSource1.firstFrame();
        screenSource2.firstFrame();
        ledSource.firstFrame();
        
        if (numPis > 1) {
            ledSource2.firstFrame();
        }
        
        screenSource1.play();
        screenSource2.play();
        ledSource.play();
        
        if (numPis > 1) {
            ledSource2.play();
        }
    }
    
    
    
    if (numPis > 1) {
        
        // get first pixel color
        unsigned char * pixels = ledSource2.getPixels();
        int nChannels = ledSource2.getPixelsRef().getNumChannels();
        int widthOfLine = ledSource2.width;  // how long is a line of pixels
        int red     = pixels[(0 * widthOfLine) * nChannels    ];
        int green   = pixels[(0 * widthOfLine) * nChannels + 1];
        int blue    = pixels[(0 * widthOfLine) * nChannels + 2];
        
        if (red != prevR2 ||
            green != prevG2 ||
            blue != prevB2){
            
            ofxOscMessage m;
            
            m.setAddress("/led");
            m.addIntArg(255);
            m.addIntArg(red);
            m.addIntArg(green);
            m.addIntArg(blue);
            
            sender2.sendMessage(m);
            
            prevR2 = red;
            prevG2 = green;
            prevB2 = blue;
        }
    }
    
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
        
        sender.sendMessage(m);
        
        prevR = red;
        prevG = green;
        prevB = blue;
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
        if(numPis > 1) {
            ledSource.draw(200,0,200,200);
        }
        drawTimeline(timelineX, timelineY);
        drawTimecode(timelineX, timelineY);
        ofSetColor(mouseColor);
        ofRect(mouseX, mouseY, 10, 10);
        ofSetColor(255,255,255);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    int currentFrameCount = screenSource1.getCurrentFrame();
    
    switch(key) {
        case OF_KEY_LEFT:
            
            
            if(currentFrameCount <= direction_increment){
                screenSource1.setFrame(0);
                screenSource2.setFrame(0);
                ledSource.setFrame(0);
            }
            
            else {
                screenSource1.setFrame(currentFrameCount - direction_increment);
                screenSource2.setFrame(currentFrameCount - direction_increment);
                ledSource.setFrame(currentFrameCount - direction_increment);
            }
            
            break;
        case OF_KEY_RIGHT:

            if(currentFrameCount + direction_increment > screenSource1.getTotalNumFrames()){
                screenSource1.setFrame(screenSource1.getTotalNumFrames());
                screenSource2.setFrame(screenSource2.getTotalNumFrames());
                ledSource.setFrame(ledSource.getTotalNumFrames());
            }
            
            else {
                screenSource1.setFrame(currentFrameCount + direction_increment);
                screenSource2.setFrame(currentFrameCount + direction_increment);
                ledSource.setFrame(currentFrameCount + direction_increment);
            }
            
            break;
            
        case '<':
        case ',':
            
            
            if(currentFrameCount <= arrow_increment){
                screenSource1.setFrame(0);
                screenSource2.setFrame(0);
                ledSource.setFrame(0);
            }
            
            else {
                screenSource1.setFrame(currentFrameCount - arrow_increment);
                screenSource2.setFrame(currentFrameCount - arrow_increment);
                ledSource.setFrame(currentFrameCount - arrow_increment);
            }
            
            break;
            
        case '>':
        case '.':
            
            if(currentFrameCount + arrow_increment > screenSource1.getTotalNumFrames()){
                screenSource1.setFrame(screenSource1.getTotalNumFrames());
                screenSource2.setFrame(screenSource2.getTotalNumFrames());
                ledSource.setFrame(ledSource.getTotalNumFrames());
            }
            
            else {
                screenSource1.setFrame(currentFrameCount + arrow_increment);
                screenSource2.setFrame(currentFrameCount + arrow_increment);
                ledSource.setFrame(currentFrameCount + arrow_increment);
            }
            
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
    if(_debug){
        
        mouseX = x;
        mouseY = y;
        
        if(x > timelineX && y > timelineY
           && x < timelineX + timelineWidth
           && y < timelineY + timelineHeight) {
            
            float newPosition = ofMap( float(x - timelineX), 0, float(timelineWidth), 0, 1);
            
            screenSource1.setPosition(newPosition);
            screenSource2.setPosition(newPosition);
            ledSource.setPosition(newPosition);
            
            if(numPis > 1){
                ledSource2.setPosition(newPosition);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // clicked within timeline
    if(_debug){
        if(x > timelineX && y > timelineY
           && x < timelineX + timelineWidth
           && y < timelineY + timelineHeight) {
            
            float newPosition = ofMap( float(x - timelineX), 0, float(timelineWidth), 0, 1);
            
            screenSource1.setPosition(newPosition);
            screenSource2.setPosition(newPosition);
            ledSource.setPosition(newPosition);
            
            if(numPis > 1){
                ledSource2.setPosition(newPosition);
            }
        }
    }
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

void ofApp::drawTimeline(int x, int y) {
    ofSetColor(0,0,255);
    ofRect(x, y, timelineWidth, timelineHeight);
    ofSetColor(255,255,255);
    ofRect(x, y +  10, int(screenSource1.getPosition()*timelineWidth), timelineHeight - 10);
    ofSetColor(255);
}

void ofApp::drawTimecode(int x, int y) {
    // calculate current time
    int currentSeconds = int(screenSource1.getDuration() * screenSource1.getPosition());
    
    string timeString = ofToString(currentSeconds / 60) + ":";
    
    if(currentSeconds % 60 > 9){
        timeString = timeString + ofToString(currentSeconds % 60);
    }
    else{
        timeString = timeString + "0" + ofToString(currentSeconds % 60);
    }
    
    ofLog() << "seconds at: " << ofToString(currentSeconds) << endl;
    
    ofSetColor(255,255,255);
    ofDrawBitmapString(timeString, x, y);
    
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