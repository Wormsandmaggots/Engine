#ifndef ENGINE_FRUSTUM_H
#define ENGINE_FRUSTUM_H

class Frustum
{
public:
    class Plane {
    public:
        glm::vec3 normal;
        float distance;

        Plane() : normal(0.f, 1.f, 0.f), distance(0.f) {}

        float getSignedDistanceToPlane(const glm::vec3& point) const {
            return glm::dot(normal, point) - distance;
        }
    };

    class AABB {
    public:
        AABB() = default;
        glm::vec3 center;
        float extents[3];

        AABB(const glm::vec3& center, float extentX, float extentY, float extentZ)
                : center(center) {
            extents[0] = extentX;
            extents[1] = extentY;
            extents[2] = extentZ;
        }

        bool isOnFrustum(const Frustum& camFrustum, const Transform2& transform) const {
            const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

            const glm::vec3 right = transform.getRight() * extents[0];
            const glm::vec3 up = transform.getUp() * extents[1];
            const glm::vec3 forward = transform.getForward() * extents[2];

            const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
                                std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
                                std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

            const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

            const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
                                std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

            const AABB globalAABB(globalCenter, newIi, newIj, newIk);

            return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
                    globalAABB.isOnOrForwardPlane(camFrustum.farFace));
        }

        bool isOnOrForwardPlane(const Plane& plane) const {
            const float r = extents[0] * std::abs(plane.normal.x) +
                            extents[1] * std::abs(plane.normal.y) + extents[2] * std::abs(plane.normal.z);

            return -r <= plane.getSignedDistanceToPlane(center);
        }
    };

    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;

    void update(const Camera& cam, float aspect, float fovY, float zNear, float zFar) {
        const float halfVSide = zFar * tanf(fovY * .5f);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = zFar * cam.Front;

        nearFace.normal = cam.Front;
        nearFace.distance = glm::dot(cam.Position + zNear * cam.Front, cam.Front);
        farFace.normal = -cam.Front;
        farFace.distance = glm::dot(cam.Position + frontMultFar, -cam.Front);
        rightFace.normal = glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up);
        rightFace.distance = glm::dot(cam.Position, rightFace.normal);
        leftFace.normal = glm::cross(cam.Up, frontMultFar + cam.Right * halfHSide);
        leftFace.distance = glm::dot(cam.Position, leftFace.normal);
        topFace.normal = glm::cross(cam.Right, frontMultFar - cam.Up * halfVSide);
        topFace.distance = glm::dot(cam.Position, topFace.normal);
        bottomFace.normal = glm::cross(frontMultFar + cam.Up * halfVSide, cam.Right);
        bottomFace.distance = glm::dot(cam.Position, bottomFace.normal);
    }
};


#endif