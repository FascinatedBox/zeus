#ifndef ZEUSMOVEPLAYBACKSTREAMACT_H
#define ZEUSMOVEPLAYBACKSTREAMACT_H
#include "actions/baseaction.h"
#include "core/pulsequery.h"

class QJsonObject;
class ZeusPulseQuery;

class ZeusMovePlaybackStreamAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusMovePlaybackStreamAct(ZeusPulseQuery *_query, QString _sinkName);

  QJsonObject intoJson(void);
  static ZeusBaseAction *maybeFromJson(QJsonObject &);

  ZeusPulseQuery *query;
  QString sinkName;
};

#endif
