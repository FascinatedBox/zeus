#include "destroyvirtualsinkact.h"
#include <QJsonObject>

ZeusDestroyVirtualSinkAct::ZeusDestroyVirtualSinkAct(QString name_,
                                                     QString desc_)
    : ZeusBaseAction(ZADestroyVirtualSink), name(name_), desc(desc_) {}

QJsonObject ZeusDestroyVirtualSinkAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "destroyvirtualsink";
  o["name"] = name;
  o["desc"] = desc;
  return o;
}

ZeusBaseAction *ZeusDestroyVirtualSinkAct::maybeFromJson(QJsonObject &o) {
  QString name = o["name"].toString("");
  QString desc = o["desc"].toString("");

  if (name.isEmpty() || desc.isEmpty())
    return nullptr;

  return new ZeusDestroyVirtualSinkAct(name, desc);
}

QString ZeusDestroyVirtualSinkAct::treeItemDesc(void) {
  return QString("DestroyVirtualSink: %1").arg(desc);
}
