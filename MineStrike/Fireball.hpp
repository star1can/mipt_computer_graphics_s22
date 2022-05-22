#include <utility>

#include "GameObject.hpp"


class Fireball: public GameObject{
private:
    bool is_blowed_ = false;
public:
    Fireball(Model model, glm::vec3 pos, glm::vec3 dir, float scale = 1.0f) : GameObject(std::move(model), pos, dir, scale) {}

    void BlowUp();
    bool IsBlowed();
    void BlowingUp(float particle_speed);
};

