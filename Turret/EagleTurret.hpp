#ifndef EAGLETURRET_HPP
#define EAGLETURRET_HPP

#include "Turret.hpp"

class EagleTurret : public Turret {
public:
    static const int Price;

    EagleTurret(float x, float y);
    void Update(float deltaTime) override;
    void CreateBullet() override;
};

#endif // EAGLETURRET_HPP