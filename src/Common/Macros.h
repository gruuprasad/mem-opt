#ifndef TAS_MACROS_H
#define TAS_MACROS_H

#include <string>

#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))
#define TAS_ANALYZE_CACHE __attribute__((annotate("tas_cacheline_count")))
#define TAS_PROCESS_BATCH __attribute__((annotate("tas_batch_process")))

const std::string TAS_MAKE_BATCH_AS_STR = "tas_batch_maker";

#endif
