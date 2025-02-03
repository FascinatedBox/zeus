#include "core/paction.h"
#include <cstdio>
#include <pulse/pulseaudio.h>

extern pa_context *zeusPulseContext;

void zeus_pa_load_null_sink(const char *arg) {
  pa_operation *o = pa_context_load_module(zeusPulseContext, "module-null-sink",
                                           arg, nullptr, nullptr);

  if (o == nullptr) {
    fprintf(stderr, "zeus_pa_load_null_sink() failed\n");
    return;
  }

  pa_operation_unref(o);
}

#define ZEUS_MOVE_FN(target)                                                   \
  void zeus_pa_move_##target##_by_index(uint32_t index,                        \
                                        uint32_t deviceIndex) {                \
    pa_operation *o = pa_context_move_##target##_by_index(                     \
        zeusPulseContext, index, deviceIndex, nullptr, nullptr);               \
                                                                               \
    if (o == nullptr) {                                                        \
      fprintf(stderr, "pa_context_move_##target##_by_index() failed\n");       \
      return;                                                                  \
    }                                                                          \
                                                                               \
    pa_operation_unref(o);                                                     \
  }

ZEUS_MOVE_FN(sink_input)
ZEUS_MOVE_FN(source_output)
