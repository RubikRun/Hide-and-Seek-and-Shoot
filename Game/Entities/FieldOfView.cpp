#include "FieldOfView.h"

#include "../utils/geometryUtils.hpp"

namespace
{

float const ANGLE_DEFAULT = 2.f;

float const LINE_SPACING_DEFAULT = 0.02f;

/* TODO: this is a temporary solution for the lengths of the lines. Just to get everything else working for now.
Later lines should go from the origin to wherever they intersect with an object or map's border,
and that is how their length will be computed */
float const LINE_LENGTH = 2000.f;

} // namespace

namespace HideAndSeekAndShoot
{

FieldOfView::FieldOfView(
    sf::Vector2f const origin,
    sf::Vector2f const targetDir)
    : _origin(origin),
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
    // Make current vector the length of a desired line
    currVec = GeometryUtils::NormaliseVector(currVec) * LINE_LENGTH;
    for (int i = 0; i < _lines.getVertexCount() / 2; i++)
    {
        _lines[i * 2].position = _origin;
        _lines[i * 2 + 1].position = _origin + currVec;

        currVec = GeometryUtils::RotateVector(currVec, _lineSpacing);
    }
}

} // namespace HideAndSeekAndShoot