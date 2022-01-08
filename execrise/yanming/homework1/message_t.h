#include "common.h"

struct message_t {
    u_int32_t message_length;
    u_int32_t message_type;
    char buf[1024];
};