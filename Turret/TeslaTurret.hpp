#ifndef TESLATURRET_HPP
#define TESLATURRET_HPP
#include "Turret.hpp"

class TeslaTurret : public Turret {
public:
    static const int Price;
    TeslaTurret(float x, float y);
    void CreateBullet() override;
};
#endif   // TeslaTurret_HPP
