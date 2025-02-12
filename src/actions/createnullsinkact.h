#ifndef ZEUSCREATENULLSINKACT_H
#define ZEUSCREATENULLSINKACT_H
#include "actions/baseaction.h"

class ZeusCreateNullSinkAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusCreateNullSinkAct(QString _name);

  QJsonObject intoJson(void);
  static ZeusBaseAction *maybeFromJson(QJsonObject &);
  QString treeItemDesc(void);

  QString name;
};

#endif
