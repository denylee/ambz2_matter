#include "matter_attribute.h"
#include "matter_flags.h"
#include "chip_porting.h"

int8_t get_data_from_attr_val(ameba_matter_attr_val_t *val, EmberAfAttributeType *attribute_type,
                                 uint16_t *attribute_size, uint8_t *value)
{
    switch (val->type) {
    case AMEBA_MATTER_VAL_TYPE_BOOLEAN:
        if (attribute_type) {
            *attribute_type = ZCL_BOOLEAN_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(bool);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<bool>;
            Traits::WorkingToStorage(val->val._b, *value);
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_INTEGER:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_INTEGER:
        if (attribute_type) {
            *attribute_type = ZCL_INT16U_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(int);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<int>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._i)) {
                Traits::SetNull(*(int *)value);
            } else {
                Traits::WorkingToStorage(val->val._i, *(int *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_FLOAT:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_FLOAT:
        if (attribute_type) {
            *attribute_type = ZCL_SINGLE_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(float);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<float>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._f)) {
                Traits::SetNull(*(float *)value);
            } else {
                Traits::WorkingToStorage(val->val._f, *(float *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_ARRAY:
        if (attribute_type) {
            *attribute_type = ZCL_ARRAY_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = val->val._a._t;
        }
        if (value) {
            int data_size_len = val->val._a._t - val->val._a._s;
            memcpy(value, (uint8_t *)&val->val._a._s, data_size_len);
            memcpy((value + data_size_len), (uint8_t *)val->val._a._b, (*attribute_size - data_size_len));
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_CHAR_STRING:
        if (attribute_type) {
            *attribute_type = ZCL_CHAR_STRING_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = val->val._a._t;
        }
        if (value) {
            int data_size_len = val->val._a._t - val->val._a._s;
            memcpy(value, (uint8_t *)&val->val._a._s, data_size_len);
            memcpy((value + data_size_len), (uint8_t *)val->val._a._b, (*attribute_size - data_size_len));
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_OCTET_STRING:
        if (attribute_type) {
            *attribute_type = ZCL_OCTET_STRING_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = val->val._a._t;
        }
        if (value) {
            int data_size_len = val->val._a._t - val->val._a._s;
            memcpy(value, (uint8_t *)&val->val._a._s, data_size_len);
            memcpy((value + data_size_len), (uint8_t *)val->val._a._b, (*attribute_size - data_size_len));
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_INT8:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_INT8:
        if (attribute_type) {
            *attribute_type = ZCL_INT8S_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(int8_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<int8_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._i8)) {
                Traits::SetNull(*(int8_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._i8, *(int8_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_UINT8:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_UINT8:
        if (attribute_type) {
            *attribute_type = ZCL_INT8U_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint8_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint8_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u8)) {
                Traits::SetNull(*(uint8_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u8, *(uint8_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_INT16:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_INT16:
        if (attribute_type) {
            *attribute_type = ZCL_INT16S_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(int16_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<int16_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._i16)) {
                Traits::SetNull(*(int16_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._i16, *(int16_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_UINT16:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_UINT16:
        if (attribute_type) {
            *attribute_type = ZCL_INT16U_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint16_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint16_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u16)) {
                Traits::SetNull(*(uint16_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u16, *(uint16_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_INT32:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_INT32:
        if (attribute_type) {
            *attribute_type = ZCL_INT32S_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(int32_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<int32_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._i32)) {
                Traits::SetNull(*(int32_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._i32, *(int32_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_UINT32:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_UINT32:
        if (attribute_type) {
            *attribute_type = ZCL_INT32U_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint32_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint32_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u32)) {
                Traits::SetNull(*(uint32_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u32, *(uint32_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_INT64:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_INT64:
        if (attribute_type) {
            *attribute_type = ZCL_INT64S_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(int64_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<int64_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._i64)) {
                Traits::SetNull(*(int64_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._i64, *(int64_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_UINT64:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_UINT64:
        if (attribute_type) {
            *attribute_type = ZCL_INT64U_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint64_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint64_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u64)) {
                Traits::SetNull(*(uint64_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u64, *(uint64_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_ENUM8:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_ENUM8:
        if (attribute_type) {
            *attribute_type = ZCL_ENUM8_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint8_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint8_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u8)) {
                Traits::SetNull(*(uint8_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u8, *(uint8_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_BITMAP8:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_BITMAP8:
        if (attribute_type) {
            *attribute_type = ZCL_BITMAP8_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint8_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint8_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u8)) {
                Traits::SetNull(*(uint8_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u8, *(uint8_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_BITMAP16:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_BITMAP16:
        if (attribute_type) {
            *attribute_type = ZCL_BITMAP16_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint16_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint16_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u16)) {
                Traits::SetNull(*(uint16_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u16, *(uint16_t *)value);
            }
        }
        break;

    case AMEBA_MATTER_VAL_TYPE_BITMAP32:
    case AMEBA_MATTER_VAL_TYPE_NULLABLE_BITMAP32:
        if (attribute_type) {
            *attribute_type = ZCL_BITMAP32_ATTRIBUTE_TYPE;
        }
        if (attribute_size) {
            *attribute_size = sizeof(uint32_t);
        }
        if (value) {
            using Traits = chip::app::NumericAttributeTraits<uint32_t>;
            if ((val->type & AMEBA_MATTER_VAL_NULLANLE_BASE) && Traits::IsNullValue(val->val._u32)) {
                Traits::SetNull(*(uint32_t *)value);
            } else {
                Traits::WorkingToStorage(val->val._u32, *(uint32_t *)value);
            }
        }
        break;

    default:
        printf("ameba_matter_attr_val_type_t not handled: %d\n", val->type);
        break;
    }

    return 0;
}

int8_t get_attr_val_from_data(ameba_matter_attr_val_t *val, EmberAfAttributeType attribute_type,
                                        uint16_t attribute_size, uint8_t *value,
                                        const EmberAfAttributeMetadata * attribute_metadata)
{
    switch (attribute_type) {
    case ZCL_BOOLEAN_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<bool>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        *val = ameba_matter_bool(attribute_value);
        break;
    }

    case ZCL_ARRAY_ATTRIBUTE_TYPE: {
        *val = ameba_matter_array(NULL, 0, 0);
        int data_size_len = val->val._a._t - val->val._a._s;
        int data_count = 0;
        memcpy(&data_count, &value[0], data_size_len);
        *val = ameba_matter_array((value + data_size_len), attribute_size, data_count);
        break;
    }

    case ZCL_CHAR_STRING_ATTRIBUTE_TYPE: {
        *val = ameba_matter_char_str(NULL, 0);
        int data_size_len = val->val._a._t - val->val._a._s;
        int data_count = 0;
        memcpy(&data_count, &value[0], data_size_len);
        *val = ameba_matter_char_str((char *)(value + data_size_len), data_count);
        break;
    }

    case ZCL_OCTET_STRING_ATTRIBUTE_TYPE: {
        *val = ameba_matter_octet_str(NULL, 0);
        int data_size_len = val->val._a._t - val->val._a._s;
        int data_count = 0;
        memcpy(&data_count, &value[0], data_size_len);
        *val = ameba_matter_octet_str((value + data_size_len), data_count);
        break;
    }

    case ZCL_INT8S_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<int8_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_int8(nullable<int8_t>());
            } else {
                *val = ameba_matter_nullable_int8(attribute_value);
            }
        } else {
            *val = ameba_matter_int8(attribute_value);
        }
        break;
    }

    case ZCL_INT8U_ATTRIBUTE_TYPE:
    case ZCL_PERCENT_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<uint8_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_uint8(nullable<uint8_t>());
            } else {
                *val = ameba_matter_nullable_uint8(attribute_value);
            }
        } else {
            *val = ameba_matter_uint8(attribute_value);
        }
        break;
    }

    case ZCL_INT16S_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<int16_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_int16(nullable<int16_t>());
            } else {
                *val = ameba_matter_nullable_int16(attribute_value);
            }
        } else {
            *val = ameba_matter_int16(attribute_value);
        }
        break;
    }

    case ZCL_INT16U_ATTRIBUTE_TYPE:
    case ZCL_PERCENT100THS_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<uint16_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_uint16(nullable<uint16_t>());
            } else {
                *val = ameba_matter_nullable_uint16(attribute_value);
            }
        } else {
            *val = ameba_matter_uint16(attribute_value);
        }
        break;
    }

    case ZCL_INT32S_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<int32_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_int32(nullable<int32_t>());
            } else {
                *val = ameba_matter_nullable_int32(attribute_value);
            }
        } else {
            *val = ameba_matter_int32(attribute_value);
        }
        break;
    }

    case ZCL_INT32U_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<uint32_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_uint32(nullable<uint32_t>());
            } else {
                *val = ameba_matter_nullable_uint32(attribute_value);
            }
        } else {
            *val = ameba_matter_uint32(attribute_value);
        }
        break;
    }

    case ZCL_INT64S_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<int64_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_int64(nullable<int64_t>());
            } else {
                *val = ameba_matter_nullable_int64(attribute_value);
            }
        } else {
            *val = ameba_matter_int64(attribute_value);
        }
        break;
    }

    case ZCL_INT64U_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<uint64_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_uint64(nullable<uint64_t>());
            } else {
                *val = ameba_matter_nullable_uint64(attribute_value);
            }
        } else {
            *val = ameba_matter_uint64(attribute_value);
        }
        break;
    }

    case ZCL_ENUM8_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<uint8_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_enum8(nullable<uint8_t>());
            } else {
                *val = ameba_matter_nullable_enum8(attribute_value);
            }
        } else {
            *val = ameba_matter_enum8(attribute_value);
        }
        break;
    }

    case ZCL_BITMAP8_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<uint8_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_bitmap8(nullable<uint8_t>());
            } else {
                *val = ameba_matter_nullable_bitmap8(attribute_value);
            }
        } else {
            *val = ameba_matter_bitmap8(attribute_value);
        }
        break;
    }

    case ZCL_BITMAP16_ATTRIBUTE_TYPE: {
        using Traits = chip::app::NumericAttributeTraits<uint16_t>;
        Traits::StorageType attribute_value;
        memcpy((uint8_t *)&attribute_value, value, sizeof(Traits::StorageType));
        if (attribute_metadata->IsNullable()) {
            if (Traits::IsNullValue(attribute_value)) {
                *val = ameba_matter_nullable_bitmap16(nullable<uint16_t>());
            } else {
                *val = ameba_matter_nullable_bitmap16(attribute_value);
            }
        } else {
            *val = ameba_matter_bitmap16(attribute_value);
        }
        break;
    }

    default:
        *val = ameba_matter_invalid(NULL);
        break;
    }

    return 0;
}

static EmberAfDefaultAttributeValue get_default_value_from_data(ameba_matter_attr_val_t *val,
                                                                EmberAfAttributeType attribute_type,
                                                                uint16_t attribute_size)
{
    EmberAfDefaultAttributeValue _default_value = (uint16_t)0;
    uint8_t *value = (uint8_t *)pvPortCalloc(1, attribute_size);

    if (!value)
    {
        printf("Could not allocate value buffer for default value\n");
        return _default_value;
    }
    get_data_from_attr_val(val, &attribute_type, &attribute_size, value);

    if (attribute_size > 2)
    {
        /* Directly set the pointer */
        _default_value = value;
    }
    else
    {
        /* This data is 2 bytes or less. This should be represented as uint16. Copy the bytes appropriately
        for 0 or 1 or 2 bytes to be converted to uint16. Then free the allocated buffer. */
        uint16_t int_value = 0;
        if (attribute_size == 2)
        {
            memcpy(&int_value, value, attribute_size);
        }
        else if (attribute_size == 1) 
        {
            int_value = (uint16_t)*value;
        }
        _default_value = int_value;
        vPortFree(value);
    }
    return _default_value;
}

Attribute::Attribute(ameba_matter_attr_val_t attribute_value, uint32_t _attribute_id, uint32_t _cluster_id, uint16_t _endpoint_id, uint16_t attribute_flags)
{
    val = attribute_value;
    endpoint_id = _endpoint_id;
    cluster_id = _cluster_id;
    attribute_id = _attribute_id;
    flags = attribute_flags | ATTRIBUTE_FLAG_EXTERNAL_STORAGE;
    int8_t err;
    default_value = (EmberAfDefaultOrMinMaxAttributeValue *) pvPortMalloc(sizeof(EmberAfDefaultOrMinMaxAttributeValue));
    // bounds = (ameba_matter_attr_bounds_t *) pvPortMalloc(sizeof(ameba_matter_attr_bounds_t));

    /* store non-volatile attributes in kvs */
    if (flags & ATTRIBUTE_FLAG_NONVOLATILE)
    {
        // setup key (endpoint_id/cluster_id/attribute_id)
        char *key = (char *) pvPortMalloc(VARIABLE_NAME_SIZE);
        sprintf(key, "%d/%d/%d", endpoint_id, cluster_id, attribute_id);
        // attempt to get from kvs
        ameba_matter_attr_val_t *ptemp_val = (ameba_matter_attr_val_t *) pvPortMalloc(sizeof(ameba_matter_attr_val_t));
        if (!ptemp_val)
        {
            printf("failed to allocate for attribute value, aborting\n");
            return;
        }

        err = get_nvs(key, ptemp_val);
        if (err == 0)
        {
            // if get from kvs success, set the attribute_value to the value gotten from kvs
            memcpy(&val, ptemp_val, sizeof(ameba_matter_attr_val_t));
        }
        else
        {
            // else (not found), set the attribute_value to the value passed in, then store in kvs
            // TODO: only enter here if error is "not found"
            // memcpy(&val, &attribute_value, sizeof(ameba_matter_attr_val_t));
            set_nvs(key, &val);
        }
        vPortFree(ptemp_val);
    }
    else
    {
        // set the attribute_value to the value passed in, don't need to store in kvs
        // memcpy(&val, &attribute_value, sizeof(ameba_matter_attr_val_t));
    }

    set_default_value_from_current_val();
}

Attribute *Attribute::get_next()
{
    return next;
}

void Attribute::set_next(Attribute *attribute)
{
    next = attribute;
}

int8_t Attribute::get_nvs(const char *key, ameba_matter_attr_val_t *value)
{
    // TODO: should we pass in outlen?
    int8_t err;
    uint8_t *buf = (uint8_t *) pvPortMalloc(sizeof(ameba_matter_attr_val_t));
    size_t outlen;
    err = getPref_bin_new(key, key, buf, sizeof(ameba_matter_attr_val_t), &outlen);
    if (err == 0)
    {
        printf("retrieved variable with key %s successfully\n", key);
        memcpy(value, buf, sizeof(ameba_matter_attr_val_t));
    }
    vPortFree(buf);
    return err;
}

int8_t Attribute::set_nvs(const char *key, ameba_matter_attr_val_t *value)
{
    int8_t err;
    uint8_t *buf = (uint8_t *) pvPortMalloc(sizeof(ameba_matter_attr_val_t));
    memcpy(buf, value, sizeof(ameba_matter_attr_val_t));
    err = setPref_new(key, key, buf, sizeof(ameba_matter_attr_val_t));
    if (err == 1)
    {
        printf("stored variable with key %s successfully\n", key);
    }
    vPortFree(buf);
    return err;
}

ameba_matter_attr_bounds_t *Attribute::get_bounds()
{
    return bounds;
}

int8_t Attribute::set_bounds(ameba_matter_attr_val_t min, ameba_matter_attr_val_t max)
{
    // Check if bounds can be set 
    if (val.type == AMEBA_MATTER_VAL_TYPE_CHAR_STRING ||
        val.type == AMEBA_MATTER_VAL_TYPE_OCTET_STRING ||
        val.type == AMEBA_MATTER_VAL_TYPE_ARRAY)
    {
        printf("Bounds cannot be set for string/array type attributes\n");
        return -1;
    }
    if ((val.type != min.type) || (val.type != max.type)) 
    {
        printf("Cannot set bounds because of val type mismatch: expected: %d, min: %d, max: %d\n",
                 val.type, min.type, max.type);
        return -1;
    }

    free_default_value();

    bounds = (ameba_matter_attr_bounds_t*) pvPortCalloc(1, sizeof(ameba_matter_attr_bounds_t));
    if (!bounds)
    {
        ChipLogError(DeviceLayer, "Could not allocate bounds");
        return -1;
    }

    // set bounds
    memset(bounds, 0, sizeof(ameba_matter_attr_bounds_t));
    memcpy((void*)&bounds->min, (void*)&min, sizeof(ameba_matter_attr_val_t));
    memcpy((void*)&bounds->max, (void*)&max, sizeof(ameba_matter_attr_val_t));
    flags |= ATTRIBUTE_FLAG_MIN_MAX;

    // set default value again after setting bounds and flags
    set_default_value_from_current_val();

    return 0;
}

int8_t Attribute::free_default_value()
{
    /* Free value if data is more than 2 bytes or if it is min max attribute */
    if (flags & ATTRIBUTE_FLAG_MIN_MAX) 
    {
        if (default_value_size > 2) {
            if (default_value->ptrToMinMaxValue->defaultValue.ptrToDefaultValue) {
                vPortFree((void *)default_value->ptrToMinMaxValue->defaultValue.ptrToDefaultValue);
            }
            if (default_value->ptrToMinMaxValue->minValue.ptrToDefaultValue) {
                vPortFree((void *)default_value->ptrToMinMaxValue->minValue.ptrToDefaultValue);
            }
            if (default_value->ptrToMinMaxValue->maxValue.ptrToDefaultValue) {
                vPortFree((void *)default_value->ptrToMinMaxValue->maxValue.ptrToDefaultValue);
            }
        }
        vPortFree((void *)default_value->ptrToMinMaxValue);
    }
    else if (default_value_size > 2)
    {
        if (default_value->ptrToDefaultValue)
        {
            vPortFree((void *)default_value->ptrToDefaultValue);
        }
    }
    return 0;
}

int8_t Attribute::set_default_value_from_current_val()
{
    /* Get size */
    EmberAfAttributeType attribute_type = 0;
    uint16_t attribute_size = 0;
    get_data_from_attr_val(&val, &attribute_type, &attribute_size, NULL);

    /* Get and set value */
    if (flags & ATTRIBUTE_FLAG_MIN_MAX)
    {
        EmberAfAttributeMinMaxValue *temp_value = (EmberAfAttributeMinMaxValue *)pvPortCalloc(1,
                                                                                sizeof(EmberAfAttributeMinMaxValue));
        if (!temp_value) 
        {
            printf("Could not allocate ptrToMinMaxValue for default value\n");
            return -1;
        }
        temp_value->defaultValue = get_default_value_from_data(&val, attribute_type, attribute_size);
        temp_value->minValue = get_default_value_from_data(&bounds->min, attribute_type,
                                                           attribute_size);
        temp_value->maxValue = get_default_value_from_data(&bounds->max, attribute_type,
                                                           attribute_size);
        default_value->ptrToMinMaxValue = temp_value;
    } else if (attribute_size > 2) {
        EmberAfDefaultAttributeValue temp_value = get_default_value_from_data(&val, attribute_type, attribute_size);
        default_value->ptrToDefaultValue = temp_value.ptrToDefaultValue;
    }
    else
    {
        EmberAfDefaultAttributeValue temp_value = get_default_value_from_data(&val, attribute_type, attribute_size);
        default_value->defaultValue = temp_value.defaultValue;
    }
    default_value_size = attribute_size;
    return 0;
}

