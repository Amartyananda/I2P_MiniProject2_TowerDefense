    #include <allegro5/allegro_primitives.h>
    #include "Enemy/HealerEnemy.hpp"
    #include "Scene/PlayScene.hpp"
    #include "Engine/AudioHelper.hpp"
    #include "Engine/Resources.hpp"

    HealerEnemy::HealerEnemy(float x, float y): Enemy("play/Wizard1.png", x, y, 20, 45, 30, 30, 40){ 

        for (int i = 1; i <= 6; ++i) {
            frames.push_back(
                Engine::Resources::GetInstance()
                    .GetBitmap("play/Wizard" + std::to_string(i) + ".png"));
        }
        bmp = frames[0];
    }

    void HealerEnemy::Update(float deltaTime) {
        Enemy::Update(deltaTime);

        timeTicks = std::fmod(timeTicks + deltaTime, timeSpan);
        int phase = static_cast<int>(std::floor(timeTicks / timeSpan * frames.size()));
        bmp = frames[phase];

        faceLeft  = (Velocity.x < -1e-4f);        // move left  → mirror
        Rotation  = 0.0f;    

        healTimer += deltaTime;
        if (healTimer >= healInterval) {
            healTimer -= healInterval;

            Heal(MAXhp * 0.1f);
            const auto& objs = getPlayScene()->EnemyGroup->GetObjects();
            for (auto* o : objs) {
                auto* other = dynamic_cast<Enemy*>(o);
                if (!other || other == this) continue;

                float dx = other->Position.x - Position.x;
                float dy = other->Position.y - Position.y;
                if (dx*dx + dy*dy <= healRadius * healRadius)
                    other->Heal(other->GetMaxHP() * 0.2f);
            }
        }
    }

    void HealerEnemy::Draw() const {
        Enemy::Draw();

        const float pulsePhase = healTimer;        // 0 … 3
        const float baseR      = healRadius;       // 40
        const float maxScale   = 1.2f;            // 40 → 56 px at peak
        const float haloAlpha  =  40;              // constant faint halo α
        const float ringWidth  =   2;              // outline thickness

        ALLEGRO_COLOR haloFill   = al_map_rgba(  0,255,  0, haloAlpha);
        ALLEGRO_COLOR haloBorder = al_map_rgba(  0,255,  0, haloAlpha+20);
        al_draw_filled_circle(Position.x, Position.y, baseR, haloFill);
        al_draw_circle        (Position.x, Position.y, baseR, haloBorder, 1.0f);

        if (pulsePhase < pulseSpan) {
            float t = pulsePhase / pulseSpan;              // 0 … 1
            float scale  = 1.0f + (maxScale - 1.0f) * t;   // linear
            float alpha  = 180   * (1.0f - t);             // fade-out
            float r = baseR * scale;
            ALLEGRO_COLOR fill   = al_map_rgba(  0,255,  0, (unsigned char)alpha/3);
            ALLEGRO_COLOR border = al_map_rgba(  0,255,  0, (unsigned char)alpha);
            al_draw_filled_circle(Position.x, Position.y, r, fill);
            al_draw_circle        (Position.x, Position.y, r, border, ringWidth);
        }

        if(faceLeft){
            const float w = al_get_bitmap_width (bmp.get());
            const float h = al_get_bitmap_height(bmp.get());

            al_draw_scaled_rotated_bitmap(bmp.get(), Anchor.x * w, Anchor.y * h, Position.x, Position.y, -1.0f,  1.0f, Rotation, 0);
        }
    }
