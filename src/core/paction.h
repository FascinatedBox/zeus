#ifndef ZEUSPACTION_H
#define ZEUSPACTION_H
#include <cstdint>

void zeus_pa_move_sink_input_by_index(uint32_t index, uint32_t deviceIndex);
void zeus_pa_move_source_output_by_index(uint32_t index, uint32_t deviceIndex);

#endif
