#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

namespace HideAndSeekAndShoot
{

class World;

/**
 * A class representing the field of view of an entity looking at some direction.
 * The field of view can be visualised as many evenly-spaced lines going out of the entity
 * towards infinity, until blocked by an object or the map's border
 */
class FieldOfView : public sf::Drawable
{

  public:

    /**
     * Constructs a field of view from an origin point towards a target point.
     * 
     * @param[in] world
     *  Pointer to the world in which the field of view will function.
     *  Needed so that the field of view knows where the world's walls are located,
     *  so that it can be blocked by those walls.
     * @param[in] origin
     *  Origin point. The point from which the field of view is "viewed"
     * @param[in] targetDir
     *  Target direction vector. (Only specified direction, length will be ignored)
     *  This is the central direction of the field of view.
     */
    FieldOfView(
        World const* world,
        sf::Vector2f const origin = {0.f, 0.f},
        sf::Vector2f const targetDir = {1.f, 1.f}
    );

    /// Updates the field of view according to current data
    void Update();

    /// Getter and setter for field of view's origin. The point from where the field is "viewed"
    sf::Vector2f GetOrigin() const;
    void SetOrigin(sf::Vector2f const origin);

    /// Getter and setter for field of view's target direction. The central deirection of the field.
    sf::Vector2f GetTargetDirection() const;
    void SetTargetDirection(sf::Vector2f const targetDir);

  private: /* functions */

    /**
     * Draws the field of view to a render target,
     * which consists of drawing the lines representing the field of view.
     * 
     * @param[in] target
     *  Render target where the field (lines) will be drawn
     * @param[in] states
     *  Render states/mode for the drawing
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /**
     * Initializes the lines that visually represent the field of view, according to current angle and spacing
     * (WARNING: does not initialize line's positions. Only their count in the vertex array.
     *  To set their positions accordingly, use UpdateLines() method)
     */
    void InitLines();

    /// Updates the lines that visually represent the field of view, according to the current origin, target direction and angle
    void UpdateLines();

    /**
     * Finds the intersection end of a line going from some origin point towards some infinite end point.
     * The intersection end of the line is the first point on the line (starting from the origin,
     * going towards the infinite end point) that is an intersection with some object in the world.
     * 
     * @param[in] lineOrigin
     *  The origin point of the line
     * @param[in] lineInfiniteEnd
     *  The infinite end point of the line
     * 
     * @return the found intersection end of the line
     */
    sf::Vector2f FindIntersectionEndOfLine(sf::Vector2f lineOrigin, sf::Vector2f lineInfiniteEnd) const;

  private: /* variables */

    /// Pointer to the world in which the field of view functions
    World const* _world;

    /// The origin of the field. The field is viewed from this point
    sf::Vector2f _origin;

    /// The target direction of the field. It is looked from the origin towards this direction.
    sf::Vector2f _targetDir;

    /// Angle of the field of view, in radians
    float _angle;

    /// Angle between two consecutive lines from the evenly-spaced lines representing the field of view
    float _lineSpacing;

    /// The evenly-spaced lines visually representing the field of view
    sf::VertexArray _lines;
};

} // namespace HideAndSeekAndShoot