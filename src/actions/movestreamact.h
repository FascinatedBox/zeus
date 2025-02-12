#ifndef ZEUSMOVESTREAMACT_H
#define ZEUSMOVESTREAMACT_H
#include "actions/baseaction.h"

class QJsonObject;
class ZeusPulseQuery;

class ZeusMoveStreamAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusMoveStreamAct(ZeusPulseQuery *_query, QString _type, QString _target);

  QJsonObject intoJson(void);
  bool isPlayback(void) { return type == "playback"; }
  static ZeusMoveStreamAct *maybeFromJson(QJsonObject &);
  QString treeItemDesc(void);

  ZeusPulseQuery *query;
  QString type;
  QString target;
};

#endif
