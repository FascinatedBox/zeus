#ifndef ZEUSCREATEVIRTUALSINKDIALOG_H
#define ZEUSCREATEVIRTUALSINKDIALOG_H

#include "actions/createvirtualsinkact.h"
#include "dialogs/basedialog.h"
#include <QLineEdit>
#include <QWidget>

class ZeusPropertyGroupBox;

class ZeusCreateVirtualSinkDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusCreateVirtualSinkDialog(QWidget *parent = nullptr);

  void connectToPulseData(ZeusPulseData *pd);
  ZeusCreateVirtualSinkAct *makeAction(void);

private:
  bool isValid(void);

  QLineEdit *m_nameLine;
  ZeusPropertyGroupBox *m_propertyBox;
};

#endif
