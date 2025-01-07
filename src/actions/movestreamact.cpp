#include "actions/movestreamact.h"
#include "core/pulsequery.h"
#include <QJsonArray>

ZeusMoveStreamAct::ZeusMoveStreamAct(ZeusPulseQuery *_query, QString _type,
                                     QString _target)
    : ZeusBaseAction(ZAMoveStream), query(_query), type(_type),
      target(_target) {}

QJsonObject ZeusMoveStreamAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "movestream";
  o["query"] = query->intoJson();
  o["target"] = target;
  o["type"] = type;
  return o;
}

ZeusMoveStreamAct *ZeusMoveStreamAct::maybeFromJson(QJsonObject &o) {
  QString target = o["target"].toString("");
  QString type = o["type"].toString("");
  ZeusPulseQuery *query = ZeusPulseQuery::maybeFromJson(o["query"]);

  if (target.isEmpty() || (type != "playback" && type != "record")) {
    delete query;
    return nullptr;
  }

  return new ZeusMoveStreamAct(query, type, target);
}
