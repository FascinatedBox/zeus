#ifndef ZEUSUSERCOMMAND_H
#define ZEUSUSERCOMMAND_H
#include "actions/baseaction.h"
#include <QHash>

class QJsonObject;
class ZeusCommandEngine;
typedef QPair<QString, QList<ZeusBaseAction *>> ZeusUserCommand;

class ZeusUserCommandManager : public QObject {
  Q_OBJECT

public:
  ZeusUserCommandManager(ZeusCommandEngine *ce);

  ZeusActionType actionTypeForName(QString name);
  QList<ZeusUserCommand> commands(void) { return m_commands; }
  void addCommandAction(int commandIndex, ZeusBaseAction *a);
  void addNewCommand(QString name);
  void execCommandAtIndex(int commandIndex);
  void loadCommands(void);
  void removeCommandAt(int commandIndex);
  void removeCommandAction(int commandIndex, int actionIndex);
  void saveCommands(void);

private:
  void loadJson(QJsonObject &o);
  void saveJson(QJsonObject &o);
  ZeusBaseAction *callLoadFnByName(QString name, QJsonObject &o);

  ZeusCommandEngine *m_ce;
  QHash<QString, int> m_loadMap;
  QList<ZeusUserCommand> m_commands;
};

#endif
