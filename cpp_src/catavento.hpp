#ifndef __CATAVENTO_HPP__
#define __CATAVENTO_HPP__

#include <glm/glm.hpp>

class Catavento
{
  private:
    float x, y, angle, rotationSpeed;
    bool paused, released;

  public:
    Catavento();
    void rotate();
    void pauseRotation();
    void changeRotationSpeed(float increment);
    void horizontalTranslation(float increment);
    void verticalTranslation(float increment);
    float getAngle();
    glm::vec3 getVec3Position();
    void buttonRelease();
};

#endif