#include "core/paction.h"
#include <cstdio>
#include <pulse/pulseaudio.h>

extern pa_context *zeusPulseContext;

#define ZEUS_MOVE_FN_CB(target)                                                \
  void zeus_pa_move_##target(uint32_t index, uint32_t deviceIndex,             \
                             zeus_move_cb move_cb, void *data) {               \
    pa_operation *o = pa_context_move_##target##_by_index(                     \
        zeusPulseContext, index, deviceIndex,                                  \
        (pa_context_success_cb_t)move_cb, data);                               \
                                                                               \
    if (o == nullptr) {                                                        \
      fprintf(stderr, "pa_context_move_##target##_by_index() failed\n");       \
      return;                                                                  \
    }                                                                          \
                                                                               \
    pa_operation_unref(o);                                                     \
  }

ZEUS_MOVE_FN_CB(sink_input)
ZEUS_MOVE_FN_CB(source_output)
