#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Bullet;
class PlayScene;
class Turret;

class Enemy : public Engine::Sprite {
protected:
    std::vector<Engine::Point> path;
    float speed;
    float hp;
    float MAXhp;
    int money;
    PlayScene *getPlayScene();
    virtual void OnExplode();

    float stunTimer = 0.0f;
    float slowTimer = 0.0f;
    float slowFactor = 1.0f;

public:
    float reachEndTime;
    std::list<Turret *> lockedTurrets;
    std::list<Bullet *> lockedBullets;
    Enemy(std::string img, float x, float y, float radius, float speed, float hp, float MAXhp, int money);
    void Hit(float damage);
    void UpdatePath(const std::vector<std::vector<int>> &mapDistance);
    void Update(float deltaTime) override;
    void Draw() const override;
    void Heal(float amount);

    int GetMaxHP() const {return MAXhp;}
    void Stun(float dur) { stunTimer = std::max(stunTimer, dur); }
    void Slow(float factor, float dur) { slowFactor = std::min(slowFactor, factor); slowTimer = std::max(slowTimer, dur); }
    bool IsStunned()     const { return stunTimer > 0.0f; }
    bool IsSlowed()      const { return slowTimer > 0.0f; }
};
#endif   // ENEMY_HPP
