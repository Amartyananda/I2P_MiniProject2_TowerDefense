// UI/Animation/AreaEffect.cpp
#include <allegro5/allegro_primitives.h>
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Resources.hpp"
#include "AreaEffect.hpp"

PlayScene *AreaEffect::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

AreaEffect::AreaEffect(float x, float y, float maxRadius, float duration):Sprite("play/blank.png", x, y), _duration(duration), _maxRadius(maxRadius){
}

void AreaEffect::Update(float deltaTime) {
    _time += deltaTime;
    if (_time >= _duration) {
        getPlayScene()->EffectGroup->RemoveObject(objectIterator);
    }
}

void AreaEffect::Draw() const {
    float t = _time / _duration;
    float radius = _maxRadius * t;
    int alpha = static_cast<int>(255 * (1 - t));

    ALLEGRO_COLOR fill = al_map_rgba(255, 0, 0, alpha/2);
    ALLEGRO_COLOR border = al_map_rgba(255, 0, 0, alpha);

    al_draw_filled_circle(Position.x, Position.y, radius, fill);
    al_draw_circle(Position.x, Position.y, radius, border, 2.0f);
}
