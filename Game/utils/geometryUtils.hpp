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
    sf::Vector2f const v)
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
    sf::Vector2f const v)
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
    sf::Vector2f const pointA,
    sf::Vector2f const pointB)
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
    sf::Vector2f const v1,
    sf::Vector2f const v2)
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
    sf::Vector2f const A,
    sf::Vector2f const B,
    sf::Vector2f const C,
    sf::Vector2f const D)
{
    int o1 = GetOrientation(GetVector(A, B), GetVector(B, C));
    if (o1 == 0) return true;
    int o2 = GetOrientation(GetVector(A, B), GetVector(B, D));
    if (o2 == 0) return true;

    return (o1 != o2);
}

/**
 * Checks if two line segments intersect.
 * The two line segments are defined by their end points A, B and C, D.
 * 
 * @param[in] A
 *  First end point of the first segment
 * @param[in] B
 *  Second end point of the first segment
 * @param[in] C
 *  First end point of the second segment
 * @param[in] D
 *  Second end point of the second segment
 * 
 * @return true when the line segments intersect, false otherwise
 */
bool SegmentsIntersect(
    sf::Vector2f const A,
    sf::Vector2f const B,
    sf::Vector2f const C,
    sf::Vector2f const D)
{
    return LineIntersectsSegment(A, B, C, D) && LineIntersectsSegment(C, D, A, B);
}

/**
 * Finds the orthogonal projection of a point P onto a line.
 * The line is defined by two points A and B lying on it.
 * Orthogonal means perpendicular.
 * If the found point is X, then PX will be perpendicular to the line
 * 
 * @param[in] P
 *  The point that will be orthogonally projected on the line AB
 * @param[in] A
 *  Some point on the line
 * @param[in] B
 *  Another point on the line
 * 
 * @return orthogonal projection of the given point
 */
sf::Vector2f FindOrthogonalProjection(
    sf::Vector2f const P,
    sf::Vector2f const A,
    sf::Vector2f const B)
{
    float deltaX = B.x - A.x;
    float deltaY = B.y - A.y;

    float r = (deltaX * (P.x - A.x) + deltaY * (P.y - A.y))
        / (deltaX * deltaX + deltaY * deltaY);

    return {
        A.x + r * deltaX,
        A.y + r * deltaY
    };
}

/**
 * Finds the point on a line segment that is closest to a given point.
 * 
 * @param[in] A
 *  First end point of the line segment
 * @param[in] B
 *  Second end point of the line segment
 * @param[in] P
 *  The point to which the found point should be closest
 */
sf::Vector2f FindClosestPointOnSegment(
    sf::Vector2f const A,
    sf::Vector2f const B,
    sf::Vector2f const P)
{
    if (A.x == B.x)
    {
        sf::Vector2f const* U = &A;
        sf::Vector2f const* D = &B;

        if (A.y > B.y)
        {
            U = &B;
            D = &A;
        }

        if (P.y > U->y && P.y < D->y)
        {
            return {A.x, P.y};
        }
        
        if (abs(A.y - P.y) < abs(B.y - P.y))
        {
            return A;
        }
        return B;
    }

    sf::Vector2f const Q = FindOrthogonalProjection(P, A, B);

    sf::Vector2f const* L = &A;
    sf::Vector2f const* R = &B;

    if (A.x > B.x)
    {
        L = &B;
        R = &A;
    }

    if (Q.x < L->x)
    {
        return *L;
    }
    if (Q.x > R->x)
    {
        return *R;
    }
    return Q;
}

/**
 * Checks if a line segment intersects a circle.
 * The segment is given by its two end points
 * and the circle is given by its center and radius
 * 
 * @param[in] A
 *  First end point of the line segment
 * @param[in] B
 *  Second end point of the line segment
 * @param[in] center
 *  Center point of the circle
 * @param[in] radius
 *  Radius of the circle
 * 
 * @return true if segment intersects circle, false otherwise
 */
bool SegmentIntersectsCircle(
    sf::Vector2f const A,
    sf::Vector2f const B,
    sf::Vector2f const center,
    float const radius)
{
    sf::Vector2f const Q = FindClosestPointOnSegment(A, B, center);

    return (radius * radius >= (Q.x - center.x) * (Q.x - center.x) + (Q.y - center.y) * (Q.y - center.y));
}

/**
 * Rotates the vector by some angle
 * 
 * @param[in] vec
 *  Vectore to rotate
 * @param[in] deltaAngle
 *  Angle by which the vector will be rotated, in radians
 * 
 * @return resulting vector after rotation
 */
sf::Vector2f RotateVector(sf::Vector2f const vec, float deltaAngle)
{
    float cosDelta = cos(deltaAngle), sinDelta = sin(deltaAngle);
    float const &cosAngle = vec.x, &sinAngle = vec.y;

    return sf::Vector2f(
        cosAngle * cosDelta - sinAngle * sinDelta,
        sinAngle * cosDelta + cosAngle * sinDelta
    );
}

} // namespace Geometry

} // namespace