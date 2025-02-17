#ifndef ZEUSCREATEPIPELINEEDITOR_H
#define ZEUSCREATEPIPELINEEDITOR_H
#include "actions/createpipelineact.h"
#include "editors/baseeditor.h"

class QLineEdit;
class ZeusDeviceComboBox;

class ZeusCreatePipelineEditor : public ZeusBaseEditor {
  Q_OBJECT

public:
  ZeusCreatePipelineEditor(ZeusPulseData *pd, QWidget *parent = nullptr);

  void loadAction(ZeusBaseAction *act);
  ZeusCreatePipelineAct *makeAction(void);

private:
  bool isValid(void);
  void reset(void);

  ZeusDeviceComboBox *m_sourceCombo;
  ZeusDeviceComboBox *m_sinkCombo;
  QLineEdit *m_pipeDesc;
};

#endif
