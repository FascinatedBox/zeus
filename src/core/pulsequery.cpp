#include "core/pulsequery.h"
#include "core/utils.h"
#include <QJsonArray>

ZeusPulseQuery::ZeusPulseQuery(void) {}

bool ZeusPulseQuery::matches(ZeusPulseBaseInfo *info) {
  ZeusPropHash props = info->props;
  bool result = false;

  foreach (ZeusPulseQueryLine line, m_queryLines) {
    QString key = std::get<0>(line);
    ZeusQueryMatchType matchType = std::get<1>(line);
    QString value = std::get<2>(line);

    switch (matchType) {
    case MTEqual:
      result = (props.value(key) == value);
      break;
    case MTNotEqual:
      result = (props.value(key) != value);
      break;
    default:
      break;
    }

    if (result == false)
      break;
  }

  return result;
}

QJsonArray ZeusPulseQuery::intoJson(void) {
  QJsonArray result;

  foreach (ZeusPulseQueryLine l, m_queryLines) {
    QJsonObject o;

    o["key"] = std::get<0>(l);
    o["action"] = std::get<1>(l);
    o["value"] = std::get<2>(l);
    result.append(o);
  }

  return result;
}

ZeusPulseQuery *ZeusPulseQuery::maybeFromJson(QJsonValue v) {
  QJsonArray a = v.toArray();
  ZeusPulseQuery *result = new ZeusPulseQuery;

  foreach (auto line, a) {
    QJsonObject o = line.toObject();
    QString key = o["key"].toString();
    auto action = static_cast<ZeusQueryMatchType>(o["action"].toInt());
    QString value = o["value"].toString();

    if (::isValidPropertyKV(key, value) == false)
      continue;

    result->addQueryLine(std::make_tuple(key, action, value));
  }

  return result;
}