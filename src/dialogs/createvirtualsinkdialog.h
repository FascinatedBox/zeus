#ifndef ZEUSCREATEVIRTUALSINKDIALOG_H
#define ZEUSCREATEVIRTUALSINKDIALOG_H

#include "actions/createvirtualsinkact.h"
#include "dialogs/basedialog.h"
#include <QLineEdit>
#include <QWidget>

class ZeusKVPropertyGroupBox;
class ZeusPulseData;

class ZeusCreateVirtualSinkDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusCreateVirtualSinkDialog(ZeusPulseData *, QWidget *parent = nullptr);

private:
  ZeusCreateVirtualSinkAct *makeAction(void);
  bool isValid(void);

  QLineEdit *m_nameLine;
  ZeusKVPropertyGroupBox *m_propertyBox;
};

#endif
