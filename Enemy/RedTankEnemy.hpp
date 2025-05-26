#ifndef REDTANKENEMY_HPP
#define REDTANKENEMY_HPP
#include "Enemy.hpp"
#include "Engine/Sprite.hpp"

class RedTankEnemy : public Enemy {
private:
    Sprite head;
    float targetRotation;

public:
    RedTankEnemy(int x, int y);
    void Draw() const override;
    void Update(float deltaTime) override;
};
#endif   // REDTANKENEMY_HPP
