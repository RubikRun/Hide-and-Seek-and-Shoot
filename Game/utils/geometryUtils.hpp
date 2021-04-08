#pragma once

/* Some geometry helper functions */

#include <SFML/Graphics.hpp>

#include <cmath>

namespace
{

namespace GeometryUtils
{

/**
 * Calculates and returns vector's length
 * 
 * @param[in] v
 *  Vector whose length we want to calculate
 * 
 * @return calculated length of the vector
 */
float GetVectorLength(
    sf::Vector2f const& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

/**
 * Calculates the vector from one point to another
 * 
 * @param[in] pointA
 *  vector's begin point
 * @param[in] pointB
 *  vector's end point
 * 
 * @return vector from point A to point B
 */
sf::Vector2f GetVector(
    sf::Vector2f pointA,
    sf::Vector2f pointB)
{
    return pointB - pointA;
}

/**
 * Normalises the given vector,
 * meaning that it returns a vector
 * pointing in the same direction as the given vector,
 * but with length = 1.
 * 
 * @param[in] v
 *  Vector that we want to normalise
 * 
 * @return normalised vector with length = 1
 */
sf::Vector2f NormaliseVector(
    sf::Vector2f const& v)
{
    return v / GetVectorLength(v);
}

/**
 * Calculates the distance between two given points
 * 
 * @return pointA
 *  First point
 * @return pointB
 *  Second point
 * 
 * @return distance between the two points
 */
float CalcDist(
    sf::Vector2f const& pointA,
    sf::Vector2f const& pointB)
{
    return GetVectorLength(GetVector(pointA, pointB));
}

/**
 * Returns the orientation of two vectors.
 * When first one is on the right of the second one,
 * the function returns 1.
 * If it's on the left, the function returns -1.
 * If the two vectors are collinear, 0 is returned
 * 
 * @param[in] v1
 *  First vector
 * @param[in] v2
 *  Second vector
 * 
 * @return 1, -1 or 0 depending on the orientation of the vectors
 */
int GetOrientation(
    sf::Vector2f const& v1,
    sf::Vector2f const& v2)
{
    int det = v1.x * v2.y - v1.y * v2.x;

    if (det < 0) return -1;
    if (det > 0) return 1;
    return 0;
}

/**
 * Checks if a line intersects a line segment.
 * The infinite line is defined by two point A and B lying on it
 * and the finite line segment is defined by its two end points C and D.
 * 
 * @param[in] A
 *  A point lying on the line
 * @param[in] B
 *  Another point lying on the line
 * @param[in] C
 *  End point of the line segment
 * @param[in] D
 *  The other endpoint of the line segment
 * 
 * @return true when the line intersects the segment, false otherwise
 */
bool LineIntersectsSegment(
    sf::Vector2f const& A,
    sf::Vector2f const& B,
    sf::Vector2f const& C,
    sf::Vector2f const& D)
{
    int o1 = GetOrientation(GetVector(A, B), GetVector(B, C));
    if (o1 == 0) return true;
    int o2 = GetOrientation(GetVector(A, B), GetVector(B, D));
    if (o2 == 0) return true;

    return (o1 != o2);
}

/**
 * Checks if two line segments intersect.
 * The two line segments are defined by their endpoints A, B and C, D.
 * 
 * @param[in] A
 *  First endpoint of the first segment
 * @param[in] B
 *  Second endpoint of the first segment
 * @param[in] C
 *  First endpoint of the second segment
 * @param[in] D
 *  Second endpoint of the second segment
 * 
 * @return true when the line segments intersect, false otherwise
 */
bool SegmentsIntersect(
    sf::Vector2f const& A,
    sf::Vector2f const& B,
    sf::Vector2f const& C,
    sf::Vector2f const& D)
{
    return LineIntersectsSegment(A, B, C, D) && LineIntersectsSegment(C, D, A, B);
}

} // namespace Geometry

} // namespace