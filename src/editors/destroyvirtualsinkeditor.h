#ifndef ZEUSDESTROYVIRTUALSINKEDITOR_H
#define ZEUSDESTROYVIRTUALSINKEDITOR_H
#include "actions/destroyvirtualsinkact.h"
#include "editors/baseeditor.h"
#include <QLineEdit>

class ZeusVirtualDeviceComboBox;
class ZeusPulseData;

class ZeusDestroyVirtualSinkEditor : public ZeusBaseEditor {
  Q_OBJECT

public:
  ZeusDestroyVirtualSinkEditor(ZeusPulseData *pd, QWidget *parent = nullptr);

  void loadAction(ZeusBaseAction *act);
  ZeusDestroyVirtualSinkAct *makeAction(void);
  void reset(void);

private:
  bool isValid(void);

  ZeusVirtualDeviceComboBox *m_sinkCombo;
};

#endif
