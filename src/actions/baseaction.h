#ifndef ZEUSBASEACTION_H
#define ZEUSBASEACTION_H
#include <QObject>

enum ZeusActionType {
  ZANone,
  ZACreateNullSink,
  ZACreateVirtualSink,
  ZACreatePipeline,
  ZADestroyPipeline,
  ZADestroyVirtualSink,
  ZAMoveStream,
};

class QJsonObject;

class ZeusBaseAction : public QObject {
  Q_OBJECT

public:
  ZeusBaseAction(ZeusActionType);

  ZeusActionType actionType(void) { return m_actionType; }
  virtual QJsonObject intoJson(void) = 0;
  virtual QString treeItemDesc(void) = 0;

private:
  ZeusActionType m_actionType;
};

#endif
