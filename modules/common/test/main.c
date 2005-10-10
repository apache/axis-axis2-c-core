#include <axis2_allocator.h>
#include <axis2_environment.h>

int main()
{
    char buff[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_environment_t *env = axis2_environment_create(allocator, NULL, NULL);
    axis2_stream_read(env->stream, buff, 10);
    axis2_stream_write(env->stream, buff, 10);
    return 0;
}
