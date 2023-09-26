#pragma once

#include "pch.h"
#include "Utils.h"
#include "gdstring.h"

class SliderSongControl : public cocos2d::CCNode {
public:
	static SliderSongControl* create(gd::SongInfoObject* song);
	bool init(gd::SongInfoObject* song);
	void update(float dt);
	void musicSliderChanged(cocos2d::CCObject* pSender);
	void createSlider();
	void createLabels();
	cocos2d::CCLabelBMFont* createLabel(const char* text, const char* font, float scale, const cocos2d::CCPoint& position, const cocos2d::CCPoint& anchor);
	unsigned int getSongLength(bool currentSong);
	void resetUI();
	void updateSliderAndTimeLabels(unsigned int currentPosition, unsigned int totalDuration);
	void tryAutoPlayMusic();

	gd::SongInfoObject* m_pSongObj = nullptr;
	gd::Slider* m_pSliderMusic = nullptr;
	cocos2d::CCLabelBMFont* m_pCurrentTimeSong = nullptr;
	cocos2d::CCLabelBMFont* m_pStartTimeSong = nullptr;
	cocos2d::CCLabelBMFont* m_pEndTimeSong = nullptr;
	bool onChanged = false;
};

class SongControlLayer : public gd::FLAlertLayer, public cocos2d::CCTextFieldDelegate, public gd::FLAlertLayerProtocol {
public:
	static bool isGameShow;
	static SongControlLayer* create(gd::GJSongBrowser* parent, int i);
	bool init(gd::GJSongBrowser* parent, int i);
	virtual void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes key);
	void onPrevSong(cocos2d::CCObject* pSender);
	void onNextSong(cocos2d::CCObject* pSender);
	void onUseSong(cocos2d::CCObject* pSender);
	void onPauseSong(cocos2d::CCObject* pSender);
	void onDeleteSong(cocos2d::CCObject* pSender);
	void changeToSongAtIndex(int index);
	void updateButtonUse();
	void updateButtonPause();
	void DeleteMusic(cocos2d::CCObject* pSender);
	void onSettings(cocos2d::CCObject* pSender);

	gd::GJSongBrowser* m_pParentA = nullptr;
	cocos2d::extension::CCScale9Sprite* m_pBG = nullptr;
	gd::CustomSongWidget* m_CSWCurrent = nullptr;
	SliderSongControl* m_pSliderMusic = nullptr;
	gd::ButtonSprite* imageButton7_ = nullptr;
	gd::ButtonSprite* imageButton6_ = nullptr;
	gd::CCMenuItemSpriteExtra* buttonUse = nullptr;
	cocos2d::CCLabelBMFont* m_lNumberSongTotal = nullptr;
	int m_iPag = 0;
	int m_iIndex = 0;
};

class SongControlSettingsLayer : public gd::GJDropDownLayer
{
public:
	cocos2d::CCMenu* m_menuButtons = nullptr;
	SongControlLayer* m_pParentE = nullptr;
	static SongControlSettingsLayer* create(SongControlLayer* lay);
	virtual void customSetup();
	virtual void exitLayer(cocos2d::CCObject*);
	void onShowMusicPosition(cocos2d::CCObject* pSender);
	void onAutoPlayNextSong(cocos2d::CCObject* pSender);
	void onAutoPlayOnSongChange(cocos2d::CCObject* pSender);
	void onSelectKeyLayer(cocos2d::CCObject* pSender);
};

class SelectKeyLayer : public gd::FLAlertLayer, public cocos2d::CCTextFieldDelegate, public gd::FLAlertLayerProtocol, public gd::TextInputDelegate {
public:

	static SelectKeyLayer* create();
	bool init();
	virtual void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes key);
	void onSetKey(cocos2d::CCObject* pSender);
	std::string getNameKeyWithCode(cocos2d::enumKeyCodes key);
	void updateKeySelected(cocos2d::enumKeyCodes key);

	cocos2d::CCMenu* m_pButtonsMenu = nullptr;
	gd::CCMenuItemSpriteExtra* m_pSetKeyBtn = nullptr;
	cocos2d::extension::CCScale9Sprite* m_pBGInputTextValue = nullptr;
	gd::CCTextInputNode* m_pInputTextValue = nullptr;
	int keySelect = 0;
};