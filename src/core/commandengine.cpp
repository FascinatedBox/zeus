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

void ZeusCommandEngine::execAction(ZeusBaseAction *action) {
  switch (action->getActionType()) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    act##TitleName(static_cast<Zeus##TitleName##Act *>(action));               \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
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

void ZeusCommandEngine::actCreateVirtualSink(ZeusCreateVirtualSinkAct *a) {
  QString prog = "pw-loopback";
  QStringList args;
  QString nodeName = QString("input-%1").arg(a->name);

  if (haveExistingSinkNamed(nodeName))
    // Assume the user doesn't actually want a duplicate device.
    return;

  args << "--capture-props" << "media.class=Audio/Sink";
  args << "--capture-props" << QString("node.name=\"input-%1\"").arg(a->name);
  args << "--capture-props" << QString("node.description=\"%1\"").arg(a->name);
  args << "--playback-props" << QString("node.name=\"output-%1\"").arg(a->name);
  args << "--playback-props" << QString("node.description=\"%1\"").arg(a->name);

  QProcess::startDetached(prog, args);
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

void ZeusCommandEngine::actCreatePipeline(ZeusCreatePipelineAct *a) {
  QString prog = "/bin/sh";
  uint32_t playbackIndex = findDeviceByName(true, a->sinkName);
  uint32_t recordIndex = findDeviceByName(false, a->sourceName);

  if (playbackIndex == INVALID_INDEX || recordIndex == INVALID_INDEX) {
    // pw-cat can technically use a fallback device for either end, but the user
    // may not appreciate that. Instead, don't do it at all.
    return;
  }

  QStringList args;

  args << "-c";
  args << QString(PIPELINE_CMD).arg(recordIndex).arg(playbackIndex);

  QProcess::startDetached(prog, args);
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

void ZeusCommandEngine::actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *a) {
  QString prog = "pw-cli";
  QString oid = findDeviceObjectIdByName(a->name);

  if (oid.isEmpty())
    return;

  QStringList args;

  args << "destroy" << oid;

  QProcess::startDetached(prog, args);
}
