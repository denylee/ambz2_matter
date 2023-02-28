#include "matter_node.h"
#include "matter_config.h"
#include "matter_endpoint.h"

int8_t Node::add_endpoint(Endpoint *endpoint, uint32_t device_type_id, uint8_t device_type_version)
{
    int8_t err;
    Endpoint *previous_endpoint = NULL;
    Endpoint *current_endpoint = endpoint_list;
    while (current_endpoint)
    {
        previous_endpoint = current_endpoint;
        current_endpoint = current_endpoint->get_next();
    }
    if (previous_endpoint == NULL)
    {
        endpoint_list = endpoint;
    }
    else
    {
        previous_endpoint->set_next(endpoint);
    }
    endpoint->endpoint_id = min_unused_endpoint_id++;
    endpoint->init_endpoint();
    err = endpoint->add_device_type(device_type_id, device_type_version);

    if (err == 0)
    {
        ChipLogProgress(DeviceLayer, "Successfully added device type %d", device_type_id);
    }

    return 0;
}

Endpoint *Node::get_endpoint_by_id(uint32_t endpoint_id)
{
    Endpoint *current_endpoint = endpoint_list;
    while (current_endpoint)
    {
        if (current_endpoint->endpoint_id == endpoint_id)
        {
            break;
        }
        current_endpoint = current_endpoint->get_next();
    }
    return current_endpoint;
}
