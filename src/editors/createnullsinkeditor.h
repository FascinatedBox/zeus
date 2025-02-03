#ifndef ZEUSCREATENULLSINKEDITOR_H
#define ZEUSCREATENULLSINKEDITOR_H
#include "actions/createnullsinkact.h"
#include "editors/baseeditor.h"
#include <QLineEdit>

class ZeusPulseData;

class ZeusCreateNullSinkEditor : public ZeusBaseEditor {
  Q_OBJECT

public:
  ZeusCreateNullSinkEditor(ZeusPulseData *, QWidget *parent = nullptr);

  void loadAction(ZeusBaseAction *act);
  void reset(void);

private:
  ZeusCreateNullSinkAct *makeAction(void);
  bool isValid(void);

  QLineEdit *m_nameLine;
};

#endif
