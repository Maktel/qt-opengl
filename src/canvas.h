#ifndef CANVAS_H
#define CANVAS_H

#include <QtWidgets>
#include "drawer.h"
#include "settings.h"

class Canvas : public QWidget {
  Q_OBJECT
 public:
  explicit Canvas(QWidget* parent = 0);

  void loadFile(QString file_name);
  void saveFile(QString file_name);
  void setDefaultMode();
  void setMode(Mode _mode);
  void applyOperation(Operation _operation);
  void applySetupDraw();
  void setDebug(bool _debug);
  void switchGrid();
  void setZoom(qreal _scale);
  void setLineType(LineType _line_type);
  void setCircleType(CircleType _circle_type);
  void setFillSettings(bool _fill_random, QColor _fill_color,
                       FillType _fill_type);
  void setGradientSettings(QColor _start_color, QColor _end_color,
                           int _gradient_steps);
  void setMainColor(QColor _main_color);
  void setCircleSteps(int _circle_steps);
  void setDebugColor(QColor _debug_color);
  void setShift(int _shift_x, int _shift_y);
  void setRotate(qreal _rotation_angle, bool _rotate_inplace);
  void setScale(qreal _scale_x, qreal _scale_y, bool _scale_inplace);
  void setShear(qreal _shear_x, qreal _shear_y, bool _shear_inplace);
  void setInterpolationType(InterpolationType _type);
  void clear(QColor color = Qt::GlobalColor::white);

  /* TESTS */
  void example1();
  void example2();
  void example3();
  void example4();

  Settings settings;  // public, use as read only

 signals:
  void changedToDefaultMode();
  void changedSetupMode(bool setup);

 protected:
  // overloaded methods of QWidget
  virtual void keyPressEvent(QKeyEvent* e) override;
  virtual void paintEvent(QPaintEvent*) override;
  virtual void mousePressEvent(QMouseEvent* e) override;
  virtual void mouseMoveEvent(QMouseEvent* e) override;

 private:
  QImage image;
  //  QImage image_scaled;

  Drawer drawer;
  QVector<QPoint> points;

  void processMousePress(int mouse_x, int mouse_y);
  void redraw();
};

#endif  // CANVAS_H
