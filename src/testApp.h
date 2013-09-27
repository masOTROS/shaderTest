#pragma once

#include "ofMain.h"
#include "ofxUI.h"

#define VIDEO_SCALE 0.5
#define VIDEO_W 1280
#define VIDEO_H 720

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void exit();
    
    ofVideoGrabber video;
    
    ofxUISuperCanvas *      gui;
	void guiEvent(ofxUIEventArgs &e);
    
    bool fishEye;
    ofShader fishEyeShader;
    float fishEyeLensradius,fishEyeSigncurvature;
    
    bool brcosa;
    ofShader brcosaShader;
    float brcosaContrast,brcosaBrightness,brcosaSaturation,brcosaAlpha;
    
    bool bloom;
    ofShader bloomShader;
    float bloomBloom;
    
    bool lut;
    ofShader lutShader;
    ofTexture lutTex;
    
    bool xpro;
    ofShader xproShader;
    ofTexture xproMapTex;
    ofTexture xproVigTex;
    
    bool toaster;
    ofShader toasterShader;
    ofTexture toasterMetal;
    ofTexture toasterSoftlight;
    ofTexture toasterCurves;
    ofTexture toasterOverlayMapWarm;
    ofTexture toasterColorshift;
    
    bool early;
    ofShader earlyShader;
    ofTexture earlyCurves;
    ofTexture earlyOverlay;
    ofTexture earlyVignette;
    ofTexture earlyBlowout;
    ofTexture earlyMap;
    
    bool shape;
    ofShader shapeShader;
    ofFbo shapeFbo;
    
    bool fire;
    ofShader fireShader;
    
    void toggleFbo();
    ofFbo ping,pong;
    bool pingPong;
    ofFbo * now;
    ofFbo * last;
};
