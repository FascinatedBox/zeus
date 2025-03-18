#ifndef ZEUSDESTROYPIPELINEEDITOR_H
#define ZEUSDESTROYPIPELINEEDITOR_H
#include "actions/destroypipelineact.h"
#include "editors/baseeditor.h"

class ZeusPipelineSinkInputComboBox;
class ZeusPulseData;

class ZeusDestroyPipelineEditor : public ZeusBaseEditor {
  Q_OBJECT

public:
  ZeusDestroyPipelineEditor(ZeusPulseData *pd, QWidget *parent = nullptr);

  void loadAction(ZeusBaseAction *act);
  ZeusDestroyPipelineAct *makeAction(void);
  void reset(void);

private:
  bool isValid(void);

  ZeusPipelineSinkInputComboBox *m_sinkInputCombo;
};

#endif
