#include "pch.h"
#include "Utils.h"
#include "gdstring.h"
#include "SongControrLayer.h"
#include "SearchMusicLayer.h"
#include "fts_fuzzy_match.h"


class ButtonsClass : public gd::FLAlertLayer, public cocos2d::CCTextFieldDelegate, public gd::FLAlertLayerProtocol
{
public:
	void onSongControlLayer(cocos2d::CCObject* pSender) {
		if (SongControlLayer::isGameShow) {
			return;
		}
		auto GJSongBrowserParent = reinterpret_cast<gd::GJSongBrowser*>(reinterpret_cast<cocos2d::CCNode*>(pSender)->getParent()->getParent()->getParent());
		Utils::shareDirectorA()->getRunningScene()->addChild(SongControlLayer::create(GJSongBrowserParent, GJSongBrowserParent->m_iPage), Utils::shareDirectorA()->getRunningScene()->getHighestChildZ());
		//std::cout << Utils::getNameObj(reinterpret_cast<cocos2d::CCNode*>(pSender)->getParent()->getParent()->getParent()) << std::endl;
	}

	void onSearchMusicLayer(cocos2d::CCObject* pSender) {
		auto GJSongBrowserParent = reinterpret_cast<gd::GJSongBrowser*>(reinterpret_cast<cocos2d::CCNode*>(pSender)->getParent()->getParent()->getParent());
		Utils::shareDirectorA()->getRunningScene()->addChild(SearchMusicLayer::create(GJSongBrowserParent), Utils::shareDirectorA()->getRunningScene()->getHighestChildZ());

	}

	void onSearchMusicOff(cocos2d::CCObject* pSender) {
		auto GJSongBrowserParent = reinterpret_cast<gd::GJSongBrowser*>(reinterpret_cast<cocos2d::CCNode*>(pSender)->getParent()->getParent()->getParent());
		(MBO(cocos2d::CCArray*, GJSongBrowserParent, 0x1dc))->removeAllObjects();
		(MBO(cocos2d::CCArray*, GJSongBrowserParent, 0x1dc))->initWithArray(gd::MusicDownloadManager::sharedState()->getDownloadedSongs());
		GJSongBrowserParent->loadPage(0);
	}
};



class GJSongBrowserHook : public gd::GJSongBrowser {
public:

	void customSetupHook() {
		matdash::orig<&GJSongBrowserHook::customSetupHook>(this);
		auto menu = cocos2d::CCMenu::create();
		menu->setPosition({ 0.f, 0.f });
		menu->setTag(13);
		this->m_pLayer->addChild(menu);

		auto openinfoLayerbutton_sprite = gd::ButtonSprite::createWithSpriteFrameName("GJ_audioOnBtn_001.png");
		openinfoLayerbutton_sprite->setScale(1.2f);
		//auto openinfoLayerbutton = gd::CCMenuItemSpriteExtra::create(openinfoLayerbutton_sprite, menu, (cocos2d::SEL_MenuHandler)&BetterPause::onInfoLevelOpen);		openinfoLayerbutton->setPosition({ 40.f, 35.f });
		auto openinfoLayerbutton = gd::CCMenuItemSpriteExtra::create(openinfoLayerbutton_sprite, this, (cocos2d::SEL_MenuHandler)&ButtonsClass::onSongControlLayer);
		openinfoLayerbutton->setPosition({ 30.f, 30.f });
		menu->addChild(openinfoLayerbutton);

		auto spriteFindMusic = cocos2d::CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
		auto openFindMusic = gd::CCMenuItemSpriteExtra::create(spriteFindMusic, this, (cocos2d::SEL_MenuHandler)&ButtonsClass::onSearchMusicLayer);
		openFindMusic->setPosition({ Utils::winSize().width - 20.f, Utils::winSize().height - 50.f });
		menu->addChild(openFindMusic);

		auto spriteFindOffMusic = cocos2d::CCSprite::createWithSpriteFrameName("gj_findBtnOff_001.png");
		auto findOffMusic = gd::CCMenuItemSpriteExtra::create(spriteFindOffMusic, this, (cocos2d::SEL_MenuHandler)&GJSongBrowserHook::onSearchMusicOff);
		findOffMusic->setPosition({ Utils::winSize().width - 20.f, Utils::winSize().height - 90.f });
		findOffMusic->setTag(14);
		findOffMusic->setVisible(false);
		menu->addChild(findOffMusic);
	}

	void loadPageHook(int index) {
		matdash::orig<&GJSongBrowserHook::loadPageHook>(this,index);
	}

	void onSearchMusicOff(cocos2d::CCObject* pSender) {
		SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
		SearchMusicLayer::stringToSearch = "";
		(MBO(cocos2d::CCArray*, this, 0x1dc))->removeAllObjects();
		(MBO(cocos2d::CCArray*, this, 0x1dc))->initWithArray(gd::MusicDownloadManager::sharedState()->getDownloadedSongs());
		this->loadPage(0);
		if (this->m_pLayer) {
			auto menuButtonsCreateExt = reinterpret_cast<cocos2d::CCMenu*>(this->m_pLayer->getChildByTag(13));
			if (menuButtonsCreateExt) {
				reinterpret_cast<gd::CCMenuItemSpriteExtra*>(menuButtonsCreateExt->getChildByTag(14))->setVisible(false);
			}
		}
		
	}

};

class CustomSongWidgetHook : public gd::CustomSongWidget {
public:

	void onPlaybackHook(cocos2d::CCObject* pSender) {
		matdash::orig<&CustomSongWidgetHook::onPlaybackHook, matdash::Thiscall>(this, pSender);

		if (Utils::shareFMOD()->isBackgroundMusicPlaying(
			gd::MusicDownloadManager::sharedState()->pathForSong(this->m_uID)
		)) {
			if (!Utils::shareManager()->getEditorLayer() &&
				Utils::shareManager()->getGameVariable("MMO") == false) {
				Utils::shareManagerSound()->playBackgroundMusic(true, "menuLoop.mp3");
			}
		}

	}

	void updatePlaybackBtn() {
		matdash::orig<&CustomSongWidgetHook::updatePlaybackBtn, matdash::Thiscall>(this);

		for (size_t i = 0; i < this->getChildrenCount(); i++)
		{
			auto node = dynamic_cast<SliderSongControl*>(this->getChildren()->objectAtIndex(i));
			if (node) {
				this->m_pInfoLabel->setString(cocos2d::CCString::createWithFormat(
					"SongID: %i\nSize: %.2fMB", this->m_songInfo->m_nSongID, this->m_songInfo->m_fFileSize)->getCString());
				this->m_pInfoLabel->setScale(0.3f);
				this->m_pInfoLabel->setPosition({ -138.f, -25.f });
				this->m_pInfoLabel->setAnchorPoint({ 0.f, 0.5f });
				break;
			}
		}

	}

};

class MusicDownloadManagerHook : public gd::MusicDownloadManager {
public:
	cocos2d::CCArray* getDownloadedSongsHook() {
		auto ret = matdash::orig<&MusicDownloadManagerHook::getDownloadedSongsHook, matdash::Thiscall>(this);

		if (SearchMusicLayer::stringToSearch.empty() || SearchMusicLayer::typeSearch == kTypeSearchMusicLayer::kTypeSearchNone) {
			SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
			SearchMusicLayer::stringToSearch = "";
			return ret;
		}

		auto retModified = cocos2d::CCArray::createWithCapacity(ret->count());

		for (size_t i = 0; i < ret->count(); i++)
		{
			auto musicObj = reinterpret_cast<gd::SongInfoObject*>(ret->objectAtIndex(i));

			int score;

			if (SearchMusicLayer::typeSearch == kTypeSearchMusicLayer::kTypeSearchSongName) {

				if (!musicObj->m_sSongName.empty() &&
					fts::fuzzy_match(SearchMusicLayer::stringToSearch.c_str(), musicObj->m_sSongName.c_str(), score)) {
					retModified->addObject(musicObj);
				}
			}
			else {
				if (!musicObj->m_sArtistName.empty() &&
					fts::fuzzy_match(SearchMusicLayer::stringToSearch.c_str(), musicObj->m_sArtistName.c_str(), score)) {
					retModified->addObject(musicObj);
				}
			}
		}

		return retModified;
	}
};

void GJDropDownLayer_exitLayer(gd::GJDropDownLayer* self, cocos2d::CCObject* pSender) {
	if (Utils::nameChildFind(self, "GJSongBrowser")) {
		SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
		SearchMusicLayer::stringToSearch = "";
	}
	matdash::orig<&GJDropDownLayer_exitLayer, matdash::Thiscall>(self, pSender);
}

void CCKeyboardDispatcher_dispatchKeyboardMSGHook(cocos2d::CCKeyboardDispatcher* self,
	cocos2d::enumKeyCodes key,
	bool down)
{
	if (key == sharedStateA().m_iKeyShowReproductor && down && !SongControlLayer::isGameShow && !Utils::shareDirectorA()->getIsTransitioning()
		&& (!Utils::getplayLayerA() || Utils::getplayLayerA()->m_bIsPaused)) {
		Utils::shareDirectorA()->getRunningScene()->addChild(SongControlLayer::create(nullptr, 0), Utils::shareDirectorA()->getRunningScene()->getHighestChildZ());
	}
	matdash::orig<&CCKeyboardDispatcher_dispatchKeyboardMSGHook, matdash::Thiscall>(self, key, down);
}

void CCNode_addChildHook(cocos2d::CCNode* self,
	cocos2d::CCNode* child,
	int zOrden,
	int tag)
{

	if (std::string(Utils::getNameObj(child)) == "GJSongBrowser") {
		zOrden = Utils::shareDirectorA()->getRunningScene()->getHighestChildZ();
	}
	
	matdash::orig<&CCNode_addChildHook, matdash::Thiscall>(self, child, zOrden, tag);
}

void AppDelegate_trySaveGameHook(gd::AppDelegate* self)
{
	sharedStateA().SaveSettings();
	matdash::orig<&AppDelegate_trySaveGameHook, matdash::Thiscall>(self);
}