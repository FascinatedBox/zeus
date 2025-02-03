#ifndef ZEUSCOMMANDENGINE_H
#define ZEUSCOMMANDENGINE_H
#include <QObject>
#include <QPair>

class ZeusBaseAction;
class ZeusCreateNullSinkAct;
class ZeusCreateVirtualSinkAct;
class ZeusCreatePipelineAct;
class ZeusDestroyVirtualSinkAct;
class ZeusMoveStreamAct;
class ZeusPulseData;
class ZeusUserCommand;

#define RESULT_IS_SUCCESS(r) (r.first == true)

typedef QPair<bool, QString> ZeusCommandResult;

class ZeusCommandEngine : public QObject {
  Q_OBJECT

public:
  ZeusCommandEngine(ZeusPulseData *pd);
  ZeusCommandResult execAction(ZeusBaseAction *a);
  QList<ZeusCommandResult> execCommand(ZeusUserCommand *c);

private:
  bool haveExistingSinkNamed(QString name);
  uint32_t findDeviceByName(bool isSink, QString name);
  QString findDeviceObjectIdByName(QString name);
  ZeusCommandResult actCreateNullSink(ZeusCreateNullSinkAct *);
  ZeusCommandResult actCreateVirtualSink(ZeusCreateVirtualSinkAct *);
  ZeusCommandResult actCreatePipeline(ZeusCreatePipelineAct *);
  ZeusCommandResult actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *);
  ZeusCommandResult actMoveStream(ZeusMoveStreamAct *);

  ZeusPulseData *m_pd;
};

#endif
