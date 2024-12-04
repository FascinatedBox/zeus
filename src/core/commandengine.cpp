#include "core/commandengine.h"
#include "actions/createvirtualsinkact.h"
#include <QProcess>

#define ZEUS_ACTION_CASE(name)                                                 \
  case ZeusActionType::ZA##name:                                               \
    act##name(static_cast<Zeus##name##Act *>(action));                         \
    break

ZeusCommandEngine::ZeusCommandEngine(void) {}

void ZeusCommandEngine::execAction(ZeusBaseAction *action) {
  switch (action->getActionType()) {
    ZEUS_ACTION_CASE(CreateVirtualSink);
  case ZeusActionType::ZANone:
    break;
  }
}

void ZeusCommandEngine::actCreateVirtualSink(ZeusCreateVirtualSinkAct *a) {
  QString prog = "pw-loopback";
  QStringList args;

  args << "--capture-props" << "media.class=Audio/Sink";
  args << "--capture-props" << QString("node.name=\"input-%1\"").arg(a->name);
  args << "--capture-props" << QString("node.description=\"%1\"").arg(a->name);
  args << "--playback-props" << QString("node.name=\"output-%1\"").arg(a->name);
  args << "--playback-props" << QString("node.description=\"%1\"").arg(a->name);

  QProcess::startDetached(prog, args);
}
