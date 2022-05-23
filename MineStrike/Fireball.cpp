#include "Fireball.hpp"


void Fireball::BlowUp() {
    is_blowed_ = true;
}

bool Fireball::IsBlowed() {
    return is_blowed_;
}

void Fireball::BlowingUp(float particle_speed) {
    std::vector<glm::vec3> &vertices = model.GetVertices();
    std::vector<glm::vec3> &normals = model.GetNormals();

    for (size_t i = 0; i < vertices.size(); i += 3) {
        vertices[i] += normals[i] * particle_speed;
        vertices[i + 1] += normals[i] * particle_speed;
        vertices[i + 2] += normals[i] * particle_speed;
    }

    model.UpdateVAO(vertices, model.GetUvs(), normals);
}