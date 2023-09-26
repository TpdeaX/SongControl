#include "pch.h"
#include "SearchMusicLayer.h"
#include "fts_fuzzy_match.h"

kTypeSearchMusicLayer SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
std::string SearchMusicLayer::stringToSearch = "";

SearchMusicLayer* SearchMusicLayer::create(gd::GJSongBrowser* parent) {
	auto ret = new SearchMusicLayer();

	if (ret && ret->init(parent)) {
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}
bool SearchMusicLayer::init(gd::GJSongBrowser* parent) {

	if (!initWithColor({ 0, 0, 0, 105 })) return false;

	this->m_pParentA = parent;

	m_pLayer = cocos2d::CCLayer::create();
	this->addChild(m_pLayer);

	this->m_pBG = cocos2d::extension::CCScale9Sprite::create("GJ_button_05.png");
	this->m_pBG->setContentSize({ 360.f, 150.f });
	this->m_pBG->setPosition({ Utils::winSize().width / 2.f, Utils::winSize().height / 2.f });
	m_pLayer->addChild(this->m_pBG);

	m_pButtonMenu = cocos2d::CCMenu::create();
	m_pButtonMenu->setPosition({ Utils::winSize().width / 2.f - 175.f, Utils::winSize().height / 2.f + 69.f }); // 280.5  160
	auto imageClose = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	imageClose->setScale(0.8f);
	auto buttonExtraItem = gd::CCMenuItemSpriteExtra::create(imageClose, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onClose);
	m_pButtonMenu->addChild(buttonExtraItem);
	m_pLayer->addChild(m_pButtonMenu);

	m_TitleLabel = cocos2d::CCLabelBMFont::create("Search Song", "bigFont.fnt");
	m_TitleLabel->setPosition({ Utils::winSize().width / 2.f,Utils::winSize().height / 2.f + 54.f });
	m_TitleLabel->setScale(0.8f);
	m_pLayer->addChild(m_TitleLabel);

	m_pDescriptionLabel = cocos2d::CCLabelBMFont::create("Enter the name of the song or\nthe name of the author", "goldFont.fnt");
	m_pDescriptionLabel->setAlignment(cocos2d::kCCTextAlignmentCenter);
	m_pDescriptionLabel->setPosition({ Utils::winSize().width / 2.f,Utils::winSize().height / 2.f + 18.f });
	m_pDescriptionLabel->setScale(0.5f);
	m_pLayer->addChild(m_pDescriptionLabel);

	m_pBGTextInput = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
	m_pBGTextInput->setContentSize({ 210.f, 35.f });
	m_pBGTextInput->setPosition({ Utils::winSize().width / 2.f - 54.5f,Utils::winSize().height / 2.f - 34.f });
	m_pBGTextInput->setOpacity(static_cast<GLubyte>(50));
	m_pLayer->addChild(m_pBGTextInput);

	m_pBGTextInput_2 = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
	m_pBGTextInput_2->setContentSize({ 255.f, 35.f });
	m_pBGTextInput_2->setScale(0.8f);
	m_pBGTextInput_2->setPosition({ Utils::winSize().width / 2.f - 54.5f,Utils::winSize().height / 2.f - 34.f });
	m_pBGTextInput_2->setOpacity(static_cast<GLubyte>(50));
	m_pLayer->addChild(m_pBGTextInput_2);

	m_pTextInput = gd::CCTextInputNode::create("Search", this, "bigFont.fnt", 190.f, 20.f);
	m_pTextInput->setLabelPlaceholderColor({ 0x75, 0xAA, 0xF0 });
	m_pTextInput->setMaxLabelScale(0.7f);
	m_pTextInput->setPosition({ -41.f, -14.f });
	m_pTextInput->setDelegate(this);
	m_pTextInput->setPosition({ Utils::winSize().width / 2.f - 54.5f,Utils::winSize().height / 2.f - 34.f });
	m_pLayer->addChild(m_pTextInput);

	m_pMenuButtons = cocos2d::CCMenu::create();
	m_pMenuButtons->setPosition({ 0.f, 0.f });
	m_pLayer->addChild(m_pMenuButtons);

	auto buttonSpriteSearchMusic = gd::ButtonSprite::create("Search", 0xFC, false, "goldFont.fnt", "GJ_button_04.png", 30.f, 0.6f);
	buttonSpriteSearchMusic->setScale(0.8f);
	m_pSearchMusicName = gd::CCMenuItemSpriteExtra::create(buttonSpriteSearchMusic, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onSearchMusic);
	m_pSearchMusicName->setPosition({ Utils::winSize().width / 2.f + 90.5f, Utils::winSize().height / 2.f - 34.f });
	m_pMenuButtons->addChild(m_pSearchMusicName);

	auto spriteSearchArtistName = cocos2d::CCSprite::createWithSpriteFrameName("GJ_longBtn05_001.png");
	spriteSearchArtistName->setScale(0.8f);
	m_pSearchArtistName = gd::CCMenuItemSpriteExtra::create(spriteSearchArtistName, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onSearchArtist);
	m_pSearchArtistName->setPosition({ Utils::winSize().width / 2.f + 145.5f, Utils::winSize().height / 2.f - 34.f });
	m_pMenuButtons->addChild(m_pSearchArtistName);

	auto settingsButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	settingsButtonImage->setScale(0.6f);
	auto settingsButton = gd::CCMenuItemSpriteExtra::create(settingsButtonImage, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onSettings); //(cocos2d::SEL_MenuHandler)&BetterPause::onOptionsLayer);
	settingsButton->setPosition({ Utils::winSize().width / 2.f + 145.5f, Utils::winSize().height / 2.f + 20.f });
	m_pMenuButtons->addChild(settingsButton);

	auto deleteTextButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png");
	settingsButtonImage->setScale(0.6f);
	auto deleteTextButton = gd::CCMenuItemSpriteExtra::create(deleteTextButtonImage, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onDeleteTextSearch); //(cocos2d::SEL_MenuHandler)&BetterPause::onOptionsLayer);
	deleteTextButton->setPosition({ Utils::winSize().width / 2.f + 145.5f, Utils::winSize().height / 2.f + 54.5f });
	m_pMenuButtons->addChild(deleteTextButton);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
	this->setKeyboardEnabled(true);
	this->setMouseEnabled(true);
	this->registerWithTouchDispatcher();
	Utils::shareDirectorA()->getTouchDispatcher()->setForcePrio(true);
	Utils::shareDirectorA()->getTouchDispatcher()->incrementForcePrio(2);
	this->setTouchPriority(2);

	return true;
}

void SearchMusicLayer::onSettings(cocos2d::CCObject* pSender) {
	auto dropDown = SearchMusicSettingsLayer::create();
	this->addChild(dropDown);
	dropDown->setPosition(0.0f, 0.0f);
	dropDown->showLayer(false);
}

void SearchMusicLayer::onDeleteTextSearch(cocos2d::CCObject* pSender) {
	this->m_pTextInput->setString("");
	this->m_pTextInput->onClickTrackNode(false);
}

void SearchMusicLayer::updateParentAContent() {
	if (this->m_pParentA) {
		auto parentAArray = MBO(cocos2d::CCArray*, this->m_pParentA, 0x1dc);
		parentAArray->removeAllObjects();
		parentAArray->initWithArray(gd::MusicDownloadManager::sharedState()->getDownloadedSongs());
		parentAArray->retain();
		this->m_pParentA->loadPage(0);

		auto menuButtonsCreateExt = reinterpret_cast<cocos2d::CCMenu*>(this->m_pParentA->m_pLayer->getChildByTag(13));
		auto searchButton = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(menuButtonsCreateExt->getChildByTag(14));
		searchButton->setVisible(!stringToSearch.empty());
	}
}


void SearchMusicLayer::keyBackClicked() {
	/*this->m_pParentA->hideLayer(true);
	this->m_pParentA = gd::GJSongBrowser::create(Utils::shareManager()->getEditorLayer() ? 
		Utils::shareManager()->getEditorLayer()->m_pLevelSettings : 0);
	cocos2d::CCDirector::sharedDirector()
		->getRunningScene()
		->addChild(this->m_pParentA, cocos2d::CCDirector::sharedDirector()->getRunningScene()->getHighestChildZ());
	this->m_pParentA->showLayer(true);*/
	updateParentAContent();

	gd::FLAlertLayer::keyBackClicked();
}

void SearchMusicLayer::keyDown(cocos2d::enumKeyCodes key) {
	switch (key) {
	case cocos2d::enumKeyCodes::KEY_Space:
		break;
	case cocos2d::enumKeyCodes::CONTROLLER_X:
		this->keyBackClicked();
		break;
	case cocos2d::enumKeyCodes::KEY_Left:
	case cocos2d::enumKeyCodes::CONTROLLER_Left:
		break;
	case cocos2d::enumKeyCodes::KEY_Right:
	case cocos2d::enumKeyCodes::CONTROLLER_Right:
		break;
	default:
		CCLayer::keyDown(key);
	}
}

void SearchMusicLayer::textChanged(gd::CCTextInputNode* input) {
	if (!sharedStateA().m_bLiveSearch) {
		return;
	}
	SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchSongName;
	SearchMusicLayer::stringToSearch = m_pTextInput->getString();

	updateParentAContent();
}

void SearchMusicLayer::onClose(cocos2d::CCObject* pSender) {
	this->keyBackClicked();
}

void SearchMusicLayer::onSearchMusic(cocos2d::CCObject* pSender) {
	SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchSongName;
	SearchMusicLayer::stringToSearch = m_pTextInput->getString();
	updateParentAContent();
	this->keyBackClicked();
}

void SearchMusicLayer::onSearchArtist(cocos2d::CCObject* pSender) {
	SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchArtistName;
	SearchMusicLayer::stringToSearch = m_pTextInput->getString();
	updateParentAContent();
	this->keyBackClicked();
}

//ListData
ListData::ListData(const char* title, unsigned int length) {
	m_sTitle = title;
	m_uMaxLength = length;
	m_uLength = 0;
	m_uOffset = 0;
	m_uIndex = 0;
}

//HorizontalList
bool HorizontalList::init() {
	m_pLabels = new cocos2d::CCLabelBMFont * [m_pData.m_uMaxLength];

	m_pMenu = cocos2d::CCMenu::create();
	this->addChild(m_pMenu);

	auto upBtn = gd::CCMenuItemSpriteExtra::create(
		cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png"),
		this,
		(cocos2d::SEL_MenuHandler)(&HorizontalList::navigate)
	);
	upBtn->setTag(kListUpBtn);
	upBtn->setPosition(m_obPosition.x - 70.0f, m_obPosition.y);
	upBtn->setRotation(180.0f);
	m_pMenu->addChild(upBtn);

	auto downBtn = gd::CCMenuItemSpriteExtra::create(
		cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png"),
		this,
		(cocos2d::SEL_MenuHandler)(&HorizontalList::navigate)
	);
	downBtn->setTag(kListDownBtn);
	downBtn->setPosition(m_obPosition.x + 70.0f, m_obPosition.y);
	m_pMenu->addChild(downBtn);

	//verify offset
	if (!m_pData.m_vEntries.empty() && m_pData.m_uOffset > m_pData.m_vEntries.size() - 1)
		m_pData.m_uOffset = m_pData.m_vEntries.size() - 1;
	else if (m_pData.m_uOffset < 0)
		m_pData.m_uOffset = 0;

	this->getLength();

	if (m_pData.m_uLength != 0) {
		m_pLabels[0] = cocos2d::CCLabelBMFont::create(m_pData.m_vEntries[m_pData.m_uOffset].c_str(), "bigFont.fnt");
		m_pLabels[0]->setPosition(m_obPosition.x, m_obPosition.y);
		m_pLabels[0]->limitLabelWidth(90.0f, 0.8f, 0.0f);
		m_pMenu->addChild(m_pLabels[0]);
	}

	return true;
}

void HorizontalList::updateList() {
	this->getLength();

	if (m_pData.m_uLength != 0) {
		m_pLabels[0]->setString(m_pData.m_vEntries[m_pData.m_uOffset].c_str());
		m_pLabels[0]->limitLabelWidth(90.0f, 0.8f, 0.0f);
	}
}

void HorizontalList::navigate(cocos2d::CCObject* btn) {
	if (!m_pData.m_vEntries.empty()) {
		if (btn->getTag() == kListUpBtn) {
			if (m_pData.m_uOffset != 0)
				--m_pData.m_uOffset;
			else
				m_pData.m_uOffset = m_pData.m_vEntries.size() - 1;
		}
		else if (btn->getTag() == kListDownBtn) {
			if (m_pData.m_uOffset < m_pData.m_vEntries.size() - 1)
				++m_pData.m_uOffset;
			else
				m_pData.m_uOffset = 0;
		}

		this->updateList();
	}
}

HorizontalList* HorizontalList::create(ListData& data) {
	HorizontalList* pRet = new HorizontalList(data);
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

void HorizontalList::setPosition(float x, float y) {
	m_obPosition = ccp(x, y);
	//this->updateList();
}

SearchMusicSettingsLayer* SearchMusicSettingsLayer::create() {
	auto ret = new SearchMusicSettingsLayer();

	if (ret && ret->init("Search Music Settings", 220.0f)) {
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void SearchMusicSettingsLayer::customSetup() {
	m_menuButtons = cocos2d::CCMenu::create();
	m_pLayer->addChild(m_menuButtons);


	auto txtLiveSearch = cocos2d::CCLabelBMFont::create("Live Search", "goldFont.fnt");
	txtLiveSearch->setAnchorPoint({ 0.f, 0.5f });
	txtLiveSearch->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f + 88.f });
	txtLiveSearch->setScale(0.6f);
	m_pLayer->addChild(txtLiveSearch);

	auto btnSelect_2 = gd::CCMenuItemToggler::createWithStandardSprites(this, (cocos2d::SEL_MenuHandler)&SearchMusicSettingsLayer::onLiveSearch, 0.6f);
	btnSelect_2->toggle(sharedStateA().m_bLiveSearch);
	btnSelect_2->setPosition({ 150.f, 88.f });
	m_menuButtons->addChild(btnSelect_2);

	auto txtLiveSearch_2 = cocos2d::CCLabelBMFont::create("Use Artist Name\nIn Live Search", "goldFont.fnt");
	txtLiveSearch_2->setAnchorPoint({ 0.f, 0.5f });
	txtLiveSearch_2->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f + 58.f });
	txtLiveSearch_2->setScale(0.6f);
	m_pLayer->addChild(txtLiveSearch_2);

	auto btnSelect_3 = gd::CCMenuItemToggler::createWithStandardSprites(this, (cocos2d::SEL_MenuHandler)&SearchMusicSettingsLayer::onLiveSearchUsingArtistName, 0.6f);
	btnSelect_3->toggle(sharedStateA().m_bLiveSearchUsingArtisName);
	btnSelect_3->setPosition({ 150.f, 58.f });
	m_menuButtons->addChild(btnSelect_3);

}

void SearchMusicSettingsLayer::exitLayer(cocos2d::CCObject*) {
	this->setKeypadEnabled(false);
	this->disableUI();
	this->hideLayer(false);
}

void SearchMusicSettingsLayer::onLiveSearch(cocos2d::CCObject* pSender) {
	sharedStateA().m_bLiveSearch = !sharedStateA().m_bLiveSearch;
}

void SearchMusicSettingsLayer::onLiveSearchUsingArtistName(cocos2d::CCObject* pSender) {
	sharedStateA().m_bLiveSearchUsingArtisName = !sharedStateA().m_bLiveSearchUsingArtisName;
}