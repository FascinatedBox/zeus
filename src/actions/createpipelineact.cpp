#include "createpipelineact.h"
#include <QJsonObject>

ZeusCreatePipelineAct::ZeusCreatePipelineAct(void)
    : ZeusBaseAction(ZACreatePipeline) {}

ZeusCreatePipelineAct *
ZeusCreatePipelineAct::fromSinkAndSource(QString sinkName_, QString sourceName_,
                                         QString pipeDesc_) {
  ZeusCreatePipelineAct *result = new ZeusCreatePipelineAct();
  result->sinkName = sinkName_;
  result->sourceName = sourceName_;
  result->pipeDesc = pipeDesc_;
  return result;
}

QJsonObject ZeusCreatePipelineAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "createpipeline";
  o["sinkname"] = sinkName;
  o["sourcename"] = sourceName;
  o["pipedesc"] = pipeDesc;
  return o;
}

ZeusBaseAction *ZeusCreatePipelineAct::maybeFromJson(QJsonObject &o) {
  QString sinkName = o["sinkname"].toString("");
  QString sourceName = o["sourcename"].toString("");
  QString pipeDesc = o["pipedesc"].toString("");

  if (sinkName.isEmpty() || sourceName.isEmpty())
    return nullptr;

  return ZeusCreatePipelineAct::fromSinkAndSource(sinkName, sourceName,
                                                  pipeDesc);
}

QString ZeusCreatePipelineAct::treeItemDesc(void) {
  return QString("CreatePipeline: %1").arg(pipeDesc);
}
