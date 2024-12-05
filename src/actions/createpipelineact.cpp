#include "createpipelineact.h"

ZeusCreatePipelineAct::ZeusCreatePipelineAct(void)
    : ZeusBaseAction(ZACreatePipeline) {}

ZeusCreatePipelineAct *
ZeusCreatePipelineAct::fromSinkAndSource(QString sinkName_,
                                         QString sourceName_) {
  ZeusCreatePipelineAct *result = new ZeusCreatePipelineAct();
  result->sinkName = sinkName_;
  result->sourceName = sourceName_;
  return result;
}
