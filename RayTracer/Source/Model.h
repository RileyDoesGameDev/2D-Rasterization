#pragma once
#include "Color.h"
#include <glm/glm.hpp>
#include <vector>
#include  <string>
#include "SceneObject.h"

using vertex_t = glm::vec3;
using vertices_t = std::vector<vertex_t>;

class Model : public SceneObject
{
public:
    Model() = default;

    Model(std::shared_ptr<Material> material) : SceneObject{material} {}
    Model(const Transform& transform, std::shared_ptr<Material> material) : SceneObject{transform, material}{}

    //Model(const Transfomr&)
    Model(const vertices_t& vertices, std::shared_ptr<Material> material) : m_vertices{ vertices }, SceneObject{ material } {}
  //  bool Load(const std::string& filename);

    void Update() override;
    
    bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

   // Model(const vertices_t& vertices, const color_t& color) : m_vertices{ vertices }, m_color{ color } {}

    void Draw(class Framebuffer& framebuffer, const glm::mat4& model, const class Camera& camera);
    bool Load(const std::string& filename);
    void SetColor(color_t color) { m_color = color; }
private:
    vertices_t m_vertices{0};
    vertices_t m_local_vertices{0};
    color_t m_color{ 0 };

    glm::vec3 m_center{ 0 };
    float m_radius{ 0 };

};