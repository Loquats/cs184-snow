// taken entirely from clothsim
#ifndef COLLISIONOBJECT
#define COLLISIONOBJECT
#include "../shader.h"

using namespace std;

class CollisionObject {
public:
  virtual void render(Shader* shader) = 0;
  virtual void collide() = 0;

private:
  double friction;
};

#endif /* COLLISIONOBJECT */
