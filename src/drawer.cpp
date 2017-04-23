#include "drawer.h"
// TODO get rid of in range checks

Drawer::Drawer() {}
Drawer::Drawer(QImage* _image, QRgb _main_color) {
  setImage(_image);
  setMainColor(_main_color);
}

void Drawer::initialize(LineType _line_type, CircleType _circle_type,
                        int _circle_steps, FillType _fill_type,
                        InterpolationType _interpolation_type) {
  setLineType(_line_type);
  setCircleType(_circle_type);
  setCircleSteps(_circle_steps);
  setFillType(_fill_type);
  setInterpolationType(_interpolation_type);
}

void Drawer::setImage(QImage* _image) {
  image = _image;
  width = image->width();
  height = image->height();
  bits = (QRgb*)image->bits();
}

void Drawer::setMainColor(QRgb _main_color) { main_color = _main_color; }

void Drawer::setDebug(bool _debug, QRgb _debug_color) {
  debug = _debug;
  debug_color = _debug_color;
}

void Drawer::setLineType(LineType _line_type) { line_type = _line_type; }

void Drawer::setCircleType(CircleType _circle_type) {
  circle_type = _circle_type;
}

void Drawer::setFillType(FillType _fill_type) { fill_type = _fill_type; }

void Drawer::setInterpolationType(InterpolationType _interpolation_type) {
  interpolation_type = _interpolation_type;
}

void Drawer::setCircleSteps(int _circle_steps) { circle_steps = _circle_steps; }


/*  ------------------------------------------------------------------------  */
/*  POINT METHODS  */
void Drawer::drawPoint(int x, int y) { drawPoint(x, y, main_color); }

void Drawer::drawPoint(QPoint point) { drawPoint(point, main_color); }

void Drawer::drawPoint(QPoint point, QRgb color) {
  drawPoint(point.x(), point.y(), color);
}

// TODO consider calling blendPoint() if alpha != 255
void Drawer::drawPoint(int x, int y, QRgb color) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    // safer
    //    image->setPixel(x, y, color);

    // faster
    bits[y * width + x] = color;
  }
}

// TODO optimise with bit shifts and getting color with bits()
inline void Drawer::blendPoint(int x, int y, QRgb _over) {
  if (x < 0 || x >= width || y < 0 || y >= height) return;

  QColor base = image->pixelColor(x, y);
  QColor over;
  over.setRgba(_over);

  qreal a = over.alphaF();
  qreal b = base.alphaF() * (1 - a);
  qreal denominator = a + b;
  // if both colors have alpha == 0, result is undefined
  int red = (over.red() * a + base.red() * b) / denominator;
  int green = (over.green() * a + base.green() * b) / denominator;
  int blue = (over.blue() * a + base.blue() * b) / denominator;
  int alpha = 255 * (a + b);

  QRgb color = (alpha << 24) | (red << 16) | (green << 8) | (blue);
  bits[y * width + x] = color;
}


/*  ------------------------------------------------------------------------  */
/*  LINE METHODS  */
void Drawer::drawLine(QPoint start, QPoint end) {
  switch (line_type) {
    case LineType::bresenham: {
      drawBresenhamLine(start, end);
    } break;

    case LineType::antialiased: {
      drawAntialiasedLine(start, end);
    } break;

    default:
      qDebug() << "Invalid line_type value:" << int(line_type);
      break;
  }
}


/*  BRESENHAM LINE FUNCTIONS  */

void Drawer::drawBresenhamLine(QPoint start, QPoint end) {
  drawBresenhamLine(start, end, main_color);
}

void Drawer::drawBresenhamLine(QPoint start, QPoint end, QRgb color) {
  int octant = determineOctant(start, end);

  // calculate new end (relative to (0, 0))
  int endx = end.x() - start.x();
  int endy = end.y() - start.y();

  // swap inputs according to wiki
  swapInputs(endx, endy, octant);

  // set new end
  end.rx() = endx;
  end.ry() = endy;


  // Bresenham's line
  int dx = end.x() - 0;  // always draw from (0, 0)
  int dy = end.y() - 0;  // always draw from (0, 0)
  int dp = 2 * dy;
  int dd = 2 * (dy - dx);
  int d = 2 * dy - dx;

  int x = 0;
  int y = 0;

  for (int i = 0; i < dx; ++i) {
    ++x;

    if (d >= 0) {
      d = d + dd;
      ++y;
    } else {
      d = d + dp;
    }
    // end of Bresenham's algorithm

    // draw debug line from (0, 0)
    if (debug) drawPoint(x, y, debug_color);

    int outx = x;
    int outy = y;

    // swap output according to wiki
    swapOutputs(outx, outy, octant);

    // transform from (0, 0) to relative to start
    outx += start.x();
    outy += start.y();

    drawPoint(outx, outy, color);
  }
}


/*  XIAOLIN WU ANTIALIASED LINE  */

void Drawer::drawAntialiasedLine(QPoint start, QPoint end) {
  drawAntialiasedLine(start, end, main_color);
}

void Drawer::drawAntialiasedLine(QPoint start, QPoint end, QRgb color) {
  _XWcolor = color;
  _XWdraw(start.x(), start.y(), end.x(), end.y());
}

void Drawer::_XWplot(int x, int y, float c) {
  if (x < 0 || x >= width || y < 0 || y >= height) return;

  // apply antialiasing
  if (c > 1.0) c = 1.0f;
  if (c < 0.0) {
    //    c = 0.0f;
    return;  // if drawing color alpha == 0, pixel color won't change at all
  }

  int alpha = _XWcolor >> 24;
  alpha *= c;

  // bitshifts to drop original alpha channel from color and add modified one
  QRgb color = (((_XWcolor << 8) >> 8) | (alpha << 24));

  blendPoint(x, y, color);
}

float Drawer::_XWipart(float x) { return (int)x; }

float Drawer::_XWround(float x) { return _XWipart(x + 0.5); }

float Drawer::_XWfpart(float x) {
  if (x < 0)
    return 1 - (x - floor(x));
  else
    return x - floor(x);
}

float Drawer::_XWrfpart(float x) { return 1 - _XWfpart(x); }

void Drawer::_XWdraw(int _x0, int _y0, int _x1, int _y1) {
  float x0 = _x0;
  float y0 = _y0;
  float x1 = _x1;
  float y1 = _y1;
  bool steep = (fabs(y1 - y0) > fabs(x1 - x0));

  if (steep) {
    std::swap(x0, y0);
    std::swap(x1, y1);
  }
  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  float dx = x1 - x0;
  float dy = y1 - y0;
  float gradient;
  if (dx == 0)
    gradient = 1;
  else
    gradient = dy / dx;

  // handle first endpoint
  float xend = _XWround(x0);
  float yend = y0 + gradient * (xend - x0);
  float xgap = _XWrfpart(x0 + 0.5f);
  float xpxl1 = xend;  // this will be used in the main loop
  float ypxl1 = _XWipart(yend);

  if (steep) {
    _XWplot(ypxl1, xpxl1, _XWrfpart(yend) * xgap);
    _XWplot(ypxl1 + 1, xpxl1, _XWfpart(yend) * xgap);
  } else {
    _XWplot(xpxl1, ypxl1, _XWrfpart(yend) * xgap);
    _XWplot(xpxl1, ypxl1 + 1, _XWfpart(yend) * xgap);
  }
  float intery = yend + gradient;

  xend = _XWround(x1);
  yend = y1 + gradient * (xend - x1);
  xgap = _XWfpart(x1 + 0.5);
  float xpxl2 = xend;  // this will be used in the main loop
  float ypxl2 = _XWipart(yend);
  if (steep) {
    _XWplot(ypxl2, xpxl2, _XWrfpart(yend) * xgap);
    _XWplot(ypxl2 + 1, xpxl2, _XWfpart(yend) * xgap);
  } else {
    _XWplot(xpxl2, ypxl2, _XWrfpart(yend) * xgap);
    _XWplot(xpxl2, ypxl2 + 1, _XWfpart(yend) * xgap);
  }

  // main loop
  if (steep) {
    for (int x = xpxl1 + 1; x < xpxl2; ++x) {
      _XWplot(_XWipart(intery), x, _XWrfpart(intery));
      _XWplot(_XWipart(intery) + 1, x, _XWfpart(intery));
      intery = intery + gradient;
    }
  } else {
    for (int x = xpxl1 + 1; x < xpxl2; ++x) {
      _XWplot(x, _XWipart(intery), _XWrfpart(intery));
      _XWplot(x, _XWipart(intery) + 1, _XWfpart(intery));
      intery = intery + gradient;
    }
  }
}


/*  ------------------------------------------------------------------------  */
/*  CIRCLE METHODS  */

void Drawer::drawCircle(QPoint centre, QPoint range) {
  switch (circle_type) {
    case CircleType::bresenham: {
      drawBresenhamCircle(centre, range, main_color);
    } break;

    case CircleType::approximated: {
      drawApproximatedCircle(centre, range, circle_steps);
    } break;

    default:
      qDebug() << "Invalid circle_type value:" << int(circle_type);
      break;
  }
}


/*  BRESENHAM'S CIRCLE FUNCTION  */

void Drawer::drawBresenhamCircle(QPoint centre, uint radius) {
  drawBresenhamCircle(centre, radius, main_color);
}

void Drawer::drawBresenhamCircle(QPoint centre, QPoint range) {
  drawBresenhamCircle(centre, range, main_color);
}

void Drawer::drawBresenhamCircle(QPoint centre, uint radius, QRgb color) {
  int x = 0;
  int y = radius;

  int d = 5 - 4 * radius;
  int da = (-2 * radius + 5) * 4;
  int db = 3 * 4;

  while (x <= y) {
    for (int octant = 0; octant < 8; ++octant) {
      int outx = x;
      int outy = y;
      swapOutputs(outx, outy, octant);
      outx = centre.x() + outx;
      outy = centre.y() + outy;

      drawPoint(outx, outy, color);
    }

    if (d > 0) {
      d += da;
      --y;
      ++x;
      da += 4 * 4;
      db += 2 * 4;
    } else {
      d += db;
      ++x;
      da += 2 * 4;
      db += 2 * 4;
    }
  }
}

void Drawer::drawBresenhamCircle(QPoint centre, QPoint range, QRgb color) {
  int dx = range.x() - centre.x();
  int dy = range.y() - centre.y();
  int radius = (int)sqrt((dx * dx) + (dy * dy));

  drawBresenhamCircle(centre, radius, color);
}


/*  CIRCLE APPROXIMATED USING LINES  */

void Drawer::drawApproximatedCircle(QPoint centre, uint radius,
                                    const int segments) {
  float prev_x = radius;  // radius * 1
  float prev_y = 0;       // radius * 0
  for (int i = 1; i <= segments; ++i) {
    float theta = 2.0f * M_PI * float(i) / float(segments);

    float x = radius * cos(theta);
    float y = radius * sin(theta);

    QPoint start(prev_x + centre.x(), prev_y + centre.y());
    QPoint end(x + centre.x(), y + centre.y());
    drawLine(start, end);
    prev_x = x;
    prev_y = y;
  }
}

void Drawer::drawApproximatedCircle(QPoint centre, QPoint range,
                                    const int segments) {
  int dx = range.x() - centre.x();
  int dy = range.y() - centre.y();
  int radius = (int)sqrt((dx * dx) + (dy * dy));

  drawApproximatedCircle(centre, radius, segments);
}


/*  ------------------------------------------------------------------------  */
/*  LINE, CIRCLE SHARED UTILITY METHODS  */

int Drawer::determineOctant(QPoint start, QPoint end) {
  int octant = 8;
  int dx = end.x() - start.x();
  int dy = end.y() - start.y();
  int da = abs(dx) - abs(dy);

  if (dx >= 0 && dy >= 0 && da >= 0) octant = 7;
  if (dx > 0 && dy > 0 && da < 0) octant = 6;
  if (dx <= 0 && dy >= 0 && da <= 0) octant = 5;
  if (dx < 0 && dy > 0 && da > 0) octant = 4;
  if (dx <= 0 && dy <= 0 && da >= 0) octant = 3;
  if (dx < 0 && dy < 0 && da < 0) octant = 2;
  if (dx >= 0 && dy <= 0 && da <= 0) octant = 1;
  if (dx > 0 && dy < 0 && da > 0) octant = 0;

  return octant;
}

void Drawer::swapInputs(int& outx, int& outy, const int octant) {
  int x, y;
  switch (octant) {
    case 7:
      x = outx;
      y = outy;
      break;

    case 0:
      x = outx;
      y = -outy;
      break;

    case 1:
      x = -outy;
      y = outx;
      break;

    case 2:
      x = -outy;
      y = -outx;
      break;

    case 3:
      x = -outx;
      y = -outy;
      break;

    case 4:
      x = -outx;
      y = outy;
      break;

    case 5:
      x = outy;
      y = -outx;
      break;

    case 6:
      x = outy;
      y = outx;
      break;
  }

  outx = x;
  outy = y;
}

void Drawer::swapOutputs(int& outx, int& outy, const int octant) {
  int x, y;
  switch (octant) {
    case 7:
      x = outx;
      y = outy;
      break;

    case 0:
      x = outx;
      y = -outy;
      break;

    case 1:  // swapped with 5 in comparison to swapInputs()
      x = outy;
      y = -outx;
      break;

    case 2:
      x = -outy;
      y = -outx;
      break;

    case 3:
      x = -outx;
      y = -outy;
      break;

    case 4:
      x = -outx;
      y = outy;
      break;

    case 5:  // swapped with 1
      x = -outy;
      y = outx;
      break;

    case 6:
      x = outy;
      y = outx;
      break;
  }

  outx = x;
  outy = y;
}


/*  ------------------------------------------------------------------------  */
/*  SIMPLE BORDER POLYGON  */

void Drawer::drawPolygon(QVector<QPoint>& points) {
  const auto size = points.size();
  for (int i = 0; i <= size - 1; ++i) {
    if (i == size - 1)
      drawLine(points[size - 1], points[0]);
    else
      drawLine(points[i], points[i + 1]);
  }
}


/*  ------------------------------------------------------------------------  */
/*  BEZIER CURVE  */

void Drawer::drawBezierCurve(QPoint p0, QPoint p1, QPoint p2, QPoint p3) {
  if (debug) {
    drawBresenhamLine(p0, p1, debug_color);
    drawBresenhamLine(p2, p3, debug_color);
  }

  int cx = 3 * (p1.x() - p0.x());
  int bx = 3 * (p2.x() - p1.x()) - cx;
  int ax = p3.x() - p0.x() - cx - bx;
  int cy = 3 * (p1.y() - p0.y());
  int by = 3 * (p2.y() - p1.y()) - cy;
  int ay = p3.y() - p0.y() - cy - by;

  const int n = 1000;
  for (int i = 0; i < n; ++i) {
    double t = (1.0 / n) * i;
    int x = ax * pow(t, 3) + bx * pow(t, 2) + cx * t + p0.x();
    int y = ay * pow(t, 3) + by * pow(t, 2) + cy * t + p0.y();

    drawPoint(x, y);
  }
}


/*  ------------------------------------------------------------------------  */
/*  BASIS SPLINE DRAWING  */

void Drawer::drawBasisSpline(QVector<QPoint>& points) {
  const auto size = points.size();
  const int segments = size - 3;
  int fx, fy, gx, gy, ex, ey;
  QPoint b;
  for (int i = 0; i < segments; ++i) {
    if (i == 0) {
      fx = points[1].x();
      fy = points[1].y();
      gx = 0.5 * points[1].x() + 0.5 * points[2].x();
      gy = 0.5 * points[1].y() + 0.5 * points[2].y();
      b = QPoint(points[0].x(), points[0].y());

      int fx1 = 2.0 / 3.0 * points[2].x() + 1.0 / 3.0 * points[3].x();
      int fy1 = 2.0 / 3.0 * points[2].y() + 1.0 / 3.0 * points[3].y();
      ex = 0.5 * gx + 0.5 * fx1;
      ey = 0.5 * gy + 0.5 * fy1;

    } else if (i == segments - 1) {
      fx = 0.5 * points[segments].x() + 0.5 * points[segments + 1].x();
      fy = 0.5 * points[segments].y() + 0.5 * points[segments + 1].y();
      gx = points[segments + 1].x();
      gy = points[segments + 1].y();
      ex = points[segments + 2].x();
      ey = points[segments + 2].y();

    } else {
      fx = 2.0 / 3.0 * points[i + 1].x() + 1.0 / 3.0 * points[i + 2].x();
      fy = 2.0 / 3.0 * points[i + 1].y() + 1.0 / 3.0 * points[i + 2].y();
      gx = 1.0 / 3.0 * points[i + 1].x() + 2.0 / 3.0 * points[i + 2].x();
      gy = 1.0 / 3.0 * points[i + 1].y() + 2.0 / 3.0 * points[i + 2].y();

      int fx1 = 2.0 / 3.0 * points[i + 2].x() + 1.0 / 3.0 * points[i + 3].x();
      int fy1 = 2.0 / 3.0 * points[i + 2].y() + 1.0 / 3.0 * points[i + 3].y();
      ex = 0.5 * gx + 0.5 * fx1;
      ey = 0.5 * gy + 0.5 * fy1;
    }

    QPoint f(fx, fy), g(gx, gy), e(ex, ey);
    drawBezierCurve(b, f, g, e);
    b = e;
  }
}


/*  ------------------------------------------------------------------------  */
/*  GRADIENT PAINTING  */

void Drawer::paintVerticalGradient(QRgb start_color, QRgb end_color,
                                   int steps) {
  // constructor QColor(QRgb) ignores alpha channel from QRgb
  QColor s_c, e_c;
  s_c.setRgba(start_color);
  e_c.setRgba(end_color);

  paintVerticalGradient(s_c, e_c, steps);
}

void Drawer::paintVerticalGradient(QColor start_color, QColor end_color,
                                   int steps) {
  if (steps == 0) {
    image->fill(end_color);
    return;
  }

  QRgb colors[steps];
  getGradientColors(colors, start_color, end_color, steps);

  int y_diff = ceil(height * 1.0 / steps);

  int current_y = 0;
  for (int i = 0; i < steps; ++i) {
    for (int j = 0; j < y_diff; ++j) {
      QRgb* line = (QRgb*)image->scanLine(current_y);
      for (int x = 0; x < width; ++x) line[x] = colors[i];

      if (++current_y >= height) return;
    }
  }
}

void Drawer::paintHorizontalGradient(QRgb start_color, QRgb end_color,
                                     int steps) {
  // constructor QColor(QRgb) ignores alpha channel from QRgb
  QColor s_c, e_c;
  s_c.setRgba(start_color);
  e_c.setRgba(end_color);

  paintHorizontalGradient(s_c, e_c, steps);
}

void Drawer::paintHorizontalGradient(QColor start_color, QColor end_color,
                                     int steps) {
  if (steps == 0) {
    image->fill(end_color);
    return;
  }

  QRgb colors[steps];
  getGradientColors(colors, start_color, end_color, steps);

  int x_diff = ceil(width * 1.0 / steps);

  for (int y = 0; y < height; ++y) {
    QRgb* line = (QRgb*)image->scanLine(y);
    int current_x = 0;
    for (int i = 0; i < steps; ++i) {
      for (int j = 0; j < x_diff && current_x < width; ++j) {
        line[current_x++] = colors[i];
      }
    }
  }
}

void Drawer::getGradientColors(QRgb* colors, QColor start_color,
                               QColor end_color, int steps) {
  colors[0] = start_color.rgba();

  float r_diff = (end_color.red() - start_color.red()) * 1.0 / steps;
  float g_diff = (end_color.green() - start_color.green()) * 1.0 / steps;
  float b_diff = (end_color.blue() - start_color.blue()) * 1.0 / steps;
  float a_diff = (end_color.alpha() - start_color.alpha()) * 1.0 / steps;

  //  qDebug() << r_diff << " " << g_diff << " " << b_diff << " " << a_diff;

  float red = start_color.red();
  float green = start_color.green();
  float blue = start_color.blue();
  float alpha = start_color.alpha();

  //  qDebug() << red << " " << green << " "
  //           << " " << blue << " " << alpha;

  for (int i = 1; i < steps; ++i) {
    red += r_diff;
    green += g_diff;
    blue += b_diff;
    alpha += a_diff;

    colors[i] =
        ((int)alpha << 24) | ((int)red << 16) | ((int)green << 8) | (int)blue;
  }
}


/*  ------------------------------------------------------------------------  */
/*  SCANLINE FLOOD FILLING ("Smith's")  */
void Drawer::fill(QPoint start, QRgb target_color) {
  QRgb prev_color = image->pixel(start);

  switch (fill_type) {
    case FillType::scanline: {
      fillScanline(start, target_color, prev_color);
    } break;

    case FillType::stack: {
      fillFloodStack(start, target_color);
    } break;

    case FillType::recursive: {
      fillBorderRecursive(start, target_color, main_color);
    } break;

    default:
      qDebug() << "Invalid fill type";
      break;
  }
}

void Drawer::fillScanline(QPoint start, QRgb target_color, QRgb prev_color) {
  if (prev_color == target_color) return;


  DebugWindow* debug_window;
  if (debug) {
    debug_window = new DebugWindow(image);
  }

  QStack<QPoint> stack;
  stack.push(start);

  while (!stack.isEmpty()) {
    QPoint point = stack.pop();
    int x = point.x();
    int y = point.y();
    int a = x;  // `a` is basically an iterator over x's
    bool span_above = false;
    bool span_below = false;

    QRgb* line = (QRgb*)image->scanLine(y);
    // rewind to the left border
    while (a >= 0 && line[a] == prev_color) --a;
    ++a;

    QRgb* line_above = (QRgb*)image->constScanLine(y + 1);
    QRgb* line_below = (QRgb*)image->constScanLine(y - 1);

    // go towards right border, mark potential down/up splits
    while (a < image->width() && line[a] == prev_color) {
      line[a] = target_color;

      if (!span_above && y > 0 && line_below[a] == prev_color) {
        // found segment below to be filled
        stack.push(QPoint(a, y - 1));  // save where it starts
        span_above = true;             // ignore other points of this segment
      } else if (span_above && y > 0 && line_below[a] != prev_color) {
        // segment ended
        span_above = false;

        // similarly to if below, but for points above current line
      } else if (!span_below && y < height - 1 && line_above[a] == prev_color) {
        stack.push(QPoint(a, y + 1));
        span_below = true;
      } else if (span_below && y < height - 1 && line_above[a] != prev_color) {
        span_below = false;
      }
      ++a;
    }

    if (debug) {
      debug_window->redraw();
      DebugWindow::waitFor(10);
    }
  }

  if (debug) delete debug_window;
}


/*  FLOOD FILLS  */

// recursive implementation
void Drawer::fillBorderRecursive(QPoint start, QRgb target_color,
                                 QRgb border_color) {
  if (image->pixel(start) != border_color)
    image->setPixelColor(start, target_color);

  fillBorderRecursive(start.x(), start.y(), target_color, border_color);
}

void Drawer::fillBorderRecursive(int x, int y, QRgb target_color,
                                 QRgb border_color) {
  drawPoint(x, y, target_color);

  if (_recursiveFloodFillTest(x, y - 1, target_color, border_color))
    fillBorderRecursive(x, y - 1, target_color, border_color);

  if (_recursiveFloodFillTest(x + 1, y, target_color, border_color))
    fillBorderRecursive(x + 1, y, target_color, border_color);

  if (_recursiveFloodFillTest(x, y + 1, target_color, border_color))
    fillBorderRecursive(x, y + 1, target_color, border_color);

  if (_recursiveFloodFillTest(x - 1, y, target_color, border_color))
    fillBorderRecursive(x - 1, y, target_color, border_color);
}

bool Drawer::_recursiveFloodFillTest(int x, int y, QRgb target_color,
                                     QRgb border_color) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    if (image->pixel(x, y) != target_color &&
        image->pixel(x, y) != border_color)
      return true;
  }

  return false;
}

// stack based implementation
void Drawer::fillFloodStack(QPoint start, QRgb target_color) {
  QRgb start_color = image->pixel(start);

  DebugWindow* debug_window;
  if (debug) {
    debug_window = new DebugWindow(image);
  }

  QStack<QPoint> stack;
  stack.push(start);

  while (!stack.isEmpty()) {
    QPoint point = stack.pop();
    int x, y;

    x = point.x();
    y = point.y() - 1;
    if (x >= 0 && x < width && y >= 0 && y < height) {
      if (image->pixel(x, y) != target_color &&
          image->pixel(x, y) == start_color)
        stack.push(QPoint(x, y));
    }

    x = point.x() + 1;
    y = point.y();
    if (x >= 0 && x < width && y >= 0 && y < height) {
      if (image->pixel(x, y) != target_color &&
          image->pixel(x, y) == start_color)
        stack.push(QPoint(x, y));
    }

    x = point.x();
    y = point.y() + 1;
    if (x >= 0 && x < width && y >= 0 && y < height) {
      if (image->pixel(x, y) != target_color &&
          image->pixel(x, y) == start_color)
        stack.push(QPoint(x, y));
    }

    x = point.x() - 1;
    y = point.y();
    if (x >= 0 && x < width && y >= 0 && y < height) {
      if (image->pixel(x, y) != target_color &&
          image->pixel(x, y) == start_color)
        stack.push(QPoint(x, y));
    }

    drawPoint(point, target_color);

    if (debug) {
      debug_window->redraw();
      DebugWindow::waitFor(1);
    }
  }

  if (debug) delete debug_window;
}


/*  ------------------------------------------------------------------------  */
/*  TRANSFORMATIONS  */

QTransform Drawer::toInplaceTransformation(QTransform matrix) {
  int t_x = -width / 2;
  int t_y = -height / 2;
  QTransform translate(1, 0, 0, 0, 1, 0, t_x, t_y, 1);
  QTransform translate_inv = translate.inverted();

  return translate * matrix * translate_inv;
}

QTransform Drawer::createShiftMatrix(int shift_x, int shift_y) {
  return QTransform(1, 0, 0, 0, 1, 0, shift_x, shift_y, 1);
}

// accepts degrees, converts to radians
QTransform Drawer::createRotateMatrix(qreal theta_degrees, bool in_place) {
  qreal theta = theta_degrees * M_PI / 180;
  qreal cos_t = cos(theta);
  qreal sin_t = sin(theta);
  QTransform matrix(cos_t, sin_t, 0, -sin_t, cos_t, 0, 0, 0, 1);

  return in_place ? toInplaceTransformation(matrix) : matrix;
}

QTransform Drawer::createScaleMatrix(qreal scale_x, qreal scale_y,
                                     bool in_place) {
  QTransform matrix(scale_x, 0, 0, 0, scale_y, 0, 0, 0, 1);

  return in_place ? toInplaceTransformation(matrix) : matrix;
}

QTransform Drawer::createShearMatrix(qreal shear_x, qreal shear_y,
                                     bool in_place) {
  QTransform matrix(1, shear_x, 0, shear_y, 1, 0, 0, 0, 1);

  return in_place ? toInplaceTransformation(matrix) : matrix;
}

void Drawer::transform(QTransform transformation) {
  // question: can under any circumstances result image have different (bigger)
  // dimensions than canvas image?
  QImage temp(width, height, QImage::Format_ARGB32);
  QRgb* temp_bits = (QRgb*)temp.bits();

  // initialize to (0, 0, 0, 0)
  for (int y = 0; y < height; ++y) {
    const int y_offset = y * width;
    for (int x = 0; x < width; ++x) {
      temp_bits[y_offset + x] = 0;
    }
  }

  DebugWindow* debug_window;
  if (debug) debug_window = new DebugWindow(&temp);


  // always invert because of reverse mapping (org := res * M^-1, res := org)
  transformation = transformation.inverted();

  for (int y = 0; y < height; ++y) {
    const int y_offset = y * width;
    for (int x = 0; x < width; ++x) {
      // QPointF needed for interpolation
      QPointF source_pixel = QPointF(x, y) * transformation;

      // TODO how to switch between interpolation functions without if in each
      // loop iteration?
      QRgb result_color;
      switch (interpolation_type) {
        case InterpolationType::nearest: {
          result_color = interpolateNearest(source_pixel);

        } break;
        case InterpolationType::bilinear: {
          result_color = interpolateBilinear(source_pixel);

        } break;
      }


      temp_bits[y_offset + x] = result_color;
    }

    if (debug) {
      DebugWindow::waitFor(2);
      debug_window->redraw();
    }
  }

  if (debug) delete debug_window;

  // copy result to canvas image
  for (int y = 0; y < height; ++y) {
    const int y_offset = y * width;
    for (int x = 0; x < width; ++x) {
      bits[y_offset + x] = temp_bits[y_offset + x];
    }
  }
}


/*  ------------------------------------------------------------------------  */
/*  INTERPOLATION FUNCTIONS  */

// TODO optimise both color picking functions
QRgb Drawer::interpolateNearest(QPointF coordinates) {
  QRgb color = qRgb(0, 0, 0);  // default value is black

  int x = round(coordinates.x());
  int y = round(coordinates.y());
  if (x >= 0 && x < width && y >= 0 && y < height) {
    color = image->pixel(x, y);
  }

  return color;
}

// this macro uses QColor constructor that properly sets alpha channel
#define BLEND_COLORS(_a, _b)                           \
  int r##_a##_b = (p##_a.red() + p##_b.red()) / 2;     \
  int g##_a##_b = (p##_a.green() + p##_b.green()) / 2; \
  int b##_a##_b = (p##_a.blue() + p##_b.blue()) / 2;   \
  int a##_a##_b = (p##_a.alpha() + p##_b.alpha()) / 2; \
  p##_a##_b = QColor(r##_a##_b, g##_a##_b, b##_a##_b, a##_a##_b)

QRgb Drawer::interpolateBilinear(QPointF coordinates) {
  QRgb color = qRgb(0, 0, 0);  // default value is black

  int x = coordinates.x();
  int y = coordinates.y();
  if (x >= 0 && x + 1 < width && y >= 0 && y + 1 < height) {
    QColor p02;
    {
      QColor p0 = image->pixelColor(x, y);
      QColor p2 = image->pixelColor(x, y + 1);
      BLEND_COLORS(0, 2);
    }
    QColor p13;
    {
      QColor p1 = image->pixelColor(x + 1, y);
      QColor p3 = image->pixelColor(x + 1, y + 1);
      BLEND_COLORS(1, 3);
    }
    QColor p0213;
    { BLEND_COLORS(02, 13); }

    color = p0213.rgba();
  }

  return color;
}

#undef BLEND_COLORS
