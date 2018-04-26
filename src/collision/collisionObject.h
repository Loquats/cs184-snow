// taken entirely from clothsim
#ifndef COLLISIONOBJECT
#define COLLISIONOBJECT


using namespace std;

class CollisionObject {
public:
  virtual void render() = 0;
  virtual void collide() = 0;

private:
  double friction;
};

#endif /* COLLISIONOBJECT */
