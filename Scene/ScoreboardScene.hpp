// ScoreboardScene.hpp
#ifndef SCOREBOARD_SCENE_HPP
#define SCOREBOARD_SCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>
#include <string>

#include "Engine/IScene.hpp"

class ScoreboardScene final : public Engine::IScene {
private:
    struct Entry { 
        std::string name; 
        int score;
        std::time_t ts;
    };
    std::vector<Entry> entries;

    int page = 0;
    std::vector<Engine::IObject*> labelObjs;    // labels of current page
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;                          

public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate()  override;
    void LoadScores();                       
    void RenderPage();                         
    void PagePrevOnClick();                     
    void PageNextOnClick();  
    void BackOnClick();                         
};

#endif