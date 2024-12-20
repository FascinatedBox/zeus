#ifndef ZEUSBASEACTION_H
#define ZEUSBASEACTION_H
#include <QObject>

enum ZeusActionType {
  ZANone,
  ZACreateVirtualSink,
  ZACreatePipeline,
  ZADestroyVirtualSink,
};

class QJsonObject;

class ZeusBaseAction : public QObject {
  Q_OBJECT

public:
  ZeusBaseAction(ZeusActionType);

  ZeusActionType getActionType(void) { return m_actionType; }
  virtual QJsonObject intoJson(void) = 0;

private:
  ZeusActionType m_actionType;
};

#endif
