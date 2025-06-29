#pragma once

/** ESP Matter Memory Allocations
 * @param[in] n number of elements to be allocated
 * @param[in] size size of elements to be allocated
 */
void *esp_matter_mem_calloc(size_t n, size_t size);

/** ESP Matter Free Memory
 * @param[in] ptr pointer to the memory to be freed.
 */
void esp_matter_mem_free(void *ptr);

/** ESP Matter realloc
 * @param[in] ptr  Pointer to reallocate
 * @param[in] size size to reallocate
 */
void *esp_matter_mem_realloc(void *ptr, size_t size);