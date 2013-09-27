#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    video.initGrabber(VIDEO_W,VIDEO_H);
    
    ping.allocate(VIDEO_W,VIDEO_H,GL_RGBA);
    pong.allocate(VIDEO_W,VIDEO_H,GL_RGBA);
    pingPong=false;
    toggleFbo();
    
    fishEye=true;
    fishEyeShader.load("shaders/fisheye/FishEye_GLSL");
    fishEyeLensradius=2;
    fishEyeSigncurvature=4;
    
    brcosa=true;
    brcosaShader.load("shaders/brcosa/brcosa_GLSL");
    brcosaContrast=1.;
    brcosaBrightness=1.;
    brcosaSaturation=1.;
    brcosaAlpha=1.;
    
    bloom=true;
    bloomShader.load("shaders/bloom/bloom_GLSL");
    bloomBloom=1.;
    
    lut=true;
    lutShader.load("shaders/LUT/LUT_GLSL");
    ofLoadImage(lutTex,"shaders/LUT/lookup_amatorka.png");
    
    xpro=true;
    xproShader.load("shaders/Xpro/Xpro_GLSL");
    ofLoadImage(xproMapTex,"shaders/Xpro/xproMap.png");
    ofLoadImage(xproVigTex,"shaders/vignetteMap.png");
    
    toaster=true;
    toasterShader.load("shaders/toaster/toaster_GLSL");
    ofLoadImage(toasterMetal,"shaders/toaster/toasterMetal.png");
    ofLoadImage(toasterSoftlight,"shaders/toaster/toasterSoftlight.png");
    ofLoadImage(toasterCurves,"shaders/toaster/toasterCurves.png");
    ofLoadImage(toasterOverlayMapWarm,"shaders/toaster/toasterOverlayMapWarm.png");
    ofLoadImage(toasterColorshift,"shaders/toaster/toasterColorshift.png");
    
    early=true;
    earlyShader.load("shaders/earlybird/earlybird_GLSL");
    ofLoadImage(earlyCurves,"shaders/earlybird/earlybirdCurves.png");
    ofLoadImage(earlyOverlay,"shaders/earlybird/earlybirdOverlayMap.png");
    ofLoadImage(earlyVignette,"shaders/vignetteMap.png");
    ofLoadImage(earlyBlowout,"shaders/earlybird/earlybirdBlowout.png");
    ofLoadImage(earlyMap,"shaders/earlybird/earlybirdMap.png");
    
    shape=true;
    shapeShader.load("shaders/shape/shape_GLSL");
    shapeFbo.allocate(VIDEO_W,VIDEO_H,GL_RGB);
    
    fire=true;
    fireShader.load("shaders/fire/fire_GLSL");
    
    gui = new ofxUISuperCanvas("Shader Test", OFX_UI_FONT_MEDIUM);
    gui->addFPS();
    gui->addSpacer();
    gui->addTextArea("shaderTest", "Parameters");
    gui->addSpacer();
    gui->addLabelToggle("FishEye", &fishEye);
    gui->addSlider("lensradius",-10,10,&fishEyeLensradius);
    gui->addSlider("signcurvature",-10,10,&fishEyeSigncurvature);
    gui->addSpacer();
    gui->addLabelToggle("Brcosa", &brcosa);
    gui->addSlider("contrast", -1,2, &brcosaContrast);
    gui->addSlider("brightness", 0,2, &brcosaBrightness);
    gui->addSlider("saturation", -10,10, &brcosaSaturation);
    gui->addSlider("alpha", 0,1, &brcosaAlpha);
    gui->addSpacer();
    gui->addLabelToggle("Bloom", &bloom);
    gui->addSlider("bloom", -5,5, &bloomBloom);
    gui->addSpacer();
    gui->addLabelToggle("LUT", &lut);
    gui->addSpacer();
    gui->addLabelToggle("Xpro", &xpro);
    gui->addSpacer();
    gui->addLabelToggle("Toaster", &toaster);
    gui->addSpacer();
    gui->addLabelToggle("EarlyBird", &early);
    gui->addSpacer();
    gui->addLabelToggle("Shape", &shape);
    gui->addSpacer();
    gui->addLabelToggle("Fire", &fire);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    if(ofFile::doesFileExist("GUI/guiSettings.xml"))
        gui->loadSettings("GUI/guiSettings.xml");
    
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    ofLoadImage(lutTex,dragInfo.files[0]);
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    if(video.isFrameNew()){
        
        toggleFbo();
        now->begin();
        ofClear(0,0);
        video.draw(0,0);
        now->end();
        
        ofEnableNormalizedTexCoords();
        
        if(brcosa){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            brcosaShader.begin();
            brcosaShader.setUniformTexture("tex",last->getTextureReference(),0);
            brcosaShader.setUniform1f("saturation", brcosaSaturation);
            brcosaShader.setUniform1f("contrast", brcosaContrast);
            brcosaShader.setUniform1f("brightness", brcosaBrightness);
            brcosaShader.setUniform1f("alpha", brcosaAlpha);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            brcosaShader.end();
            now->end();
        }
        
        if(bloom){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            bloomShader.begin();
            bloomShader.setUniformTexture("tex0",last->getTextureReference(),0);
            bloomShader.setUniform1f("bloom",bloomBloom);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            bloomShader.end();
            now->end();
        }
        
        if(lut){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            lutShader.begin();
            lutShader.setUniformTexture("tex",last->getTextureReference(),0);
            lutShader.setUniformTexture("texLUT",lutTex,1);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            lutShader.end();
            now->end();
        }
        
        if(xpro){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            xproShader.begin();
            xproShader.setUniformTexture("tex",last->getTextureReference(),0);
            xproShader.setUniformTexture("texMap",xproMapTex,1);
            xproShader.setUniformTexture("texVig",xproVigTex,2);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            xproShader.end();
            now->end();
        }
        
        if(toaster){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            toasterShader.begin();
            toasterShader.setUniformTexture("inputImageTexture",last->getTextureReference(),0);
            toasterShader.setUniformTexture("inputImageTexture2",toasterMetal,1);
            toasterShader.setUniformTexture("inputImageTexture3",toasterSoftlight,2);
            toasterShader.setUniformTexture("inputImageTexture4",toasterCurves,3);
            toasterShader.setUniformTexture("inputImageTexture5",toasterOverlayMapWarm,4);
            toasterShader.setUniformTexture("inputImageTexture6",toasterColorshift,5);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            toasterShader.end();
            now->end();
        }
        
        if(early){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            earlyShader.begin();
            earlyShader.setUniformTexture("inputImageTexture",last->getTextureReference(),0);
            earlyShader.setUniformTexture("inputImageTexture2",earlyCurves,1);
            earlyShader.setUniformTexture("inputImageTexture3",earlyOverlay,2);
            earlyShader.setUniformTexture("inputImageTexture4",earlyVignette,3);
            earlyShader.setUniformTexture("inputImageTexture5",earlyBlowout,4);
            earlyShader.setUniformTexture("inputImageTexture6",earlyMap,5);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            earlyShader.end();
            now->end();
        }
        
        if(fire){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            fireShader.begin();
            fireShader.setUniformTexture("tex",last->getTextureReference(),0);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            fireShader.end();
            now->end();
        }
        
        if(shape){
            shapeFbo.begin();
            ofClear(255);
            ofSetColor(0);
            ofFill();
            ofBeginShape();
            ofVertex(shapeFbo.getWidth()*0.15,shapeFbo.getHeight()*0.05);
            ofVertex(shapeFbo.getWidth()*0.325+200*sin(5*ofGetElapsedTimef()*1.05),shapeFbo.getHeight()*0.5);
            ofVertex(shapeFbo.getWidth()*0.5,shapeFbo.getHeight()*0.95);
            ofVertex(shapeFbo.getWidth()*0.675+200*sin(5*ofGetElapsedTimef()*1.033),shapeFbo.getHeight()*0.5);
            ofVertex(shapeFbo.getWidth()*0.85,shapeFbo.getHeight()*0.05);
            ofEndShape();
            shapeFbo.end();
            
            toggleFbo();
            now->begin();
            ofClear(0,0);
            shapeShader.begin();
            shapeShader.setUniformTexture("tex",last->getTextureReference(),0);
            shapeShader.setUniformTexture("shape",shapeFbo.getTextureReference(),1);
            shapeShader.setUniform1f("width",shapeFbo.getWidth());
            shapeShader.setUniform1f("height",shapeFbo.getHeight());
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            shapeShader.end();
            now->end();
        }
        
        if(fishEye){
            toggleFbo();
            now->begin();
            ofClear(0,0);
            fishEyeShader.begin();
            fishEyeShader.setUniformTexture("tex0",last->getTextureReference(),0);
            fishEyeShader.setUniform1f("lensradius",fishEyeLensradius);
            fishEyeShader.setUniform1f("signcurvature",fishEyeSigncurvature);
            ofSetColor(255);
            last->getTextureReference().bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(0,0,0);
            glTexCoord2f(1,0); glVertex3f(VIDEO_W,0,0);
            glTexCoord2f(1,1); glVertex3f(VIDEO_W,VIDEO_H,0);
            glTexCoord2f(0,1); glVertex3f(0,VIDEO_H,0);
            glEnd();
            last->getTextureReference().unbind();
            fishEyeShader.end();
            now->end();
        }
    }
}

//--------------------------------------------------------------
void testApp::toggleFbo(){
    pingPong=!pingPong;
    if(pingPong){
        now=&ping;
        last=&pong;
    }
    else{
        now=&pong;
        last=&ping;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofPushMatrix();
    ofScale(VIDEO_SCALE,VIDEO_SCALE);
    
    video.draw(0,0);

    now->draw(0,VIDEO_H);
    
    ofPopMatrix();

}

//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("GUI/guiSettings.xml");
    
    delete gui;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}