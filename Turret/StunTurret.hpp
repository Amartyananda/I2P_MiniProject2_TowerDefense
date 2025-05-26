#ifndef STUNTURRET_HPP
#define STUNTURRET_HPP
#include "Turret.hpp"

class StunTurret : public Turret {
public:
    static const int Price;
    StunTurret(float x, float y);
    void CreateBullet() override;
};
#endif   // STUNTURRET_HPP
