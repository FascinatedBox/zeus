#ifndef ZEUSPULSEDATA_H
#define ZEUSPULSEDATA_H
#include <QHash>
#include <QMap>
#include <QObject>
#include <pulse/introspect.h>

class ZeusPulseQuery;

enum ZeusPulseInfoType {
  ZIClient,
  ZISink,
  ZISinkInput,
  ZISource,
  ZISourceOutput,
};

#define PULSE_DATA_DECLARE(camelName, snake_name, UpperName, clsName)          \
public:                                                                        \
  void add##UpperName(const pa_##snake_name##_info *i);                        \
  void remove##UpperName(uint32_t index);                                      \
  QMapIterator<uint32_t, clsName *> camelName##Iterator(void);                 \
                                                                               \
private:                                                                       \
  QMap<uint32_t, clsName *> m_##camelName##s

#define PULSE_DATA_SIGNALS(camelName, clsName)                                 \
  void camelName##Added(clsName *);                                            \
  void camelName##Removed(uint32_t);

typedef QHash<QString, QString> ZeusPropHash;

class ZeusPulseBaseInfo : public QObject {
  Q_OBJECT
public:
  ZeusPulseBaseInfo(ZeusPulseInfoType _type, uint32_t _index,
                    ZeusPropHash _props);

  ZeusPulseInfoType type;
  uint32_t index;
  ZeusPropHash props;
};

class ZeusPulseClientInfo : public ZeusPulseBaseInfo {
  Q_OBJECT
public:
  ZeusPulseClientInfo(ZeusPulseInfoType _type, uint32_t _index, QString _name,
                      ZeusPropHash _props);

  QString name;
};

class ZeusPulseDeviceInfo : public ZeusPulseBaseInfo {
  Q_OBJECT
public:
  ZeusPulseDeviceInfo(ZeusPulseInfoType _type, uint32_t _index, int _flags,
                      QString _name, QString _desc, ZeusPropHash _props);

  int flags;
  QString name;
  QString desc;
};

class ZeusPulseStreamInfo : public ZeusPulseBaseInfo {
  Q_OBJECT
public:
  ZeusPulseStreamInfo(ZeusPulseInfoType _type, uint32_t _index,
                      uint32_t _client, uint32_t _target, QString _name,
                      ZeusPropHash _props);

  uint32_t client;
  uint32_t target;
  QString name;
};

class ZeusPulseData : public QObject {
  Q_OBJECT
public:
  ZeusPulseData(void);

  QString clientNameByIndexOr(uint32_t index, QString fallback);
  QList<ZeusPulseStreamInfo *> selectStreams(ZeusPulseInfoType,
                                             ZeusPulseQuery *);

  PULSE_DATA_DECLARE(client, client, Client, ZeusPulseClientInfo);
  PULSE_DATA_DECLARE(sink, sink, Sink, ZeusPulseDeviceInfo);
  PULSE_DATA_DECLARE(sinkInput, sink_input, SinkInput, ZeusPulseStreamInfo);
  PULSE_DATA_DECLARE(source, source, Source, ZeusPulseDeviceInfo);
  PULSE_DATA_DECLARE(sourceOutput, source_output, SourceOutput,
                     ZeusPulseStreamInfo);

  // Can't include 'signals:' into the above macros (moc will fail).
signals:
  PULSE_DATA_SIGNALS(client, ZeusPulseClientInfo);
  PULSE_DATA_SIGNALS(sink, ZeusPulseDeviceInfo);
  PULSE_DATA_SIGNALS(sinkInput, ZeusPulseStreamInfo);
  PULSE_DATA_SIGNALS(source, ZeusPulseDeviceInfo);
  PULSE_DATA_SIGNALS(sourceOutput, ZeusPulseStreamInfo);
};

#undef PULSE_DATA_DECLARE
#undef PULSE_DATA_SIGNALS

#define ZEUS_PULSE_CONNECT_LOAD(p_, v_, cls, camelName, UpperName)             \
  do {                                                                         \
    connect(p_, &ZeusPulseData::camelName##Added, v_,                          \
            &cls::on##UpperName##Added);                                       \
    connect(p_, &ZeusPulseData::camelName##Removed, v_,                        \
            &cls::on##UpperName##Removed);                                     \
                                                                               \
    auto iter = p_->camelName##Iterator();                                     \
                                                                               \
    while (iter.hasNext()) {                                                   \
      iter.next();                                                             \
      v_->on##UpperName##Added(iter.value());                                  \
    }                                                                          \
  } while (0)

#endif
