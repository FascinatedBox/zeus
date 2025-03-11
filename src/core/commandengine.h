#ifndef ZEUSCOMMANDENGINE_H
#define ZEUSCOMMANDENGINE_H
#include <QObject>
#include <QPair>

class ZeusBaseAction;
class ZeusCommandEngine;
class ZeusCreateNullSinkAct;
class ZeusCreateVirtualSinkAct;
class ZeusCreatePipelineAct;
class ZeusDestroyVirtualSinkAct;
class ZeusMoveStreamAct;
class ZeusPulseData;
class ZeusPulseDeviceInfo;
class ZeusUserCommand;

#define RESULT_IS_SUCCESS(r) (r.first == true)

typedef QPair<bool, QString> ZeusCommandResult;

class ZeusCommandContext : public QObject {
  Q_OBJECT

public:
  ZeusCommandContext(ZeusPulseData *pd, ZeusCommandEngine *ce);

  QString commandName(void) { return m_commandName; }
  QList<ZeusCommandResult> results(void) { return m_results; }
  bool executing(void) { return m_cursor != -1; }
  void setCommandName(QString name) { m_commandName = name; }
  void setWaitForSink(QString name) { m_awaitSink = name; }
  void startCommand(QString name, ZeusUserCommand *cmd);

signals:
  void commandComplete(void);

private slots:
  void onSinkAdded(ZeusPulseDeviceInfo *);

private:
  void commandExec(void);
  void finishCommand(void);

  ZeusPulseData *m_pd;
  ZeusCommandEngine *m_ce;
  ZeusUserCommand *m_command;
  QList<ZeusCommandResult> m_results;
  QString m_awaitSink;
  QString m_commandName;
  int m_cursor;
};

class ZeusCommandEngine : public QObject {
  Q_OBJECT

public:
  ZeusCommandEngine(ZeusPulseData *pd);
  ZeusCommandResult execAction(ZeusBaseAction *a,
                               ZeusCommandContext *ctx = nullptr);
  QList<ZeusCommandResult> execCommand(ZeusUserCommand *c);

private:
  ZeusCommandResult actCreateNullSink(ZeusCreateNullSinkAct *,
                                      ZeusCommandContext *);
  ZeusCommandResult actCreateVirtualSink(ZeusCreateVirtualSinkAct *,
                                         ZeusCommandContext *);
  ZeusCommandResult actCreatePipeline(ZeusCreatePipelineAct *,
                                      ZeusCommandContext *);
  ZeusCommandResult actDestroyVirtualSink(ZeusDestroyVirtualSinkAct *,
                                          ZeusCommandContext *);
  ZeusCommandResult actMoveStream(ZeusMoveStreamAct *, ZeusCommandContext *);

  ZeusPulseData *m_pd;
};

#endif
