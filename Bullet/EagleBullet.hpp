#ifndef EAGLEBULLET_HPP
#define EAGLEBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class EagleBullet : public Bullet {
public:
    explicit EagleBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
    void Update(float deltaTime) override;

private:
    static constexpr float ExplosionRadius = 50.0f;
    static constexpr float SplashRatio    = 0.8f;
};
#endif   // EAGLEBULLET_HPP
