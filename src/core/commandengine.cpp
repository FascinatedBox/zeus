#include "core/commandengine.h"
#include "actions/createnullsinkact.h"
#include "actions/createpipelineact.h"
#include "actions/createvirtualsinkact.h"
#include "actions/destroyvirtualsinkact.h"
#include "actions/movestreamact.h"
#include "core/paction.h"
#include "core/pulsedata.h"
#include "core/pulsequery.h"
#include "core/usercommand.h"
#include <QProcess>

#define PIPELINE_CMD "pw-cat --target %1 -r - | pw-cat --target %2 -p -"
#define PROP_OBJECT_ID "object.id"

ZeusCommandEngine::ZeusCommandEngine(ZeusPulseData *pd) : m_pd(pd) {}

#define FAILURE(value) qMakePair(false, (value))
#define SUCCESS(value) qMakePair(true, (value))

ZeusCommandResult ZeusCommandEngine::execAction(ZeusBaseAction *action) {
  switch (action->actionType()) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    return act##TitleName(static_cast<Zeus##TitleName##Act *>(action));        \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
    return FAILURE("");
    break;
  }
}

QList<ZeusCommandResult> ZeusCommandEngine::execCommand(ZeusUserCommand *c) {
  QList<ZeusCommandResult> result;
  auto iter = c->actionIterator();

  while (iter.hasNext())
    result.append(execAction(iter.next()));

  return result;
}

ZeusCommandResult
ZeusCommandEngine::actCreateNullSink(ZeusCreateNullSinkAct *a) {
  QString nodeName = QString("input-%1").arg(a->name);

  if (m_pd->deviceByName(ZISink, nodeName))
    // Assume the user doesn't actually want a duplicate device.
    return SUCCESS(
        QString("CreateNullSink: Device '%1' already exists.").arg(nodeName));

  // Tag this as a virtual node so DestroyVirtualSink can find it later.
  QString virt = QString("node.virtual=true");
  QString s = QString("sink_name=\"%1\" sink_properties=\"%2\"")
                  .arg(nodeName)
                  .arg(virt);
  const char *arg = qPrintable(s);

  zeus_pa_load_null_sink(arg);
  return SUCCESS(QString("CreateNullSink: Created '%1'.").arg(nodeName));
}

ZeusCommandResult
ZeusCommandEngine::actCreateVirtualSink(ZeusCreateVirtualSinkAct *a) {
  QString prog = "pw-loopback";
  QStringList args;
  QString nodeName = QString("input-%1").arg(a->name);

  if (m_pd->deviceByName(ZISink, nodeName))
    // Assume the user doesn't actually want a duplicate device.
    return SUCCESS(QString("CreateVirtualSink: Device '%1' already exists.")
                       .arg(nodeName));

  foreach (auto p, a->props)
    args << "--capture-props" << QString("%1=\"%2\"").arg(p.first, p.second);

  args << "--capture-props" << "media.class=Audio/Sink";
  args << "--capture-props" << QString("node.name=\"input-%1\"").arg(a->name);
  args << "--capture-props" << QString("node.description=\"%1\"").arg(a->name);
  args << "--playback-props" << QString("node.name=\"output-%1\"").arg(a->name);
  args << "--playback-props" << QString("node.description=\"%1\"").arg(a->name);

  QProcess::startDetached(prog, args);
  return SUCCESS(QString("CreateVirtualSink: Created '%1'.").arg(nodeName));
}

ZeusCommandResult
ZeusCommandEngine::actCreatePipeline(ZeusCreatePipelineAct *a) {
  QString prog = "/bin/sh";
  ZeusPulseDeviceInfo *playDevice = m_pd->deviceByName(ZISink, a->sinkName);
  ZeusPulseDeviceInfo *recordDevice =
      m_pd->deviceByName(ZISource, a->sourceName);

  if (playDevice == nullptr)
    return FAILURE(QString("CreatePipeline: Invalid playback device '%1'")
                       .arg(a->sinkName));

  if (recordDevice == nullptr)
    return FAILURE(QString("CreatePipeline: Invalid recording device '%1'")
                       .arg(a->sourceName));

  QStringList args;

  args << "-c";
  args << QString(PIPELINE_CMD).arg(recordDevice->index).arg(playDevice->index);
  QProcess::startDetached(prog, args);
  return SUCCESS(QString("CreatePipeline: Pipeline established."));
}

ZeusCommandResult
ZeusCommandEngine::actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *a) {
  QString prog = "pw-cli";
  QString oid;
  ZeusPulseDeviceInfo *playDevice = m_pd->deviceByName(ZISink, a->name);

  if (playDevice)
    oid = playDevice->props.value(PROP_OBJECT_ID, "");

  if (oid.isEmpty())
    return FAILURE(QString("DestroyVirtualSink: Cannot find device named '%1'.")
                       .arg(a->name));

  QStringList args;

  args << "destroy" << oid;
  QProcess::startDetached(prog, args);
  return SUCCESS(
      QString("DestroyVirtualSink: Destroyed sink '%1'.").arg(a->name));
}

ZeusCommandResult ZeusCommandEngine::actMoveStream(ZeusMoveStreamAct *a) {
  bool isSink = a->isPlayback();
  auto streamType = isSink ? ZISinkInput : ZISourceOutput;
  auto targets = m_pd->selectStreams(streamType, a->query);
  auto deviceType = isSink ? ZISink : ZISource;
  ZeusPulseDeviceInfo *device = m_pd->deviceByName(deviceType, a->target);

  if (device == nullptr)
    return FAILURE(
        QString("MoveStream: Cannot find device named '%1'.").arg(a->target));

  uint32_t targetIndex = device->index;

  foreach (auto t, targets) {
    if (isSink)
      zeus_pa_move_sink_input_by_index(t->index, targetIndex);
    else
      zeus_pa_move_source_output_by_index(t->index, targetIndex);
  }

  return SUCCESS(
      QString("MoveStream: Moved %1 stream(s).").arg(targets.size()));
}
