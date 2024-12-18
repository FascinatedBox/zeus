#ifndef ZEUSCOMMANDENGINE_H
#define ZEUSCOMMANDENGINE_H
#include <QObject>
#include <QPair>
#include <pulse/pulseaudio.h>

class ZeusBaseAction;
class ZeusCreateVirtualSinkAct;
class ZeusCreatePipelineAct;
class ZeusDestroyVirtualSinkAct;
class ZeusPulseData;

enum ZeusCommandEngineResult {
  ZROk = 0,
  ZRIgnored,
  ZRBadValue,
};

class ZeusCommandEngine : public QObject {
  Q_OBJECT

public:
  ZeusCommandEngine(ZeusPulseData *pd);
  QPair<int, QString> execAction(ZeusBaseAction *a);

private:
  bool haveExistingSinkNamed(QString name);
  uint32_t findDeviceByName(bool isSink, QString name);
  QString findDeviceObjectIdByName(QString name);
  QPair<int, QString> actCreateVirtualSink(ZeusCreateVirtualSinkAct *);
  QPair<int, QString> actCreatePipeline(ZeusCreatePipelineAct *);
  QPair<int, QString> actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *);

  ZeusPulseData *m_pd;
};

#endif
