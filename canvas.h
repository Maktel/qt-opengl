#ifndef CANVAS_H
#define CANVAS_H

#include <QtWidgets>
#include "drawer.h"

enum class Mode {
  click,
  line,
  circle,
  bezier,
  spline,
  polygon,
  fill,
  vgradient,
  hgradient,
  transform
};

enum class LineType { bresenham, antialiased };

enum class CircleType { bresenham, approximated };

enum class FillType { scanline, stack, recursive };

enum class Transform { shift, rotate, scale, shear };

enum class InterpolationType { nearest, bilinear };

struct Settings {
  int width;
  int height;

  Mode mode;
  bool debug;
  bool grid;
  qreal scale;
  LineType line_type;
  CircleType circle_type;
  QColor fill_color;
  bool fill_random;
  FillType fill_type;
  QColor start_color;
  QColor end_color;
  QColor main_color;
  int circle_steps;
  QColor debug_color;
  InterpolationType interpolation_type;

  int shift_x;
  int shift_y;
  qreal rotate_angle;
  bool rotate_inplace;
  qreal scale_x;
  qreal scale_y;
  bool scale_inplace;
  qreal shear_x;
  qreal shear_y;
  bool shear_inplace;

  // defaults
  Settings() {
    width = 600;
    height = 600;

    mode = Mode::click;
    debug = false;
    grid = false;
    scale = 1.0;
    line_type = LineType::bresenham;
    circle_type = CircleType::bresenham;
    fill_color = QColor(255, 255, 255, 255);
    fill_random = true;
    fill_type = FillType::scanline;
    start_color = QColor(255, 0, 0);
    end_color = QColor(128, 0, 255);
    main_color = QColor(0, 0, 255);
    circle_steps = 15;
    debug_color = QColor(0, 255, 0);

    shift_x = 20;
    shift_y = 35;
    rotate_angle = 30;
    rotate_inplace = true;
    scale_x = 1.05;
    scale_y = 0.75;
    scale_inplace = true;
    shear_x = 0.05;
    shear_y = -0.05;
    shear_inplace = false;
  }

  friend QDebug operator<<(QDebug dbg, const Settings& sett) {
    dbg << "-------------------\nSettings: ";
    dbg.nospace() << "\nwidth: " << sett.width;
    dbg.nospace() << "\nheight: " << sett.height;
    dbg.nospace() << "\nmode: " << int(sett.mode);
    dbg.nospace() << "\ndebug: " << sett.debug;
    dbg.nospace() << "\ngrid: " << sett.grid;
    dbg.nospace() << "\nscale: " << sett.scale;
    dbg.nospace() << "\nline_type: " << int(sett.line_type);
    dbg.nospace() << "\ncircle_type: " << int(sett.circle_type);
    dbg.nospace() << "\nfill_color: " << sett.fill_color;
    dbg.nospace() << "\nfill_random: " << sett.fill_random;
    dbg.nospace() << "\nfill_type: " << int(sett.fill_type);
    dbg.nospace() << "\nstart_color: " << sett.start_color;
    dbg.nospace() << "\nend_color: " << sett.end_color;
    dbg.nospace() << "\nmain_color: " << sett.main_color;
    dbg.nospace() << "\ncircle_steps: " << sett.circle_steps;
    dbg.nospace() << "\ndebug_color: " << sett.debug_color;
    dbg.nospace() << "\nshift_x: " << sett.shift_x;
    dbg.nospace() << "\nshift_y: " << sett.shift_y;
    dbg.nospace() << "\nrotation angle: " << sett.rotate_angle;
    dbg.nospace() << "\nrotate_inplace: " << sett.rotate_inplace;
    dbg.nospace() << "\nscale_x: " << sett.scale_x;
    dbg.nospace() << "\nscale_y: " << sett.scale_y;
    dbg.nospace() << "\nscale_inplace: " << sett.scale_inplace;
    dbg.nospace() << "\nshear_x: " << sett.shear_x;
    dbg.nospace() << "\nshear_y: " << sett.shear_y;
    dbg.nospace() << "\nshear_inplace: " << sett.shear_inplace;
    dbg.nospace() << "\ninterpolation_type: " << int(sett.interpolation_type);

    return dbg;
  }
};

class Canvas : public QWidget {
  Q_OBJECT
 public:
  explicit Canvas(QWidget* parent = 0);

  void loadFile(QString file_name);
  void saveFile(QString file_name);

  void setMode(Mode _mode);
  void setDebug(bool _debug);
  void switchGrid();
  void setScale(qreal _scale);
  void setLineMode(LineType _line_type);
  void setCircleMode(CircleType _circle_type);
  void setFillSettings(bool _fill_random, QColor _fill_color,
                       FillType _fill_type);
  void setGradientSettings(QColor _start_color, QColor _end_color);
  void setMainColor(QColor _main_color);
  void setCircleSteps(int _circle_steps);
  void setDebugColor(QColor _debug_color);
  void setShift(int _shift_x, int _shift_y);
  void setRotate(qreal _rotation_angle, bool _rotate_inplace);
  void setScale(qreal _scale_x, qreal _scale_y, bool _scale_inplace);
  void setShear(qreal _shear_x, qreal _shear_y, bool _shear_inplace);
  void setInterpolation(InterpolationType _type);

  Settings settings;  // public, use as read only
 protected:
  // overloaded methods of QWidget
  virtual void keyPressEvent(QKeyEvent* e) override;
  virtual void paintEvent(QPaintEvent*) override;
  //  virtual void mousePressEvent(QMouseEvent*) override;

 private:
  QImage image;
  //  QImage image_scaled;

  //  Drawer drawer;
  //  QVector<QPoint> points;
};

#endif  // CANVAS_H
