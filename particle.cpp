#include "particle.h"

#include <iostream>
using namespace std;

Particle::Particle()
{
  colors.append(Qt::white);
  colors.append(Qt::red);
  colors.append(Qt::yellow);
  colors.append(Qt::blue);
  colors.append(Qt::green);
  colors.append(Qt::cyan);
  colors.append(Qt::magenta);
  colors.append(Qt::darkGreen);
  colors.append(Qt::darkBlue);
  colors.append(Qt::darkCyan);
  colors.append(Qt::darkMagenta);
}

void Particle::Move(QVector<Particle>& dir)
{
  direction = position;
  QPointF a(position);

  QVector2D acc;

  for (Particle& g : dir) {
    if (g.id == id) continue;

    QPointF b(g.position);
    QPointF vector(b.x() - a.x(), b.y() - a.y());
    double length = qSqrt(vector.x() * vector.x() + vector.y() * vector.y());
    if (length > 0.9*max(size, g.size)) {
      QPointF unitVector(vector.x() / length, vector.y() / length);
      acc += QVector2D(unitVector)*size;
    }
    else {
#if 1
      if (colision == false) continue;
      if (g.id == 0) continue;
      //double scalar = qSqrt(velocity.x() * velocity.x() + velocity.y() * velocity.y());
      //if (scalar > 7) continue;

      QVector2D resultante;
      if (g.size > size)
        resultante = (g.velocity*g.size + velocity) / (g.size*2.0);
      else
        resultante = (g.velocity + velocity*size) / (size*2.0);

      if (g.size < size) {
        g.position = position;
        g.color = color;
      }

      g.velocity = resultante;
      g.size = max(g.size,size)+1;

      dir.removeOne(*this);
      return;
#endif
    }
  }

  double length = qSqrt(acc.x() * acc.x() + acc.y() * acc.y());
  if (length > 0.9) {
    QPointF unitVector(acc.x() / length, acc.y() / length);
    acceleration = QVector2D(unitVector);
  }

  double scalar = qSqrt(velocity.x() * velocity.x() + velocity.y() * velocity.y());
  //cout << "vel " << scalar << endl;
  double slow = scalar < delta ? 2.0 : 1.0;
  delta = scalar;
  velocity += slow*acceleration*0.1;
  position = QPointF(a.x() + velocity.x(), a.y() + velocity.y());
}

bool Particle::operator==(const Particle &p)
{
  return id == p.id;
}
