#pragma once

#include "pch.h"
#include "Utils.h"
#include "gdstring.h"

enum kTypeSearchMusicLayer {
	kTypeSearchNone = 0,
	kTypeSearchSongName = 1,
	kTypeSearchArtistName = 2
};

class SearchMusicLayer : public gd::FLAlertLayer, public cocos2d::CCTextFieldDelegate, public gd::FLAlertLayerProtocol, public gd::TextInputDelegate {
public:

	static SearchMusicLayer* create(gd::GJSongBrowser* parent);
	bool init(gd::GJSongBrowser* parent);
	virtual void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes key);
	void onClose(cocos2d::CCObject* pSender);
	void onSearchMusic(cocos2d::CCObject* pSender);
	void onSearchArtist(cocos2d::CCObject* pSender);
	virtual void textChanged(gd::CCTextInputNode* input);
	void updateParentAContent();
	void onSettings(cocos2d::CCObject* pSender);
	void onDeleteTextSearch(cocos2d::CCObject* pSender);

	static kTypeSearchMusicLayer typeSearch;
	static std::string stringToSearch;
	cocos2d::extension::CCScale9Sprite* m_pBG = nullptr;
	cocos2d::CCMenu* m_pMenuButtons = nullptr;
	cocos2d::CCLabelBMFont* m_TitleLabel = nullptr;
	cocos2d::CCLabelBMFont* m_pDescriptionLabel = nullptr;
	cocos2d::extension::CCScale9Sprite* m_pBGTextInput = nullptr;
	cocos2d::extension::CCScale9Sprite* m_pBGTextInput_2 = nullptr;
	gd::CCTextInputNode* m_pTextInput = nullptr;
	gd::CCMenuItemSpriteExtra* m_pSearchMusicName = nullptr;
	gd::CCMenuItemSpriteExtra* m_pSearchArtistName = nullptr;
	gd::GJSongBrowser* m_pParentA = nullptr;

};

struct ListData {
	const char* m_sTitle;
	std::vector<std::string> m_vEntries;
	unsigned int m_uMaxLength;
	unsigned int m_uLength;
	unsigned int m_uOffset;
	unsigned int m_uIndex;

	ListData(const char* title, unsigned int length);
	ListData() { /*doesn't matter since only used in default ctor of LoaderManager*/ };
};

enum {
	kListUpBtn,
	kListDownBtn,
	kVerticalListSwapUpBtn,
	kVerticalListSwapDownBtn,
	kVerticalListMoveBtn,
	kVerticalListBackground
};


class HorizontalList : public cocos2d::CCNode {
protected:
	ListData& m_pData;
	cocos2d::CCLabelBMFont** m_pLabels = nullptr;
	cocos2d::CCMenu* m_pMenu = nullptr;

protected:
	HorizontalList(ListData& data) : m_pData{ data } {}
	virtual ~HorizontalList() { delete[] m_pLabels; }

	virtual bool init();

	inline void getLength() {
		m_pData.m_uLength = m_pData.m_vEntries.size() -
			m_pData.m_uOffset < m_pData.m_uMaxLength ?
			(m_pData.m_vEntries.size() - m_pData.m_uOffset)
			: m_pData.m_uMaxLength;
	}
	virtual void navigate(cocos2d::CCObject* btn);

public:
	static HorizontalList* create(ListData& data);

	virtual void updateList();
	virtual void setPosition(float x, float y);
};


class SearchMusicSettingsLayer : public gd::GJDropDownLayer
{
public:
	cocos2d::CCMenu* m_menuButtons = nullptr;

	static SearchMusicSettingsLayer* create();
	virtual void customSetup();
	virtual void exitLayer(cocos2d::CCObject*);
	void onLiveSearch(cocos2d::CCObject* pSender);
	void onLiveSearchUsingArtistName(cocos2d::CCObject* pSender);
};