#include "MockEngine.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "subhook/subhook.c"

int RegisterOneMock(void * original, void * mock)
{
    subhook_t hook = NULL;
    hook = subhook_new(original, mock, SUBHOOK_64BIT_OFFSET);
    if(hook == NULL)
    {
        return MOCK_ENGINE_STATUS_OUT_OF_RESOURCE;
    }

	if (hook->trampoline == NULL)
	{
		/*
		subhook has a very simple length disassmebler engine (LDE) that works only with most common prologue instructions like push, mov, call, etc.
		When it encounters an unknown instruction subhook_get_trampoline() will return NULL, which means the trampoline creation failed.
		But such failure doesn't jeopardize the function redirection so far.
		So I just add a warning.
		*/
		printf("[Mock Warn]Trampoline creation failed for function at %lx.\n", (uint64_t)hook->src);
	}

    if (subhook_install(hook) < 0)
	{
		return MOCK_ENGINE_STATUS_HOOK_INSTALLATION_FAIL;
	}

    return MOCK_ENGINE_STATUS_OK;
}

int RegisterMocks(void * originals[], void * mocks[])
{
	subhook_t hook = NULL;
	int hooksCount = 0;
	int status = MOCK_ENGINE_STATUS_OK;

	/*
	The last element of originals[] must be NULL to serve as the ending sentinel.
	*/
	while (originals[hooksCount] != NULL)
	{
		hooksCount++;
	}

	if (hooksCount > 0)
	{
		int i = 0;
		while (i < hooksCount)
		{
			status = RegisterOneMock(originals[i], mocks[i]);
			if (status != MOCK_ENGINE_STATUS_OK)
			{
				return status;
			}
			i++;
		}
	}
	printf("Registered %d mocks.\n", hooksCount);

  return MOCK_ENGINE_STATUS_OK;
}