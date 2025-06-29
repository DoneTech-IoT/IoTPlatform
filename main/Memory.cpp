#include "esp_attr.h"
#include "esp_heap_caps.h"
#include "Memory.h"

IRAM_ATTR void *esp_matter_mem_calloc(size_t n, size_t size)
{
#if CONFIG_ESP_MATTER_MEM_ALLOC_MODE_INTERNAL
    return heap_caps_calloc(n, size, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
#elif CONFIG_ESP_MATTER_MEM_ALLOC_MODE_EXTERNAL
    return heap_caps_calloc(n, size, MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);
#elif CONFIG_ESP_MATTER_MEM_ALLOC_MODE_IRAM_8BIT
    return heap_caps_calloc_prefer(n, size, 2, MALLOC_CAP_INTERNAL|MALLOC_CAP_IRAM_8BIT, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
#else
    return calloc(n, size);
#endif
}

IRAM_ATTR void *esp_matter_mem_realloc(void *ptr, size_t size)
{
#if CONFIG_ESP_MATTER_MEM_ALLOC_MODE_INTERNAL
    return heap_caps_realloc(ptr, size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#elif CONFIG_ESP_MATTER_MEM_ALLOC_MODE_EXTERNAL
    return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
#elif CONFIG_ESP_MATTER_MEM_ALLOC_MODE_IRAM_8BIT
    return heap_caps_realloc_prefer(ptr, size, 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_IRAM_8BIT, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
    return realloc(ptr, size);
#endif
}

IRAM_ATTR void esp_matter_mem_free(void *ptr)
{
    free(ptr);
}
