#if !defined LEAK_CHECK && !defined NDEBUG
#define LEAK_CHECK

void *lcMalloc(size_t size, const char *sFile, const char *sFunc, const unsigned int iLine);
void lcFree(void *ptr, const char *sFile, const char *sFunc, const unsigned int uiLine);
void lcExamin();

#define malloc(x)        lcMalloc(x, __FILE__, __FUNCTION__, __LINE__)
#define calloc(n, x)     lcCalloc(n, x, __FILE__, __FUNCTION__, __LINE__)
#define realloc(p, x)    lcRealloc(p, x, __FILE__, __FUNCTION__, __LINE__)
#define free(x)          lcFree(x, __FILE__, __FUNCTION__, __LINE__)

#endif //LEAK_CHECK
