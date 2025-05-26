#ifndef StunBullet_HPP
#define StunBullet_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class StunBullet : public Bullet {
public:
    explicit StunBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
};
#endif   // StunBullet_HPP
