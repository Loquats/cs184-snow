// taken entirely from clothsim
#ifndef COLLISIONOBJECT
#define COLLISIONOBJECT
#include "../shader.h"

using namespace glm;

class CollisionObject {
public:
  virtual void render(Shader* shader) = 0;
  virtual vec3 collide(vec3 position, vec3 velocity, float delta_t) = 0;
  virtual bool is_stationary() = 0;
  virtual void set_velocity(vec3 velocity) = 0;
  virtual void update_position(float delta_t) = 0;

private:
  double friction;
};

#endif /* COLLISIONOBJECT */
