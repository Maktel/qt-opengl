#include "debugwindow.h"

DebugWindow::DebugWindow(QImage* _image, QWidget* parent) : QWidget(parent) {
  image = _image;
  this->setFixedSize(image->width(), image->height());
  this->show();
}

void DebugWindow::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.drawImage(0, 0, *image);
}

void DebugWindow::redraw() { update(); }

void DebugWindow::waitFor(int milliseconds) {
  QTime dieTime = QTime::currentTime().addMSecs(milliseconds);
  while (QTime::currentTime() < dieTime) {
    QCoreApplication::processEvents(QEventLoop::AllEvents,
                                    (milliseconds < 100) ? milliseconds : 100);
  }
}
