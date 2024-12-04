#ifndef ZEUSBASEDIALOG_H
#define ZEUSBASEDIALOG_H
#include "actions/baseaction.h"
#include <QDialog>

class QDialogButtonBox;

class ZeusBaseDialog : public QDialog {
  Q_OBJECT

public:
  ZeusBaseDialog(QWidget *parent = nullptr);

  virtual ZeusBaseAction *makeAction(void) = 0;

signals:
  void actionAccepted(void);

protected:
  virtual bool isValid(void) = 0;
  QDialogButtonBox *m_buttonBox;
};

#endif
