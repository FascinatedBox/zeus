#include "createpipelineact.h"
#include <QJsonObject>

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

QJsonObject ZeusCreatePipelineAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "createpipeline";
  o["sinkname"] = sinkName;
  o["sourcename"] = sourceName;
  return o;
}

ZeusBaseAction *ZeusCreatePipelineAct::maybeFromJson(QJsonObject &o) {
  QString sinkName = o["sinkname"].toString("");
  QString sourceName = o["sourcename"].toString("");

  if (sinkName.isEmpty() || sourceName.isEmpty())
    return nullptr;

  auto result = ZeusCreatePipelineAct::fromSinkAndSource(sinkName, sourceName);
  return static_cast<ZeusBaseAction *>(result);
}
