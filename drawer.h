#ifndef DRAWER_H
#define DRAWER_H

#include <QtWidgets>
#include "debugwindow.h"
#include "settings.h"

class Drawer {
 public:
  Drawer();
  Drawer(QImage* _image, QRgb _main_color);
  void initialize(LineType _line_type, CircleType _circle_type,
                  int _circle_steps, FillType _fill_type,
                  InterpolationType _interpolation_type);

  void setImage(QImage* _image);
  void setMainColor(QRgb _main_color);
  void setDebug(bool _debug, QRgb _debug_color);

  void setLineType(LineType _line_type);
  void setCircleType(CircleType _circle_type);
  void setFillType(FillType _fill_type);
  void setInterpolationType(InterpolationType _interpolation_type);
  void setCircleSteps(int _circle_steps);


  /*  DRAWING  */
  void drawPoint(QPoint point);
  void drawPoint(int x, int y);
  void blendPoint(int x, int y, QRgb _over);

  void drawLine(QPoint start, QPoint end);
  void drawBresenhamLine(QPoint start, QPoint end);
  void drawAntialiasedLine(QPoint start, QPoint end);

  void drawCircle(QPoint centre, QPoint range);
  void drawBresenhamCircle(QPoint centre, uint radius);
  void drawBresenhamCircle(QPoint centre, QPoint range);
  void drawApproximatedCircle(QPoint centre, uint radius, const int segments);
  void drawApproximatedCircle(QPoint centre, QPoint range, const int segments);

  void drawPolygon(QVector<QPoint>& points);

  void drawBezierCurve(QPoint p0, QPoint p1, QPoint p2, QPoint p3);

  void drawBasisSpline(QVector<QPoint>& points);

  /*  PAINTING  */
  void fill(QPoint start, QRgb target_color);

  void fillScanline(QPoint start, QRgb target_color, QRgb prev_color);
  void fillBorderRecursive(QPoint start, QRgb target_color, QRgb border_color);
  void fillBorderRecursive(int x, int y, QRgb target_color, QRgb border_color);
  void fillFloodStack(QPoint start, QRgb target_color);

  void paintVerticalGradient(QRgb start_color, QRgb end_color, int steps);
  void paintVerticalGradient(QColor start_color, QColor end_color, int steps);
  void paintHorizontalGradient(QRgb start_color, QRgb end_color, int steps);
  void paintHorizontalGradient(QColor start_color, QColor end_color, int steps);

  /*  TRANSFORMATIONS  */
  QTransform createShiftMatrix(int shift_x, int shift_y);
  QTransform createRotateMatrix(qreal theta_degrees, bool in_place);
  QTransform createScaleMatrix(qreal scale_x, qreal scale_y, bool in_place);
  QTransform createShearMatrix(qreal shear_x, qreal shear_y, bool in_place);

  void transform(QTransform transformation);

 protected:
  void drawPoint(QPoint point, QRgb color);
  void drawPoint(int x, int y, QRgb color);

  void drawBresenhamLine(QPoint start, QPoint end, QRgb color);
  void drawAntialiasedLine(QPoint start, QPoint end, QRgb color);

  void drawBresenhamCircle(QPoint centre, uint radius, QRgb color);
  void drawBresenhamCircle(QPoint centre, QPoint range, QRgb color);

 private:
  QImage* image;

  QRgb main_color;  // private drawing functions don't use it directly
  bool debug = false;
  QRgb debug_color;

  LineType line_type;
  CircleType circle_type;
  FillType fill_type;
  InterpolationType interpolation_type;
  int circle_steps;

  int width;
  int height;
  QRgb* bits;  // speed up; possibly it can cause problems, not sure

  /*  Line and circle helper methods  */
  inline int determineOctant(QPoint start, QPoint end);
  inline void swapInputs(int& outx, int& outy, const int octant);
  inline void swapOutputs(int& outx, int& outy, const int octant);

  /*  Xiaolin Wu antialiased line  */
  // TODO get rid of it somehow
  inline void _XWdraw(int x0, int y0, int x1, int y1);
  inline void _XWplot(int x, int y, float c);
  inline float _XWipart(float x);
  inline float _XWround(float x);
  inline float _XWfpart(float x);
  inline float _XWrfpart(float x);
  QRgb _XWcolor;

  void getGradientColors(QRgb* colors, QColor start_color, QColor end_color,
                         int steps);

  inline bool _recursiveFloodFillTest(int x, int y, QRgb target_color,
                                      QRgb border_color);

  QTransform toInplaceTransformation(QTransform matrix);

  // image member is always a source for color values
  QRgb interpolateNearest(QPointF coordinates);
  QRgb interpolateBilinear(QPointF coordinates);
};

#endif  // DRAWER_H
