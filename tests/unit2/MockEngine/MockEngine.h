#ifndef MOCK_ENGINE
#define MOCK_ENGINE

#define MOCK_ENGINE_STATUS_OK 0
#define MOCK_ENGINE_STATUS_OUT_OF_RESOURCE 1
#define MOCK_ENGINE_STATUS_HOOK_INSTALLATION_FAIL  2
#define MOCK_ENGINE_STATUS_HOOK_REMOVAL_FAIL  3

int RegisterOneMock(void * original, void * mock);

int RegisterMocks(void * originals[], void * mocks[]);

#endif
