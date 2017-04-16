#ifndef CANVAS_H
#define CANVAS_H

#include <QtWidgets>
#include "drawer.h"

class Canvas : public QWidget {
  Q_OBJECT
 public:
  explicit Canvas(QWidget* parent = 0);

 protected:
  // overloaded methods of QWidget
  //  virtual void keyPressEvent(QKeyEvent*) override;
  virtual void paintEvent(QPaintEvent*) override;
  //  virtual void mousePressEvent(QMouseEvent*) override;

 private:
  QImage image;
  //  QImage image_scaled;

  //  Drawer drawer;
  //  QVector<QPoint> points;
};

#endif  // CANVAS_H
