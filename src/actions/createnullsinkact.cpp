#include "actions/createnullsinkact.h"
#include "core/utils.h"
#include <QJsonArray>
#include <QJsonObject>

ZeusCreateNullSinkAct::ZeusCreateNullSinkAct(QString _name)
    : ZeusBaseAction(ZACreateNullSink), name(_name) {}

QJsonObject ZeusCreateNullSinkAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "createnullsink";
  o["name"] = name;
  return o;
}

ZeusBaseAction *ZeusCreateNullSinkAct::maybeFromJson(QJsonObject &o) {
  QString name = o["name"].toString("");

  if (::isValidPulseDeviceName(name) == false)
    return nullptr;

  return new ZeusCreateNullSinkAct(name);
}

QString ZeusCreateNullSinkAct::treeItemDesc(void) {
  return QString("CreateNullSink: %1").arg(name);
}
