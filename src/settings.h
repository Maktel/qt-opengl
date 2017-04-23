#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets>

// operations applied with additional info from user (mouse clicks)
enum class Mode { none, click, line, circle, bezier, spline, polygon, fill };

// operations that can be applied immediately
enum class Operation { vgradient, hgradient, shift, rotate, scale, shear };

enum class LineType { bresenham, antialiased };
enum class CircleType { bresenham, approximated };
enum class FillType { scanline, stack, recursive };
enum class InterpolationType { nearest, bilinear };

struct Settings {
  int width;
  int height;

  Mode mode;
  bool debug;
  bool grid;
  qreal zoom;
  LineType line_type;
  CircleType circle_type;
  QColor fill_color;
  bool fill_random;
  FillType fill_type;
  QColor start_color;
  QColor end_color;
  int gradient_steps;
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
    width = 800;
    height = 800;

    mode = Mode::click;
    debug = false;
    grid = false;
    zoom = 1.0;
    line_type = LineType::bresenham;
    circle_type = CircleType::bresenham;
    fill_color = QColor(255, 255, 255, 255);
    fill_random = true;
    fill_type = FillType::scanline;
    start_color = QColor(255, 0, 0);
    end_color = QColor(128, 0, 255);
    gradient_steps = 16;
    main_color = QColor(192, 255, 63);
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
    dbg.nospace() << "\nscale: " << sett.zoom;
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

#endif  // SETTINGS_H
