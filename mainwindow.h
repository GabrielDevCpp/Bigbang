#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMap>
#include <QPoint>
#include <QTimer>
#include <QVector>
#include <QVector2D>
#include <QtMath>

#include <iostream>

#include "particle.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mousePressRelease(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent* event);

public slots:
    void Update();

private:
    double Distance(QPoint p1, QPoint p2);

    double Angle(double d1, double d2, double d3);

    QPoint Mid(QPoint p1, QPoint p2);

    void CalculateGravityCenter();

    QPointF WindowCenter();

    void ShowUniverseInfo(QPainter* painter);
    void ShowBiggerParticlesInfo(QPainter* painter);
    void ShowFasterParticlesInfo(QPainter* painter);

    Ui::MainWindow *ui;

    QVector<Particle> planets;

    QTimer* timer;

    QVector<QPair<QPointF, QColor>> rastro;

    int bigbang = 1;

    bool showTrace = true;
    bool showDirection = false;
    bool showGravityCenter = true;

    QPointF gravityCenter;
};

#endif // MAINWINDOW_H
