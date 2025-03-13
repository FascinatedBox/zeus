#ifndef ZEUSCREATEPIPELINEACT_H
#define ZEUSCREATEPIPELINEACT_H
#include "actions/baseaction.h"

class QJsonObject;

class ZeusCreatePipelineAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusCreatePipelineAct(void);

  // Use a constructor name that makes the order obvious.
  static ZeusCreatePipelineAct *
  fromSinkAndSource(QString sinkName_, QString sinkDesc_, QString sourceName_,
                    QString sourceDesc_, QString pipeDesc_);
  QJsonObject intoJson(void);
  static ZeusBaseAction *maybeFromJson(QJsonObject &);
  QString treeItemDesc(void);

  QString sinkName;
  QString sinkDesc;
  QString sourceName;
  QString sourceDesc;
  QString pipeDesc;
};

#endif
