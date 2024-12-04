#include "dialogs/basedialog.h"
#include <QDialogButtonBox>

ZeusBaseDialog::ZeusBaseDialog(QWidget *parent) : QDialog(parent) {
  m_buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(m_buttonBox, &QDialogButtonBox::accepted, [this]() {
    if (isValid())
      emit actionAccepted();
  });
  connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
