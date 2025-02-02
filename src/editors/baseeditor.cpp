#include "editors/baseeditor.h"
#include <QDialogButtonBox>

ZeusBaseEditor::ZeusBaseEditor(QWidget *parent) : QWidget(parent) {
  m_buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(m_buttonBox, &QDialogButtonBox::accepted, [this]() {
    if (isValid())
      emit actionAccepted();
  });
  connect(m_buttonBox, &QDialogButtonBox::rejected,
          [this]() { emit actionRejected(); });
}
