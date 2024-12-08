#include "core/commandengine.h"
#include "actions/createvirtualsinkact.h"
#include "core/pulsedata.h"
#include <QProcess>

#define ZEUS_ACTION_CASE(name)                                                 \
  case ZeusActionType::ZA##name:                                               \
    act##name(static_cast<Zeus##name##Act *>(action));                         \
    break

ZeusCommandEngine::ZeusCommandEngine(ZeusPulseData *pd) : m_pd(pd) {}

void ZeusCommandEngine::execAction(ZeusBaseAction *action) {
  switch (action->getActionType()) {
    ZEUS_ACTION_CASE(CreateVirtualSink);
  case ZeusActionType::ZANone:
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
