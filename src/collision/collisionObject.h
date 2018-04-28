// taken entirely from clothsim
#ifndef COLLISIONOBJECT
#define COLLISIONOBJECT


using namespace glm;

class CollisionObject {
public:
  virtual void render() = 0;
  /* Returns the corrected velocity of the particle/gridnode */
  virtual vec3 collide(vec3 position, vec3 next_position, vec3 velocity) = 0;
//  virtual void collide(GridNode *n, float delta_t) = 0;

private:
  double friction;
};

#endif /* COLLISIONOBJECT */
