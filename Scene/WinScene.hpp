#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class WinScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    bool saved = false;         
    int  finalScore = 0;

    bool            askingName = true;     // dialog is up until Enter pressed
    std::string     nameBuf;               // what the player has typed
    Engine::Label*  nameLabel  = nullptr;  // shows the text in the box
    Engine::Label*  promptLabel = nullptr;
    Engine::Sprite* dimmer    = nullptr;  // semi-transparent backdrop
    Engine::Sprite* panel     = nullptr;  // white rectangle behind text
    static std::string PlayerName;

public:
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void NameInput();
    void WriteScore();
    void OnKeyDown(int keycode) override;
};

#endif   // WINSCENE_HPP
