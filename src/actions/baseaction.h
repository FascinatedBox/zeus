#ifndef ZEUSBASEACTION_H
#define ZEUSBASEACTION_H
#include <QObject>

enum ZeusActionType {
  ZANone,
  ZACreateVirtualSink,
  ZACreatePipeline,
};

class ZeusBaseAction : public QObject {
  Q_OBJECT

public:
  ZeusBaseAction(ZeusActionType);

  ZeusActionType getActionType(void) { return m_actionType; }

private:
  ZeusActionType m_actionType;
};

#endif
