#include "canvas.h"

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
  settings = Settings();
  image = QImage(settings.width, settings.height, QImage::Format_ARGB32);

  setFocusPolicy(Qt::FocusPolicy::StrongFocus);
  resize(settings.width, settings.height);

  drawer = Drawer(&image, settings.main_color.rgba());
  drawer.initialize(settings.line_type, settings.circle_type,
                    settings.circle_steps, settings.fill_type,
                    settings.interpolation_type);

  // TODO support settings.clear_color
  clear(QColor(130, 51, 214, 255));
}

void Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);

  painter.drawImage(0, 0, image.scaled(image.width() * settings.zoom,
                                       image.height() * settings.zoom));
}

void Canvas::mousePressEvent(QMouseEvent* e) {
  if (e->button() == Qt::LeftButton) {
    processMousePress(e->pos().x() / settings.zoom,
                      e->pos().y() / settings.zoom);
    qDebug().nospace() << "Recorded mouse click: ("
                       << e->pos().x() / settings.zoom << ", "
                       << e->pos().y() / settings.zoom << ")";
  }
}

void Canvas::mouseMoveEvent(QMouseEvent* e) {
  if (settings.mode == Mode::click && e->buttons() == Qt::LeftButton) {
    processMousePress(e->pos().x() / settings.zoom,
                      e->pos().y() / settings.zoom);
  }
}

void Canvas::redraw() {
  update();
  // TODO make grid drawn on widget placed over canvas
  if (settings.grid) {
    for (int y = 0; y < settings.height; ++y) {
      for (int x = 0; x < settings.width; ++x) {
        if (!(x % 100) || !(y % 100))
          image.setPixel(x, y, settings.debug_color.rgba());
      }
    }
  }
}

// accepts converted click coordinates (not raw data)
void Canvas::processMousePress(int pos_x, int pos_y) {
  switch (settings.mode) {
    case Mode::click: {
      drawer.drawPoint(pos_x, pos_y);
    } break;

    case Mode::line: {
      if (points.size() < 2) {
        points.push_back(QPoint(pos_x, pos_y));
      }
      if (points.size() == 2) {
        drawer.drawLine(points[0], points[1]);
        setDefaultMode();
      }
    } break;

    case Mode::circle: {
      if (points.size() < 2) {
        points.push_back(QPoint(pos_x, pos_y));
      }
      if (points.size() == 2) {
        drawer.drawCircle(points[0], points[1]);
        setDefaultMode();
      }
    } break;

    case Mode::bezier: {
      if (points.size() < 4) {
        points.push_back(QPoint(pos_x, pos_y));
      }
      if (points.size() == 4) {
        drawer.drawBezierCurve(points[0], points[1], points[2], points[3]);
        setDefaultMode();
      }
    } break;

    case Mode::polygon: {
      points.push_back(QPoint(pos_x, pos_y));
      qDebug() << "Current number of points:" << points.size();
      if (points.size() >= 2) emit changedSetupMode(true);
    } break;

    case Mode::spline: {
      points.push_back(QPoint(pos_x, pos_y));
      qDebug() << "Current number of points:" << points.size();
      if (points.size() >= 4) emit changedSetupMode(true);
    } break;

    case Mode::fill: {
      if (settings.fill_random) {
        settings.fill_color =
            QColor(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
      }
      drawer.fill(QPoint(pos_x, pos_y), settings.fill_color.rgba());
    } break;

    case Mode::none:
    default: { } break; }

  redraw();
}

void Canvas::setMode(Mode _mode) {
  qDebug() << "Current mode:" << int(_mode);
  settings.mode = _mode;
  points.clear();

  emit changedSetupMode(false);
}

void Canvas::setDefaultMode() {
  qDebug() << "Resetting mode";
  setMode(Mode::none);
  emit changedToDefaultMode();  // updates menu labels in main window
  redraw();  // update widget, since it means some action has been completed
}

void Canvas::applyOperation(Operation operation) {
  qDebug() << "Applying operation:" << int(operation);
  switch (operation) {
    case Operation::hgradient: {
      drawer.paintHorizontalGradient(settings.start_color.rgba(),
                                     settings.end_color.rgba(),
                                     settings.gradient_steps);
    } break;

    case Operation::vgradient: {
      drawer.paintVerticalGradient(settings.start_color.rgba(),
                                   settings.end_color.rgba(),
                                   settings.gradient_steps);
    } break;

    case Operation::shift: {
      drawer.transform(
          drawer.createShiftMatrix(settings.shift_x, settings.shift_y));
    } break;

    case Operation::rotate: {
      drawer.transform(drawer.createRotateMatrix(settings.rotate_angle,
                                                 settings.rotate_inplace));
    } break;

    case Operation::scale: {
      drawer.transform(drawer.createScaleMatrix(
          settings.scale_x, settings.scale_y, settings.scale_inplace));
    } break;

    case Operation::shear: {
      drawer.transform(drawer.createShearMatrix(
          settings.shear_x, settings.shear_y, settings.shear_inplace));
    } break;
  }

  redraw();
}

void Canvas::applySetupDraw() {
  switch (settings.mode) {
    case Mode::polygon: {
      drawer.drawPolygon(points);
      setDefaultMode();
    } break;

    case Mode::spline: {
      drawer.drawBasisSpline(points);
      setDefaultMode();
    } break;

    default: { qDebug() << "No action to be taken"; } break;
  }
}

void Canvas::loadFile(QString file_name) {
  // TODO check permissions to read
  image = QImage(file_name);

  drawer = Drawer(&image, settings.main_color.rgba());
  drawer.initialize(settings.line_type, settings.circle_type,
                    settings.circle_steps, settings.fill_type,
                    settings.interpolation_type);

  update();
}

void Canvas::saveFile(QString file_name) {
  // TODO check permissions to read
  // 0 = try to guess extension; 100 = best quality
  bool successful = image.save(file_name, 0, -1);
  if (!successful) qWarning() << "Image has not been saved successfully";
}

void Canvas::setDebug(bool _debug) {
  settings.debug = _debug;
  drawer.setDebug(_debug, settings.debug_color.rgba());
}

void Canvas::switchGrid() { settings.grid = !(settings.grid); }

void Canvas::setZoom(qreal _zoom) {
  settings.zoom = _zoom;
  resize(800 * _zoom, 800 * _zoom);
}

void Canvas::setLineType(LineType _line_type) {
  settings.line_type = _line_type;
  drawer.setLineType(_line_type);
}

void Canvas::setCircleType(CircleType _circle_type) {
  settings.circle_type = _circle_type;
  drawer.setCircleType(_circle_type);
}

void Canvas::setFillSettings(bool _fill_random, QColor _fill_color,
                             FillType _fill_type) {
  settings.fill_random = _fill_random;
  settings.fill_color = _fill_color;
  settings.fill_type = _fill_type;
  drawer.setFillType(_fill_type);
}

void Canvas::setGradientSettings(QColor _start_color, QColor _end_color,
                                 int _gradient_steps) {
  settings.start_color = _start_color;
  settings.end_color = _end_color;
  settings.gradient_steps = _gradient_steps;
}

void Canvas::setMainColor(QColor _main_color) {
  settings.main_color = _main_color;
  drawer.setMainColor(_main_color.rgba());
}

void Canvas::setCircleSteps(int _circle_steps) {
  settings.circle_steps = _circle_steps;
  drawer.setCircleSteps(_circle_steps);
}

void Canvas::setDebugColor(QColor _debug_color) {
  settings.debug_color = _debug_color;
  drawer.setDebug(settings.debug, _debug_color.rgba());
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

void Canvas::setInterpolationType(InterpolationType _interpolation_type) {
  settings.interpolation_type = _interpolation_type;
  drawer.setInterpolationType(settings.interpolation_type);
}

void Canvas::clear(QColor color) {
  image.fill(color);
  redraw();
}

void Canvas::keyPressEvent(QKeyEvent* e) {
  switch (e->key()) {
    case 'X':
      qDebug() << settings;
      break;
    case Qt::Key_Space:
      applySetupDraw();
      break;

    // tests
    case Qt::Key_1: {
      clock_t st = clock();

      example1();

      clock_t fi = clock();
      qDebug() << "example1 run time:"
               << QString().setNum((fi - st) * 1.0 / CLOCKS_PER_SEC, 'f', 10);
      redraw();
    } break;

    case Qt::Key_2: {
      clock_t st = clock();

      example2();

      clock_t fi = clock();
      qDebug() << "example2 run time:"
               << QString().setNum((fi - st) * 1.0 / CLOCKS_PER_SEC, 'f', 10);
      redraw();
    } break;

    case Qt::Key_3: {
      clock_t st = clock();

      example3();

      clock_t fi = clock();
      qDebug() << "example3 run time:"
               << QString().setNum((fi - st) * 1.0 / CLOCKS_PER_SEC, 'f', 10);
      redraw();
    } break;

    case Qt::Key_4: {
      clock_t st = clock();

      example4();

      clock_t fi = clock();
      qDebug() << "example4 run time:"
               << QString().setNum((fi - st) * 1.0 / CLOCKS_PER_SEC, 'f', 10);
      redraw();
    } break;
  }
}

/*  ------------------------------------------------------------------------  */
/*  EXAMPLE TESTS  */

void Canvas::example1() {
  QColor bg(130, 51, 214);
  image.fill(bg.rgba());

  drawer.drawBresenhamLine(QPoint(10, 15), QPoint(580, 440));
  setLineType(LineType::antialiased);
  drawer.drawApproximatedCircle(QPoint(240, 670), QPoint(456, 789), 35);
  setMainColor(QColor(243, 1, 203));
  QVector<QPoint> vec(
      {QPoint(45, 67), QPoint(873, 145), QPoint(652, 147), QPoint(2, 356)});
  drawer.drawPolygon(vec);

  drawer.drawBezierCurve(QPoint(479, 451), QPoint(488, 687), QPoint(692, 362),
                         QPoint(725, 604));

  vec = QVector<QPoint>({QPoint(386, 543), QPoint(314, 494), QPoint(147, 484),
                         QPoint(119, 546), QPoint(223, 577), QPoint(306, 599),
                         QPoint(322, 676), QPoint(293, 748), QPoint(140, 736),
                         QPoint(144, 659)});
  setMainColor(QColor(1, 203, 203));
  drawer.drawBasisSpline(vec);
}

void Canvas::example2() {
  QColor bg(255, 155, 116, 153);
  image.fill(bg.rgba());

  QColor fg(82, 15, 217, 89);
  setMainColor(fg);

  setLineType(LineType::antialiased);
  drawer.drawLine(QPoint(0, 0), QPoint(800, 800));
  drawer.drawLine(QPoint(800, 0), QPoint(0, 800));
  drawer.drawLine(QPoint(100, 100), QPoint(100, 700));
  drawer.drawApproximatedCircle(QPoint(400, 400), 300, 16);

  setFillSettings(
      true,
      QColor(rand() % 255, rand() % 255, rand() % 255, rand() % 255).rgba(),
      FillType::stack);
  drawer.fill(QPoint(110, 125), settings.fill_color.rgba());
  setFillSettings(
      true,
      QColor(rand() % 255, rand() % 255, rand() % 255, rand() % 255).rgba(),
      FillType::stack);
  drawer.fill(QPoint(105, 550), settings.fill_color.rgba());

  drawer.drawPoint(331, 526);
  drawer.drawPoint(332, 527);
  drawer.drawPoint(332, 525);
  drawer.drawPoint(330, 525);
  drawer.drawPoint(330, 527);

  drawer.drawPoint(337, 605);
  drawer.drawPoint(338, 606);
  drawer.drawPoint(338, 604);
  drawer.drawPoint(336, 604);
  drawer.drawPoint(336, 606);

  drawer.drawPoint(427, 521);
  drawer.drawPoint(428, 522);
  drawer.drawPoint(428, 520);
  drawer.drawPoint(426, 520);
  drawer.drawPoint(426, 522);

  drawer.drawPoint(438, 641);
  drawer.drawPoint(437, 640);
  drawer.drawPoint(437, 642);
  drawer.drawPoint(439, 642);
  drawer.drawPoint(439, 640);

  setFillSettings(false, QColor(255, 255, 255, 255).rgba(), FillType::scanline);
  drawer.fill(QPoint(200, 15), settings.fill_color.rgba());
  setDebug(true);
  setFillSettings(false, QColor(33, 0, 0, 255).rgba(), FillType::scanline);
  drawer.fill(QPoint(400, 541), settings.fill_color.rgba());
  setDebug(false);
}

void Canvas::example3() {
  setGradientSettings(QColor(76, 224, 162), QColor(218, 255, 56), 8);
  drawer.paintVerticalGradient(settings.start_color, settings.end_color,
                               settings.gradient_steps);

  setMainColor(QColor(0, 0, 0, 255));
  QVector<QPoint> vec({QPoint(198, 162), QPoint(197, 278), QPoint(329, 158),
                       QPoint(350, 393), QPoint(173, 392), QPoint(393, 242),
                       QPoint(438, 443), QPoint(348, 544), QPoint(181, 552)});
  drawer.drawBasisSpline(vec);

  drawer.transform(drawer.createRotateMatrix(30, true));
  drawer.transform(drawer.createShiftMatrix(-25, 25));
}

void Canvas::example4() {
  drawer.transform(drawer.createShiftMatrix(15, 15));
  drawer.transform(drawer.createShiftMatrix(-15, -15));
  for (int i = 0; i < 360 / 30; ++i) {
    drawer.transform(drawer.createRotateMatrix(30, true));
    DebugWindow::waitFor(40);
    redraw();
  }
  drawer.transform(drawer.createScaleMatrix(1.2, 1.2, true));
}
