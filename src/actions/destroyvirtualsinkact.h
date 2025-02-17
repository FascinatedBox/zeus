#ifndef ZEUSDESTROYVIRTUALSINKACT_H
#define ZEUSDESTROYVIRTUALSINKACT_H
#include "actions/baseaction.h"

class ZeusDestroyVirtualSinkAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusDestroyVirtualSinkAct(QString _name, QString _desc);

  QJsonObject intoJson(void);
  static ZeusBaseAction *maybeFromJson(QJsonObject &);
  QString treeItemDesc(void);

  QString name;
  QString desc;
};

#endif
