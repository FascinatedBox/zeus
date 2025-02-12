#ifndef ZEUSACTIONTAB_H
#define ZEUSACTIONTAB_H
#include "actions/baseaction.h"
#include <QWidget>

class QButtonGroup;
class QStackedWidget;
class QTreeWidgetItem;
class QTreeWidget;
class ZeusBaseEditor;
class ZeusCommandEngine;
class ZeusPulseData;

class ZeusActionTab : public QWidget {
  Q_OBJECT

public:
  ZeusActionTab(ZeusPulseData *pd, ZeusCommandEngine *ce);

signals:
  void sendActionResult(QPair<int, QString>);

private slots:
  void onActionAccepted(void);
  void onActionRejected(void);
  void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);

private:
  void setupActions(void);

  ZeusActionType m_actionType;
  ZeusCommandEngine *m_ce;
  ZeusPulseData *m_pd;
  QTreeWidget *m_actionTree;
  QStackedWidget *m_editorStack;
  QStackedWidget *m_buttonGroupStack;
};

#endif
