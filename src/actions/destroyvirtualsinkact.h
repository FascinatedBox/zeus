#ifndef ZEUSDESTROYVIRTUALSINKACT_H
#define ZEUSDESTROYVIRTUALSINKACT_H
#include "actions/baseaction.h"

class ZeusDestroyVirtualSinkAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusDestroyVirtualSinkAct(QString _name);

  QJsonObject intoJson(void);
  static ZeusBaseAction *maybeFromJson(QJsonObject &);

  QString name;
};

#endif
