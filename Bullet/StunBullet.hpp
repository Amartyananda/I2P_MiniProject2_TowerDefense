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
    void Update(float deltaTime) override;

private:
    static constexpr float ExplosionRadius = 10.0f;
    static constexpr float SplashRatio    = 0.675f;
};
#endif   // StunBullet_HPP
