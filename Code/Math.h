#pragma once

#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 position;
    glm::vec3 direction;
};

struct Triangle
{
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
};

struct IntersectionResult {
    float Distance;
    glm::vec3 HitLocation;
};

// Optimized ray-triangle intersection function
const float EPSILON = 0.000001f;
glm::vec3 edge1;
glm::vec3 edge2;
glm::vec3 pvec;
glm::vec3 tvec;
glm::vec3 qvec;


float RayIntersectTriangle(const Ray& ray, const Triangle& triangle, glm::vec3& hitPoint) {

    // Precompute the triangle edges
    edge1 = triangle.v2 - triangle.v1;
    edge2 = triangle.v3 - triangle.v1;

    // Begin calculating determinant and auxiliary values
    pvec = glm::cross(ray.direction, edge2);
    float det = glm::dot(edge1, pvec);

    // If determinant is near zero, ray lies in the plane of the triangle
    if (det > -EPSILON && det < EPSILON) {
        return std::numeric_limits<float>::max();  // No intersection
    }

    float invDet = 1.0f / det;

    // Calculate distance from Vert0 to ray origin
    tvec = ray.position - triangle.v1;

    // Calculate u parameter and test bounds
    float u = glm::dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f) {
        return std::numeric_limits<float>::max();  // No intersection
    }

    // Prepare to test v parameter
    qvec = glm::cross(tvec, edge1);

    // Calculate v parameter and test bounds
    float v = glm::dot(ray.direction, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f) {
        return std::numeric_limits<float>::max();  // No intersection
    }

    // Calculate t, the distance along the ray to the intersection
    float t = glm::dot(edge2, qvec) * invDet;

    if (t > EPSILON) {
        // Ray intersection, calculate hit location
        hitPoint = ray.position + ray.direction * t;
        return t;  // Return distance to intersection
    }

    // If t <= EPSILON, no valid intersection
    return std::numeric_limits<float>::max();  // No intersection
}

glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t)
{
    return a + t * (b - a);
}

bool IsNearMe2D(const glm::vec3& Subject, const glm::vec3& Me, float Range)
{
    return (Subject.x > Me.x - Range && Subject.x < Me.x + Range) &&
        (Subject.z > Me.z - Range && Subject.z < Me.z + Range);
}
