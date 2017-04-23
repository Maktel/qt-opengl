#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QtWidgets>

class DebugWindow : public QWidget {
  Q_OBJECT

 private:
  QImage* image;

 protected:
  virtual void paintEvent(QPaintEvent*);

 public:
  DebugWindow(QImage* _image, QWidget* parent = 0);
  static void waitFor(int milliseconds);
  void redraw();
};
#endif  // DEBUGWINDOW_H
