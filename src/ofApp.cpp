#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	lut.resize(256);
	originalImg.load("pic.png");
	img = originalImg;
	checkImageReady();
	ofDisableArbTex();
	if (img.isAllocated()) {
		fbo.allocate(img.getWidth(), img.getHeight(), GL_RGBA);
	}

	cam.setDistance(2000);
	setupGUI();
	updateLUT();
	generateGlitches();
}
void ofApp::setupGUI() {
	gui.setup("Vaporwave Effect Settings");
	ColorParams.setName("Color Mapping");
	ColorParams.add(color1.set("Deep Blue", ofColor(0, 0, 128)));
	ColorParams.add(color2.set("Magenta", ofColor(255, 0, 255)));
	ColorParams.add(color3.set("Cyan", ofColor(0, 255, 255)));
	ColorParams.add(color4.set("White", ofColor(255, 255, 255)));
	ColorParams.add(threshold1.set("Thresh 1", 0.33, 0.01f, 1.0));
	ColorParams.add(threshold2.set("Thresh 2", 0.66, 0.01f, 1.0));
	ColorParams.add(applyEffect.set("Apply Effect", false));
	GlitchParams.setName("Glitch Settings");
	GlitchParams.add(glitchCount.set("Glitch Count", 5, 0, 50));
	GlitchParams.add(maxOffset.set("Max Offset", 50, 0, 200));
	GlitchParams.add(minHeight.set("Min Height", 10, 1, 200));
	GlitchParams.add(maxHeight.set("Max Height", 100, 1, 400));
	GlitchParams.add(applyGlitch.set("Apply Glitch", false));
	TextureParams.setName("Texture Settings");
	TextureParams.add(scanlineDensity.set("Scanline Density", 4, 1, 20));
	TextureParams.add(scanlineAlpha.set("Scanline Alpha", 40, 0, 255));
	TextureParams.add(applyScanlines.set("Apply Scanlines", false));
	TextureParams.add(noiseCount.set("Noise Count", 1000, 0, 5000));
	TextureParams.add(noiseAlpha.set("Noise Alpha", 50, 0, 255));
	TextureParams.add(applyNoise.set("Apply Noise", false));
	
	gui.add(ColorParams);
	gui.add(GlitchParams);
	gui.add(TextureParams);

	ofAddListener(ColorParams.parameterChangedE(), this, &ofApp::onParameterChanged);
	ofAddListener(GlitchParams.parameterChangedE(), this, &ofApp::onParameterChanged);
}
void ofApp::onParameterChanged(ofAbstractParameter& param){
	string name = param.getName();
	if (name == "Glitch Count" || name == "Max Offset" || name == "Min Height" || name == " Max Height" || name == "Apply Glitch") {
		generateGlitches();
	}
	updateLUT();
	img = originalImg;
	if (applyEffect) {
		applyVaporwaveEffect(img, lut);
	}
}
//--------------------------------------------------------------
void ofApp::update(){
	if (gui.getShape().inside(ofGetMouseX(), ofGetMouseY())) {
		cam.disableMouseInput();
	}
	else {
		cam.enableMouseInput();
	}
}
void ofApp::updateLUT() {
	for (int i = 0; i < 256; i++) {
		float pct = i / 255.0f;
		if (pct < threshold1) {
			float localPct = pct / threshold1;
			lut[i] = color1.get().getLerped(color2.get(), localPct);
		}
		else if (pct < threshold2) {
			float localPct = (pct - threshold1) / (threshold2 - threshold1);
			lut[i] = color2.get().getLerped(color3.get(), localPct);
		}
		else {
			float localPct = (pct - threshold2) / (1.0f - threshold2);
			lut[i] = color3.get().getLerped(color4.get(), localPct);
		}
	}
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(50);

	fbo.begin();
	ofClear(255, 255, 255, 255);

	img.draw(0, 0); 
	if (applyGlitch) {
		for (auto& g : glitchSettings) {
			// 画像の一部を切り取って、オフセットを加えて上書き描画
			// drawSubsection(描画先x, 描画先y, 幅, 高さ, 転送元x, 転送元y)
			img.drawSubsection(g.offset, g.y, img.getWidth(), g.height, 0, g.y);
		}
	}
	if(applyScanlines) {
		ofSetColor(0, 0, 0, scanlineAlpha);
		ofSetLineWidth(1);
		for (int i = 0; i < fbo.getHeight(); i += scanlineDensity) {
			ofDrawLine(0, i, fbo.getWidth(), i);
		}
	}
	if (applyNoise) {
		ofSetColor(0, 255, 255, noiseAlpha);
		for (int i = 0; i < noiseCount; i++) {
			ofDrawCircle(ofRandom(fbo.getWidth()), ofRandom(fbo.getHeight()), 4);
		}
	}
	ofSetColor(255); // 色をリセット

	fbo.end();

	cam.begin();
	ofEnableDepthTest();

	float scale = 0.5f;
	float w = fbo.getWidth() * scale;
	float h = fbo.getHeight() * scale;
	fbo.getTexture().draw(-w / 2, -h / 2, w, h);
	ofDisableDepthTest();
	cam.end();
	// 3. 最後にGUI（最前面）
	gui.draw();
}
void ofApp::applyVaporwaveEffect(ofImage& img, const vector<ofColor>& lut) {
	if (!img.isAllocated()) return;

	ofPixels& pix = img.getPixels();
	unsigned char* data = pix.getData();
	size_t numPixels = pix.getWidth() * pix.getHeight();
	size_t channels = pix.getNumChannels();

	for (int i = 0; i < numPixels; i++) {
		size_t idx = i * channels;
		unsigned char r = data[idx];
		unsigned char g = (channels >= 3) ? data[idx + 1] : r;	
		unsigned char b = (channels >= 3) ? data[idx + 2] : r;

		int brightness = static_cast<int>(0.299f * r + 0.587f * g + 0.114f * b);
		brightness = ofClamp(brightness, 0, 255);

		ofColor target = lut[brightness];
		data[idx] = target.r;
		data[idx + 1] = (channels >= 3) ? target.g : target.r;
		data[idx + 2] = (channels >= 3) ? target.b : target.r;
	}
	img.update();
}
void ofApp::generateGlitches() {
	glitchSettings.clear();
	for (int i = 0; i < glitchCount; i++) {
		GlitchRect r;
		r.y = ofRandom(0, originalImg.getHeight());
		r.height = ofRandom(10, 100);
		r.offset = ofRandom(-maxOffset, maxOffset);
		glitchSettings.push_back(r);
	}
}
void ofApp::checkImageReady() {
	if (img.isAllocated()) {
		// Image is ready, you can perform further operations if needed
	}
	else {
		ofLog() << "Image not allocated properly.";
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
