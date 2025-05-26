#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/TeslaBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "TeslaTurret.hpp"
#include "Scene/PlayScene.hpp"

const int TeslaTurret::Price = 250;
TeslaTurret::TeslaTurret(float x, float y) : Turret("play/tower-base.png", "play/TeslaTurret.png", x, y, 300, Price, 0.4) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();  
}
void TeslaTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new TeslaBullet(Position + normalized * 36 - normal * 6, diff, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new TeslaBullet(Position + normalized * 36 + normal * 6, diff, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new TeslaBullet(Position + normalized * 36 + normal * 6 * 2, diff, rotation, this));
    AudioHelper::PlayAudio("laser.wav");
}
