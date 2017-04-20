#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "canvas.h"
#include "uihelpers.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = 0);

 protected:
 private slots:
  void loadFile();
  void saveFile();
  void quit();

  void modeClick();
  void switchDebug();
  void switchGrid();
  void changeScale();
  void settingsGeneral();

  void modeLine();
  void modePolygon();
  void modeCircle();
  void modeBezier();
  void modeSpline();
  void settingsDraw();

  void modeFill();
  void modeHGradient();
  void modeVGradient();
  void settingsPaint();

  void trShift();
  void trRotate();
  void trScale();
  void trShear();
  void settingsTr();

  void about();

 private:
  Canvas* canvas;
  bool debug = false;

  void createActions();
  void createMenus();

  QMenu* file_menu;
  QMenu* general_menu;
  QMenu* draw_menu;
  QMenu* paint_menu;
  QMenu* tr_menu;
  QMenu* about_menu;
  QActionGroup* mode_group;

  QAction* load_file_act;
  QAction* save_file_act;
  QAction* quit_act;

  QAction* mode_click_act;
  QAction* switch_debug_act;
  QAction* switch_grid_act;
  QAction* change_scale_act;
  QAction* settings_general_act;

  QAction* mode_line_act;
  QAction* mode_polygon_act;
  QAction* mode_circle_act;
  QAction* mode_bezier_act;
  QAction* mode_spline_act;
  QAction* settings_draw_act;

  QAction* mode_fill_act;
  QAction* mode_hgradient_act;
  QAction* mode_vgradient_act;
  QAction* settings_paint_act;

  QAction* tr_shift_act;
  QAction* tr_rotate_act;
  QAction* tr_scale_act;
  QAction* tr_shear_act;
  QAction* settings_tr_act;

  QAction* about_act;
  QAction* about_qt_act;
};

#endif  // MAINWINDOW_H
