#include "FieldOfView.h"

#include "../World.h"

#include "../utils/geometryUtils.hpp"

namespace
{

float const ANGLE_DEFAULT = 2.f;

float const LINE_SPACING_DEFAULT = 0.02f;

/* Some upper limit of a line length.
    Needed for when we build lines from origin to "infinity" to check where they intersect with objects.
    This upper limit is the "infinity". It should be a line length that would never fit on the screen.
    Ideally, something slightly bigger than the screen's diagonal.
    But since we don't know the resolution, we give it some value that should be enough for all normal screens. */
float const INFINITE_LINE_LENGTH = 5000.f;

} // namespace

namespace HideAndSeekAndShoot
{

FieldOfView::FieldOfView(
    World const* world,
    sf::Vector2f const origin,
    sf::Vector2f const targetDir)
    : _world(world),
    _origin(origin),
    _targetDir(targetDir)
{
    _lineSpacing = LINE_SPACING_DEFAULT;
    _angle = ANGLE_DEFAULT;

    InitLines();
    UpdateLines();
}

void FieldOfView::Update()
{
    UpdateLines();
}

sf::Vector2f FieldOfView::GetOrigin() const
{
    return _origin;
}

void FieldOfView::SetOrigin(sf::Vector2f const origin)
{
    _origin = origin;
}

sf::Vector2f FieldOfView::GetTargetDirection() const
{
    return _targetDir;
}

void FieldOfView::SetTargetDirection(sf::Vector2f const targetDir)
{
    _targetDir = targetDir;
}

void FieldOfView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_lines, states);
}

void FieldOfView::InitLines()
{
    int linesCount = _angle / _lineSpacing + 1;
    _lines = sf::VertexArray(sf::Lines, linesCount * 2);

    for (int i = 0; i < _lines.getVertexCount(); i++)
    {
        _lines[i].color = sf::Color::Blue;
    }
}

void FieldOfView::UpdateLines()
{
    // Set current vector to leftmost vector of the field
    sf::Vector2f currVec = GeometryUtils::RotateVector(_targetDir, -_angle / 2.f);
    // Make current vector to be of "infinite" length
    currVec = GeometryUtils::NormaliseVector(currVec) * INFINITE_LINE_LENGTH;
    for (int i = 0; i < _lines.getVertexCount() / 2; i++)
    {
        _lines[i * 2].position = _origin;
        _lines[i * 2 + 1].position = FindIntersectionEndOfLine(_origin, _origin + currVec);

        currVec = GeometryUtils::RotateVector(currVec, _lineSpacing);
    }
}

sf::Vector2f FieldOfView::FindIntersectionEndOfLine(sf::Vector2f lineOrigin, sf::Vector2f lineInfiniteEnd) const
{
    sf::Vector2f intersectionEnd = lineInfiniteEnd;

    // Traverse all walls in the world
    for (sf::ConvexShape const& wall : _world->GetWalls())
    {
        // Traverse lines of the wall
        int pointCount = wall.getPointCount();
        for (int i = 0; i < pointCount; i++)
        {
            // Get point A and B, where AB is the current line of the wall
            sf::Vector2f A = wall.getPoint(i);
            sf::Vector2f B = wall.getPoint(
                (i + 1) % pointCount
            );

            if (!GeometryUtils::SegmentsIntersect(A, B, lineOrigin, lineInfiniteEnd))
            {
                continue;
            }
            sf::Vector2f currIntersection = GeometryUtils::FindSegmentsIntersection(
                A, B, lineOrigin, lineInfiniteEnd
            );
            if ((currIntersection.x == lineOrigin.x
                && fabs(currIntersection.y - lineOrigin.y) < fabs(intersectionEnd.y - lineOrigin.y))
                || fabs(currIntersection.x - lineOrigin.x) < fabs(intersectionEnd.x - lineOrigin.x))
            {
                intersectionEnd = currIntersection;
            }
        }
    }

    return intersectionEnd;
}

} // namespace HideAndSeekAndShoot