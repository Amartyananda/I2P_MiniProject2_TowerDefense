#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/TeslaBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "TeslaTurret.hpp"
#include "Scene/PlayScene.hpp"

const int TeslaTurret::Price = 300;
TeslaTurret::TeslaTurret(float x, float y) : Turret("play/tower-base.png", "play/TeslaTurret.png", x, y, 400, Price, 0.275) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();  
}
void TeslaTurret::CreateBullet() {
    const float angleOffset = ALLEGRO_PI / 36; // ~10 degrees in radians
    float baseAngle = Rotation - ALLEGRO_PI / 2;

    for (int i = -1; i <= 1; ++i) {
        float angle = baseAngle + i * angleOffset;
        Engine::Point dir = Engine::Point(cos(angle), sin(angle));
        Engine::Point posOffset = dir.Normalize() * 36;
        float rotation = atan2(dir.y, dir.x);

        getPlayScene()->BulletGroup->AddNewObject(new TeslaBullet(Position + posOffset, dir, rotation, this));
    }
    AudioHelper::PlayAudio("tesla.mp3");
}

