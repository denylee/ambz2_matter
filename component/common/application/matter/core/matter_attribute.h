#pragma once

#include <platform_stdlib.h>
#include <app/util/attribute-storage.h>
#include "matter_config.h"
#include <app/util/attribute-storage.h>

class Attribute
{
public:
    Attribute(ameba_matter_attr_val_t attribute_value, uint32_t _attribute_id, uint32_t _cluster_id, uint16_t _endpoint_id, uint16_t attribute_flags); // create
    ~ Attribute(); // destroy
    Attribute *get_next(void);
    void set_next(Attribute *attribute);
    int8_t get_nvs(const char* key, ameba_matter_attr_val_t *value);
    int8_t set_nvs(const char* key, ameba_matter_attr_val_t *value);
    ameba_matter_attr_bounds_t *get_bounds(void);
    int8_t set_bounds(ameba_matter_attr_val_t min, ameba_matter_attr_val_t max);
    int8_t free_default_value(void);
    int8_t set_default_value_from_current_val(void);
    uint32_t attribute_id;
    uint32_t cluster_id;
    // TODO: endpoint_id is 16bits
    uint16_t endpoint_id;
    uint16_t flags;
    ameba_matter_attr_val_t val;
    ameba_matter_attr_bounds_t *bounds;
    EmberAfDefaultOrMinMaxAttributeValue *default_value;
    uint16_t default_value_size;
    // Callback override_callback;
    Attribute *next = NULL;
};

int8_t get_data_from_attr_val(ameba_matter_attr_val_t *val, EmberAfAttributeType *attribute_type, uint16_t *attribute_size, uint8_t *value);
int8_t get_attr_val_from_data(ameba_matter_attr_val_t *val, EmberAfAttributeType attribute_type, uint16_t attribute_size, uint8_t *value, const EmberAfAttributeMetadata * attribute_metadata);
