#include "core/pulsedata.h"
#include "core/pulsequery.h"

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
    if (i == nullptr)                                                          \
      return;                                                                  \
                                                                               \
    uint32_t index = i->index;                                                 \
                                                                               \
    if (m_##camelName##s.contains(index))                                      \
      return;                                                                  \
                                                                               \
    ZeusPropHash propHash = makePropHash(i->proplist);                         \
    auto o = new cls(ZI##UpperName, index, __VA_ARGS__, propHash);             \
                                                                               \
    m_##camelName##s.insert(index, o);                                         \
    emit camelName##Added(o);                                                  \
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

#define ZEUS_PULSE_QUERY_IMPL(name, source_fn, resultCls)                      \
  QList<resultCls *> ZeusPulseData::select##name(ZeusPulseQuery *q) {          \
    QList<resultCls *> result;                                                 \
    QMapIterator<uint32_t, resultCls *> iter(m_sinkInputs);                    \
                                                                               \
    while (iter.hasNext()) {                                                   \
      iter.next();                                                             \
                                                                               \
      if (q->matches(iter.value()) == false)                                   \
        continue;                                                              \
                                                                               \
      result.append(iter.value());                                             \
    }                                                                          \
                                                                               \
    return result;                                                             \
  }

ZEUS_PULSE_DATA_IMPL(client, client, Client, ZeusPulseClientInfo, i->name)
ZEUS_PULSE_DATA_IMPL(sink, sink, Sink, ZeusPulseDeviceInfo, i->flags, i->name,
                     i->description)
ZEUS_PULSE_DATA_IMPL(source, source, Source, ZeusPulseDeviceInfo, i->flags,
                     i->name, i->description)
ZEUS_PULSE_DATA_IMPL(sink_input, sinkInput, SinkInput, ZeusPulseStreamInfo,
                     i->client, i->sink, i->name)
ZEUS_PULSE_DATA_IMPL(source_output, sourceOutput, SourceOutput,
                     ZeusPulseStreamInfo, i->client, i->source, i->name)

// Only define necessary queries.
ZEUS_PULSE_QUERY_IMPL(Playback, sinkInputIterator, ZeusPulseStreamInfo)

ZeusPulseBaseInfo::ZeusPulseBaseInfo(ZeusPulseInfoType _type, uint32_t _index,
                                     ZeusPropHash _props)
    : type(_type), index(_index), props(_props) {}

ZeusPulseClientInfo::ZeusPulseClientInfo(ZeusPulseInfoType _type,
                                         uint32_t _index, QString _name,
                                         ZeusPropHash _props)
    : ZeusPulseBaseInfo(_type, _index, _props), name(_name) {}

ZeusPulseDeviceInfo::ZeusPulseDeviceInfo(ZeusPulseInfoType _type,
                                         uint32_t _index, int _flags,
                                         QString _name, QString _desc,
                                         ZeusPropHash _props)
    : ZeusPulseBaseInfo(_type, _index, _props), flags(_flags), name(_name),
      desc(_desc) {}

ZeusPulseStreamInfo::ZeusPulseStreamInfo(ZeusPulseInfoType _type,
                                         uint32_t _index, uint32_t _client,
                                         uint32_t _target, QString _name,
                                         ZeusPropHash _props)
    : ZeusPulseBaseInfo(_type, _index, _props), client(_client),
      target(_target), name(_name) {}

QString ZeusPulseData::clientNameByIndexOr(uint32_t index, QString fallback) {
  QString result = fallback;
  ZeusPulseClientInfo *info = m_clients.value(index, nullptr);

  if (info)
    result = info->name;

  return result;
}

ZeusPulseData::ZeusPulseData(void) {}
