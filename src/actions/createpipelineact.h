#ifndef ZEUSCREATEPIPELINEACT_H
#define ZEUSCREATEPIPELINEACT_H
#include "actions/baseaction.h"

class ZeusCreatePipelineAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusCreatePipelineAct(void);

  // Use a constructor name that makes the order obvious.
  static ZeusCreatePipelineAct *fromSinkAndSource(QString sinkName_,
                                                  QString sourceName_);

  QString sinkName;
  QString sourceName;
};

#endif
