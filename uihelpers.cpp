#include "uihelpers.h"

ColorPicker::ColorPicker(QColor* _color, QString button_text) {
  color = _color;

  layout = new QGridLayout(this);
  layout->setColumnStretch(1, 1);

  button = new QPushButton(button_text);
  layout->addWidget(button, 0, 0);

  label = new QLabel(color->name());
  label->setPalette(QPalette(*color));
  label->setAutoFillBackground(true);
  layout->addWidget(label, 0, 1);

  connect(button, &QPushButton::pressed, this, [&]() {
    QColor c = QColorDialog::getColor(*color, nullptr, QString(),
                                      QColorDialog::ShowAlphaChannel);
    if (c.isValid()) {
      *color = c;
      label->setText(color->name());
      label->setPalette(QPalette(*color));
    }
  });

  this->setLayout(layout);
}

ColorPicker::~ColorPicker() {
  delete button;
  delete label;
  delete layout;
}


/*  ------------------------------------------------------------------------  */

Separator::Separator() : QFrame() {
  setFrameShape(QFrame::HLine);
  setFrameShadow(QFrame::Sunken);
}


/*  ------------------------------------------------------------------------  */

Inputs::Inputs(QString label_text) {
  layout = new QVBoxLayout(this);
  setMainLabel(label_text);

  fields = new QWidget;
  fields_layout = new QHBoxLayout(fields);
  fields->setLayout(fields_layout);

  layout->addWidget(fields);
  this->setLayout(layout);
}

void Inputs::addIntInput(int min, int val, int max) {
  QSpinBox* box = new QSpinBox;
  box->setMinimum(min);
  box->setValue(val);
  box->setMaximum(max);
  box->setSingleStep((((max - min) / 50 > 0) ? (max - min) / 50 : 1));
  fields_layout->addWidget(box);
  ints.push_back(box);
}

void Inputs::addDoubleInput(qreal min, qreal val, qreal max) {
  QDoubleSpinBox* box = new QDoubleSpinBox;
  box->setMinimum(min);
  box->setValue(val);
  box->setMaximum(max);
  box->setSingleStep((max - min) / 50);
  fields_layout->addWidget(box);
  doubles.push_back(box);
}

void Inputs::addLabel(QString label_text) {
  QLabel* label = new QLabel(label_text);
  fields_layout->addWidget(label);
  labels.push_back(label);
}

void Inputs::setMainLabel(QString label_text) {
  main_label = new QLabel(label_text);
  layout->addWidget(main_label);
}

void Inputs::addCheckbox(QString text, bool initial) {
  QCheckBox* checkbox = new QCheckBox(text);
  checkbox->setChecked(initial);
  layout->addWidget(checkbox);
  checkboxes.push_back(checkbox);
}

void Inputs::switchInputs(bool on_off) {
  foreach (auto& o, ints)
    o->setDisabled(on_off);
  foreach (auto& o, doubles)
    o->setDisabled(on_off);
}

Inputs::~Inputs() {
  foreach (auto& o, ints)
    delete o;
  foreach (auto& o, doubles)
    delete o;
  foreach (auto& o, labels)
    delete o;
  foreach (auto& o, checkboxes)
    delete o;
  if (main_label != nullptr) delete main_label;
  delete layout;
  delete fields_layout;
  delete fields;
}


/*  ------------------------------------------------------------------------  */

DialogStandardButtons::DialogStandardButtons(QWidget* parent)
    : QDialogButtonBox() {
  setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  setOrientation(Qt::Horizontal);
  setCenterButtons(true);

  connect(this, SIGNAL(accepted()), parent, SLOT(accept()));
  connect(this, SIGNAL(rejected()), parent, SLOT(reject()));
}
