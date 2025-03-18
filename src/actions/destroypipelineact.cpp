#include "destroypipelineact.h"
#include <QJsonObject>

ZeusDestroyPipelineAct::ZeusDestroyPipelineAct(QString _desc)
    : ZeusBaseAction(ZADestroyPipeline), desc(_desc) {}

QJsonObject ZeusDestroyPipelineAct::intoJson(void) {
  QJsonObject o;

  o["action"] = "destroypipeline";
  o["desc"] = desc;
  return o;
}

ZeusBaseAction *ZeusDestroyPipelineAct::maybeFromJson(QJsonObject &o) {
  QString desc = o["desc"].toString("");

  if (desc.isEmpty())
    return nullptr;

  return new ZeusDestroyPipelineAct(desc);
}

QString ZeusDestroyPipelineAct::treeItemDesc(void) {
  return QString("DestroyPipeline: %1").arg(desc);
}
