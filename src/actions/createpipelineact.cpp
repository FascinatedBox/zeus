#include "createpipelineact.h"
#include <QJsonObject>

ZeusCreatePipelineAct::ZeusCreatePipelineAct(void)
    : ZeusBaseAction(ZACreatePipeline) {}

ZeusCreatePipelineAct *ZeusCreatePipelineAct::fromSinkAndSource(
    QString sinkName_, QString sinkDesc_, QString sourceName_,
    QString sourceDesc_, QString pipeDesc_) {
  ZeusCreatePipelineAct *result = new ZeusCreatePipelineAct();
  result->sinkName = sinkName_;
  result->sinkDesc = sinkDesc_;
  result->sourceName = sourceName_;
  result->sourceDesc = sourceDesc_;
  result->pipeDesc = pipeDesc_;
  return result;
}

QJsonObject ZeusCreatePipelineAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "createpipeline";
  o["sinkname"] = sinkName;
  o["sinkdesc"] = sinkDesc;
  o["sourcename"] = sourceName;
  o["sourcedesc"] = sourceDesc;
  o["pipedesc"] = pipeDesc;
  return o;
}

ZeusBaseAction *ZeusCreatePipelineAct::maybeFromJson(QJsonObject &o) {
  QString sinkName = o["sinkname"].toString("");
  QString sinkDesc = o["sinkdesc"].toString("");
  QString sourceName = o["sourcename"].toString("");
  QString sourceDesc = o["sourcedesc"].toString("");
  QString pipeDesc = o["pipedesc"].toString("");

  if (sinkName.isEmpty() || sinkDesc.isEmpty() || sourceName.isEmpty() ||
      sourceDesc.isEmpty() || pipeDesc.isEmpty())
    return nullptr;

  return ZeusCreatePipelineAct::fromSinkAndSource(
      sinkName, sinkDesc, sourceName, sourceDesc, pipeDesc);
}

QString ZeusCreatePipelineAct::treeItemDesc(void) {
  return QString("CreatePipeline: %1").arg(pipeDesc);
}
