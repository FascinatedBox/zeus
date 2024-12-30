#ifndef ZEUSCREATEPIPELINEDIALOG_H
#define ZEUSCREATEPIPELINEDIALOG_H
#include "actions/createpipelineact.h"
#include "dialogs/basedialog.h"

class ZeusDeviceComboBox;

class ZeusCreatePipelineDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusCreatePipelineDialog(ZeusPulseData *pd, QWidget *parent = nullptr);

  ZeusCreatePipelineAct *makeAction(void);

private:
  bool isValid(void);

  ZeusDeviceComboBox *m_sourceCombo;
  ZeusDeviceComboBox *m_sinkCombo;
};

#endif
