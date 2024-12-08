#include "core/pulsedata.h"

static ZeusPropHash makePropHash(pa_proplist *p) {
  ZeusPropHash result;
  const char *key;
  void *state = nullptr;

  while ((key = pa_proplist_iterate(p, &state))) {
    const char *value = pa_proplist_gets(p, key);
    result[key] = value;
  }

  return result;
}

#define ZEUS_PULSE_DATA_REMOVER(camelName, UpperName)                          \
  void ZeusPulseData::remove##UpperName(uint32_t index) {                      \
    if (m_##camelName##s.contains(index) == false)                             \
      return;                                                                  \
                                                                               \
    delete m_##camelName##s.take(index);                                       \
    emit camelName##Removed(index);                                            \
  }

#define ZEUS_PULSE_DATA_STREAM(snake_name, camelName, UpperName)               \
  ZEUS_PULSE_DATA_REMOVER(camelName, UpperName)                                \
  void ZeusPulseData::add##UpperName##Info(const pa_##snake_name##_info *i) {  \
    uint32_t index = i->index;                                                 \
                                                                               \
    if (m_##camelName##s.contains(index))                                      \
      return;                                                                  \
                                                                               \
    QString name = i->name;                                                    \
    ZeusPropHash propHash = makePropHash(i->proplist);                         \
    ZeusPulseStreamInfo *o = new ZeusPulseStreamInfo(name, propHash);          \
                                                                               \
    m_##camelName##s.insert(index, o);                                         \
    emit camelName##Added(index, o);                                           \
  }                                                                            \
                                                                               \
  QMapIterator<uint32_t, ZeusPulseStreamInfo *>                                \
      ZeusPulseData::camelName##Iterator(void) {                               \
    return QMapIterator(m_##camelName##s);                                     \
  }

#define ZEUS_PULSE_DATA_DEVICE(snake_name, camelName, UpperName)               \
  ZEUS_PULSE_DATA_REMOVER(camelName, UpperName)                                \
  void ZeusPulseData::add##UpperName##Info(const pa_##snake_name##_info *i) {  \
    uint32_t index = i->index;                                                 \
                                                                               \
    if (m_##camelName##s.contains(index))                                      \
      return;                                                                  \
                                                                               \
    QString name = i->name;                                                    \
    QString desc = i->description;                                             \
    ZeusPropHash propHash = makePropHash(i->proplist);                         \
    ZeusPulseDeviceInfo *o = new ZeusPulseDeviceInfo(name, desc, propHash);    \
                                                                               \
    m_##camelName##s.insert(index, o);                                         \
    emit camelName##Added(index, o);                                           \
  }                                                                            \
                                                                               \
  QMapIterator<uint32_t, ZeusPulseDeviceInfo *>                                \
      ZeusPulseData::camelName##Iterator(void) {                               \
    return QMapIterator(m_##camelName##s);                                     \
  }

ZEUS_PULSE_DATA_DEVICE(sink, sink, Sink)
ZEUS_PULSE_DATA_STREAM(sink_input, sinkInput, SinkInput)
ZEUS_PULSE_DATA_DEVICE(source, source, Source)
ZEUS_PULSE_DATA_STREAM(source_output, sourceOutput, SourceOutput)

ZeusPulseDeviceInfo::ZeusPulseDeviceInfo(QString _name, QString _desc,
                                         ZeusPropHash _props)
    : name(_name), desc(_desc), props(_props) {}

ZeusPulseStreamInfo::ZeusPulseStreamInfo(QString _name, ZeusPropHash _props)
    : name(_name), props(_props) {}

ZeusPulseData::ZeusPulseData(void) {}
