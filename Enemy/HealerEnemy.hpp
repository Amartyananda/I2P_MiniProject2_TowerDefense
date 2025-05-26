#ifndef HEALERENEMY_HPP
#define HEALERENEMY_HPP

#include <vector>
#include <allegro5/allegro.h>
#include <cmath>
#include <memory>
#include "Enemy.hpp"

class HealerEnemy : public Enemy {
public:
    HealerEnemy(float x, float y);
    void Update(float deltaTime) override;
    void Draw() const override;

private:
    std::vector<std::shared_ptr<ALLEGRO_BITMAP>> frames;
    bool faceLeft = false;
    float timeTicks = 0.0f;
    static constexpr float timeSpan = 1.0f;      // total cycle length (seconds)

    float healTimer   = 0;                      
    static constexpr float healInterval = 1.8f; // seconds between healing
    static constexpr float healRadius = 72.0f;
    static constexpr float healAmount = 2;
    static constexpr float pulseSpan = 0.60f;
};

#endif // HEALERENEMY_HPP
