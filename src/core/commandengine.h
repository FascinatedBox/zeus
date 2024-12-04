#ifndef ZEUSCOMMANDENGINE_H
#define ZEUSCOMMANDENGINE_H
#include <QObject>
#include <pulse/pulseaudio.h>

class ZeusBaseAction;
class ZeusCreateVirtualSinkAct;

class ZeusCommandEngine : public QObject {
  Q_OBJECT

public:
  ZeusCommandEngine(void);
  void execAction(ZeusBaseAction *a);

private:
  void actCreateVirtualSink(ZeusCreateVirtualSinkAct *);
};

#endif
