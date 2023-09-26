#include "pch.h"
#include "SongControrLayer.h"

bool SongControlLayer::isGameShow = false;

SongControlLayer* SongControlLayer::create(gd::GJSongBrowser* parent, int i) {
	auto ret = new SongControlLayer();

	if (ret && ret->init(parent, i)) {
		SongControlLayer::isGameShow = true;
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool SongControlLayer::init(gd::GJSongBrowser* parent, int i) {

	if (!initWithColor({ 0, 0, 0, 105 })) return false;

	this->m_iPag = i;
	this->m_pParentA = parent;

	m_pLayer = cocos2d::CCLayer::create();
	m_pLayer->setPosition({ Utils::winSize().width / 2.f, -200.f });
	this->addChild(m_pLayer);

	//std::cout << i << std::endl;

	this->m_pBG = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
	this->m_pBG->setContentSize({ Utils::winSize().width * 2.f, Utils::winSize().height });
	m_pLayer->addChild(this->m_pBG);


	auto m_pButtonsMenu = cocos2d::CCMenu::create();
	m_pLayer->addChild(m_pButtonsMenu);

	auto spriteButton = cocos2d::CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
	auto imageButton = gd::ButtonSprite::create("", 30, true, "bigFont.fnt", "GJ_button_01.png", 35, 0.5f);
	imageButton->addChild(spriteButton);
	spriteButton->setPosition({ 20.f, 18.f });
	imageButton->setScale(0.8f);
	auto btnPrevSong = gd::CCMenuItemSpriteExtra::create(imageButton, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onPrevSong);//(cocos2d::SEL_MenuHandler)&ButtonsClass::onSongControlLayer);
	btnPrevSong->setPosition({ -100.f, -40.f });
	m_pButtonsMenu->addChild(btnPrevSong);

	auto spriteButton_2 = cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
	auto imageButton_2 = gd::ButtonSprite::create("", 30, true, "bigFont.fnt", "GJ_button_01.png", 35, 0.5f);
	spriteButton_2->setPosition({ 20.f, 18.f });
	imageButton_2->addChild(spriteButton_2);
	imageButton_2->setScale(0.8f);
	auto btnNextSong = gd::CCMenuItemSpriteExtra::create(imageButton_2, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onNextSong);//(cocos2d::SEL_MenuHandler)&ButtonsClass::onSongControlLayer);
	btnNextSong->setPosition({ -50.f, -40.f });
	m_pButtonsMenu->addChild(btnNextSong);

	auto prevImage = cocos2d::CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
	prevImage->setScale(0.5f);
	prevImage->setPosition({ Utils::winSize().width / 2 - 110.f, 80.f });
	auto prevImageText = cocos2d::CCLabelBMFont::create("Prev. Song", "bigFont.fnt");
	prevImageText->setScale(0.3f);
	prevImageText->setPosition({ Utils::winSize().width / 2 - 95.f, 80.f });
	prevImageText->setAnchorPoint({ 0.f, 0.5f });
	auto nextImage = cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
	nextImage->setScale(0.5f);
	nextImage->setPosition({ Utils::winSize().width / 2 - 109.f, 60.f });
	auto nextImageText = cocos2d::CCLabelBMFont::create("Next Song", "bigFont.fnt");
	nextImageText->setScale(0.3f);
	nextImageText->setPosition({ Utils::winSize().width / 2 - 95.f, 60.f });
	nextImageText->setAnchorPoint({ 0.f, 0.5f });
	m_pLayer->addChild(prevImage);
	m_pLayer->addChild(nextImage);
	m_pLayer->addChild(prevImageText);
	m_pLayer->addChild(nextImageText);

	auto downloadedSongs = gd::MusicDownloadManager::sharedState()->getDownloadedSongs();
	this->m_iIndex = 10 * this->m_iPag;

	for (size_t i = 0; i < downloadedSongs->count(); i++) {
		auto musicI = reinterpret_cast<gd::SongInfoObject*>(downloadedSongs->objectAtIndex(i));
		if (Utils::shareFMOD()->isBackgroundMusicPlaying(gd::MusicDownloadManager::pathForSong(musicI->m_nSongID))) {
			this->m_iIndex = i;
			break;
		}
	}

	changeToSongAtIndex(this->m_iIndex);

	auto imageButton5_ = gd::ButtonSprite::create("Delete", 80.f, false, "bigFont.fnt", "GJ_button_06.png", 35.f, 0.6f);
	imageButton5_->setScale(0.7f);
	auto buttonDelete = gd::CCMenuItemSpriteExtra::create(imageButton5_, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onDeleteSong);//(cocos2d::SEL_MenuHandler)&MusicOffsetSetupLayer::onStop);
	buttonDelete->setPosition({ 40.f - Utils::winSize().width, -100.f });
	m_pButtonsMenu->addChild(buttonDelete);

	imageButton6_ = gd::ButtonSprite::create("Pause", 70.f, true, "bigFont.fnt", "GJ_button_02.png", 35.f, 0.6f);
	imageButton6_->setScale(0.7f);
	auto buttonPause = gd::CCMenuItemSpriteExtra::create(imageButton6_, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onPauseSong);//(cocos2d::SEL_MenuHandler)&MusicOffsetSetupLayer::onStop);
	buttonPause->setPosition({ 40.f - Utils::winSize().width, -70.f });
	m_pButtonsMenu->addChild(buttonPause);

	updateButtonPause();

	imageButton7_ = gd::ButtonSprite::create("Use", 70.f, true, "bigFont.fnt", "GJ_button_03.png", 35.f, 0.6f);
	imageButton7_->setScale(0.7f);
	buttonUse = gd::CCMenuItemSpriteExtra::create(imageButton7_, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onUseSong);//(cocos2d::SEL_MenuHandler)&MusicOffsetSetupLayer::onStop);
	buttonUse->setPosition({ 40.f - Utils::winSize().width, -40.f });
	m_pButtonsMenu->addChild(buttonUse);

	buttonUse->setVisible(Utils::shareManager()->getEditorLayer());

	updateButtonUse();

	auto settingsButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	settingsButtonImage->setScale(0.6f);
	auto settingsButton = gd::CCMenuItemSpriteExtra::create(settingsButtonImage, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onSettings); //(cocos2d::SEL_MenuHandler)&BetterPause::onOptionsLayer);
	settingsButton->setPosition({ 104.f - Utils::winSize().width, -70.f });
	m_pButtonsMenu->addChild(settingsButton);

	m_lNumberSongTotal = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat(
		"Sound %i of %i", this->m_iIndex + 1, downloadedSongs->count())->getCString(), "goldFont.fnt");
	m_lNumberSongTotal->setScale(0.5f);
	m_lNumberSongTotal->setPositionY(148.f);
	m_pLayer->addChild(m_lNumberSongTotal);

	auto animationEntry = cocos2d::CCEaseElasticOut::create((cocos2d::CCActionInterval*)cocos2d::CCMoveTo::create(0.5f, { Utils::winSize().width / 2.f, -30.f }), 1.f);
	m_pLayer->runAction(cocos2d::CCSequence::create((cocos2d::CCFiniteTimeAction*)animationEntry, nullptr));



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

void SongControlLayer::keyBackClicked() {
	SongControlLayer::isGameShow = false;
	gd::FLAlertLayer::keyBackClicked();
}

void SongControlLayer::keyDown(cocos2d::enumKeyCodes key) {
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

void SongControlLayer::changeToSongAtIndex(int index) {
	auto downloadedSongs = gd::MusicDownloadManager::sharedState()->getDownloadedSongs();
	if (index < 0) {
		index = m_iIndex = downloadedSongs->count() - 1;
	}
	else if (index > (downloadedSongs->count() - 1)) {
		index = m_iIndex = 0;
	}
	else if (downloadedSongs->count() == 0) {
		index = m_iIndex = -1;
	}

	if (m_lNumberSongTotal) {
		m_lNumberSongTotal->setString(cocos2d::CCString::createWithFormat(
			"Sound %i of %i", this->m_iIndex + 1, downloadedSongs->count())->getCString());
	}
	

	auto musicSelect = downloadedSongs->count() == 0 ? 0 : reinterpret_cast<gd::SongInfoObject*>(downloadedSongs->objectAtIndex(index));

	if (m_CSWCurrent) {
		m_CSWCurrent->removeFromParentAndCleanup(true);
		CC_SAFE_DELETE(m_CSWCurrent);
	}

	m_CSWCurrent = gd::CustomSongWidget::create(musicSelect,
		Utils::shareManager()->getEditorLayer() == nullptr ? 0 : Utils::shareManager()->getEditorLayer()->m_pLevelSettings, 0, 0, true, 0, 0);
	m_CSWCurrent->setPositionY(90.f);
	m_CSWCurrent->m_pInfoLabel->setString(cocos2d::CCString::createWithFormat(
		"SongID: %i\nSize: %.2fMB", (m_CSWCurrent->m_songInfo ? m_CSWCurrent->m_songInfo->m_nSongID : 0), (m_CSWCurrent->m_songInfo ? m_CSWCurrent->m_songInfo->m_fFileSize : 0))->getCString());
	m_CSWCurrent->m_pInfoLabel->setScale(0.3f);
	m_CSWCurrent->m_pInfoLabel->setPosition({ -138.f, -25.f });
	m_CSWCurrent->m_pInfoLabel->setAnchorPoint({ 0.f, 0.5f });
	m_pLayer->addChild(m_CSWCurrent);

	if (m_pSliderMusic) {
		m_CSWCurrent->removeChild(m_pSliderMusic, true);
	}


	m_pSliderMusic = SliderSongControl::create(musicSelect);
	m_pSliderMusic->setScale(0.7f);
	m_pSliderMusic->setPosition({ 44.f, -43.f });
	m_CSWCurrent->addChild(m_pSliderMusic);

	updateButtonUse();
	updateButtonPause();

}

void SongControlLayer::onPrevSong(cocos2d::CCObject* pSender) {
	if (!this->m_CSWCurrent->m_songInfo) {
		return;
	}
	m_iIndex--;
	changeToSongAtIndex(m_iIndex);
	if (sharedStateA().m_bAutoPlayOnSongChange) {
		this->m_CSWCurrent->m_pPlayMusicBtn->activate();
	}
}

void SongControlLayer::onNextSong(cocos2d::CCObject* pSender) {
	if (!this->m_CSWCurrent->m_songInfo) {
		return;
	}
	m_iIndex++;
	changeToSongAtIndex(m_iIndex);
	if (sharedStateA().m_bAutoPlayOnSongChange) {
		this->m_CSWCurrent->m_pPlayMusicBtn->activate();
	}
}

void SongControlLayer::updateButtonUse() {
	if (!m_CSWCurrent->m_pLevelSettingsObject || !m_CSWCurrent->m_songInfo) {
		return;
	}

	bool isUseMusic = m_CSWCurrent->m_pLevelSettingsObject->m_level->songID == m_CSWCurrent->m_songInfo->m_nSongID;

	if (imageButton7_) {
		imageButton7_->updateBGImage(isUseMusic ? "GJ_button_04.png" : "GJ_button_03.png");
		imageButton7_->m_pLabel->setString(isUseMusic ? "In Use" : "Use");
	}
	if (buttonUse) {
		buttonUse->setEnabled(!isUseMusic);
	}
}

void SongControlLayer::onUseSong(cocos2d::CCObject* pSender) {

	if (!m_CSWCurrent->m_songInfo) {
		return;
	}

	m_CSWCurrent->onSelect(pSender);
	updateButtonUse();
	m_CSWCurrent->m_pInfoLabel->setString(cocos2d::CCString::createWithFormat(
		"SongID: %i\nSize: %.2fMB", m_CSWCurrent->m_songInfo->m_nSongID, m_CSWCurrent->m_songInfo->m_fFileSize)->getCString());
	m_CSWCurrent->m_pInfoLabel->setScale(0.3f);
	m_CSWCurrent->m_pInfoLabel->setPosition({ -140.f, -25.f });
	m_CSWCurrent->m_pInfoLabel->setAnchorPoint({ 0.f, 0.5f });
	if (this->m_pParentA) {
		this->m_pParentA->loadPage(m_pParentA->m_iPage);
	}
}

void SongControlLayer::updateButtonPause() {
	if (!m_CSWCurrent->m_songInfo) {
		return;
	}

	if (Utils::shareFMOD()->isBackgroundMusicPlaying(gd::MusicDownloadManager::pathForSong(this->m_CSWCurrent->m_songInfo->m_nSongID))) {

		bool isPaused = false;

		Utils::shareFMOD()->m_pGlobalChannel->getPaused(&isPaused);

		if (imageButton6_) {
			if (isPaused) {
				imageButton6_->updateBGImage("GJ_button_01.png");
				imageButton6_->m_pLabel->setString("Resume");
			}
			else {
				imageButton6_->updateBGImage("GJ_button_02.png");
				imageButton6_->m_pLabel->setString("Pause");
			}
		}

		return;
	}

	if (imageButton6_) {
		imageButton6_->updateBGImage("GJ_button_02.png");
		imageButton6_->m_pLabel->setString("Pause");
	}
}

void SongControlLayer::onPauseSong(cocos2d::CCObject* pSender) {

	if (!m_CSWCurrent->m_songInfo) {
		return;
	}

	if (Utils::shareFMOD()->isBackgroundMusicPlaying(gd::MusicDownloadManager::pathForSong(this->m_CSWCurrent->m_songInfo->m_nSongID))) {

		bool isPaused = false;

		Utils::shareFMOD()->m_pGlobalChannel->getPaused(&isPaused);
		Utils::shareFMOD()->m_pGlobalChannel->setPaused(!isPaused);
	}

	updateButtonPause();

}

bool onDeleteMusic(std::string ID) {
	std::string delcommand = Utils::GetFolderMusicCurrent() + ID + ".mp3";
	if (std::filesystem::remove(delcommand.c_str())) return true;
	else return false;
}

void SongControlLayer::DeleteMusic(cocos2d::CCObject* pSender) {

	reinterpret_cast<gd::FLAlertLayer*>(reinterpret_cast<cocos2d::CCNode*>(pSender)->getParent()->getParent()->getParent())->keyBackClicked();

	auto songId = static_cast<int>(this->m_CSWCurrent->m_songInfo->m_nSongID);
	auto deleteComplete = onDeleteMusic(std::to_string(songId).c_str());


	if (deleteComplete) {
		this->onNextSong(nullptr);
	}
	else {
		auto alert = gd::FLAlertLayer::create(this, "Delete Song", "OK", 0, 300.0f, "Deletion of the song failed.");
		alert->show();
	}
	if (this->m_pParentA) {
		this->m_pParentA->loadPage(this->m_pParentA->m_iPage);
	}
}

void SongControlLayer::onDeleteSong(cocos2d::CCObject* pSender) {
	if (!m_CSWCurrent->m_songInfo) {
		return;
	}

	auto alert = gd::FLAlertLayer::create(this, "Delete Song", "Cancel", "Delete", 300.0f, "Do you want to <cr>Delete</c> this song?");
	alert->setTag(13);
	alert->show();
	reinterpret_cast<gd::CCMenuItemSpriteExtra*>(alert->m_pButton2->getParent())->setTarget(this, (cocos2d::SEL_MenuHandler)&SongControlLayer::DeleteMusic);
}

SliderSongControl* SliderSongControl::create(gd::SongInfoObject* song) {
	auto ret = new SliderSongControl();

	if (ret && ret->init(song)) {
		ret->autorelease();
		ret->scheduleUpdate();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool SliderSongControl::init(gd::SongInfoObject* song) {
	if (!cocos2d::CCNode::init()) {
		return false;
	}

	this->m_pSongObj = song;

	createSlider();
	createLabels();

	return true;
}

void SliderSongControl::createSlider() {
	m_pSliderMusic = gd::Slider::create(this, (cocos2d::SEL_MenuHandler)&SliderSongControl::musicSliderChanged, 1.f);
	m_pSliderMusic->setValue(0.f);
	m_pSliderMusic->updateBar();
	m_pSliderMusic->setScale(0.8f);
	m_pSliderMusic->setAnchorPoint({ 0.f, 0.5f });
	this->addChild(m_pSliderMusic);
}

void SliderSongControl::createLabels() {

	auto showMusicTime = sharedStateA().m_bShowPositionMusic ? "%.2f" : "%02d:%02d";
	auto showMusicTimeWF = sharedStateA().m_bShowPositionMusic ? "0.00" : "00:00";

	m_pCurrentTimeSong = createLabel(showMusicTimeWF, "goldFont.fnt", 0.5f, { 0.f, 15.f }, { 0.f, 0.5f });
	m_pStartTimeSong = createLabel(showMusicTimeWF, "bigFont.fnt", 0.4f, { -90.f, 32.f }, { 1.f, 0.5f });

	unsigned int length = getSongLength(false);
	unsigned int minutes = length / 60000;
	unsigned int seconds = (length % 60000) / 1000;

	if (sharedStateA().m_bShowPositionMusic) {
		m_pEndTimeSong = createLabel(cocos2d::CCString::createWithFormat(showMusicTime, length / 1000.f)->getCString(), "bigFont.fnt", 0.4f, { 90.f, 32.f }, { 0.f, 0.5f });
	}
	else {
		m_pEndTimeSong = createLabel(cocos2d::CCString::createWithFormat(showMusicTime, minutes, seconds)->getCString(), "bigFont.fnt", 0.4f, { 90.f, 32.f }, { 0.f, 0.5f });

	}
}

cocos2d::CCLabelBMFont* SliderSongControl::createLabel(const char* text, const char* font, float scale, const cocos2d::CCPoint& position, const cocos2d::CCPoint& anchor) {
	auto label = cocos2d::CCLabelBMFont::create(text, font);
	label->setScale(scale);
	label->setPosition(position);
	label->setAnchorPoint(anchor);
	this->addChild(label);
	return label;
}

unsigned int SliderSongControl::getSongLength(bool currentSong) {
	if (currentSong && Utils::shareFMOD()->m_pSound) {
		// Utilizamos el FMOD::Sound actual si existe
		unsigned int length;
		Utils::shareFMOD()->m_pSound->getLength(&length, FMOD_TIMEUNIT_MS);
		return length;
	}

	if (!this->m_pSongObj) {
		return 0;
	}

	FMOD::Sound* sound = nullptr;

	if (!currentSong) {
		Utils::shareFMOD()->m_pSystem->createSound(gd::MusicDownloadManager::pathForSong(this->m_pSongObj->m_nSongID).c_str(), FMOD_DEFAULT, nullptr, &sound);
	}

	unsigned int length = 0;

	if (sound) {
		sound->getLength(&length, FMOD_TIMEUNIT_MS);
		sound->release();
		CC_SAFE_DELETE(sound);
	}

	return length;
}



void SliderSongControl::musicSliderChanged(cocos2d::CCObject* pSender) {

	if (!this->m_pSongObj) {
		return;
	}

	if (!Utils::shareFMOD()->isBackgroundMusicPlaying(gd::MusicDownloadManager::pathForSong(this->m_pSongObj->m_nSongID))) {
		resetUI();
		return;
	}

	this->onChanged = true;

	unsigned int currentPosition = 0;
	unsigned int totalDuration = getSongLength(true);

	auto valueSlider = this->m_pSliderMusic->getValue();

	gd::FMODAudioEngine::sharedEngine()->m_pGlobalChannel->setPosition(static_cast<unsigned int>(totalDuration * valueSlider), FMOD_TIMEUNIT_MS);

	gd::FMODAudioEngine::sharedEngine()->m_pGlobalChannel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);



	updateSliderAndTimeLabels(currentPosition, totalDuration);
}

void SliderSongControl::update(float dt) {
	if (this->onChanged) {
		this->onChanged = false;
		return;
	}

	if (!this->m_pSongObj || !Utils::shareFMOD()->isBackgroundMusicPlaying(gd::MusicDownloadManager::pathForSong(this->m_pSongObj->m_nSongID))) {
		resetUI();
		return;
	}

	unsigned int currentPosition = 0;
	unsigned int totalDuration = getSongLength(true);

	gd::FMODAudioEngine::sharedEngine()->m_pGlobalChannel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);

	updateSliderAndTimeLabels(currentPosition, totalDuration);

	if (currentPosition >= totalDuration && sharedStateA().m_bAutoPlayNextSong) {
		tryAutoPlayMusic();
		unscheduleUpdate();
	}

}

void SliderSongControl::tryAutoPlayMusic() {
	auto tryAch = reinterpret_cast<SongControlLayer*>(this->getParent()->getParent()->getParent());
	tryAch->m_iIndex++;
	tryAch->changeToSongAtIndex(tryAch->m_iIndex);
	tryAch->m_CSWCurrent->m_pPlayMusicBtn->activate();
}

void SliderSongControl::resetUI() {
	m_pSliderMusic->setValue(0.f);
	m_pSliderMusic->updateBar();
	m_pCurrentTimeSong->setString(sharedStateA().m_bShowPositionMusic ? "0.00" : "00:00");
	m_pCurrentTimeSong->setPositionX(m_pSliderMusic->m_pTouchLogic->m_pThumb->getPositionX());
	m_pCurrentTimeSong->setAnchorPoint({ 0.f, 0.5f });
}

void SliderSongControl::updateSliderAndTimeLabels(unsigned int currentPosition, unsigned int totalDuration) {
	float valueV = static_cast<float>(currentPosition) / static_cast<float>(totalDuration);

	m_pSliderMusic->setValue(valueV);
	m_pSliderMusic->updateBar();

	int currentMinutes = static_cast<int>(currentPosition / 1000.f / 60.f);
	int currentSeconds = static_cast<int>(currentPosition / 1000.f) % 60;
	int totalMinutes = static_cast<int>(totalDuration / 1000.f / 60.f);
	int totalSeconds = static_cast<int>(totalDuration / 1000.f) % 60;

	auto showMusicTime = sharedStateA().m_bShowPositionMusic ? "%.2f" : "%02d:%02d";

	if (sharedStateA().m_bShowPositionMusic) {
		m_pCurrentTimeSong->setString(cocos2d::CCString::createWithFormat(
			showMusicTime, currentPosition / 1000.f)->getCString());
	}
	else {
		m_pCurrentTimeSong->setString(cocos2d::CCString::createWithFormat(
			showMusicTime, currentMinutes, currentSeconds, totalMinutes, totalSeconds)->getCString());
	}

	m_pCurrentTimeSong->setPositionX(m_pSliderMusic->m_pTouchLogic->m_pThumb->getPositionX());
	m_pCurrentTimeSong->setAnchorPoint({ valueV, 0.5f });
}


SongControlSettingsLayer* SongControlSettingsLayer::create(SongControlLayer* lay) {
	auto node = new SongControlSettingsLayer();
	if (node && node->init("Song Control Settings", 220.0f)) {
		node->m_pParentE = lay;
		node->autorelease();
	}
	else {
		CC_SAFE_DELETE(node);
	}
	return node;
}

void SongControlSettingsLayer::customSetup() {

	m_menuButtons = cocos2d::CCMenu::create();
	m_pLayer->addChild(m_menuButtons);

	auto txtShowMusicPosition = cocos2d::CCLabelBMFont::create("Only Seconds Music: ", "goldFont.fnt");
	txtShowMusicPosition->setAnchorPoint({ 0.f, 0.5f });
	txtShowMusicPosition->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f + 88.f });
	txtShowMusicPosition->setScale(0.6f);
	m_pLayer->addChild(txtShowMusicPosition);

	//m_pSwitchQuest = HorizontalList::create(BetterPauseManager::sharedState()->m_pSwitchQuest);
	//m_pSwitchQuest->setPosition(80.f, 60.f);
	//m_pLayer->addChild(m_pSwitchQuest);

	auto btnSelect = gd::CCMenuItemToggler::createWithStandardSprites(this, (cocos2d::SEL_MenuHandler)&SongControlSettingsLayer::onShowMusicPosition, 0.6f);
	btnSelect->toggle(sharedStateA().m_bShowPositionMusic);
	btnSelect->setTag(-2);
	btnSelect->setPosition({ 150.f, 88.f });
	m_menuButtons->addChild(btnSelect);

	auto txtAutoPlayNextSong = cocos2d::CCLabelBMFont::create("AutoPlay Next Song: ", "goldFont.fnt");
	txtAutoPlayNextSong->setAnchorPoint({ 0.f, 0.5f });
	txtAutoPlayNextSong->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f + 58.f });
	txtAutoPlayNextSong->setScale(0.6f);
	m_pLayer->addChild(txtAutoPlayNextSong);

	auto btnSelect_2 = gd::CCMenuItemToggler::createWithStandardSprites(this, (cocos2d::SEL_MenuHandler)&SongControlSettingsLayer::onAutoPlayNextSong, 0.6f);
	btnSelect_2->toggle(sharedStateA().m_bAutoPlayNextSong);
	btnSelect_2->setTag(-2);
	btnSelect_2->setPosition({ 150.f, 58.f });
	m_menuButtons->addChild(btnSelect_2);

	auto txtAutoPlayOnSongChange = cocos2d::CCLabelBMFont::create("AutoPlay On Song Change: ", "goldFont.fnt");
	txtAutoPlayOnSongChange->setAnchorPoint({ 0.f, 0.5f });
	txtAutoPlayOnSongChange->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f + 28.f });
	txtAutoPlayOnSongChange->setScale(0.6f);
	m_pLayer->addChild(txtAutoPlayOnSongChange);

	auto btnSelect_3 = gd::CCMenuItemToggler::createWithStandardSprites(this, (cocos2d::SEL_MenuHandler)&SongControlSettingsLayer::onAutoPlayOnSongChange, 0.6f);
	btnSelect_3->toggle(sharedStateA().m_bAutoPlayOnSongChange);
	btnSelect_3->setTag(-2);
	btnSelect_3->setPosition({ 150.f, 28.f });
	m_menuButtons->addChild(btnSelect_3);

	auto txtKeybindShowLayout = cocos2d::CCLabelBMFont::create("Key To Open: ", "goldFont.fnt");
	txtKeybindShowLayout->setAnchorPoint({ 0.f, 0.5f });
	txtKeybindShowLayout->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f });
	txtKeybindShowLayout->setScale(0.6f);
	m_pLayer->addChild(txtKeybindShowLayout);

	auto txtKeybindShowLayoutButton = gd::ButtonSprite::create("Select", 50, true, "bigFont.fnt", "GJ_button_01.png", 35, 0.5f);
	txtKeybindShowLayoutButton->setScale(0.8f);
	auto btnImageSLB = gd::CCMenuItemSpriteExtra::create(txtKeybindShowLayoutButton, this, (cocos2d::SEL_MenuHandler)&SongControlSettingsLayer::onSelectKeyLayer);
	btnImageSLB->setPosition({ 140.f, 0.f });
	m_menuButtons->addChild(btnImageSLB);

	/*m_pSwitchButtonsPos = HorizontalList::create(BetterPauseManager::sharedState()->m_pSwitchButtonsPos);
	m_pSwitchButtonsPos->setPosition(80.f, 30.f);
	m_pLayer->addChild(m_pSwitchButtonsPos);*/

	/*auto txtSwitchButtonsPos = cocos2d::CCLabelBMFont::create("Buttons Pos: ", "goldFont.fnt");
	txtSwitchButtonsPos->setAnchorPoint({ 0.f, 0.5f });
	txtSwitchButtonsPos->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f + 28.f });
	txtSwitchButtonsPos->setScale(0.6f);
	m_pLayer->addChild(txtSwitchButtonsPos);

	auto txtSelectQuickSettings = cocos2d::CCLabelBMFont::create("Select Quick Settings: ", "goldFont.fnt");
	txtSelectQuickSettings->setAnchorPoint({ 0.f, 0.5f });
	txtSelectQuickSettings->setPosition({ Utils::winSize().width / 2.f - 165.f, Utils::winSize().height / 2.f - 2.f });
	txtSelectQuickSettings->setScale(0.5f);
	m_pLayer->addChild(txtSelectQuickSettings);

	auto txtSelectQuickPause = cocos2d::CCLabelBMFont::create("Select", "goldFont.fnt");
	txtSelectQuickPause->setAnchorPoint({ 0.5f, 0.5f });
	txtSelectQuickPause->setPosition({ 73.f, 17.f });
	auto m_pBGSelectQuickPause = cocos2d::extension::CCScale9Sprite::create("GJ_button_01.png");
	m_pBGSelectQuickPause->setContentSize({ 150.f, 30.f });
	m_pBGSelectQuickPause->addChild(txtSelectQuickPause);
	m_pBGSelectQuickPause->setScale(0.8f);
	auto m_pRealOptionsPause = gd::CCMenuItemSpriteExtra::create(m_pBGSelectQuickPause, this, (cocos2d::SEL_MenuHandler)&PauseSettingsLayer::onSelectQuickSettings);
	m_pRealOptionsPause->setPosition({ 80.f, -2.f });
	m_menuButtons->addChild(m_pRealOptionsPause);*/

}

void SongControlSettingsLayer::onSelectKeyLayer(cocos2d::CCObject* pSender) {
	SelectKeyLayer::create()->show();
}


void SongControlSettingsLayer::exitLayer(cocos2d::CCObject*) {
	this->setKeypadEnabled(false);
	this->disableUI();
	this->hideLayer(false);
	m_pParentE->changeToSongAtIndex(m_pParentE->m_iIndex);
}

void SongControlSettingsLayer::onShowMusicPosition(cocos2d::CCObject* pSender) {
	sharedStateA().m_bShowPositionMusic = !sharedStateA().m_bShowPositionMusic;
}

void SongControlSettingsLayer::onAutoPlayNextSong(cocos2d::CCObject* pSender) {
	sharedStateA().m_bAutoPlayNextSong = !sharedStateA().m_bAutoPlayNextSong;
}

void SongControlSettingsLayer::onAutoPlayOnSongChange(cocos2d::CCObject* pSender) {
	sharedStateA().m_bAutoPlayOnSongChange = !sharedStateA().m_bAutoPlayOnSongChange;
}

void SongControlLayer::onSettings(cocos2d::CCObject* pSender) {
	auto dropDown = SongControlSettingsLayer::create(this);
	this->addChild(dropDown);
	dropDown->setPosition(0.0f, 0.0f);
	dropDown->showLayer(false);
}

SelectKeyLayer* SelectKeyLayer::create() {
	auto ret = new SelectKeyLayer();

	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool SelectKeyLayer::init() {
	if (!initWithColor({ 0, 0, 0, 105 })) return false;

	this->keySelect = sharedStateA().m_iKeyShowReproductor;

	m_pLayer = cocos2d::CCLayer::create();
	m_pLayer->setPosition({ Utils::winSize().width / 2.f, Utils::winSize().height / 2.f });
	m_pLayer->setAnchorPoint({ 0.f, 0.f });
	this->addChild(m_pLayer);

	auto background = cocos2d::extension::CCScale9Sprite::create("GJ_square02.png");
	background->setContentSize({ 200.f, 100.f });
	m_pLayer->addChild(background);

	m_pButtonMenu = cocos2d::CCMenu::create();
	m_pButtonMenu->setPosition({ -98.f, 50.f });
	auto imageClose = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	imageClose->setScale(0.7f);
	auto buttonExtraItem = gd::CCMenuItemSpriteExtra::create(imageClose, this, (cocos2d::SEL_MenuHandler)&SelectKeyLayer::onClose);
	m_pButtonMenu->addChild(buttonExtraItem);
	m_pLayer->addChild(m_pButtonMenu);

	auto titleLayer = cocos2d::CCLabelBMFont::create("- Add Keybind -", "goldFont.fnt");
	titleLayer->setScale(0.7f);
	titleLayer->setPositionY(35.f);
	m_pLayer->addChild(titleLayer);

	auto underline = cocos2d::CCSprite::createWithSpriteFrameName("floorLine_001.png");
	underline->setPosition({ 0.f, 18.f });
	underline->setScaleX(0.4f);
	underline->setScaleY(0.5f);
	underline->setOpacity(100);
	m_pLayer->addChild(underline);

	m_pButtonsMenu = cocos2d::CCMenu::create();
	m_pButtonsMenu->setPosition({ 0.f, 0.f });
	m_pLayer->addChild(m_pButtonsMenu, 1);

	auto buttonSetKey = gd::ButtonSprite::create("Set", 0xFC, false, "goldFont.fnt", "GJ_button_01.png", 30.f, 0.6f);
	m_pSetKeyBtn = gd::CCMenuItemSpriteExtra::create(buttonSetKey, this, (cocos2d::SEL_MenuHandler)&SelectKeyLayer::onSetKey);
	m_pSetKeyBtn->setPosition({ 48.f, -15.f });
	m_pButtonsMenu->addChild(m_pSetKeyBtn);

	m_pBGInputTextValue = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
	m_pBGInputTextValue->setContentSize({ 70.f, 30.f });
	m_pBGInputTextValue->setScale(1.f);
	m_pBGInputTextValue->setPosition({ -40.f, -14.f });
	m_pBGInputTextValue->setColor({ 255, 255, 255 });
	m_pBGInputTextValue->setOpacity(100);
	m_pLayer->addChild(m_pBGInputTextValue);

	m_pInputTextValue = gd::CCTextInputNode::create("Key", this, "bigFont.fnt", 50.f, 20.f);
	m_pInputTextValue->setString(SelectKeyLayer::getNameKeyWithCode(static_cast<cocos2d::enumKeyCodes>(sharedStateA().m_iKeyShowReproductor)));
	m_pInputTextValue->setLabelPlaceholderColor({ 0x75, 0xAA, 0xF0 });
	m_pInputTextValue->setMaxLabelScale(0.7f);
	m_pInputTextValue->setPosition({ -41.f, -14.f });
	m_pInputTextValue->setDelegate(this);
	m_pInputTextValue->setTouchEnabled(false);
	m_pLayer->addChild(m_pInputTextValue);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
	this->setKeyboardEnabled(true);
	this->setMouseEnabled(true);
	this->registerWithTouchDispatcher();
	Utils::shareDirectorA()->getTouchDispatcher()->setForcePrio(true);
	Utils::shareDirectorA()->getTouchDispatcher()->incrementForcePrio(2);
	this->setTouchPriority(3);


	return true;
}

void SelectKeyLayer::keyBackClicked() {
	gd::FLAlertLayer::keyBackClicked();
}

void SelectKeyLayer::updateKeySelected(cocos2d::enumKeyCodes key) {
	this->keySelect = key;
	m_pInputTextValue->setString(SelectKeyLayer::getNameKeyWithCode(static_cast<cocos2d::enumKeyCodes>(this->keySelect)));
}

void SelectKeyLayer::keyDown(cocos2d::enumKeyCodes key) {
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
		this->updateKeySelected(key);
		CCLayer::keyDown(key);
	}
}
void SelectKeyLayer::onSetKey(cocos2d::CCObject* pSender) {
	 sharedStateA().m_iKeyShowReproductor = this->keySelect;
	 this->keyBackClicked();
}

std::string SelectKeyLayer::getNameKeyWithCode(cocos2d::enumKeyCodes key)
{
	switch (key)
	{
	case cocos2d::enumKeyCodes::KEY_Backspace: return "KEY_Backspace";
	case cocos2d::enumKeyCodes::KEY_Tab: return "KEY_Tab";
	case cocos2d::enumKeyCodes::KEY_Clear: return "KEY_Clear";
	case cocos2d::enumKeyCodes::KEY_Enter: return "KEY_Enter";
	case cocos2d::enumKeyCodes::KEY_Shift: return "KEY_Shift";
	case cocos2d::enumKeyCodes::KEY_Control: return "KEY_Control";
	case cocos2d::enumKeyCodes::KEY_Alt: return "KEY_Alt";
	case cocos2d::enumKeyCodes::KEY_Pause: return "KEY_Pause";
	case cocos2d::enumKeyCodes::KEY_CapsLock: return "KEY_CapsLock";
	case cocos2d::enumKeyCodes::KEY_Escape: return "KEY_Escape";
	case cocos2d::enumKeyCodes::KEY_Space: return "KEY_Space";
	case cocos2d::enumKeyCodes::KEY_PageUp: return "KEY_PageUp";
	case cocos2d::enumKeyCodes::KEY_PageDown: return "KEY_PageDown";
	case cocos2d::enumKeyCodes::KEY_End: return "KEY_End";
	case cocos2d::enumKeyCodes::KEY_Home: return "KEY_Home";
	case cocos2d::enumKeyCodes::KEY_Left: return "KEY_Left";
	case cocos2d::enumKeyCodes::KEY_Up: return "KEY_Up";
	case cocos2d::enumKeyCodes::KEY_Right: return "KEY_Right";
	case cocos2d::enumKeyCodes::KEY_Down: return "KEY_Down";
	case cocos2d::enumKeyCodes::KEY_Select: return "KEY_Select";
	case cocos2d::enumKeyCodes::KEY_Print: return "KEY_Print";
	case cocos2d::enumKeyCodes::KEY_Execute: return "KEY_Execute";
	case cocos2d::enumKeyCodes::KEY_PrintScreen: return "KEY_PrintScreen";
	case cocos2d::enumKeyCodes::KEY_Insert: return "KEY_Insert";
	case cocos2d::enumKeyCodes::KEY_Delete: return "KEY_Delete";
	case cocos2d::enumKeyCodes::KEY_Help: return "KEY_Help";
	case cocos2d::enumKeyCodes::KEY_Zero: return "KEY_Zero";
	case cocos2d::enumKeyCodes::KEY_One: return "KEY_One";
	case cocos2d::enumKeyCodes::KEY_Two: return "KEY_Two";
	case cocos2d::enumKeyCodes::KEY_Three: return "KEY_Three";
	case cocos2d::enumKeyCodes::KEY_Four: return "KEY_Four";
	case cocos2d::enumKeyCodes::KEY_Five: return "KEY_Five";
	case cocos2d::enumKeyCodes::KEY_Six: return "KEY_Six";
	case cocos2d::enumKeyCodes::KEY_Seven: return "KEY_Seven";
	case cocos2d::enumKeyCodes::KEY_Eight: return "KEY_Eight";
	case cocos2d::enumKeyCodes::KEY_Nine: return "KEY_Nine";
	case cocos2d::enumKeyCodes::KEY_A: return "KEY_A";
	case cocos2d::enumKeyCodes::KEY_B: return "KEY_B";
	case cocos2d::enumKeyCodes::KEY_C: return "KEY_C";
	case cocos2d::enumKeyCodes::KEY_D: return "KEY_D";
	case cocos2d::enumKeyCodes::KEY_E: return "KEY_E";
	case cocos2d::enumKeyCodes::KEY_F: return "KEY_F";
	case cocos2d::enumKeyCodes::KEY_G: return "KEY_G";
	case cocos2d::enumKeyCodes::KEY_H: return "KEY_H";
	case cocos2d::enumKeyCodes::KEY_I: return "KEY_I";
	case cocos2d::enumKeyCodes::KEY_J: return "KEY_J";
	case cocos2d::enumKeyCodes::KEY_K: return "KEY_K";
	case cocos2d::enumKeyCodes::KEY_L: return "KEY_L";
	case cocos2d::enumKeyCodes::KEY_M: return "KEY_M";
	case cocos2d::enumKeyCodes::KEY_N: return "KEY_N";
	case cocos2d::enumKeyCodes::KEY_O: return "KEY_O";
	case cocos2d::enumKeyCodes::KEY_P: return "KEY_P";
	case cocos2d::enumKeyCodes::KEY_Q: return "KEY_Q";
	case cocos2d::enumKeyCodes::KEY_R: return "KEY_R";
	case cocos2d::enumKeyCodes::KEY_S: return "KEY_S";
	case cocos2d::enumKeyCodes::KEY_T: return "KEY_T";
	case cocos2d::enumKeyCodes::KEY_U: return "KEY_U";
	case cocos2d::enumKeyCodes::KEY_V: return "KEY_V";
	case cocos2d::enumKeyCodes::KEY_W: return "KEY_W";
	case cocos2d::enumKeyCodes::KEY_X: return "KEY_X";
	case cocos2d::enumKeyCodes::KEY_Y: return "KEY_Y";
	case cocos2d::enumKeyCodes::KEY_Z: return "KEY_Z";
	case cocos2d::enumKeyCodes::KEY_LeftWindowsKey: return "KEY_LeftWindowsKey";
	case cocos2d::enumKeyCodes::KEY_RightWindowsKey: return "KEY_RightWindowsKey";
	case cocos2d::enumKeyCodes::KEY_ApplicationsKey: return "KEY_ApplicationsKey";
	case cocos2d::enumKeyCodes::KEY_Sleep: return "KEY_Sleep";
	case cocos2d::enumKeyCodes::KEY_NumPad0: return "KEY_NumPad0";
	case cocos2d::enumKeyCodes::KEY_NumPad1: return "KEY_NumPad1";
	case cocos2d::enumKeyCodes::KEY_NumPad2: return "KEY_NumPad2";
	case cocos2d::enumKeyCodes::KEY_NumPad3: return "KEY_NumPad3";
	case cocos2d::enumKeyCodes::KEY_NumPad4: return "KEY_NumPad4";
	case cocos2d::enumKeyCodes::KEY_NumPad5: return "KEY_NumPad5";
	case cocos2d::enumKeyCodes::KEY_NumPad6: return "KEY_NumPad6";
	case cocos2d::enumKeyCodes::KEY_NumPad7: return "KEY_NumPad7";
	case cocos2d::enumKeyCodes::KEY_NumPad8: return "KEY_NumPad8";
	case cocos2d::enumKeyCodes::KEY_NumPad9: return "KEY_NumPad9";
	case cocos2d::enumKeyCodes::KEY_Multiply: return "KEY_Multiply";
	case cocos2d::enumKeyCodes::KEY_Add: return "KEY_Add";
	case cocos2d::enumKeyCodes::KEY_Seperator: return "KEY_Seperator";
	case cocos2d::enumKeyCodes::KEY_Subtract: return "KEY_Subtract";
	case cocos2d::enumKeyCodes::KEY_Decimal: return "KEY_Decimal";
	case cocos2d::enumKeyCodes::KEY_Divide: return "KEY_Divide";
	case cocos2d::enumKeyCodes::KEY_F1: return "KEY_F1";
	case cocos2d::enumKeyCodes::KEY_F2: return "KEY_F2";
	case cocos2d::enumKeyCodes::KEY_F3: return "KEY_F3";
	case cocos2d::enumKeyCodes::KEY_F4: return "KEY_F4";
	case cocos2d::enumKeyCodes::KEY_F5: return "KEY_F5";
	case cocos2d::enumKeyCodes::KEY_F6: return "KEY_F6";
	case cocos2d::enumKeyCodes::KEY_F7: return "KEY_F7";
	case cocos2d::enumKeyCodes::KEY_F8: return "KEY_F8";
	case cocos2d::enumKeyCodes::KEY_F9: return "KEY_F9";
	case cocos2d::enumKeyCodes::KEY_F10: return "KEY_F10";
	case cocos2d::enumKeyCodes::KEY_F11: return "KEY_F11";
	case cocos2d::enumKeyCodes::KEY_F12: return "KEY_F12";
	case cocos2d::enumKeyCodes::KEY_F13: return "KEY_F13";
	case cocos2d::enumKeyCodes::KEY_F14: return "KEY_F14";
	case cocos2d::enumKeyCodes::KEY_F15: return "KEY_F15";
	case cocos2d::enumKeyCodes::KEY_F16: return "KEY_F16";
	case cocos2d::enumKeyCodes::KEY_F17: return "KEY_F17";
	case cocos2d::enumKeyCodes::KEY_F18: return "KEY_F18";
	case cocos2d::enumKeyCodes::KEY_F19: return "KEY_F19";
	case cocos2d::enumKeyCodes::KEY_F20: return "KEY_F20";
	case cocos2d::enumKeyCodes::KEY_F21: return "KEY_F21";
	case cocos2d::enumKeyCodes::KEY_F22: return "KEY_F22";
	case cocos2d::enumKeyCodes::KEY_F23: return "KEY_F23";
	case cocos2d::enumKeyCodes::KEY_F24: return "KEY_F24";
	case cocos2d::enumKeyCodes::KEY_Numlock: return "KEY_Numlock";
	case cocos2d::enumKeyCodes::KEY_ScrollLock: return "KEY_ScrollLock";
	case cocos2d::enumKeyCodes::KEY_LeftShift: return "KEY_LeftShift";
	case cocos2d::enumKeyCodes::KEY_RightShift: return "KEY_RightShift";
	case cocos2d::enumKeyCodes::KEY_LeftControl: return "KEY_LeftControl";
	case cocos2d::enumKeyCodes::KEY_RightContol: return "KEY_RightContol";
	case cocos2d::enumKeyCodes::KEY_LeftMenu: return "KEY_LeftMenu";
	case cocos2d::enumKeyCodes::KEY_RightMenu: return "KEY_RightMenu";
	case cocos2d::enumKeyCodes::KEY_BrowserBack: return "KEY_BrowserBack";
	case cocos2d::enumKeyCodes::KEY_BrowserForward: return "KEY_BrowserForward";
	case cocos2d::enumKeyCodes::KEY_BrowserRefresh: return "KEY_BrowserRefresh";
	case cocos2d::enumKeyCodes::KEY_BrowserStop: return "KEY_BrowserStop";
	case cocos2d::enumKeyCodes::KEY_BrowserSearch: return "KEY_BrowserSearch";
	case cocos2d::enumKeyCodes::KEY_BrowserFavorites: return "KEY_BrowserFavorites";
	case cocos2d::enumKeyCodes::KEY_BrowserHome: return "KEY_BrowserHome";
	case cocos2d::enumKeyCodes::KEY_VolumeMute: return "KEY_VolumeMute";
	case cocos2d::enumKeyCodes::KEY_VolumeDown: return "KEY_VolumeDown";
	case cocos2d::enumKeyCodes::KEY_VolumeUp: return "KEY_VolumeUp";
	case cocos2d::enumKeyCodes::KEY_NextTrack: return "KEY_NextTrack";
	case cocos2d::enumKeyCodes::KEY_PreviousTrack: return "KEY_PreviousTrack";
	case cocos2d::enumKeyCodes::KEY_StopMedia: return "KEY_StopMedia";
	case cocos2d::enumKeyCodes::KEY_PlayPause: return "KEY_PlayPause";
	case cocos2d::enumKeyCodes::KEY_LaunchMail: return "KEY_LaunchMail";
	case cocos2d::enumKeyCodes::KEY_SelectMedia: return "KEY_SelectMedia";
	case cocos2d::enumKeyCodes::KEY_LaunchApp1: return "KEY_LaunchApp1";
	case cocos2d::enumKeyCodes::KEY_LaunchApp2: return "KEY_LaunchApp2";
	case cocos2d::enumKeyCodes::KEY_OEM1: return "KEY_OEM1";
	case cocos2d::enumKeyCodes::KEY_OEMPlus: return "KEY_OEMPlus";
	case cocos2d::enumKeyCodes::KEY_OEMComma: return "KEY_OEMComma";
	case cocos2d::enumKeyCodes::KEY_OEMMinus: return "KEY_OEMMinus";
	case cocos2d::enumKeyCodes::KEY_OEMPeriod: return "KEY_OEMPeriod";
	case cocos2d::enumKeyCodes::KEY_OEM2: return "KEY_OEM2";
	case cocos2d::enumKeyCodes::KEY_OEM3: return "KEY_OEM3";
	case cocos2d::enumKeyCodes::KEY_OEM4: return "KEY_OEM4";
	case cocos2d::enumKeyCodes::KEY_OEM5: return "KEY_OEM5";
	case cocos2d::enumKeyCodes::KEY_OEM6: return "KEY_OEM6";
	case cocos2d::enumKeyCodes::KEY_OEM7: return "KEY_OEM7";
	case cocos2d::enumKeyCodes::KEY_OEM8: return "KEY_OEM8";
	case cocos2d::enumKeyCodes::KEY_OEM102: return "KEY_OEM102";
	case cocos2d::enumKeyCodes::KEY_Process: return "KEY_Process";
	case cocos2d::enumKeyCodes::KEY_Packet: return "KEY_Packet";
	case cocos2d::enumKeyCodes::KEY_Attn: return "KEY_Attn";
	case cocos2d::enumKeyCodes::KEY_CrSel: return "KEY_CrSel";
	case cocos2d::enumKeyCodes::KEY_ExSel: return "KEY_ExSel";
	case cocos2d::enumKeyCodes::KEY_EraseEOF: return "KEY_EraseEOF";
	case cocos2d::enumKeyCodes::KEY_Play: return "KEY_Play";
	case cocos2d::enumKeyCodes::KEY_Zoom: return "KEY_Zoom";
	case cocos2d::enumKeyCodes::KEY_PA1: return "KEY_PA1";
	case cocos2d::enumKeyCodes::KEY_OEMClear: return "KEY_OEMClear";
	case cocos2d::enumKeyCodes::KEY_ArrowUp: return "KEY_ArrowUp";
	case cocos2d::enumKeyCodes::KEY_ArrowDown: return "KEY_ArrowDown";
	case cocos2d::enumKeyCodes::KEY_ArrowLeft: return "KEY_ArrowLeft";
	case cocos2d::enumKeyCodes::KEY_ArrowRight: return "KEY_ArrowRight";
	case cocos2d::enumKeyCodes::CONTROLLER_A: return "CONTROLLER_A";
	case cocos2d::enumKeyCodes::CONTROLLER_B: return "CONTROLLER_B";
	case cocos2d::enumKeyCodes::CONTROLLER_Y: return "CONTROLLER_Y";
	case cocos2d::enumKeyCodes::CONTROLLER_X: return "CONTROLLER_X";
	case cocos2d::enumKeyCodes::CONTROLLER_Start: return "CONTROLLER_Start";
	case cocos2d::enumKeyCodes::CONTROLLER_Back: return "CONTROLLER_Back";
	case cocos2d::enumKeyCodes::CONTROLLER_RB: return "CONTROLLER_RB";
	case cocos2d::enumKeyCodes::CONTROLLER_LB: return "CONTROLLER_LB";
	case cocos2d::enumKeyCodes::CONTROLLER_RT: return "CONTROLLER_RT";
	case cocos2d::enumKeyCodes::CONTROLLER_LT: return "CONTROLLER_LT";
	case cocos2d::enumKeyCodes::CONTROLLER_Up: return "CONTROLLER_Up";
	case cocos2d::enumKeyCodes::CONTROLLER_Down: return "CONTROLLER_Down";
	case cocos2d::enumKeyCodes::CONTROLLER_Left: return "CONTROLLER_Left";
	case cocos2d::enumKeyCodes::CONTROLLER_Right: return "CONTROLLER_Right";
	default: return "Unknown"; // Si el código de tecla no se encuentra en la enumeración
	}
}
