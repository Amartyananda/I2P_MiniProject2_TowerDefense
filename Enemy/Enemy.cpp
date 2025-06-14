#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Bullet/Bullet.hpp"
#include "Enemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

PlayScene *Enemy::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Enemy::OnExplode() {
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}

Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, float MAXhp, int money) : Engine::Sprite(img, x, y), speed(speed), hp(hp), MAXhp(MAXhp), money(money) {
    CollisionRadius = radius;
    reachEndTime = 0;
}

void Enemy::Hit(float damage) {
    hp -= damage;
    if (hp <= 0) {
        OnExplode();
        // Remove all turret's reference to target.
        for (auto &it : lockedTurrets)
            it->Target = nullptr;
        for (auto &it : lockedBullets)
            it->Target = nullptr;
        getPlayScene()->EarnMoney(money);
        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }
}
void Enemy::UpdatePath(const std::vector<std::vector<int>> &mapDistance) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    if (x < 0) x = 0;
    if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;
    Engine::Point pos(x, y);
    int num = mapDistance[y][x];
    if (num == -1) {
        num = 0;
        Engine::LOG(Engine::ERROR) << "Enemy path finding error";
    }
    path = std::vector<Engine::Point>(num + 1);
    while (num != 0) {
        std::vector<Engine::Point> nextHops;
        for (auto &dir : PlayScene::directions) {
            int x = pos.x + dir.x;
            int y = pos.y + dir.y;
            if (x < 0 || x >= PlayScene::MapWidth || y < 0 || y >= PlayScene::MapHeight || mapDistance[y][x] != num - 1)
                continue;
            nextHops.emplace_back(x, y);
        }
        // Choose arbitrary one.
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
        pos = nextHops[dist(rng)];
        path[num] = pos;
        num--;
    }
    path[0] = PlayScene::EndGridPoint;
}
void Enemy::Update(float deltaTime) {
    // Pre-calculate the velocity.
    if (stunTimer > 0.0f) {
        stunTimer -= deltaTime;
        if (stunTimer < 0.0f) stunTimer = 0.0f;
        return;
    }

    float speedModifier = 1.0f;
    if (slowTimer > 0.0f) {
        slowTimer -= deltaTime;
        if (slowTimer < 0.0f) {
            slowTimer  = 0.0f;
            slowFactor = 1.0f;
        }
        speedModifier = slowFactor;   // e.g. 0.5 → half speed
    }

    float remainSpeed = speed * speedModifier * deltaTime;
    while (remainSpeed != 0) {
        if (path.empty()) {
            // Reach end point.
            Hit(hp);
            getPlayScene()->Hit();
            reachEndTime = 0;
            return;
        }
        Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        // Add up the distances:
        // 1. to path.back()
        // 2. path.back() to border
        // 3. All intermediate block size
        // 4. to end point
        reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
        Engine::Point normalized = vec.Normalize();
        if (remainSpeed - vec.Magnitude() > 0) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        } else {
            Velocity = normalized * remainSpeed / deltaTime;
            remainSpeed = 0;
        }
    }
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
}

void Enemy::Draw() const {
    ALLEGRO_COLOR originalTint = Tint;
    
    if (stunTimer > 0.0f) const_cast<Enemy*>(this)->Tint = al_map_rgb(200, 180, 0);
    if (slowTimer > 0.0f) const_cast<Enemy*>(this)->Tint = al_map_rgb(80, 140, 255);
    Sprite::Draw();

    const_cast<Enemy*>(this)->Tint = originalTint;
    if (hp > 0) {
        const float barW   = 40;      // width  of the bar (pixels)
        const float barH   =  6;      // height of the bar
        const float yOff   = CollisionRadius + 12;      // 12 px above sprite
        const float left   = Position.x - barW / 2;
        const float right  = Position.x + barW / 2;
        const float top    = Position.y - yOff;
        const float fillW  = barW * (hp / MAXhp);

        // background (dark grey)
        al_draw_filled_rectangle(left, top, right, top + barH,
                                 al_map_rgb(40, 40, 40));
        // foreground (green → red as HP drops)
        float ratio = hp / MAXhp;
        ALLEGRO_COLOR col = al_map_rgb(255 * (1 - ratio), 255 * ratio, 0);
        al_draw_filled_rectangle(left, top, left + fillW, top + barH, col);

        // thin white border
        al_draw_rectangle(left, top, right, top + barH,
                          al_map_rgb(255,255,255), 1);
    }

    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}

void Enemy::Heal(float amount) {
    hp = std::min(hp + amount, MAXhp);
}