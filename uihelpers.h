#ifndef UIHELPERS_H
#define UIHELPERS_H

#include <QtWidgets>

struct ColorPicker : public QWidget {
  QPushButton* button;
  QColor* color;
  QLabel* label;
  QGridLayout* layout;

  ColorPicker(QColor* _color, QString button_text);
  ~ColorPicker();
};

class Separator : public QFrame {
 public:
  Separator();
};


struct Inputs : public QWidget {
  QVBoxLayout* layout;
  QWidget* fields;
  QHBoxLayout* fields_layout;
  QVector<QSpinBox*> ints;
  QVector<QDoubleSpinBox*> doubles;
  QVector<QLabel*> labels;
  QLabel* main_label = nullptr;
  QVector<QCheckBox*> checkboxes;

  Inputs(QString label_text);
  void addIntInput(int min, int val, int max);
  void addDoubleInput(qreal min, qreal val, qreal max);
  void addLabel(QString label_text);
  void setMainLabel(QString label_text);
  void addCheckbox(QString text, bool initial);
  void switchInputs(bool on_off);
  virtual ~Inputs();
};

class DialogStandardButtons : public QDialogButtonBox {
 public:
  DialogStandardButtons(QWidget* parent);
};

/*  ------------------------------------------------------------------------  */

template <typename T>
class TypeRadioButton : public QRadioButton {
  T m_type;

 public:
  TypeRadioButton(QString s) : QRadioButton(s) {}
  void setType(T _type) { m_type = _type; }
  T type() { return m_type; }
};

template <typename T, typename L = QVBoxLayout>
struct ButtonBox : public QWidget {
  QVBoxLayout* layout;
  QGroupBox* box;
  L* box_layout;
  QVector<TypeRadioButton<T>*> buttons;
  QLabel* label = nullptr;
  T checked_value;

  ButtonBox(QString top_label, T _checked_value) : QWidget() {
    checked_value = _checked_value;

    layout = new QVBoxLayout(this);

    box = new QGroupBox(top_label);
    box_layout = new L(box);
    box->setLayout(box_layout);

    layout->addWidget(box);
    this->setLayout(layout);
  }

  void addButton(QString text, T type) {
    TypeRadioButton<T>* button = new TypeRadioButton<T>(text);
    button->setType(type);
    box_layout->addWidget(button);
    buttons.push_back(button);
  }

  void setLabel(QString label_text, bool word_wrap = false) {
    label = new QLabel(label_text);
    label->setWordWrap(word_wrap);
    layout->addWidget(label);
  }

  void initializeChecked() {
    foreach (TypeRadioButton<T>* b, buttons) {
      if (b->type() == checked_value) b->setChecked(true);
    }
  }

  T selectedType() {
    T type;
    foreach (TypeRadioButton<T>* b, buttons) {
      if (b->isChecked()) return type = b->type();
    }
    return type;
  }

  ~ButtonBox() {
    foreach (auto& o, buttons)
      delete o;
    if (label != nullptr) delete label;
    delete box_layout;
    delete box;
    delete layout;
  }
};

#endif  // UIHELPERS_H
