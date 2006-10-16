#include <tcpmon_util.h>
#include <axis2_string.h>

typedef struct tcpmon_util_allocator
{
    int allocated;
    int index;
    axis2_char_t* buffer;
}
tcpmon_util_allocator_t;


static void add_string(const axis2_env_t* env,
        tcpmon_util_allocator_t* allocator,
        axis2_char_t* string);

static void add_axis2_char_t(const axis2_env_t* env,
        tcpmon_util_allocator_t* allocator,
        axis2_char_t c,
        int turns);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
tcpmon_util_format_as_xml(const axis2_env_t* env, axis2_char_t* data)
{
		return data;
}

void add_string(const axis2_env_t* env,
        tcpmon_util_allocator_t* allocator,
        axis2_char_t* string)
{
    int additional_len = 0;
    void* dest = NULL;
    void* src = NULL;
    int count = 0;

    additional_len = AXIS2_STRLEN(string) + 1;
    if (allocator-> index + additional_len  >= allocator-> allocated)
    {
        if (allocator-> allocated == 0)
        {
            allocator-> buffer =
                AXIS2_MALLOC(env-> allocator, additional_len);
        }
        else
        {
            allocator-> buffer =
                AXIS2_REALLOC(env-> allocator, allocator-> buffer,
                        allocator-> allocated + additional_len);
        }
        allocator-> allocated += additional_len;
    }

    /* copy memory */
    dest = allocator-> buffer + allocator-> index;
    src = string;
    count = additional_len; /* this is with the terminating zero */
    memcpy(dest, src, count);

    allocator-> index += count - 1;
}

void add_axis2_char_t(const axis2_env_t* env,
        tcpmon_util_allocator_t* allocator,
        axis2_char_t c,
        int turns)
{
    int additional_len = 0;

    additional_len = turns + 1;
    if (allocator-> index + additional_len  >= allocator-> allocated)
    {
        if (allocator-> allocated == 0)
        {
            allocator-> buffer =
                AXIS2_MALLOC(env-> allocator, additional_len);
        }
        else
        {
            allocator-> buffer =
                AXIS2_REALLOC(env-> allocator, allocator-> buffer,
                        allocator-> allocated + additional_len);
        }
        allocator-> allocated += additional_len;
    }

    /* copy memory */
    memset(allocator-> buffer + allocator-> index, c, turns);

    allocator-> index += turns;

}
