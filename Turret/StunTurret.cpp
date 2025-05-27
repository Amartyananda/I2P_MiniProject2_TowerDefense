#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/StunBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "StunTurret.hpp"
#include "Scene/PlayScene.hpp"

const int StunTurret::Price = 350;
StunTurret::StunTurret(float x, float y) : Turret("play/tower-base.png", "play/StunTurret.png", x, y, 175, Price, 1.2) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();  
}
void StunTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new StunBullet(Position + normalized * 36 - normal * 6, diff, rotation, this));
    AudioHelper::PlayAudio("stun.mp3");
}
