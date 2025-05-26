#ifndef TeslaBullet_HPP
#define TeslaBullet_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class TeslaBullet : public Bullet {
public:
    explicit TeslaBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
};
#endif   // TeslaBullet_HPP
