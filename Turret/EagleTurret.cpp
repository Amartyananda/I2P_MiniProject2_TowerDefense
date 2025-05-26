// EagleTurret.cpp
#include "EagleTurret.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/AudioHelper.hpp"
#include "Scene/PlayScene.hpp"
#include "Bullet/EagleBullet.hpp"  
#include <cmath>

const int EagleTurret::Price = 225;

EagleTurret::EagleTurret(float x, float y)
    : Turret("play/tower-base.png", "play/eagle.png", x, y, 650, Price, 0.3){

    Anchor.y += 8.0f / GetBitmapHeight();
}

void EagleTurret::Update(float deltaTime) {
    // track mouse position
    auto scene = getPlayScene();
    Engine::Point mouse = Engine::GameEngine::GetInstance().GetMousePosition();
    Engine::Point diff  = mouse - Position;
    // turret sprite points up at rotation = 0, so offset by +π/2:
    Rotation = std::atan2(diff.y, diff.x) + ALLEGRO_PI/2;
    // run base update (handles cooldown and calls CreateBullet)
    Turret::Update(deltaTime);
}

void EagleTurret::CreateBullet() {
    // direction vector from rotation
    Engine::Point dir{ (float)std::cos(Rotation - ALLEGRO_PI/2),
                       (float)std::sin(Rotation - ALLEGRO_PI/2) };
    float rot = std::atan2(dir.y, dir.x) + ALLEGRO_PI/2;
    Engine::Point norm = dir.Normalize();

    auto start = Position + norm * 48;
    getPlayScene()->BulletGroup->AddNewObject(new EagleBullet(start, dir, rot, this));
    AudioHelper::PlayAudio("missile.wav"); // add this sample to Resources
}

