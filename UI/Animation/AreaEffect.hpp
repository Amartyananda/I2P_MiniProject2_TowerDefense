#ifndef EXPLOSIONEFFECT_HPP
#define EXPLOSIONEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"

class PlayScene;

class AreaEffect : public Engine::Sprite {
protected:
    PlayScene *getPlayScene();

    void Update(float deltaTime) override;
    void Draw() const override;


public:
    AreaEffect(float x, float y, float maxRadius = 96.0f, float duration = 0.5f);
    float _time     = 0.0f;
    float _duration;
    float _maxRadius;
};
#endif   // EXPLOSIONEFFECT_HPP
