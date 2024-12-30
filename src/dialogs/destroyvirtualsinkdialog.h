#ifndef ZEUSDESTROYVIRTUALSINKDIALOG_H
#define ZEUSDESTROYVIRTUALSINKDIALOG_H
#include "actions/destroyvirtualsinkact.h"
#include "dialogs/basedialog.h"
#include <QLineEdit>

class ZeusVirtualDeviceComboBox;
class ZeusPulseData;

class ZeusDestroyVirtualSinkDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusDestroyVirtualSinkDialog(ZeusPulseData *pd, QWidget *parent = nullptr);

  ZeusDestroyVirtualSinkAct *makeAction(void);

private:
  bool isValid(void);

  ZeusVirtualDeviceComboBox *m_sinkCombo;
};

#endif
