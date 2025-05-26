#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "EagleBullet.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Collider.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/AreaEffect.hpp"

class Turret;

EagleBullet::EagleBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent) : Bullet("play/bullet-3.png", 1000, 3, position, forwardDirection, rotation, parent) {
}

void EagleBullet::OnExplode(Enemy *enemy) {
    auto* scene = getPlayScene();

    scene->EffectGroup->AddNewObject(
        new AreaEffect(enemy->Position.x, enemy->Position.y, ExplosionRadius, 0.5f));

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-1.png", dist(rng), enemy->Position.x, enemy->Position.y));

    for (auto* obj : scene->EnemyGroup->GetObjects()) {
        auto* e = dynamic_cast<Enemy*>(obj);
        if (!e || !e->Visible) continue;

        Engine::Point d = e->Position - enemy->Position;
        if (d.x*d.x + d.y*d.y <= ExplosionRadius*ExplosionRadius){
            e->Hit(SplashRatio * damage);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        }
    } 
}

void EagleBullet::Update(float dt){
    Sprite::Update(dt);
    PlayScene* scene = getPlayScene();

    for (auto* obj : scene->EnemyGroup->GetObjects()) {
        auto* enemy = dynamic_cast<Enemy*>(obj);
        if (!enemy || !enemy->Visible) continue;

        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)){
            //  visual ring
            scene->EffectGroup->AddNewObject(
                new AreaEffect(enemy->Position.x, enemy->Position.y, ExplosionRadius, 0.5f));

            //  ground decal
            static std::mt19937 rng{ std::random_device{}() };
            std::uniform_int_distribution<int> distDur(2,10);
            scene->GroundEffectGroup->AddNewObject(
                new DirtyEffect("play/dirty-1.png", distDur(rng), enemy->Position.x, enemy->Position.y));

            //  damage everyone inside radius
            for (auto* o : scene->EnemyGroup->GetObjects()) {
                auto* e = dynamic_cast<Enemy*>(o);
                if (!e || !e->Visible) continue;
                Engine::Point d = e->Position - enemy->Position;
                if (d.x*d.x + d.y*d.y <= ExplosionRadius*ExplosionRadius)
                    e->Hit(SplashRatio * damage);
            }

            scene->BulletGroup->RemoveObject(objectIterator);
            return;              
        }
    }

    // bullet flew out of screen
    if (!Engine::Collider::IsRectOverlap(Position - Size/2, Position + Size/2, Engine::Point(0,0),PlayScene::GetClientSize()))
        scene->BulletGroup->RemoveObject(objectIterator);
}
