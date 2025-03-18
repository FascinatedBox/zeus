#ifndef ZEUSDESTROYPIPELINEACT_H
#define ZEUSDESTROYPIPELINEACT_H
#include "actions/baseaction.h"

class ZeusDestroyPipelineAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusDestroyPipelineAct(QString _desc);

  QJsonObject intoJson(void);
  static ZeusBaseAction *maybeFromJson(QJsonObject &);
  QString treeItemDesc(void);

  QString desc;
};

#endif
