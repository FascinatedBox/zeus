#include "core/paction.h"
#include <cstdio>
#include <pulse/pulseaudio.h>

extern pa_context *zeusPulseContext;

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
