#include "matter_endpoint.h"
#include "matter_flags.h"
#include "matter_node.h"
#include "matter_core.h"
#include "matter_attribute.h"

using namespace chip;
using namespace chip::app::Clusters;
using chip::CommandId;

Endpoint::Endpoint(config_t config, uint16_t endpoint_flags)
{
    flags = endpoint_flags;
    parent_endpoint_id = chip::kInvalidEndpointId;
    endpoint_config = config;
}

void Endpoint::init_endpoint()
{
    if (endpoint_config.identify.enabled)
    {
        Cluster *identify = new Cluster(endpoint_config, Identify::Id, endpoint_id, (endpoint_config.identify.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(identify);
    }
    if (endpoint_config.groups.enabled)
    {
        Cluster *groups = new Cluster(endpoint_config, Groups::Id, endpoint_id, (endpoint_config.groups.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(groups);
    }
    if (endpoint_config.scenes.enabled)
    {
        Cluster *scenes = new Cluster(endpoint_config, Scenes::Id, endpoint_id, (endpoint_config.scenes.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(scenes);
    }
    if (endpoint_config.on_off.enabled)
    {
        Cluster *on_off = new Cluster(endpoint_config, OnOff::Id, endpoint_id, (endpoint_config.on_off.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(on_off);
    }
    if (endpoint_config.level_control.enabled)
    {
        Cluster *level_control = new Cluster(endpoint_config, LevelControl::Id, endpoint_id, (endpoint_config.level_control.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(level_control);
    }
    if (endpoint_config.basic_information.enabled)
    {
        Cluster *basic_information = new Cluster(endpoint_config, BasicInformation::Id, endpoint_id, (endpoint_config.basic_information.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(basic_information);
    }
    if (endpoint_config.general_commissioning.enabled)
    {
        Cluster *general_commissioning = new Cluster(endpoint_config, GeneralCommissioning::Id, endpoint_id, (endpoint_config.general_commissioning.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(general_commissioning);
    }
    if (endpoint_config.network_commissioning.enabled)
    {
        Cluster *network_commissioning = new Cluster(endpoint_config, NetworkCommissioning::Id, endpoint_id, (endpoint_config.network_commissioning.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(network_commissioning);
    }
    if (endpoint_config.general_diagnostics.enabled)
    {
        Cluster *general_diagnostics = new Cluster(endpoint_config, GeneralDiagnostics::Id, endpoint_id, (endpoint_config.general_diagnostics.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(general_diagnostics);
    }
#if 0 // thread enabled
    if (endpoint_config.diagnostics_network_thread.enabled)
    {
        Cluster *diagnostics_network_thread = new Cluster(endpoint_config, ThreadNetworkDiagnostics::Id, endpoint_id, (endpoint_config.diagnostics_network_thread.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(diagnostics_network_thread);
    }
#endif
#if 1 // wifi enabled
    if (endpoint_config.diagnostics_network_wifi.enabled)
    {
        Cluster *diagnostics_network_wifi = new Cluster(endpoint_config, WiFiNetworkDiagnostics::Id, endpoint_id, (endpoint_config.diagnostics_network_wifi.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(diagnostics_network_wifi);
    }
#endif
    if (endpoint_config.administrator_commissioning.enabled)
    {
        Cluster *administrator_commissioning = new Cluster(endpoint_config, AdministratorCommissioning::Id, endpoint_id, (endpoint_config.administrator_commissioning.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(administrator_commissioning);
    }
    if (endpoint_config.operational_credentials.enabled)
    {
        Cluster *operational_credentials = new Cluster(endpoint_config, OperationalCredentials::Id, endpoint_id, (endpoint_config.operational_credentials.server) ? CLUSTER_FLAG_SERVER : CLUSTER_FLAG_CLIENT);
        add_cluster(operational_credentials);
    }
}

Endpoint *Endpoint::get_next()
{
    return next;
}

void Endpoint::set_next(Endpoint *endpoint)
{
    next = endpoint;
}

Cluster *Endpoint::get_cluster_by_id(uint32_t cluster_id)
{
    Cluster *current_cluster = cluster_list;
    while (current_cluster)
    {
        if (current_cluster->cluster_id == cluster_id)
        {
            break;
        }
        current_cluster = current_cluster->get_next();
    }
    return current_cluster;
}

uint16_t Endpoint::get_cluster_count()
{
    uint16_t count = 0;
    Cluster *current_cluster = cluster_list;
    while (current_cluster)
    {
        current_cluster = current_cluster->get_next();
        count++;
    }
    return count;
}

void Endpoint::add_cluster(Cluster *cluster)
{
    Cluster *previous_cluster = NULL;
    Cluster *current_cluster = cluster_list;
    while (current_cluster)
    {
        previous_cluster = current_cluster;
        current_cluster = current_cluster->get_next();
    }
    if (previous_cluster == NULL)
    {
        cluster_list = cluster;
    }
    else
    {
        previous_cluster->set_next(cluster);
    }
}

int8_t Endpoint::add_device_type(uint32_t device_type_id, uint8_t device_type_version)
{
    if (device_type_count >= AMEBA_MATTER_MAX_DEVICE_TYPE_COUNT)
    {
        ChipLogError(DeviceLayer, "Max device type count reached, failed to add device type: %d", device_type_id);
        return -1;
    }

    device_type_ids[device_type_count] = device_type_id;
    device_type_versions[device_type_count] = device_type_version;
    device_type_count++;
    return 0;
}

int8_t Endpoint::set_parent_endpoint(Endpoint *endpoint, Endpoint *parent_endpoint)
{
    if (!endpoint || !parent_endpoint) {
        ChipLogError(DeviceLayer, "Endpoint or parent_endpoint cannot be NULL");
        return -1;
    }
    Endpoint *current_endpoint = (Endpoint *)endpoint;
    Endpoint *current_parent_endpoint = (Endpoint *)parent_endpoint;
    current_endpoint->parent_endpoint_id = current_parent_endpoint->endpoint_id;
    return 0;
}

int8_t Endpoint::enable()
{
    /* Endpoint Type */
    EmberAfEndpointType *temp_endpoint_type = (EmberAfEndpointType *) pvPortCalloc(1, sizeof(EmberAfEndpointType));
    if (!endpoint_type) {
        ChipLogError(DeviceLayer, "Couldn't allocate endpoint_type");
        /* goto cleanup is not used here to avoid 'crosses initialization' of data_versions below */
        return -1;
    }
    endpoint_type = temp_endpoint_type;

    /* Device types */
    EmberAfDeviceType *temp_device_types_ptr = (EmberAfDeviceType *) pvPortCalloc(device_type_count, sizeof(EmberAfDeviceType));
    if (!temp_device_types_ptr) {
        ChipLogError(DeviceLayer, "Couldn't allocate device_types");
        vPortFree(temp_endpoint_type);
        endpoint_type = NULL;
        /* goto cleanup is not used here to avoid 'crosses initialization' of device_types below */
        return -1;
    }
    for (size_t i = 0; i < device_type_count; ++i) {
        temp_device_types_ptr[i].deviceId = device_type_ids[i];
        temp_device_types_ptr[i].deviceVersion = device_type_versions[i];
    }
    chip::Span<EmberAfDeviceType> _device_types(temp_device_types_ptr, device_type_count);
    device_types_ptr = temp_device_types_ptr;

    /* Clusters */
    Cluster *cluster = cluster_list;
    int cluster_count = get_cluster_count();
    int cluster_index = 0;

    DataVersion *temp_data_versions_ptr = (DataVersion *) pvPortCalloc(1, cluster_count * sizeof(DataVersion));
    if (!temp_data_versions_ptr) {
        ChipLogError(DeviceLayer, "Couldn't allocate data_versions");
        vPortFree(temp_data_versions_ptr);
        vPortFree(temp_endpoint_type);
        data_versions_ptr = NULL;
        endpoint_type = NULL;
        /* goto cleanup is not used here to avoid 'crosses initialization' of data_versions below */
        return -1;
    }
    chip::Span<chip::DataVersion> _data_versions(temp_data_versions_ptr, cluster_count);
    data_versions_ptr = temp_data_versions_ptr;

    /* Variables */
    /* This is needed to avoid 'crosses initialization' errors because of goto */
    uint8_t err = 0;
    lock::status_t lock_status = lock::LOCK_FAILED;
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    EmberAfCluster *matter_clusters = NULL;
    Attribute *attribute = NULL;
    int attribute_count = 0;
    int attribute_index = 0;
    EmberAfAttributeMetadata *matter_attributes = NULL;
    CommandId *accepted_command_ids = NULL;
    CommandId *generated_command_ids = NULL;
    Command *command = NULL;
    int command_count = 0;
    int command_index = 0;
    int command_flag = COMMAND_FLAG_NONE;
    int endpoint_index = 0;

    matter_clusters = (EmberAfCluster *) pvPortCalloc(1, cluster_count * sizeof(EmberAfCluster));
    if (!matter_clusters) {
        ChipLogError(DeviceLayer, "Couldn't allocate matter_clusters");
        err = -1;
        goto cleanup;
    }

    while (cluster) {
        /* Attributes */
        attribute = cluster->attribute_list;
        attribute_count = cluster->get_attribute_count();
        // attribute_count = attribute::get_count(attribute);
        attribute_index = 0;
        matter_attributes = (EmberAfAttributeMetadata *) pvPortCalloc(1, attribute_count * sizeof(EmberAfAttributeMetadata));
        if (!matter_attributes) {
            if (attribute_count != 0) {
                ChipLogError(DeviceLayer, "Couldn't allocate matter_attributes");
                err = -1;
                break;
            }
        }

        while (attribute) {
            matter_attributes[attribute_index].attributeId = attribute->attribute_id;
            matter_attributes[attribute_index].mask = attribute->flags;
            matter_attributes[attribute_index].defaultValue = *(attribute->default_value);
            get_data_from_attr_val(&attribute->val, &matter_attributes[attribute_index].attributeType,
                                              &matter_attributes[attribute_index].size, NULL);

            matter_clusters[cluster_index].clusterSize += matter_attributes[attribute_index].size;
            attribute = attribute->get_next();
            attribute_index++;
        }

        /* Commands */
        command = NULL;
        command_count = 0;
        command_index = 0;
        command_flag = COMMAND_FLAG_NONE;
        accepted_command_ids = NULL;
        generated_command_ids = NULL;

        /* Client Generated Commands */
        command_flag = COMMAND_FLAG_ACCEPTED;
        command = cluster->command_list;
        command_count = cluster->get_command_count(command_flag);
        // command_count = command::get_count(command, command_flag);
        if (command_count > 0) {
            command_index = 0;
            accepted_command_ids = (CommandId *) pvPortCalloc(1, (command_count + 1) * sizeof(CommandId));
            if (!accepted_command_ids) {
                ChipLogError(DeviceLayer, "Couldn't allocate accepted_command_ids");
                err = -1;
                break;
            }
            while (command) {
                if (command->flags & command_flag) {
                    accepted_command_ids[command_index] = command->command_id;
                    command_index++;
                }
                command = command->get_next();
            }
            accepted_command_ids[command_index] = kInvalidCommandId;
        }

        /* Server Generated Commands */
        command_flag = COMMAND_FLAG_GENERATED;
        command = cluster->command_list;
        command_count = cluster->get_command_count(command_flag);
        // command_count = command::get_count(command, command_flag);
        if (command_count > 0) {
            command_index = 0;
            generated_command_ids = (CommandId *) pvPortCalloc(1, (command_count + 1) * sizeof(CommandId));
            if (!generated_command_ids) {
                ChipLogError(DeviceLayer, "Couldn't allocate generated_command_ids");
                err = -1;
                break;
            }
            while (command) {
                if (command->flags & command_flag) {
                    generated_command_ids[command_index] = command->command_id;
                    command_index++;
                }
                command = command->get_next();
            }
            generated_command_ids[command_index] = kInvalidCommandId;
        }

        /* Fill up the cluster */
        matter_clusters[cluster_index].clusterId = cluster->cluster_id;
        matter_clusters[cluster_index].attributes = matter_attributes;
        matter_clusters[cluster_index].attributeCount = attribute_count;
        matter_clusters[cluster_index].mask = cluster->flags;
        matter_clusters[cluster_index].functions = (EmberAfGenericClusterFunction *)cluster->function_list;
        matter_clusters[cluster_index].acceptedCommandList = accepted_command_ids;
        matter_clusters[cluster_index].generatedCommandList = generated_command_ids;

        /* Get next cluster */
        temp_endpoint_type->endpointSize += matter_clusters[cluster_index].clusterSize;
        cluster = cluster->get_next();
        cluster_index++;

        /* This is to avoid double free in case of errors */
        matter_attributes = NULL;
        accepted_command_ids = NULL;
        generated_command_ids = NULL;
    }
    if (err != 0) {
        goto cleanup;
    }

    temp_endpoint_type->cluster = matter_clusters;
    temp_endpoint_type->clusterCount = cluster_count;

    /* Take lock if not already taken */
    lock_status = lock::chip_stack_lock(portMAX_DELAY);
    if (lock_status == lock::LOCK_FAILED) {
        ChipLogError(DeviceLayer, "Could not get task context");
        goto cleanup;
    }
printf("%s, %d\r\n", __FUNCTION__, __LINE__);

    /* Add Endpoint */
    endpoint_index = matter_core_get_next_endpoint_index();
    status = emberAfSetDynamicEndpoint(endpoint_index, endpoint_id, temp_endpoint_type, _data_versions,
                                       _device_types, parent_endpoint_id);
printf("%s, %d\r\n", __FUNCTION__, __LINE__);
    if (status != EMBER_ZCL_STATUS_SUCCESS) {
        ChipLogError(DeviceLayer, "Error adding dynamic endpoint %d: 0x%x", endpoint_id, status);
        err = -1;
        if (lock_status == lock::LOCK_SUCCESS) {
            lock::chip_stack_unlock();
        }
        goto cleanup;
    }
    if (lock_status == lock::LOCK_SUCCESS) {
        lock::chip_stack_unlock();
    }
    ChipLogError(DeviceLayer, "Dynamic endpoint %d added", endpoint_id);
    return err;

cleanup:
    if (generated_command_ids) {
        vPortFree(generated_command_ids);
    }
    if (accepted_command_ids) {
        vPortFree(accepted_command_ids);
    }
    if (matter_attributes) {
        vPortFree(matter_attributes);
    }
    if (matter_clusters) {
        for (int cluster_index = 0; cluster_index < cluster_count; cluster_index++) {
            /* Free attributes */
            if (matter_clusters[cluster_index].attributes) {
                vPortFree((void *)matter_clusters[cluster_index].attributes);
            }
            /* Free commands */
            if (matter_clusters[cluster_index].acceptedCommandList) {
                vPortFree((void *)matter_clusters[cluster_index].acceptedCommandList);
            }
            if (matter_clusters[cluster_index].generatedCommandList) {
                vPortFree((void *)matter_clusters[cluster_index].generatedCommandList);
            }
        }
        vPortFree(matter_clusters);
    }
    if (temp_data_versions_ptr) {
        vPortFree(temp_data_versions_ptr);
        data_versions_ptr = NULL;
    }
    if (temp_device_types_ptr) {
        vPortFree(temp_device_types_ptr);
        device_types_ptr = NULL;
    }
    if (temp_endpoint_type) {
        vPortFree(temp_endpoint_type);
        endpoint_type = NULL;
    }
    return err;
}
