#pragma once

#include <GameObject.h>

struct Vertex;

class Cube : public GameObject
{
public:
  Cube();
  Cube(Vector3 position);

  void Initialize(Graphics *graphics);
  void Update(float dt);
  void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);
  void SetColor(float r, float g, float b, float a);
  bool& IsColored();
protected:
  //void *_vertexPointer;

  Vector3 *vertices;
  Vector4 *colours;

  bool isColored;
  int size;
  int offset;

  //Vertex *vertices;
  unsigned int* indices;

  void SetVertex(int index, float x, float y, float z, float r, float g, float b, float a);
};