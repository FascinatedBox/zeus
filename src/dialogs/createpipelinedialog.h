#ifndef ZEUSCREATEPIPELINEDIALOG_H
#define ZEUSCREATEPIPELINEDIALOG_H
#include "actions/createpipelineact.h"
#include "dialogs/basedialog.h"

class ZeusDeviceComboBox;

class ZeusCreatePipelineDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusCreatePipelineDialog(QWidget *parent = nullptr);
  void connectToPulseData(ZeusPulseData *pd);
  ZeusCreatePipelineAct *makeAction(void);

private slots:
  void onSinkAdded(uint32_t index, ZeusPulseDeviceInfo *info);
  void onSinkRemoved(uint32_t index);
  void onSourceAdded(uint32_t index, ZeusPulseDeviceInfo *info);
  void onSourceRemoved(uint32_t index);

private:
  bool isValid(void);
  void loadDeviceList(ZeusPulseData *pd, ZeusDeviceComboBox *combo);

  ZeusDeviceComboBox *m_sourceCombo;
  ZeusDeviceComboBox *m_sinkCombo;
};

#endif
