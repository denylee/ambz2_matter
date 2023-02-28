#pragma once

#include <platform_stdlib.h>
#include "matter_config.h"
#include "matter_cluster.h"

// class Endpoint
// {
// public:
//     Endpoint(config_t config); // create
//     // ~ Endpoint(); // destroy
//     Endpoint *get_next(void);
//     void set_next(Endpoint *endpoint);
//     uint16_t endpoint_id;

// protected:
//     Endpoint *next = NULL;
//     Cluster *cluster_list;
// };

using chip::DataVersion;

#define AMEBA_MATTER_MAX_DEVICE_TYPE_COUNT  16

class Endpoint
{
public:
    Endpoint(config_t config, uint16_t endpoint_flags); // create
    // ~ Endpoint(); // destroy
    Endpoint *get_next(void);
    void init_endpoint(void);
    int8_t add_device_type(uint32_t device_type_id, uint8_t device_type_version);
    void set_next(Endpoint *endpoint);
    Cluster *get_cluster_by_id(uint32_t cluster_id);
    uint16_t get_cluster_count(void);
    void add_cluster(Cluster *cluster);
    int8_t set_parent_endpoint(Endpoint *endpoint, Endpoint *parent_endpoint);
    int8_t enable(void);
    uint16_t endpoint_id;
    uint8_t device_type_count = 0;
    uint32_t device_type_ids[AMEBA_MATTER_MAX_DEVICE_TYPE_COUNT];
    uint32_t device_type_versions[AMEBA_MATTER_MAX_DEVICE_TYPE_COUNT];
    uint16_t flags;
    Cluster *cluster_list = NULL;
    EmberAfEndpointType *endpoint_type;
    DataVersion *data_versions_ptr;
    EmberAfDeviceType *device_types_ptr;
    uint16_t parent_endpoint_id;
    config_t endpoint_config;

protected:
    Endpoint *next = NULL;
};
