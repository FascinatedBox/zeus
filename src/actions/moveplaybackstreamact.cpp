#include "actions/moveplaybackstreamact.h"
#include "core/pulsequery.h"
#include <QJsonArray>

ZeusMovePlaybackStreamAct::ZeusMovePlaybackStreamAct(ZeusPulseQuery *_query,
                                                     QString _sinkName)
    : ZeusBaseAction(ZAMovePlaybackStream), query(_query), sinkName(_sinkName) {
}

QJsonObject ZeusMovePlaybackStreamAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "moveplaybackstream";
  o["query"] = query->intoJson();
  o["sinkName"] = sinkName;
  return o;
}

ZeusBaseAction *ZeusMovePlaybackStreamAct::maybeFromJson(QJsonObject &o) {
  QString sinkName = o["sinkName"].toString("");
  ZeusPulseQuery *query = ZeusPulseQuery::maybeFromJson(o["query"]);

  if (sinkName.isEmpty()) {
    delete query;
    return nullptr;
  }

  auto result = new ZeusMovePlaybackStreamAct(query, sinkName);
  return static_cast<ZeusBaseAction *>(result);
}
