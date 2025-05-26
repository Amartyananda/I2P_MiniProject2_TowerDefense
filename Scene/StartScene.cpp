#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Engine/Sprite.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void StartScene::Initialize() {
    // resources load
    const auto  scr   = Engine::GameEngine::GetInstance().GetScreenSize();
    const int   halfX = scr.x / 2;
    const int   halfY  = scr.y / 2;       
    const int   BTN_W = 500;
    const int   BTN_H = 100;
    const int   GAP   = 40;               // vertical space between buttons
    bgmInstance = AudioHelper::PlaySample("exploration-bgm.mp3", true, AudioHelper::BGMVolume);

    // start menu
    AddNewObject(new Engine::Image( "stage-select/menu.jpg", 0, 0, scr.x, scr.y));
    AddNewObject(new Engine::Image( "stage-select/logo.png", halfX - 550, halfY - 480, halfX + 275, halfY + 240));

    auto addBtn = [this, halfX, BTN_W, BTN_H](int cx, int cy, const std::string& text, std::function<void()> fn) {
        auto* b = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", cx - BTN_W / 2, cy - BTN_H / 2,BTN_W, BTN_H);
        b->SetOnClickCallback(std::move(fn));
        AddNewControlObject(b);
        AddNewObject(new Engine::Label(text, "pirulen.ttf", 48, cx, cy, 0, 0, 0, 255, 0.5, 0.5));
    };

    const int playY       = halfY - (BTN_H + GAP) + 150;
    const int scoreboardY = halfY + 150;
    const int settingsY   = halfY + (BTN_H + GAP) + 150;
    addBtn(halfX, playY, "Play", [this]{ PlayOnClick(1); });
    addBtn(halfX, scoreboardY, "Scoreboard", [this]{ ScoreboardOnClick(); });
    addBtn(halfX, settingsY, "Settings", [this]{ SettingsOnClick(); });
}

void StartScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void StartScene::SettingsOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}
void StartScene::ScoreboardOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}