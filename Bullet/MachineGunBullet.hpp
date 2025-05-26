#ifndef MACHINEGUNBULLET_HPP
#define MACHINEGUNBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class MachineGunBullet : public Bullet {
public:
    explicit MachineGunBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
};
#endif   // MACHINEGUNBULLET_HPP
