#ifndef ZEUSUSERCOMMAND_H
#define ZEUSUSERCOMMAND_H
#include "actions/baseaction.h"
#include <QHash>

class QJsonObject;
class ZeusCommandEngine;

class ZeusUserCommand : public QObject {
  Q_OBJECT

public:
  ZeusUserCommand(QString _name) : m_name(_name) {}
  ~ZeusUserCommand(void) {
    foreach (auto a, m_actions)
      delete a;
  }

  ZeusBaseAction *actionAt(int index) { return m_actions[index]; }
  QListIterator<ZeusBaseAction *> actionIterator(void) {
    return QListIterator(m_actions);
  }
  void append(ZeusBaseAction *a) { m_actions.append(a); }
  QString name(void) { return m_name; }
  void deleteActionAt(int index) { delete m_actions.takeAt(index); }
  void moveActionDown(int index);
  void moveActionUp(int index);
  void replaceActionAt(int index, ZeusBaseAction *act);

private:
  QString m_name;
  QList<ZeusBaseAction *> m_actions;
};

class ZeusUserCommandManager : public QObject {
  Q_OBJECT

public:
  ZeusUserCommandManager(ZeusCommandEngine *ce);

  QHash<QString, ZeusUserCommand *> loadCommands(void);
  void saveCommands(QHash<QString, ZeusUserCommand *> commands);

private:
  QHash<QString, ZeusUserCommand *> loadJson(QJsonObject &o);
  void saveJson(QJsonObject &o, QHash<QString, ZeusUserCommand *> commands);
  ZeusBaseAction *callLoadFnByName(QString name, QJsonObject &o);

  ZeusCommandEngine *m_ce;
  QHash<QString, int> m_loadMap;
};

#endif
