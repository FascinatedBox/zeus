#ifndef ZEUSCREATEVIRTUALSINKEDITOR_H
#define ZEUSCREATEVIRTUALSINKEDITOR_H
#include "actions/createvirtualsinkact.h"
#include "editors/baseeditor.h"
#include <QLineEdit>
#include <QWidget>

class ZeusKVPropertyGroupBox;
class ZeusPulseData;

class ZeusCreateVirtualSinkEditor : public ZeusBaseEditor {
  Q_OBJECT

public:
  ZeusCreateVirtualSinkEditor(ZeusPulseData *, QWidget *parent = nullptr);

  void loadAction(ZeusBaseAction *act);
  void reset(void);

private:
  ZeusCreateVirtualSinkAct *makeAction(void);
  bool isValid(void);

  QLineEdit *m_nameLine;
  ZeusKVPropertyGroupBox *m_propertyBox;
};

#endif
