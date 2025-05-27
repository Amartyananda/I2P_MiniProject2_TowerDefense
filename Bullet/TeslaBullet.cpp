#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Collider.hpp"
#include "TeslaBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

class Turret;

TeslaBullet::TeslaBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent) : Bullet("play/teslabullet.png", 1000, 1.25, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
}
void TeslaBullet::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), enemy->Position.x, enemy->Position.y));
}

void TeslaBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();
    for (auto &it : scene->EnemyGroup->GetObjects()) {
        Enemy *enemy = dynamic_cast<Enemy *>(it);
        if (!enemy->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            OnExplode(enemy);
            enemy->Hit(damage);
            enemy->Slow(0.6f, 1.5f);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    // Check if out of boundary.
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}

