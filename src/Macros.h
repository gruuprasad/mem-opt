#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))
#define TAS_CACHELINE_COUNT __attribute__((annotate("tas_cacheline_count")))
#define TAS_PROCESS_BATCH __attribute__((annotate("tas_batch_process")))

#define CACHELINESIZE_BYTES 64
