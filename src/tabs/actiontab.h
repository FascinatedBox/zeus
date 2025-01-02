#ifndef ZEUSACTIONTAB_H
#define ZEUSACTIONTAB_H
#include "actions/baseaction.h"
#include <QWidget>

class QButtonGroup;
class QStackedWidget;
class QTreeWidgetItem;
class QTreeWidget;
class ZeusBaseDialog;
class ZeusCommandEngine;
class ZeusPropertyWindow;
class ZeusPulseData;
class ZeusUserCommand;
class ZeusUserCommandManager;

class ZeusActionTab : public QWidget {
  Q_OBJECT

public:
  ZeusActionTab(ZeusPulseData *pd, ZeusCommandEngine *ce,
                ZeusUserCommandManager *cm);

  void saveCommands(void);
  void takeCommands(QHash<QString, ZeusUserCommand *> commands);

signals:
  void sendActionResult(QPair<int, QString>);
  void sendCommandResults(QPair<QString, QList<QPair<bool, QString>>>);

private slots:
  void onItemDoubleClicked(QTreeWidgetItem *item);
  void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *prev);
  void onActionAccepted(void);
  void onButtonIdClicked(int);

private:
  void addUserCommand(ZeusUserCommand *);
  void createButtonPage(QButtonGroup *group, int start, QStringList textList);
  void showActionDialog(ZeusActionType);
  void showToolWindow(int);
  void setupActionTree(void);
  void setupButtonGroupStack(void);

  ZeusActionType m_actionType;
  ZeusCommandEngine *m_ce;
  ZeusPulseData *m_pd;
  ZeusUserCommandManager *m_cm;
  QHash<QString, ZeusUserCommand *> m_commands;
  QTreeWidget *m_actionTree;
  QTreeWidgetItem *m_userCommandItem;
  QStackedWidget *m_buttonGroupStack;
  ZeusBaseDialog *m_activeDialog;
  ZeusPropertyWindow *m_propertyWindow;
};

#endif
