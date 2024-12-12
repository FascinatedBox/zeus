#ifndef ZEUSDESTROYVIRTUALSINKDIALOG_H
#define ZEUSDESTROYVIRTUALSINKDIALOG_H
#include "actions/destroyvirtualsinkact.h"
#include "dialogs/basedialog.h"
#include <QLineEdit>

class ZeusDeviceComboBox;

class ZeusDestroyVirtualSinkDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusDestroyVirtualSinkDialog(QWidget *parent = nullptr);
  void connectToPulseData(ZeusPulseData *pd);
  ZeusDestroyVirtualSinkAct *makeAction(void);

private slots:
  void onSinkAdded(uint32_t index, ZeusPulseDeviceInfo *info);
  void onSinkRemoved(uint32_t index);

private:
  bool isValid(void);
  void loadDeviceList(ZeusPulseData *pd);

  ZeusDeviceComboBox *m_sinkCombo;
};

#endif
