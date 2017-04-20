#include "canvas.h"

Canvas::Canvas(QWidget*) {
  settings = Settings();
  image = QImage(800, 800, QImage::Format_ARGB32);
  setFocusPolicy(Qt::FocusPolicy::StrongFocus);

  image.fill(qRgb(255, 0, 0));
}

void Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);

  painter.drawImage(0, 0, image.scaled(image.width() * settings.scale,
                                       image.height() * settings.scale));
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

void Canvas::setMode(Mode _mode) { settings.mode = _mode; }

void Canvas::setDebug(bool _debug) { settings.debug = _debug; }

void Canvas::switchGrid() { settings.grid = !(settings.grid); }

void Canvas::setScale(qreal _scale) { settings.scale = _scale; }

void Canvas::setLineMode(LineType _line_type) {
  settings.line_type = _line_type;
}

void Canvas::setCircleMode(CircleType _circle_type) {
  settings.circle_type = _circle_type;
}

void Canvas::setFillSettings(bool _fill_random, QColor _fill_color,
                             FillType _fill_type) {
  settings.fill_random = _fill_random;
  settings.fill_color = _fill_color;
  settings.fill_type = _fill_type;
}

void Canvas::setGradientSettings(QColor _start_color, QColor _end_color) {
  settings.start_color = _start_color;
  settings.end_color = _end_color;
}

void Canvas::setMainColor(QColor _main_color) {
  settings.main_color = _main_color;
}

void Canvas::setCircleSteps(int _circle_steps) {
  settings.circle_steps = _circle_steps;
}

void Canvas::setDebugColor(QColor _debug_color) {
  settings.debug_color = _debug_color;
}

void Canvas::setShift(int _shift_x, int _shift_y) {
  settings.shift_x = _shift_x;
  settings.shift_y = _shift_y;
}

void Canvas::setRotate(qreal _rotation_angle, bool _rotate_inplace) {
  settings.rotate_angle = _rotation_angle;
  settings.rotate_inplace = _rotate_inplace;
}

void Canvas::setScale(qreal _scale_x, qreal _scale_y, bool _scale_inplace) {
  settings.scale_x = _scale_x;
  settings.scale_y = _scale_y;
  settings.scale_inplace = _scale_inplace;
}

void Canvas::setShear(qreal _shear_x, qreal _shear_y, bool _shear_inplace) {
  settings.shear_x = _shear_x;
  settings.shear_y = _shear_y;
  settings.shear_inplace = _shear_inplace;
}

void Canvas::keyPressEvent(QKeyEvent* e) {
  switch (e->key()) {
    case 'X':
      qDebug() << settings;
      break;
  }
}
