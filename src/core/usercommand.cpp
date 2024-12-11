#include "core/usercommand.h"
#include "actions/createpipelineact.h"
#include "actions/createvirtualsinkact.h"
#include "core/commandengine.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#define ZEUS_DIR ".config/zeus/"
#define ZEUS_JSON_PATH (QDir::homePath() + ("/" ZEUS_DIR "zeus.json"))

ZeusUserCommandManager::ZeusUserCommandManager(ZeusCommandEngine *ce)
    : m_ce(ce) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  m_loadMap[#lowername] = ZeusActionType::ZA##TitleName;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
}

ZeusBaseAction *ZeusUserCommandManager::callLoadFnByName(QString name,
                                                         QJsonObject &o) {
  if (m_loadMap.contains(name) == false)
    return nullptr;

  ZeusBaseAction *result = nullptr;

  switch ((ZeusActionType)m_loadMap[name]) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    result = Zeus##TitleName##Act::maybeFromJson(o);                           \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
    break;
  }

  return result;
}

void ZeusUserCommandManager::loadJson(QJsonObject &root) {
  QJsonArray commandList = root["commands"].toArray();
  QList<ZeusUserCommand> userCommands;

  for (int command_i = 0; command_i < commandList.size(); command_i++) {
    QJsonObject o = commandList[command_i].toObject();
    QString commandName = o["name"].toString();
    QJsonArray actionList = o["actions"].toArray();
    QList<ZeusBaseAction *> commandActions;

    if (commandName.isEmpty())
      continue;

    for (int action_j = 0; action_j < actionList.size(); action_j++) {
      QJsonObject act = actionList[action_j].toObject();
      QString actionName = act["action"].toString();
      ZeusBaseAction *a = callLoadFnByName(actionName, act);

      if (a == nullptr)
        continue;

      commandActions.append(a);
    }

    auto p = qMakePair(commandName, commandActions);
    userCommands.append(p);
  }

  m_commands = userCommands;
}

void ZeusUserCommandManager::loadCommands(void) {
  QFile f(ZEUS_JSON_PATH);

  if (f.open(QIODevice::ReadOnly) == false)
    return;

  QByteArray ba = f.readAll();
  QJsonDocument d(QJsonDocument::fromJson(ba));
  QJsonObject o = d.object();

  loadJson(o);
  f.close();
}

void ZeusUserCommandManager::saveJson(QJsonObject &root) {
  QJsonArray commandList;

  for (int commands_i = 0; commands_i < m_commands.size(); commands_i++) {
    auto cmd = m_commands[commands_i];
    auto actions = cmd.second;
    QJsonObject cmdObject;
    QJsonArray actionList;

    cmdObject["name"] = cmd.first;

    for (int action_j = 0; action_j < actions.size(); action_j++) {
      ZeusBaseAction *a = actions[action_j];

      actionList.append(a->intoJson());
    }

    cmdObject["actions"] = actionList;
    commandList.append(cmdObject);
  }

  root["commands"] = commandList;
}

void ZeusUserCommandManager::saveCommands(void) {
  QFile f(ZEUS_JSON_PATH);
  QJsonObject o;

  if (f.open(QIODevice::WriteOnly) == false) {
    // Maybe ~/.config/zeus/ needs to be made first?
    QDir configDir = QDir(QDir::homePath());

    if (configDir.exists(ZEUS_DIR) == true ||
        configDir.mkdir(ZEUS_DIR) == false ||
        f.open(QIODevice::WriteOnly) == false)
      // I have no more ideas. :(
      return;
  }

  saveJson(o);
  f.write(QJsonDocument(o).toJson());
  f.close();
}

ZeusActionType ZeusUserCommandManager::actionTypeForName(QString name) {
  return static_cast<ZeusActionType>(
      m_loadMap.value(name, ZeusActionType::ZANone));
}

void ZeusUserCommandManager::addCommandAction(int commandIndex,
                                              ZeusBaseAction *a) {
  m_commands[commandIndex].second.append(a);
}

void ZeusUserCommandManager::addNewCommand(QString name) {
  auto c = qMakePair(name, QList<ZeusBaseAction *>());
  m_commands.append(c);
}

void ZeusUserCommandManager::execCommandAtIndex(int commandIndex) {
  QList<ZeusBaseAction *> actionList = m_commands.at(commandIndex).second;

  foreach (ZeusBaseAction *a, actionList)
    m_ce->execAction(a);
}

void ZeusUserCommandManager::removeCommandAt(int commandIndex) {
  ZeusUserCommand c = m_commands.takeAt(commandIndex);

  foreach (ZeusBaseAction *a, c.second)
    delete a;
}

void ZeusUserCommandManager::removeCommandAction(int commandIndex,
                                                 int actionIndex) {
  delete m_commands[commandIndex].second.takeAt(actionIndex);
}
