#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMapIterator>
#include <QPainter>
#include <QPaintEvent>
#include <QLine>
#include <QtMath>

#include <math.h>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();

    Particle p0;
    p0.id = 0;
    p0.color = Qt::white;
    p0.position = QPoint(800, 600);
    planets.append(p0);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect rect = event->rect();

    painter.fillRect(rect, Qt::black);

    if (showTrace) {
      for (int i = 0; i < rastro.size(); i++) {
          painter.setPen(rastro[i].second);
          painter.drawPoint(rastro[i].first);
      }
    }

    if (bigbang < 50) {
      Particle& p = planets[0];
      painter.setPen(p.color);
      painter.setBrush(p.color);
      painter.drawEllipse(p.position, bigbang, bigbang);
      bigbang++;
    }
    else if (bigbang < 60) {
      bigbang++;
      if (bigbang == 60) {
        for (int i = 1; i < planets.size(); i++) {
          Particle& p = planets[i];
          p.colision = true;
        }
      }
    }

    for (int i = 1; i < planets.size(); i++) {
      Particle& p = planets[i];
      painter.setPen(p.color);
      if (p.size > 1) painter.setBrush(p.color);
      painter.drawEllipse(p.position, p.size+1, p.size+1);
      p.Move(planets);

      if (p.size > 1) rastro.append(QPair<QPointF, QColor>(p.position, p.color));
    }

    if (showDirection) {
      for (int i = 1; i < planets.size(); i++) {
        Particle& p = planets[i];
        painter.setPen(Qt::white);
        painter.drawLine(p.position, p.direction);
      }
    }

    ShowUniverseInfo(&painter);
    ShowBiggerParticlesInfo(&painter);
    ShowFasterParticlesInfo(&painter);

    if (showGravityCenter) {
      CalculateGravityCenter();
      painter.setPen(Qt::white);
      painter.drawPoint(gravityCenter);
    }
}

void MainWindow::ShowUniverseInfo(QPainter* painter)
{
    int mass = 0;
    for (int i = 1; i < planets.size(); i++) {
      mass += planets[i].size;
    }
    int textpos = 10;
    painter->setPen(Qt::white);
    painter->drawText(10, textpos+=10, QString("Particulas: %1").arg(QString::number(planets.size())));
    painter->drawText(10, textpos+=10, QString("Massa     : %1").arg(QString::number(mass)));
    for (int i = 1; i < planets.size(); i++) {
      Particle& p = planets[i];

      painter->setBrush(p.color);
      painter->drawRect(10, textpos, 10, 10);

      painter->setPen(p.size > 1 ? Qt::green : Qt::white);
      QString text = QString("%1: size %2 - v %3").arg(p.id).arg(p.size).arg((int)p.delta);
      painter->drawText(21, textpos+=10, text);

    }
}

void MainWindow::ShowBiggerParticlesInfo(QPainter *painter)
{
    int textpos = 10;

    for (int i = 1; i < planets.size(); i++) {
      Particle& p = planets[i];
      if (p.size > 1) {
        painter->setPen(Qt::white);
        if (p.size > 3) painter->setPen(Qt::yellow);
        if (p.size > 5) painter->setPen(Qt::green);
        if (p.size > 10) painter->setPen(Qt::red);
        QString text = QString("%1: size %2 - v %3").arg(p.id).arg(p.size).arg((int)p.delta);
        painter->drawText(1200, textpos+=10, text);
      }
    }
}

void MainWindow::ShowFasterParticlesInfo(QPainter *painter)
{
    float speed = 0;

    for (int i = 1; i < planets.size(); i++) {
      Particle& p = planets[i];
      speed += p.delta;
    }

    speed /= (float)(planets.size()/1.5);

    QMap<int, int> speedParticle;

    for (int i = 1; i < planets.size(); i++) {
      Particle& p = planets[i];
      if (p.delta > speed)
        speedParticle[p.id] = p.delta;
    }

    int textpos = 10;
    painter->setPen(Qt::white);
    QMapIterator<int, int> it(speedParticle);
    while (it.hasNext()) {
        it.next();
        QString text = QString("%1: v %2").arg(it.key()).arg(it.value());
        painter->drawText(1300, textpos+=10, text);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  Particle p;
  p.color = QColor(Qt::red);
  p.id = planets.size()+1;
  p.position = QPoint(event->x(), event->y());
  planets.append(p);

  update();
}

void MainWindow::mousePressRelease(QMouseEvent *event)
{
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_C) {
    rastro.clear();
  }
  else if (event->key() == Qt::Key_D) {
    showDirection = !showDirection;
  }
  else if (event->key() == Qt::Key_T) {
    showTrace = !showTrace;
  }
  else if (event->key() == Qt::Key_F) {
    QPointF windowCenter = WindowCenter();

    double x = gravityCenter.x() - windowCenter.x();
    double y = gravityCenter.y() - windowCenter.y();
    for (Particle& p : planets) {
      if (p.id > 0) {
        p.position.setX(p.position.x()-x);
        p.position.setY(p.position.y()-y);
      }
    }
    rastro.clear();
  }
  else if (event->key() == Qt::Key_Up) {
    for (Particle& p : planets) {
      if (p.id > 0)
        p.position.setY(p.position.y()-10);
    }
    rastro.clear();
  }
  else if (event->key() == Qt::Key_Down) {
    for (Particle& p : planets) {
      if (p.id > 0)
        p.position.setY(p.position.y()+10);
    }
    rastro.clear();
  }
  else if (event->key() == Qt::Key_Left) {
    for (Particle& p : planets) {
      if (p.id > 0)
        p.position.setX(p.position.x()-10);
    }
    rastro.clear();
  }
  else if (event->key() == Qt::Key_Right) {
    for (Particle& p : planets) {
      if (p.id > 0)
        p.position.setX(p.position.x()+10);
    }
    rastro.clear();
  }
}

void MainWindow::Update()
{
  if (bigbang == 50) {
    QPointF point = planets.first().position;
    for (int i = 1; i < 100; i++) {
      Particle p;
      p.color = QColor(qrand()%255, qrand()%255, qrand()%255);
      p.id = planets.size()+1;
      p.position = point;
      p.acceleration = QVector2D(float(qrand()%20-10)/10.0f, float(qrand()%20-10)/10.0f);
      p.velocity = p.acceleration*50;
      planets.append(p);
    }
    bigbang++;
  }
  update();
}

double MainWindow::Distance(QPoint p1, QPoint p2)
{
    return qSqrt(
                qPow(p1.x()-p2.x(), 2) +
                qPow(p1.y()-p2.y(), 2)
          );
}

double MainWindow::Angle(double d1, double d2, double d3)
{
  double angle = (d1*d1 + d2*d2 - d3*d3)/(2*d1*d2);
  angle = acos(angle)*180/M_PI;

  return angle;
}

QPoint MainWindow::Mid(QPoint p1, QPoint p2)
{
    double x = (p1.x() + p2.x())/2.0;
    double y = (p1.y() + p2.y())/2.0;
    return QPoint((int)x, (int)y);
}

void MainWindow::CalculateGravityCenter()
{
  gravityCenter = QPointF(0.0, 0.0);
  for (Particle p : planets) {
    gravityCenter += p.position;
  }
  gravityCenter/=planets.size();
}

QPointF MainWindow::WindowCenter()
{
  double x = geometry().width()/2;
  double y = geometry().height()/2;
  return QPointF(x, y);
}
