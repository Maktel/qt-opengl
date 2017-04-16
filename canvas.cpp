#include "canvas.h"

Canvas::Canvas(QWidget*) {
  image = QImage(800, 800, QImage::Format_ARGB32);
  image.fill(qRgb(255, 0, 0));
}

void Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);

  painter.drawImage(0, 0, image);
}

void Canvas::loadFile(QString file_name) {
  // TODO check permissions to read
  image = QImage(file_name);
  update();
}

void Canvas::saveFile(QString file_name) {
  // TODO check permissions to read
  // 0 = try to guess extension; 100 = best quality
  bool successful = image.save(file_name, 0, -1);
  if (!successful) qWarning() << "Image has not been saved successfully";
}
