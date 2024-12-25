#ifndef ZEUSCREATEVIRTUALSINKDIALOG_H
#define ZEUSCREATEVIRTUALSINKDIALOG_H

#include "actions/createvirtualsinkact.h"
#include "dialogs/basedialog.h"
#include <QLineEdit>
#include <QWidget>

class ZeusKVPropertyGroupBox;

class ZeusCreateVirtualSinkDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusCreateVirtualSinkDialog(QWidget *parent = nullptr);

  void connectToPulseData(ZeusPulseData *pd);
  ZeusCreateVirtualSinkAct *makeAction(void);

private:
  bool isValid(void);

  QLineEdit *m_nameLine;
  ZeusKVPropertyGroupBox *m_propertyBox;
};

#endif
