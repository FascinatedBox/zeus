#ifndef ZEUSCREATEVIRTUALSINKACT_H
#define ZEUSCREATEVIRTUALSINKACT_H
#include "actions/baseaction.h"

class ZeusCreateVirtualSinkAct : public ZeusBaseAction {
  Q_OBJECT

public:
  ZeusCreateVirtualSinkAct(QString _name,
                           QList<QPair<QString, QString>> _props);

  QJsonObject intoJson(void);
  static ZeusBaseAction *maybeFromJson(QJsonObject &);
  QString treeItemDesc(void);

  QString name;
  QList<QPair<QString, QString>> props;
};

#endif
