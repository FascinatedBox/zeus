#include "actions/createvirtualsinkact.h"
#include "core/utils.h"
#include <QJsonArray>
#include <QJsonObject>

ZeusCreateVirtualSinkAct::ZeusCreateVirtualSinkAct(
    QString _name, QList<QPair<QString, QString>> _props)
    : ZeusBaseAction(ZACreateVirtualSink), name(_name), props(_props) {}

QJsonObject ZeusCreateVirtualSinkAct::intoJson(void) {
  QJsonObject o;
  QJsonArray a;

  for (int i = 0; i < props.size(); i++)
    a.append(QJsonArray({props[i].first, props[i].second}));

  o["action"] = "createvirtualsink";
  o["name"] = name;
  o["properties"] = a;
  return o;
}

ZeusBaseAction *ZeusCreateVirtualSinkAct::maybeFromJson(QJsonObject &o) {
  QString name = o["name"].toString("");

  if (::isValidPulseDeviceName(name) == false)
    return nullptr;

  QJsonArray a = o["properties"].toArray();
  QList<QPair<QString, QString>> props;

  for (int i = 0; i < a.size(); i++) {
    QString k = a[0].toString();
    QString v = a[1].toString();

    if (::isValidPropertyKV(k, v) == false)
      continue;

    props.append(qMakePair(k, v));
  }

  auto result = new ZeusCreateVirtualSinkAct(name, props);
  return static_cast<ZeusBaseAction *>(result);
}
