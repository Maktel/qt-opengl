#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "canvas.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = 0);

 protected:
 private slots:
  void loadFile();
  void saveFile();
  void quit();

 private:
  Canvas* canvas;

  void createActions();
  void createMenus();

  QMenu* file_menu;

  QAction* load_file_act;
  QAction* save_file_act;
  QAction* quit_act;
};

#endif  // MAINWINDOW_H
