#ifndef ZEUSPACTION_H
#define ZEUSPACTION_H
#include <cstdint>

// Context is the raw PulseAudio context.
typedef void(zeus_move_cb)(void *ctx, int success, void *data);

void zeus_pa_move_sink_input(uint32_t index, uint32_t deviceIndex,
                             zeus_move_cb move_cb, void *move_data);
void zeus_pa_move_source_output(uint32_t index, uint32_t deviceIndex,
                                zeus_move_cb move_cb, void *move_data);

#endif
