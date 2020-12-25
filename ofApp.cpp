#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofNoFill();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 500, true, true, true);
	this->word = "A";
	int sample_count = 300;
	vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word_path.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			this->font_location_list.insert(this->font_location_list.end(), vertices.begin(), vertices.end());
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 3;
	while (this->log_list.size() < 1000) {

		int font_location_index = ofRandom(this->font_location_list.size());
		vector<glm::vec2> log;
		log.push_back(this->font_location_list[font_location_index]);
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(30, 60));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec3(this->log_list[i].back() * 0.005, ofGetFrameNum() * 0.003)), 0, 1, -360, 360);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5 - this->font.stringWidth(this->word) * 0.5, ofGetHeight() * 0.5 + this->font.stringHeight(this->word) * 0.5);

	ofSetColor(39);
	this->font.drawStringAsShapes(this->word, 0, 0);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1.5);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1.5));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}