#ifndef ENGINE_FRUSTUMREADY_H
#define ENGINE_FRUSTUMREADY_H
#include "Scene/Scene2.h"
#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "AABB.hpp"
#include "AABBComponent.h"
#include "LineRenderer.h"


class FrustumReady {
public:

    void addAABBToEntitiesWithModel(Scene2* scene) {
        const std::vector<Entity*>& entities = scene->getSceneEntities();
        for (Entity* entity : entities) {
            Model* modelComponent = entity->getComponent<Model>();
            if (modelComponent != nullptr) {
                // Entity has a Model component, print its name to the console
                std::cout << "Entity with Model component: " << entity->getName() << std::endl;
                // Get all vertices of the model
                std::vector<glm::vec3> vertices = modelComponent->getVertices();
                // If the model has vertices, calculate the AABB
                if (!vertices.empty()) {
                    // Create an AABB for the model
                    CPM_GLM_AABB_NS::AABB modelAABB(vertices[0], vertices[0]);

                    // Extend the AABB to include all vertices of the model
                    for (const glm::vec3& vertex : vertices) {
                        modelAABB.extend(vertex);
                    }

                    // Create a new AABBComponent and set its AABB to the calculated AABB
                    AABBComponent* aabbComponent = new AABBComponent();
                    aabbComponent->setAABB(modelAABB);

                    // Add the AABBComponent to the entity
                    entity->addComponent(aabbComponent);
                }
            }
        }
    }

    void drawAABB(const CPM_GLM_AABB_NS::AABB& aabb, LineRenderer& lineRenderer) {
        //std::cout << "Drawing AABBs\n";
        glm::vec3 min = aabb.getMin();
        glm::vec3 max = aabb.getMax();

        // Generate the 8 corners of the AABB
        std::vector<glm::vec3> corners = {
                glm::vec3(min.x, min.y, min.z),
                glm::vec3(min.x, min.y, max.z),
                glm::vec3(min.x, max.y, min.z),
                glm::vec3(min.x, max.y, max.z),
                glm::vec3(max.x, min.y, min.z),
                glm::vec3(max.x, min.y, max.z),
                glm::vec3(max.x, max.y, min.z),
                glm::vec3(max.x, max.y, max.z),
        };

        // Generate the 12 edges of the AABB
        std::vector<std::pair<glm::vec3, glm::vec3>> edges = {
                {corners[0], corners[1]},
                {corners[0], corners[2]},
                {corners[0], corners[4]},
                {corners[3], corners[1]},
                {corners[3], corners[2]},
                {corners[3], corners[7]},
                {corners[5], corners[1]},
                {corners[5], corners[4]},
                {corners[5], corners[7]},
                {corners[6], corners[2]},
                {corners[6], corners[4]},
                {corners[6], corners[7]},
        };

        // Draw the edges
        for (const auto& edge : edges) {
            lineRenderer.drawLine(edge.first, edge.second);
        }
    }

    void drawAABBs(Scene2* scene, LineRenderer& lineRenderer) {
        const std::vector<Entity*>& entities = scene->getSceneEntities();
        for (Entity* entity : entities) {
            AABBComponent* aabbComponent = entity->getComponent<AABBComponent>();
            if (aabbComponent != nullptr) {
                drawAABB(aabbComponent->getAABB(), lineRenderer);
            }
        }
    }
};
#endif
