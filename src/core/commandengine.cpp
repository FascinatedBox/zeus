#include "core/commandengine.h"
#include "actions/createpipelineact.h"
#include "actions/createvirtualsinkact.h"
#include "actions/destroyvirtualsinkact.h"
#include "core/pulsedata.h"
#include <QProcess>

#define PIPELINE_CMD "pw-cat --target %1 -r - | pw-cat --target %2 -p -"
#define PROP_OBJECT_ID "object.id"
#define INVALID_INDEX ((uint32_t)-1)

ZeusCommandEngine::ZeusCommandEngine(ZeusPulseData *pd) : m_pd(pd) {}

QPair<int, QString> ZeusCommandEngine::execAction(ZeusBaseAction *action) {
  switch (action->getActionType()) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    return act##TitleName(static_cast<Zeus##TitleName##Act *>(action));        \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
    return qMakePair(0, "");
    break;
  }
}

bool ZeusCommandEngine::haveExistingSinkNamed(QString name) {
  QMapIterator<uint32_t, ZeusPulseDeviceInfo *> iter = m_pd->sinkIterator();
  bool result = false;

  while (iter.hasNext()) {
    iter.next();

    QString sinkName = iter.value()->name;

    if (sinkName == name) {
      result = true;
      break;
    }
  }

  return result;
}

QPair<int, QString>
ZeusCommandEngine::actCreateVirtualSink(ZeusCreateVirtualSinkAct *a) {
  QString prog = "pw-loopback";
  QStringList args;
  QString nodeName = QString("input-%1").arg(a->name);

  if (haveExistingSinkNamed(nodeName))
    // Assume the user doesn't actually want a duplicate device.
    return qMakePair(ZRIgnored,
                     QString("CreateVirtualSink: Device '%1' already exists.")
                         .arg(nodeName));

  args << "--capture-props" << "media.class=Audio/Sink";
  args << "--capture-props" << QString("node.name=\"input-%1\"").arg(a->name);
  args << "--capture-props" << QString("node.description=\"%1\"").arg(a->name);
  args << "--playback-props" << QString("node.name=\"output-%1\"").arg(a->name);
  args << "--playback-props" << QString("node.description=\"%1\"").arg(a->name);

  QProcess::startDetached(prog, args);
  return qMakePair(ZROk,
                   QString("CreateVirtualSink: Created '%1'.").arg(nodeName));
}

uint32_t ZeusCommandEngine::findDeviceByName(bool isSink, QString name) {
  QMapIterator<uint32_t, ZeusPulseDeviceInfo *> iter = m_pd->sinkIterator();
  uint32_t result = INVALID_INDEX;

  if (isSink == false)
    iter = m_pd->sourceIterator();

  while (iter.hasNext()) {
    iter.next();

    QString deviceName = iter.value()->name;

    if (deviceName == name) {
      result = iter.key();
      break;
    }
  }

  return result;
}

QPair<int, QString>
ZeusCommandEngine::actCreatePipeline(ZeusCreatePipelineAct *a) {
  QString prog = "/bin/sh";
  uint32_t playbackIndex = findDeviceByName(true, a->sinkName);
  uint32_t recordIndex = findDeviceByName(false, a->sourceName);

  if (playbackIndex == INVALID_INDEX)
    return qMakePair(ZRBadValue,
                     QString("CreatePipeline: Invalid playback device '%1'")
                         .arg(a->sinkName));

  if (recordIndex == INVALID_INDEX)
    return qMakePair(ZRBadValue,
                     QString("CreatePipeline: Invalid recording device '%1'")
                         .arg(a->sourceName));

  QStringList args;

  args << "-c";
  args << QString(PIPELINE_CMD).arg(recordIndex).arg(playbackIndex);
  QProcess::startDetached(prog, args);
  return qMakePair(ZROk, QString("CreatePipeline: Pipeline established."));
}

QString ZeusCommandEngine::findDeviceObjectIdByName(QString name) {
  QMapIterator<uint32_t, ZeusPulseDeviceInfo *> iter = m_pd->sinkIterator();
  QString result = "";

  while (iter.hasNext()) {
    iter.next();

    QString deviceName = iter.value()->name;

    if (deviceName == name) {
      result = iter.value()->props.value(PROP_OBJECT_ID, "");
      break;
    }
  }

  return result;
}

QPair<int, QString>
ZeusCommandEngine::actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *a) {
  QString prog = "pw-cli";
  QString oid = findDeviceObjectIdByName(a->name);

  if (oid.isEmpty())
    return qMakePair(
        ZRIgnored, QString("DestroyVirtualSink: Cannot find device named '%1'.")
                       .arg(a->name));

  QStringList args;

  args << "destroy" << oid;
  QProcess::startDetached(prog, args);
  return qMakePair(
      ZROk, QString("DestroyVirtualSink: Destroyed sink '%1'.").arg(oid));
}
