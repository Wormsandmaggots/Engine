#ifndef ENGINE_FRUSTUM_H
#define ENGINE_FRUSTUM_H

#include "glm/vec3.hpp"
#include "Editor/Camera.h"
#include "Scene/Transform2.h"

class FrustumCulling {
public:
    struct Plane {
        // unit vector
        glm::vec3 normal = {0.f, 1.f, 0.f};

        // point on the plane
        glm::vec3 point = {0.f, 0.f, 0.f};

        Plane() = default;

        Plane(const glm::vec3& point, const glm::vec3& normal)
        {
            this->normal = glm::normalize(normal);
            this->point = point;
        }

        float getSignedDistanceToPlane(const glm::vec3& point) const {
            return glm::dot(normal, point - this->point);
        }
    };

    struct Frustum {
        Plane topFace;
        Plane bottomFace;

        Plane rightFace;
        Plane leftFace;

        Plane farFace;
        Plane nearFace;
    };

    struct Volume
    {
        virtual bool isOnFrustum(const Frustum& camFrustum,
                                 const Transform2& modelTransform) const = 0;
    };

    struct Sphere : public Volume
    {
        glm::vec3 center{ 0.f, 0.f, 0.f };
        float radius{ 0.f };

        Sphere() = default;

        Sphere(const glm::vec3& center, float radius)
                : center(center), radius(radius) {}

        bool isOnFrustum(const Frustum& camFrustum, const Transform2& transform) const final
        {
            //Get global scale is computed by doing the magnitude of
            //X, Y and Z model matrix's column.
            const glm::vec3 globalScale = transform.getGlobalScale();
            //Get our global center with process it with the global model matrix of our transform
            const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

            //To wrap correctly our shape, we need the maximum scale scalar.
            const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

            //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
            Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

            //Check Firstly the result that have the most chance
            //to faillure to avoid to call all functions.
            return (globalSphere.isOnOrForwardPlane(camFrustum.leftFace) &&
                    globalSphere.isOnOrForwardPlane(camFrustum.rightFace) &&
                    globalSphere.isOnOrForwardPlane(camFrustum.farFace) &&
                    globalSphere.isOnOrForwardPlane(camFrustum.nearFace) &&
                    globalSphere.isOnOrForwardPlane(camFrustum.topFace) &&
                    globalSphere.isOnOrForwardPlane(camFrustum.bottomFace));
        };

        bool isOnOrForwardPlane(const Plane& plane) const {
            // Compute the distance from the center of the sphere to the plane
            float dist = glm::dot(plane.normal, center - plane.point);

            // Check if the distance is less than or equal to the radius of the sphere
            return dist <= radius;
        }
        //in case of not working implement this:
        /*
        bool isOnOrForwardPlane(const Plane& plane) const
        {
            return plane.getSignedDistanceToPlane(center) > -radius;
        }

        float getSignedDistanceToPlane(const glm::vec3& point) const
        {
            return glm::dot(normal, point) - distance;
        }
        */
    };

    Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY,
                                    float zNear, float zFar)
    {
        Frustum     frustum;
        const float halfVSide = zFar * tanf(fovY * .5f);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = zFar * cam.Front;

        frustum.nearFace = Plane(cam.Position + zNear * cam.Front, cam.Front);
        frustum.farFace = Plane(cam.Position + frontMultFar, -cam.Front);
        frustum.rightFace = Plane(cam.Position,
                                  glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up));
        frustum.leftFace = Plane(cam.Position,
                                 glm::cross(cam.Up,frontMultFar + cam.Right * halfHSide));
        frustum.topFace = Plane(cam.Position,
                                glm::cross(cam.Right, frontMultFar - cam.Up * halfVSide));
        frustum.bottomFace = Plane(cam.Position,
                                   glm::cross(frontMultFar + cam.Up * halfVSide, cam.Right));

        return frustum;
    }

    //for AABB
    struct BoundingVolume
    {
        virtual bool isOnFrustum(const Frustum& camFrustum,
                                 const Transform2& modelTransform) const = 0;
    };

    struct AABB : public BoundingVolume
    {
        glm::vec3 center{ 0.f, 0.f, 0.f };
        glm::vec3 extents{ 0.f, 0.f, 0.f };

        AABB(const glm::vec3& min, const glm::vec3& max)
                : BoundingVolume{},
                  center{ (max + min) * 0.5f },
                  extents{ max.x - center.x, max.y - center.y, max.z - center.z }
        {}

        AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
                : BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
        {}

        bool isOnFrustum(const Frustum& camFrustum, const Transform2& transform) const final
        {
            //Get global scale thanks to our transform
            const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

            // Scaled orientation
            const glm::vec3 right = transform.getRight() * extents.x;
            const glm::vec3 up = transform.getUp() * extents.y;
            const glm::vec3 forward = transform.getForward() * extents.z;

            const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
                                std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
                                std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

            const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

            const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

            //We not need to divise scale because it's based on the half extention of the AABB
            const AABB globalAABB(globalCenter, newIi, newIj, newIk);

            return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.farFace));
        };

        bool isOnOrForwardPlane(const Plane& plane) const
        {
            // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
            const float r = extents.x * std::abs(plane.normal.x) +
                            extents.y * std::abs(plane.normal.y) + extents.z * std::abs(plane.normal.z);

            return -r <= plane.getSignedDistanceToPlane(center);
        }
    };
};
#endif