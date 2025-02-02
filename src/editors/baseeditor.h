#ifndef ZEUSBASEEDITOR_H
#define ZEUSBASEEDITOR_H
#include "actions/baseaction.h"
#include "core/pulsedata.h"
#include <QWidget>

class QDialogButtonBox;

class ZeusBaseEditor : public QWidget {
  Q_OBJECT

public:
  ZeusBaseEditor(QWidget *parent = nullptr);

  virtual ZeusBaseAction *makeAction(void) = 0;
  virtual void loadAction(ZeusBaseAction *) = 0;
  virtual void reset(void) = 0;

signals:
  void actionAccepted(void);
  void actionRejected(void);

protected:
  virtual bool isValid(void) = 0;
  QDialogButtonBox *m_buttonBox;
};

#endif
