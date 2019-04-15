#include <catavento.hpp>

Catavento::Catavento() : x(0), y(0), angle(0), rotationSpeed(0.01),
                         paused(false), released(true)
  {}

void Catavento::rotate()
{
  if (!paused)
    angle -= rotationSpeed;
}

void Catavento::pauseRotation()
{
  if (released)
  {
    if (paused)
      paused = false;
    else
      paused = true;
  }
  released = false;
}

void Catavento::changeRotationSpeed(float increment)
{
  if (!paused)
    rotationSpeed += increment;
}

void Catavento::horizontalTranslation(float increment)
{
  x += increment;
}

void Catavento::verticalTranslation(float increment)
{
  y += increment;
}

float Catavento::getAngle()
{
  return angle;
}

glm::vec3 Catavento::getVec3Position()
{
  return glm::vec3(x, y, 0.0f);
}

void Catavento::buttonRelease()
{
  released = true;
}
