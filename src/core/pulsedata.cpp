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

#define ZEUS_PULSE_DATA_IMPL(snake_name, camelName, UpperName, cls, ...)       \
  void ZeusPulseData::add##UpperName##Info(const pa_##snake_name##_info *i) {  \
    uint32_t index = i->index;                                                 \
                                                                               \
    if (m_##camelName##s.contains(index))                                      \
      return;                                                                  \
                                                                               \
    ZeusPropHash propHash = makePropHash(i->proplist);                         \
    auto o = new cls(__VA_ARGS__, propHash);                                   \
                                                                               \
    m_##camelName##s.insert(index, o);                                         \
    emit camelName##Added(index, o);                                           \
  }                                                                            \
                                                                               \
  QMapIterator<uint32_t, cls *> ZeusPulseData::camelName##Iterator(void) {     \
    return QMapIterator(m_##camelName##s);                                     \
  }                                                                            \
                                                                               \
  void ZeusPulseData::remove##UpperName(uint32_t index) {                      \
    if (m_##camelName##s.contains(index) == false)                             \
      return;                                                                  \
                                                                               \
    delete m_##camelName##s.take(index);                                       \
    emit camelName##Removed(index);                                            \
  }

ZEUS_PULSE_DATA_IMPL(sink, sink, Sink, ZeusPulseDeviceInfo, i->flags, i->name,
                     i->description)
ZEUS_PULSE_DATA_IMPL(source, source, Source, ZeusPulseDeviceInfo, i->flags,
                     i->name, i->description)
ZEUS_PULSE_DATA_IMPL(sink_input, sinkInput, SinkInput, ZeusPulseStreamInfo,
                     i->name)
ZEUS_PULSE_DATA_IMPL(source_output, sourceOutput, SourceOutput,
                     ZeusPulseStreamInfo, i->name)

ZeusPulseDeviceInfo::ZeusPulseDeviceInfo(int _flags, QString _name,
                                         QString _desc, ZeusPropHash _props)
    : flags(_flags), name(_name), desc(_desc), props(_props) {}

ZeusPulseStreamInfo::ZeusPulseStreamInfo(QString _name, ZeusPropHash _props)
    : name(_name), props(_props) {}

ZeusPulseData::ZeusPulseData(void) {}
