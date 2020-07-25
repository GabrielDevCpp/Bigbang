#ifndef PARTICLE_H
#define PARTICLE_H

#include <QColor>
#include <QPoint>
#include <QVector>
#include <QVector2D>
#include <QtMath>

#include <QLineF>

class Particle
{
public:
  Particle();

  void Move(QVector<Particle>& dir);

  bool operator==(const Particle& p);

  QColor color;

  QPointF position;
  QPointF direction;

  QVector2D velocity;
  QVector2D acceleration;
  double delta = 0.0;

  int id = 0;
  int size = 1;

  bool colision = false;

  QVector<QColor> colors;
};

#endif // PARTICLE_H
