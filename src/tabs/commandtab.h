#ifndef ZEUSCOMMANDTAB_H
#define ZEUSCOMMANDTAB_H
#include <QWidget>

class QStackedWidget;
class QTreeWidgetItem;
class QTreeWidget;
class ZeusBaseAction;
class ZeusBaseEditor;
class ZeusCommandContext;
class ZeusCommandEngine;
class ZeusPulseData;
class ZeusUserCommand;
class ZeusUserCommandManager;

class ZeusCommandTab : public QWidget {
  Q_OBJECT

public:
  ZeusCommandTab(ZeusPulseData *pd, ZeusCommandEngine *ce,
                 ZeusUserCommandManager *cm);

  void saveCommands(void);
  void takeCommands(QHash<QString, ZeusUserCommand *> commands);

signals:
  void sendMessage(QString);

public slots:
  void onSendSaveCommands(void);

private slots:
  void onActionAccepted(void);
  void onActionDown(void);
  void onActionRejected(void);
  void onActionUp(void);
  void onCollapseAll(void);
  void onCommandComplete(void);
  void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);
  void onDeleteAction(void);
  void onDeleteCommand(void);
  void onEditAction(void);
  void onErrorClear(void);
  void onExecuteCommand(void);
  void onExpandAll(void);
  void onNewAction(void);
  void onNewCommand(void);

private:
  ZeusBaseAction *actionForTreeItem(QTreeWidgetItem *item);
  QTreeWidgetItem *createItemForAction(ZeusBaseAction *action);
  ZeusBaseEditor *editorForTreeItem(QTreeWidgetItem *item);
  void addCommandErrors(QString name, QList<QString> errors);
  QTreeWidgetItem *addUserCommand(int index, ZeusUserCommand *c);
  void setupEditors(void);
  void setupButtonStack(void);

  bool m_actionToBeAdded;
  ZeusCommandEngine *m_ce;
  ZeusPulseData *m_pd;
  ZeusUserCommandManager *m_cm;
  QHash<QString, ZeusUserCommand *> m_commands;
  QStringList m_actionNameList;
  QTreeWidget *m_commandTree;
  QTreeWidget *m_commandErrorTree;
  QTreeWidgetItem *m_errorItem;
  QTreeWidgetItem *m_userCommandItem;
  QStackedWidget *m_buttonStack;
  QStackedWidget *m_editorStack;
  ZeusBaseEditor *m_activeEditor;
  ZeusCommandContext *m_commandContext;
};

#endif
