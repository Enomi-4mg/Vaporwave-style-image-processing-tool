#pragma once

#include "ofMain.h"
#include "ofxGui.h"

struct GlitchRect {
	float y;       // 開始高さ
	float height;  // 矩形の高さ
	float offset;  // 左右のズレ幅
};

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		std::vector<ofColor> lut;
		ofEasyCam cam;
		ofImage img, originalImg;
		ofxPanel gui;
		ofParameterGroup ColorParams;
		ofParameterGroup GlitchParams;
		ofParameterGroup params;
		ofParameter<ofColor> color1, color2, color3, color4;
		ofParameter<float> threshold1, threshold2;
		ofParameter<bool> applyEffect;

		ofFbo fbo; // グリッチ描画用のバッファ
		vector<GlitchRect> glitchSettings; // 固定されたグリッチ位置のリスト

		ofParameter<int> glitchCount;  // 矩形の数
		ofParameter<float> maxOffset;  // 最大のズレ幅
		ofParameter<float> minHeight, maxHeight; // 矩形の高さの範囲
		ofParameter<bool> applyGlitch; // グリッチ効果の適用

		ofParameterGroup TextureParams;
		ofParameter<int> scanlineDensity; // 走査線の間隔
		ofParameter<int> scanlineAlpha;   // 走査線の濃さ
		ofParameter<bool> applyScanlines; // 走査線効果の適用
		ofParameter<int> noiseCount;      // ノイズの粒の数
		ofParameter<int> noiseAlpha;      // ノイズの透明度
		ofParameter<bool> applyNoise;     // ノイズ効果の適用

		void setupGUI();
		void updateLUT();
		void applyVaporwaveEffect(ofImage& img, const vector<ofColor>& lut);
		void onParameterChanged(ofAbstractParameter& param);
		void generateGlitches();
		
	private:
		void checkImageReady();
};
