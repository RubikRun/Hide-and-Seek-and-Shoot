#include "Person.h"

namespace HideAndSeekAndShoot
{

/**
 * Player class for the player's entity.
 * The player is a person, so class Player inherits from class Person,
 * with the added functionality that the player objects can be moved from outside.
 */
class Player : public Person
{

  public:

    // using base class Person's constructor
    using Person::Person;

    // making MoveInDirection method public
    using Person::MoveInDirection;
    // making SetTargetPoint method public
    using Person::SetTargetPoint;
};

} // namespace HideAndSeekAndShoot