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

private slots:
  void onDeviceTextChanged(QString);

private:
  bool isValid(void);
  void reset(void);

  ZeusDeviceComboBox *m_sourceCombo;
  ZeusDeviceComboBox *m_sinkCombo;
  QLineEdit *m_pipeDesc;
  QRegularExpression *m_pipeRegex;
  bool m_skipChange;
};

#endif
