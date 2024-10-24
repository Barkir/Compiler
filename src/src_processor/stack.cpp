#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "stack.h"
#include "hash.h"

int StackCtorFunc(Stack * stk, size_t el_size, size_t stk_capacity ON_DEBUG(COMMA const char * NAME COMMA int LINE COMMA const char * FILE COMMA const char * FUNC))
{
#ifdef DEBUG
    stk->NAME = NAME;
    stk->LINE = LINE;
    stk->FUNC = FUNC;
    stk->FILE = FILE;
#endif

    if (stk_capacity == 0) return SUCCESS;
    if (stk_capacity > MAX_SIZE || stk_capacity < 0) return BADCAP;
    stk->capacity = stk_capacity < 16 ? 16 : stk_capacity;
    stk->size = 0;
    stk->el_size = el_size;
    stk->data = malloc(stk->capacity * stk->el_size ON_DEBUG(+ CannaryShift(stk->capacity * stk->el_size)));
    memset(stk->data, 0, stk->capacity * stk->el_size ON_DEBUG(+ CannaryShift(stk->capacity * stk->el_size)));

#ifdef DEBUG
    stk->data = (void*)((size_t) stk->data + CannarySize);
    MurMur(stk, StackSize, SEED, &stack_hash);
    MurMur(stk->data, stk->capacity * stk->el_size, SEED, &data_hash);
    CannaryInstall(stk);
#endif

    STACK_ASSERT(stk);
    return SUCCESS;
}

int StackDtorFunc(Stack * stk)
{
    STACK_ASSERT(stk);
    stk->capacity = 0;
    stk->size = 0;
    stk->el_size = 0;

    stk->data = (void*)((size_t) stk->data - CannarySize);
    free(stk->data);
    fprintf(stderr, "Stack Terminated!\n");
    return SUCCESS;
}

int StackPush(Stack * stk, void * elem)
{
    STACK_ASSERT(stk);
    if ((stk->size + 1) >= stk->capacity)
        if(StackExpand(stk) == ERROR) return ERROR;

    stk->size++;
    memcpy((void*)(((size_t)stk->data) + (stk->size - 1) * stk->el_size), elem, stk->el_size);

#ifdef DEBUG
    MurMur(stk, StackSize, SEED, &stack_hash);
    MurMur(stk->data, stk->capacity * stk->el_size, SEED, &data_hash);
#endif

    STACK_ASSERT(stk);
    return SUCCESS;
}

// std::vector::shrink_to_fit
// TODO: to shrink pop, no shrink pop

int StackPop(Stack * stk)
{
    STACK_ASSERT(stk);
    if (stk->size <= stk->capacity / 4)
        if (StackShrink(stk) == ERROR) return ERROR;

    memset((void*)((size_t)stk->data + (stk->size - 1) * stk->el_size), 0, stk->el_size);
    stk->size--;

#ifdef DEBUG
    MurMur(stk, StackSize, SEED, &stack_hash);
    MurMur(stk->data, stk->capacity * stk->el_size, SEED, &data_hash);
#endif

    STACK_ASSERT(stk);
    return SUCCESS;
}

void * StackTop(Stack * stk)
{
    STACK_ASSERT(stk);
    return (void*)((size_t)stk->data + (stk->size - 1) * stk->el_size);
}

int StackExpand(Stack * stk)
{
    STACK_ASSERT(stk);
    void * check = 0;

    ON_DEBUG(stk->data = (void*)((size_t) stk->data - CannarySize);)
    if ((check = realloc(stk->data, (stk->capacity * 2 * stk->el_size ON_DEBUG(+ CannaryShift(stk->capacity * 2 * stk->el_size))))) == NULL)
        return BADDATA;

    stk->data = check;
    stk->capacity *= 2;

#ifdef DEBUG
    stk->data = (void*)((size_t) stk->data + CannarySize);
    MurMur(stk, StackSize, SEED, &stack_hash);
    MurMur(stk->data, stk->capacity * stk->el_size, SEED, &data_hash);
    CannaryInstall(stk);
#endif

    STACK_ASSERT(stk);
    return SUCCESS;
}

int StackShrink(Stack * stk)
{
    STACK_ASSERT(stk);
    void * check = 0;

    ON_DEBUG(stk->data = (void*)((size_t) stk->data - CannarySize);)
    if ((check = realloc(stk->data, ((stk->capacity / 4) * stk->el_size ON_DEBUG(+ CannaryShift((stk->capacity / 4) * stk->el_size))))) == NULL)
        return BADDATA;

    stk->data = check;
    stk->capacity /= 4;

#ifdef DEBUG
    stk->data = (void*)((size_t) stk->data + CannarySize);
    MurMur(stk, StackSize, SEED, &stack_hash);
    MurMur(stk->data, stk->capacity * stk->el_size, SEED, &data_hash);
    CannaryInstall(stk);
#endif

    STACK_ASSERT(stk);
    return SUCCESS;
}


// TODO: Func param
void StackAssertFunc(Stack * stk, int LINE, const char * FILE, const char * FUNC)
{
#ifdef DEBUG
    Error = StackError(stk);
    if (Error)
    {
        fprintf(stderr, "StackAssert failed! %s: %d (%s) (error code: %d)", FILE, LINE, FUNC, Error);
        StackDump(stk, LINE, FILE, FUNC);
        abort();
    }
#endif
}

int StackError(Stack * stk)
{
#ifdef DEBUG
    hash_t stack_hash_check = 0;
    hash_t data_hash_check = 0;

    MurMur(stk, StackSize, SEED, &stack_hash_check);
    // fprintf(stderr, "stack_hash = %d\n", stack_hash_check);
    MurMur(stk->data, stk->capacity * stk->el_size, SEED, &data_hash_check);
    // fprintf(stderr, "data_hash = %d\n", data_hash_check);

    if (stk == NULL)                                                        return BADSTK;
    if ((ssize_t)stk->size < 0 || stk->size > MAX_SIZE)                     return BADSIZE;
    if ((ssize_t)stk->capacity < 0 || stk->capacity > MAX_SIZE)             return BADCAP;
    if ((ssize_t)stk->el_size < 0 || stk->el_size > 32)                              return BADELSIZE;
    if (stk->data == NULL)                                                  return BADDATA;
    if (stack_hash_check != stack_hash)                                     return WRONGSTACKHASH;
    if (data_hash_check != data_hash)                                       return WRONGDATAHASH;
    if (LEFT_CANNARY && RIGHT_CANNARY)
        if (*LEFT_CANNARY != 0xDEADBEEF || *RIGHT_CANNARY != 0xDEADBEEF)    return DEADCANARY;
#endif
    return SUCCESS;

}

void StackDump(Stack * stk, int LINE, const char * CALL_FILE, const char * FUNC)
{
#ifdef DEBUG
    FILE * fp = fopen("stack.dmp", "wb");

    fprintf(fp, "Stack [%p]\n", stk);
    if (Error == WRONGSTACKHASH)         fprintf(fp, "MurMurStack: (imposter is here): %d 𐐘💥╾━╤デ╦︻ඞා\n", stack_hash);
    else                                 fprintf(fp, "MurMurStack: %d\n", stack_hash);


    if (*LEFT_CANNARY == 0xDEADBEEF)     fprintf(fp, "LeftCannary [%p] = %ld\n",           LEFT_CANNARY,    *LEFT_CANNARY);
    else                                 fprintf(fp, "LeftCannary [%p] = %ld [DEAD⚰]\n",  LEFT_CANNARY,    *LEFT_CANNARY);
    if (*RIGHT_CANNARY == 0xDEADBEEF)    fprintf(fp, "RightCannary [%p] = %ld\n",          RIGHT_CANNARY,   *RIGHT_CANNARY);
    else                                 fprintf(fp, "RightCannary [%p] = %ld [DEAD⚰]\n", RIGHT_CANNARY,   *RIGHT_CANNARY);

    fprintf(fp, "called from %s: %d -> (%s)\n", CALL_FILE, LINE, FUNC);
    fprintf(fp, "name \"%s\" born at %s: %d -> (%s)\n", stk->NAME, stk->FILE, stk->LINE, stk->FUNC);
    fprintf(fp, "\ncapacity = %lu\nsize = %lu\ndata[%p]\n", stk->capacity, stk->size, stk->data);
    if (Error == WRONGDATAHASH)          fprintf(fp, "MurMurData: (imposter is here): %d 𐐘💥╾━╤デ╦︻ඞා\n", data_hash);
    else                                 fprintf(fp, "MurMurData: %d\n", data_hash);
    if (stk->data)
    {
        for (int i = stk->size - 1; i >= 0; i--)
            PrintEl(fp, (void*)((size_t)stk->data + i * stk->el_size), stk, i);
    }
    fclose(fp);
#endif
}

void PrintEl(FILE * fp, void * el, Stack * stk, int i)
{
    int toPrint = *(int*) el;
    if (toPrint != 666)         fprintf(fp, "*[%lu] = %c\n", stk->size - i - 1, toPrint);
    else                        fprintf(fp, " [%lu] = %c (pzn)\n", stk->size - i - 1, toPrint);
}

void CannaryInstall(Stack * stk)
{
#ifdef DEBUG
    Cannary_t CanVal = 0xDEADBEEF;
    stk->data = (void*)((size_t) stk->data - CannarySize);
    memcpy(stk->data, &CanVal, CannarySize);
    LEFT_CANNARY = (uint64_t*)(stk->data);
    stk->data = (void*)((size_t) stk->data + CannarySize);

    size_t adr = (size_t)stk->data + stk->capacity * stk->el_size;
    memcpy((void*)(adr + CannaryShift(adr) - 2 * CannarySize), &CanVal, CannarySize);
    RIGHT_CANNARY = (uint64_t*)(adr + CannaryShift(adr) - 2 * CannarySize);

    STACK_ASSERT(stk);
#endif
}

size_t CannaryShift(size_t size)
{
#ifdef DEBUG
    if (size % CannarySize == 0) return 2 * CannarySize;
    return 2 * (CannarySize) + CannarySize - (size % CannarySize);
#endif
}
