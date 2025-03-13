#include "actions/movestreamact.h"
#include "core/pulsequery.h"
#include <QJsonArray>

ZeusMoveStreamAct::ZeusMoveStreamAct(ZeusPulseQuery *_query, QString _type,
                                     QString _targetName, QString _targetDesc)
    : ZeusBaseAction(ZAMoveStream), query(_query), type(_type),
      targetName(_targetName), targetDesc(_targetDesc) {}

QJsonObject ZeusMoveStreamAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "movestream";
  o["query"] = query->intoJson();
  o["targetname"] = targetName;
  o["targetdesc"] = targetDesc;
  o["type"] = type;
  return o;
}

ZeusMoveStreamAct *ZeusMoveStreamAct::maybeFromJson(QJsonObject &o) {
  QString targetName = o["targetname"].toString("");
  QString targetDesc = o["targetdesc"].toString("");
  QString type = o["type"].toString("");
  ZeusPulseQuery *query = ZeusPulseQuery::maybeFromJson(o["query"]);
  bool isValidType = (type == "playback" || type == "record");

  if (targetName.isEmpty() || isValidType == false || targetDesc.isEmpty()) {
    delete query;
    return nullptr;
  }

  return new ZeusMoveStreamAct(query, type, targetName, targetDesc);
}

QString ZeusMoveStreamAct::treeItemDesc(void) {
  return QString("MoveStream (%1): %2\n%3")
      .arg(type)
      .arg(targetDesc)
      .arg(query->explain());
}
