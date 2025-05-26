#include <functional>
#include <string>
#include <fstream>
#include <ctime>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "StartScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"

std::string WinScene::PlayerName = "";
void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    
    bgmId = AudioHelper::PlayAudio("win.wav");

    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    askingName = true;
    saved = false;
    NameInput();
}

void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}
void WinScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void WinScene::OnKeyDown(int keyCode) {
    if (!askingName) {
        Group::OnKeyDown(keyCode);
        return;
    }

    // ----- accept letters & digits -----------------------------------------
    if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
        if (nameBuf.size() < 10)
            nameBuf.push_back('A' + keyCode - ALLEGRO_KEY_A);
    } else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        if (nameBuf.size() < 12)
            nameBuf.push_back('0' + keyCode - ALLEGRO_KEY_0);
    }
    // ----- backspace --------------------------------------------------------
    else if (keyCode == ALLEGRO_KEY_BACKSPACE) {
        if (!nameBuf.empty())
            nameBuf.pop_back();
    }
    // ----- confirm ----------------------------------------------------------
    else if (keyCode == ALLEGRO_KEY_ENTER) {
        if (!nameBuf.empty()) {
            askingName        = false;
            dimmer->Visible   = false;
            panel->Visible    = false;
            nameLabel->Visible = false;
            promptLabel->Visible = false;
            PlayerName = nameBuf;
            WriteScore();

            return;
        }
    }
    // Update live preview
    nameLabel->Text = nameBuf.empty() ? "_" : nameBuf;
}

void WinScene::NameInput(){
    const auto  scr   = Engine::GameEngine::GetInstance().GetScreenSize();
    const int   halfX = scr.x / 2;
    const int   halfY  = scr.y / 2;        
    
    dimmer = new Engine::Sprite("ui/panels/LargeBackground.png", halfX, halfY, 1200, 1000);
    dimmer->Tint = al_map_rgba(0, 0, 0, 160);
    AddNewObject(dimmer);
    
    panel = new Engine::Sprite("ui/panels/LargeBackground.png", halfX, halfY, 800, 350);
    panel->Tint = al_map_rgba(255, 255, 255, 255);
    AddNewObject(panel);      
    
    // ── title & live-updating label ─────────────────────────────────────────
    promptLabel = new Engine::Label("Enter your name", "pirulen.ttf", 36, halfX, halfY - 60, 0, 0, 0, 255, 0.5, 0.5);
    nameLabel = new Engine::Label("_", "pirulen.ttf", 32, halfX, halfY, 0, 0, 0, 255, 0.5, 0.5);
    AddNewObject(nameLabel);  
    AddNewObject(promptLabel);  
}

void WinScene::WriteScore() {
    if (saved || PlayerName.empty()) return;

    auto* play = dynamic_cast<PlayScene*>(
        Engine::GameEngine::GetInstance().GetScene("play"));
    if (!play) return;

    finalScore = play->GetLives() * 100 + play->GetMoney();
    std::time_t now = std::time(nullptr);

    std::ofstream fout("Resource/scoreboard.txt", std::ios::app);
    fout << PlayerName << ' ' << finalScore << ' ' << now << '\n';
    saved = true;
}