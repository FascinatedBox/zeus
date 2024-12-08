#ifndef ZEUSACTIONTAB_H
#define ZEUSACTIONTAB_H
#include "actions/baseaction.h"
#include <QWidget>
#include <pulse/pulseaudio.h>

class QTreeWidgetItem;
class QTreeWidget;
class ZeusBaseDialog;
class ZeusCommandEngine;

class ZeusActionTab : public QWidget {
  Q_OBJECT

public:
  ZeusActionTab(ZeusCommandEngine *ce);

private slots:
  void onItemDoubleClicked(QTreeWidgetItem *item);
  void onActionAccepted(void);

private:
  void setupActionTree(void);

  ZeusActionType m_actionType;
  ZeusCommandEngine *m_ce;
  QTreeWidget *m_actionTree;
  ZeusBaseDialog *m_activeDialog;
};

#endif
