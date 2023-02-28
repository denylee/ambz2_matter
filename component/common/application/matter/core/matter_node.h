# pragma once

#include <platform_stdlib.h>
#include "matter_config.h"
#include "matter_endpoint.h"

class Node
{
public:
    // Node(Callback attribute_callback, Callback identify_callback); // create
    // ~ Node(); // destroy
    int8_t add_endpoint(Endpoint *endpoint, uint32_t device_type_id, uint8_t device_type_version);
    Endpoint *get_endpoint_by_id(uint32_t endpoint_id);
    Endpoint *endpoint_list = NULL;
    uint16_t min_unused_endpoint_id = 0;

private:
};
