#include "actions/createvirtualsinkact.h"
#include <QJsonObject>

ZeusCreateVirtualSinkAct::ZeusCreateVirtualSinkAct(QString _name)
    : ZeusBaseAction(ZACreateVirtualSink), name(_name) {}

QJsonObject ZeusCreateVirtualSinkAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "createvirtualsink";
  o["name"] = name;
  return o;
}

ZeusBaseAction *ZeusCreateVirtualSinkAct::maybeFromJson(QJsonObject &o) {
  QString name = o["name"].toString("");

  if (name.isEmpty())
    return nullptr;

  auto result = new ZeusCreateVirtualSinkAct(name);
  return static_cast<ZeusBaseAction *>(result);
}
