#include <Geode/Geode.hpp>

using namespace geode::prelude;

static float lastPercent  = 0;
static float levelPercent = 0;

static bool isEnabled = false;

static void playJingle() {
	auto system = FMODAudioEngine::get()->m_system;
	FMOD::Channel* channel;
	FMOD::Sound* sound;

	auto soundPath = Mod::get()->getSettingValue<std::filesystem::path>("customSound");

	if (soundPath.string() == "No custom sound")
		soundPath = Mod::get()->getResourcesDir() / "weird-route-jingle.mp3";

	system->createSound(soundPath.string().c_str(), FMOD_DEFAULT, nullptr, &sound);
	system->playSound(sound, nullptr, false, &channel);
	channel->setVolume((float)Mod::get()->getSettingValue<int>("volume") / 100.0f);
}

#include <Geode/modify/PlayLayer.hpp>
class $modify(MyPlayLayer, PlayLayer) {
	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);

		if (!isEnabled)
			return;

		if (levelPercent == 0.0f || levelPercent == 100.0f || m_isPlatformer)
			return;

		float currentPercent = getCurrentPercent();

		if (lastPercent < levelPercent && currentPercent >= levelPercent)
			playJingle();

		lastPercent = currentPercent;
	}

	void resetLevel() {
		PlayLayer::resetLevel();

		isEnabled = Mod::get()->getSettingValue<bool>("enabled");

		if (!isEnabled)
			return;

		levelPercent = m_level->getNormalPercent();
		lastPercent = 0;
	}
};