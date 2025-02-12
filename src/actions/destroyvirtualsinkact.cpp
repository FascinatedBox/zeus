#include "destroyvirtualsinkact.h"
#include <QJsonObject>

ZeusDestroyVirtualSinkAct::ZeusDestroyVirtualSinkAct(QString name_)
    : ZeusBaseAction(ZADestroyVirtualSink), name(name_) {}

QJsonObject ZeusDestroyVirtualSinkAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "destroyvirtualsink";
  o["name"] = name;
  return o;
}

ZeusBaseAction *ZeusDestroyVirtualSinkAct::maybeFromJson(QJsonObject &o) {
  QString name = o["name"].toString("");

  if (name.isEmpty())
    return nullptr;

  return new ZeusDestroyVirtualSinkAct(name);
}

QString ZeusDestroyVirtualSinkAct::treeItemDesc(void) {
  return QString("DestroyVirtualSink: %1").arg(name);
}
