#ifndef ZEUSCOMMANDTAB_H
#define ZEUSCOMMANDTAB_H
#include <QWidget>

class QStackedWidget;
class QTreeWidgetItem;
class QTreeWidget;
class ZeusBaseAction;
class ZeusBaseEditor;
class ZeusCommandEngine;
class ZeusPropertyWindow;
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
  void sendActionResult(QPair<bool, QString>);
  void sendCommandResults(QPair<QString, QList<QPair<bool, QString>>>);
  void sendMessage(QString);

public slots:
  void onSendSaveCommands(void);

private slots:
  void onActionAccepted(void);
  void onActionDown(void);
  void onActionRejected(void);
  void onActionUp(void);
  void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);
  void onDeleteAction(void);
  void onDeleteCommand(void);
  void onEditAction(void);
  void onExecuteCommand(void);
  void onNewAction(void);
  void onNewCommand(void);

private:
  ZeusBaseAction *actionForTreeItem(QTreeWidgetItem *item);
  QTreeWidgetItem *createItemForAction(ZeusBaseAction *action);
  ZeusBaseEditor *editorForTreeItem(QTreeWidgetItem *item);
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
  QTreeWidgetItem *m_userCommandItem;
  QStackedWidget *m_buttonStack;
  QStackedWidget *m_editorStack;
  ZeusBaseEditor *m_activeEditor;
  ZeusPropertyWindow *m_propertyWindow;
};

#endif
