#ifndef ZEUSCOMMANDENGINE_H
#define ZEUSCOMMANDENGINE_H
#include <QObject>
#include <pulse/pulseaudio.h>

class ZeusBaseAction;
class ZeusCreateVirtualSinkAct;
class ZeusCreatePipelineAct;
class ZeusDestroyVirtualSinkAct;
class ZeusPulseData;

class ZeusCommandEngine : public QObject {
  Q_OBJECT

public:
  ZeusCommandEngine(ZeusPulseData *pd);
  void execAction(ZeusBaseAction *a);

private:
  bool haveExistingSinkNamed(QString name);
  uint32_t findDeviceByName(bool isSink, QString name);
  QString findDeviceObjectIdByName(QString name);
  void actCreateVirtualSink(ZeusCreateVirtualSinkAct *);
  void actCreatePipeline(ZeusCreatePipelineAct *);
  void actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *);

  ZeusPulseData *m_pd;
};

#endif
