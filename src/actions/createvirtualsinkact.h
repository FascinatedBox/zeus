#ifndef ZEUSCREATEVIRTUALSINKACT_H
#define ZEUSCREATEVIRTUALSINKACT_H
#include "actions/baseaction.h"

class ZeusCreateVirtualSinkAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusCreateVirtualSinkAct(QString _name);

  QString name;
};

#endif
