#include "core/usercommand.h"
#include "actions/createnullsinkact.h"
#include "actions/createpipelineact.h"
#include "actions/createvirtualsinkact.h"
#include "actions/destroypipelineact.h"
#include "actions/destroyvirtualsinkact.h"
#include "actions/movestreamact.h"
#include "core/commandengine.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#define ZEUS_DIR ".config/zeus/"
#define ZEUS_JSON_PATH (QDir::homePath() + ("/" ZEUS_DIR "zeus.json"))

void ZeusUserCommand::moveActionDown(int index) {
  ZeusBaseAction *act = m_actions.takeAt(index);

  m_actions.insert(index + 1, act);
}

void ZeusUserCommand::moveActionUp(int index) {
  ZeusBaseAction *act = m_actions.takeAt(index);

  m_actions.insert(index - 1, act);
}

void ZeusUserCommand::replaceActionAt(int index, ZeusBaseAction *act) {
  ZeusBaseAction *oldAct = m_actions.at(index);

  m_actions.replace(index, act);
  delete oldAct;
}

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

QHash<QString, ZeusUserCommand *>
ZeusUserCommandManager::loadJson(QJsonObject &root) {
  QHash<QString, ZeusUserCommand *> result;
  QJsonObject cmdObj = root["commands"].toObject();
  QJsonObject::iterator cmdIter;

  for (cmdIter = cmdObj.begin(); cmdIter != cmdObj.end(); cmdIter++) {
    QString commandName = cmdIter.key();
    QJsonArray actionList = cmdIter.value().toArray();

    if (commandName.isEmpty())
      continue;

    ZeusUserCommand *c = new ZeusUserCommand(commandName);
    QJsonArray::iterator actIter;

    for (actIter = actionList.begin(); actIter != actionList.end(); actIter++) {
      QJsonObject act = (*actIter).toObject();
      QString actionName = act["action"].toString();
      ZeusBaseAction *a = callLoadFnByName(actionName, act);

      if (a == nullptr)
        continue;

      c->append(a);
    }

    result[commandName] = c;
  }

  return result;
}

QHash<QString, ZeusUserCommand *> ZeusUserCommandManager::loadCommands(void) {
  QFile f(ZEUS_JSON_PATH);

  if (f.open(QIODevice::ReadOnly) == false)
    return QHash<QString, ZeusUserCommand *>();

  QByteArray ba = f.readAll();
  QJsonDocument d(QJsonDocument::fromJson(ba));
  QJsonObject o = d.object();

  f.close();
  return loadJson(o);
}

void ZeusUserCommandManager::saveJson(
    QJsonObject &root, QHash<QString, ZeusUserCommand *> commands) {
  QJsonObject toplevelCmdObj;
  auto cmdIter = QHashIterator(commands);

  while (cmdIter.hasNext()) {
    cmdIter.next();

    auto cmd = cmdIter.value();
    auto iter = cmd->actionIterator();
    QJsonArray actionList;

    while (iter.hasNext())
      actionList.append(iter.next()->intoJson());

    toplevelCmdObj[cmd->name()] = actionList;
  }

  root["commands"] = toplevelCmdObj;
}

void ZeusUserCommandManager::saveCommands(
    QHash<QString, ZeusUserCommand *> commands) {
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

  saveJson(o, commands);
  f.write(QJsonDocument(o).toJson());
  f.close();
}
