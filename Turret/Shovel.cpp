#include "Shovel.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/AudioHelper.hpp"

const int Shovel::Price = 0;
Shovel::Shovel(float x, float y)
    : Turret("play/shovel.png", "play/shovel.png", x, y, 0.0f, Price, 0.0f){
    // center the icon on its Position
    Anchor = Engine::Point{ 0.5f, 0.5f };
}
