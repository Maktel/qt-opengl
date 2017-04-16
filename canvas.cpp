#include "canvas.h"

Canvas::Canvas(QWidget*) { image = QImage("../example.png"); }

void Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);

  painter.drawImage(0, 0, image);
}
