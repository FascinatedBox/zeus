#ifndef ZEUSCOMMANDENGINE_H
#define ZEUSCOMMANDENGINE_H
#include <QObject>
#include <QPair>
#include <pulse/pulseaudio.h>

class ZeusBaseAction;
class ZeusCreateVirtualSinkAct;
class ZeusCreatePipelineAct;
class ZeusDestroyVirtualSinkAct;
class ZeusMovePlaybackStreamAct;
class ZeusPulseData;

#define RESULT_IS_SUCCESS(r) (r.first == true)

typedef QPair<bool, QString> ZeusCommandResult;

class ZeusCommandEngine : public QObject {
  Q_OBJECT

public:
  ZeusCommandEngine(ZeusPulseData *pd);
  ZeusCommandResult execAction(ZeusBaseAction *a);

private:
  bool haveExistingSinkNamed(QString name);
  uint32_t findDeviceByName(bool isSink, QString name);
  QString findDeviceObjectIdByName(QString name);
  ZeusCommandResult actCreateVirtualSink(ZeusCreateVirtualSinkAct *);
  ZeusCommandResult actCreatePipeline(ZeusCreatePipelineAct *);
  ZeusCommandResult actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *);
  ZeusCommandResult actMovePlaybackStream(ZeusMovePlaybackStreamAct *);

  ZeusPulseData *m_pd;
};

#endif
