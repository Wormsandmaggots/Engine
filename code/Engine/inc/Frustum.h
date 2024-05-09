#ifndef ENGINE_FRUSTUM_H
#define ENGINE_FRUSTUM_H

#include "glm/vec3.hpp"
#include "Editor/Camera.h"


struct Plane
{
    glm::vec3 normal = { 0.f, 1.f, 0.f };

    float     distance = 0.f;

    float getSignedDistanceToPlane(const glm::vec3& point) const
    {
        // The signed distance from a point to a plane is (P . N) - D
        // where P is the point, N is the plane normal, and D is the plane distance
        return glm::dot(point, normal) - distance;
    }
};

struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
};

class FrustumView {
public:
    Frustum createFrustumFromCamera(const Camera &cam, float aspect, float fovY,
                                    float zNear, float zFar) {
        Frustum frustum;
        const float halfVSide = zFar * tanf(fovY * .5f);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = zFar * cam.Front;

        frustum.nearFace.normal = cam.Position + zNear * cam.Front;
        frustum.nearFace.distance = glm::dot(cam.Position + zNear * cam.Front, cam.Front);
        frustum.farFace.normal = cam.Position + frontMultFar;
        frustum.farFace.distance = glm::dot(cam.Position + frontMultFar, -cam.Front);
        frustum.rightFace.normal = cam.Position;
        frustum.rightFace.distance = glm::dot(cam.Position, glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up));
        frustum.leftFace.normal = cam.Position;
        frustum.leftFace.distance = glm::dot(cam.Position, glm::cross(cam.Up, frontMultFar + cam.Right * halfHSide));
        frustum.topFace.normal = cam.Position;
        frustum.topFace.distance = glm::dot(cam.Position, glm::cross(cam.Right, frontMultFar - cam.Up * halfVSide));
        frustum.bottomFace.normal = cam.Position;
        frustum.bottomFace.distance = glm::dot(cam.Position, glm::cross(frontMultFar + cam.Up * halfVSide, cam.Right));

        return frustum;
    }



};

#endif