#ifndef ZEUSPULSEQUERY_H
#define ZEUSPULSEQUERY_H
#include "core/pulsedata.h"
#include <QJsonObject>
#include <tuple>

enum ZeusQueryMatchType {
  MTEqual,
  MTNotEqual,
};

enum ZeusQueryObjectType {
  OTPlayback,
  OTRecord,
};

class ZeusPulseDeviceInfo;
class ZeusPulseStreamInfo;

// Key <action> value
typedef std::tuple<QString, ZeusQueryMatchType, QString> ZeusPulseQueryLine;

class ZeusPulseQuery : public QObject {
  Q_OBJECT

public:
  ZeusPulseQuery(void);

  // Primary api
  void addQueryLine(ZeusPulseQueryLine line) { m_queryLines.append(line); }
  static ZeusPulseQuery *make(QString, ZeusQueryMatchType, QString);
  bool matches(ZeusPulseBaseInfo *);
  QListIterator<ZeusPulseQueryLine> lineIterator(void) {
    return QListIterator(m_queryLines);
  }

  // For MoveStreamAct's treeitem text.
  QString explain(void);

  // Save + restore
  QJsonArray intoJson(void);
  static ZeusPulseQuery *maybeFromJson(QJsonValue);

private:
  QList<ZeusPulseQueryLine> m_queryLines;
};

#endif
