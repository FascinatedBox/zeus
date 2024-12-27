#ifndef ZEUSPULSEDATA_H
#define ZEUSPULSEDATA_H
#include <QHash>
#include <QMap>
#include <QObject>
#include <pulse/pulseaudio.h>

class ZeusPulseQuery;

#define ZEUS_PULSE_DATA_DECLARE(camelName, snake_name, UpperName, clsName)     \
public:                                                                        \
  void add##UpperName##Info(const pa_##snake_name##_info *i);                  \
  void remove##UpperName(uint32_t index);                                      \
  QMapIterator<uint32_t, clsName *> camelName##Iterator(void);                 \
                                                                               \
private:                                                                       \
  QMap<uint32_t, clsName *> m_##camelName##s

#define ZEUS_PULSE_DATA_SIGNALS(camelName, clsName)                            \
  void camelName##Added(uint32_t, clsName *);                                  \
  void camelName##Removed(uint32_t);

typedef QHash<QString, QString> ZeusPropHash;

class ZeusPulseBaseInfo : public QObject {
  Q_OBJECT
public:
  ZeusPulseBaseInfo(ZeusPropHash _props);

  ZeusPropHash props;
};

class ZeusPulseClientInfo : public ZeusPulseBaseInfo {
  Q_OBJECT
public:
  ZeusPulseClientInfo(QString _name, ZeusPropHash _props);

  QString name;
};

class ZeusPulseDeviceInfo : public ZeusPulseBaseInfo {
  Q_OBJECT
public:
  ZeusPulseDeviceInfo(int _flags, QString _name, QString _desc,
                      ZeusPropHash _props);

  int flags;
  QString name;
  QString desc;
};

class ZeusPulseStreamInfo : public ZeusPulseBaseInfo {
  Q_OBJECT
public:
  ZeusPulseStreamInfo(uint32_t _client, uint32_t _target, QString _name,
                      ZeusPropHash _props);

  uint32_t client;
  uint32_t target;
  QString name;
};

class ZeusPulseData : public QObject {
  Q_OBJECT
public:
  ZeusPulseData(void);

  QList<QPair<uint32_t, ZeusPulseStreamInfo *>>
  selectPlayback(ZeusPulseQuery *);

  ZEUS_PULSE_DATA_DECLARE(client, client, Client, ZeusPulseClientInfo);
  ZEUS_PULSE_DATA_DECLARE(sink, sink, Sink, ZeusPulseDeviceInfo);
  ZEUS_PULSE_DATA_DECLARE(sinkInput, sink_input, SinkInput,
                          ZeusPulseStreamInfo);
  ZEUS_PULSE_DATA_DECLARE(source, source, Source, ZeusPulseDeviceInfo);
  ZEUS_PULSE_DATA_DECLARE(sourceOutput, source_output, SourceOutput,
                          ZeusPulseStreamInfo);

  // Can't include 'signals:' into the above macros (moc will fail).
signals:
  ZEUS_PULSE_DATA_SIGNALS(client, ZeusPulseClientInfo);
  ZEUS_PULSE_DATA_SIGNALS(sink, ZeusPulseDeviceInfo);
  ZEUS_PULSE_DATA_SIGNALS(sinkInput, ZeusPulseStreamInfo);
  ZEUS_PULSE_DATA_SIGNALS(source, ZeusPulseDeviceInfo);
  ZEUS_PULSE_DATA_SIGNALS(sourceOutput, ZeusPulseStreamInfo);
};

#undef ZEUS_PULSE_DATA_DECLARE
#undef ZEUS_PULSE_DATA_SIGNALS

#endif
