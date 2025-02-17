#include "actions/movestreamact.h"
#include "core/pulsequery.h"
#include <QJsonArray>

ZeusMoveStreamAct::ZeusMoveStreamAct(ZeusPulseQuery *_query, QString _type,
                                     QString _target, QString _targetDesc)
    : ZeusBaseAction(ZAMoveStream), query(_query), type(_type), target(_target),
      targetDesc(_targetDesc) {}

QJsonObject ZeusMoveStreamAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "movestream";
  o["query"] = query->intoJson();
  o["target"] = target;
  o["type"] = type;
  o["targetdesc"] = targetDesc;
  return o;
}

ZeusMoveStreamAct *ZeusMoveStreamAct::maybeFromJson(QJsonObject &o) {
  QString target = o["target"].toString("");
  QString type = o["type"].toString("");
  QString targetDesc = o["targetdesc"].toString("");
  ZeusPulseQuery *query = ZeusPulseQuery::maybeFromJson(o["query"]);
  bool isValidType = (type == "playback" || type == "record");

  if (target.isEmpty() || isValidType == false || targetDesc.isEmpty()) {
    delete query;
    return nullptr;
  }

  return new ZeusMoveStreamAct(query, type, target, targetDesc);
}

QString ZeusMoveStreamAct::treeItemDesc(void) {
  return QString("MoveStream (%1): %2\n%3")
      .arg(type)
      .arg(targetDesc)
      .arg(query->explain());
}
