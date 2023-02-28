#include <stdlib.h>
#include <stdint.h>

#include "matter_core.h"
#include "matter_ota.h"
#include "matter_node.h"
#include "matter_endpoint.h"
#include <DeviceInfoProviderImpl.h>

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Commands.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/util/af-types.h>
#include <app/util/attribute-storage.h>
#include <app/util/basic-types.h>
#include <app/util/util.h>
#include <app/clusters/identify-server/identify-server.h>
#include <app/clusters/network-commissioning/network-commissioning.h>
#include <app/server/Dnssd.h>
#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

#include <lib/dnssd/Advertiser.h>

#include <platform/Ameba/AmebaConfig.h>
#include <platform/Ameba/FactoryDataProvider.h>
#include <platform/Ameba/NetworkCommissioningDriver.h>

#include <route_hook/ameba_route_hook.h>

#include <support/CHIPMem.h>
#include <support/CodeUtils.h>
#include <support/ErrorStr.h>

using namespace ::chip;
using namespace ::chip::DeviceLayer;

extern Node *node;

app::Clusters::NetworkCommissioning::Instance
    sWiFiNetworkCommissioningInstance(0 /* Endpoint Id */, &(NetworkCommissioning::AmebaWiFiDriver::GetInstance()));

chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
chip::DeviceLayer::FactoryDataProvider mFactoryDataProvider;

void matter_core_device_callback_internal(const ChipDeviceEvent * event, intptr_t arg)
{
    switch (event->Type)
    {
    case DeviceEventType::kInternetConnectivityChange:
#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
        static bool isOTAInitialized = false;
#endif
        if (event->InternetConnectivityChange.IPv4 == kConnectivity_Established)
        {
            ChipLogProgress(DeviceLayer, "IPv4 Server ready...");
            chip::app::DnssdServer::Instance().StartServer();
        }
        else if (event->InternetConnectivityChange.IPv4 == kConnectivity_Lost)
        {
            ChipLogProgress(DeviceLayer, "Lost IPv4 connectivity...");
        }
        if (event->InternetConnectivityChange.IPv6 == kConnectivity_Established)
        {
            ChipLogProgress(DeviceLayer, "IPv6 Server ready...");
            chip::app::DnssdServer::Instance().StartServer();

            ChipLogProgress(DeviceLayer, "Initializing route hook...");
            ameba_route_hook_init();
#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
            // Init OTA requestor only when we have gotten IPv6 address
            if (!isOTAInitialized)
            {
                matter_ota_init();
                isOTAInitialized = true;
            }
#endif
        }
        else if (event->InternetConnectivityChange.IPv6 == kConnectivity_Lost)
        {
            ChipLogProgress(DeviceLayer, "Lost IPv6 connectivity...");
        }
        break;

    case DeviceEventType::kInterfaceIpAddressChanged:
        if ((event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV4_Assigned) ||
            (event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV6_Assigned))
        {
            // MDNS server restart on any ip assignment: if link local ipv6 is configured, that
            // will not trigger a 'internet connectivity change' as there is no internet
            // connectivity. MDNS still wants to refresh its listening interfaces to include the
            // newly selected address.
            chip::app::DnssdServer::Instance().StartServer();
        }
        break;
    }
}

void matter_core_init_server(intptr_t context)
{
    xTaskHandle task_to_notify = reinterpret_cast<xTaskHandle>(context);
    // Init ZCL Data Model and CHIP App Server
    static chip::CommonCaseDeviceServerInitParams initParams;
    initParams.InitializeStaticResourcesBeforeServerInit();
    chip::Server::GetInstance().Init(initParams);
    gExampleDeviceInfoProvider.SetStorageDelegate(&Server::GetInstance().GetPersistentStorage());
    chip::DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

printf("%s, %d\r\n", __FUNCTION__, __LINE__);
    if (matter_core_enable_all_endpoints() != 0)
    {
printf("%s, %d\r\n", __FUNCTION__, __LINE__);
        ChipLogError(DeviceLayer, "Enable all endpoints failure");
printf("%s, %d\r\n", __FUNCTION__, __LINE__);
    }

    sWiFiNetworkCommissioningInstance.Init();

    // We only have network commissioning on endpoint 0.
    // TODO: configure the endpoint
    emberAfEndpointEnableDisable(0xFFFE, false);

    if (RTW_SUCCESS != wifi_is_connected_to_ap())
    {
        // QR code will be used with CHIP Tool
        PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
    }
    xTaskNotifyGive(task_to_notify);
}

CHIP_ERROR matter_core_init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    err = Platform::MemoryInit();
    SuccessOrExit(err);

    // Initialize the CHIP stack.
    err = PlatformMgr().InitChipStack();
    SuccessOrExit(err);

    // TODO: update this when upstream PR merged
    SetCommissionableDataProvider(&mFactoryDataProvider);
    SetDeviceAttestationCredentialsProvider(&mFactoryDataProvider);

    if (CONFIG_NETWORK_LAYER_BLE)
    {
        ConnectivityMgr().SetBLEAdvertisingEnabled(true);
    }

    // Start a task to run the CHIP Device event loop.
    err = PlatformMgr().StartEventLoopTask();
    SuccessOrExit(err);

    // Register a function to receive events from the CHIP device layer.  Note that calls to
    // this function will happen on the CHIP event loop thread, not the app_main thread.
    PlatformMgr().AddEventHandler(matter_core_device_callback_internal, reinterpret_cast<intptr_t>(NULL));

    // PlatformMgr().ScheduleWork(matter_core_init_server, 0);
    PlatformMgr().ScheduleWork(matter_core_init_server, reinterpret_cast<intptr_t>(xTaskGetCurrentTaskHandle()));
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

exit:
    return err;
}

CHIP_ERROR matter_core_start()
{
    return matter_core_init();
    // matter_core_init_server();
}

uint16_t matter_core_get_next_endpoint_index()
{
    uint16_t endpoint_id = 0;
    for (uint16_t index=0; index<MAX_ENDPOINT_COUNT; index++)
    {
        endpoint_id = emberAfEndpointFromIndex(index);
        if (endpoint_id == kInvalidEndpointId)
            return index;
    }
    return 0xFFFF;
}

int32_t matter_core_enable_all_endpoints()
{
    if (!node)
    {
        ChipLogError(DeviceLayer, "Node not initialized!");
        // return 0, bypass error for applications who use zap datamodel
        return 0;
    }

    Endpoint *current_endpoint = node->endpoint_list;
    while(current_endpoint)
    {
        if (current_endpoint->enable() != 0)
        {
            ChipLogError(DeviceLayer, "Failed to enable endpoint %d", current_endpoint->endpoint_id);
            return -1;
        }
        current_endpoint = current_endpoint->get_next();
    }
}

namespace lock {

#define DEFAULT_TICKS (500 / portTICK_PERIOD_MS) /* 500 ms in ticks */
status_t chip_stack_lock(uint32_t ticks_to_wait)
{
#if CHIP_STACK_LOCK_TRACKING_ENABLED
    if (PlatformMgr().IsChipStackLockedByCurrentThread()) {
        return LOCK_ALREADY_TAKEN;
    }
#endif
    if (ticks_to_wait == portMAX_DELAY) {
        /* Special handling for max delay */
        PlatformMgr().LockChipStack();
        return LOCK_SUCCESS;
    }
    uint32_t ticks_remaining = ticks_to_wait;
    uint32_t ticks = DEFAULT_TICKS;
    while (ticks_remaining > 0) {
        if (PlatformMgr().TryLockChipStack()) {
            return LOCK_SUCCESS;
        }
        ticks = ticks_remaining < DEFAULT_TICKS ? ticks_remaining : DEFAULT_TICKS;
        ticks_remaining -= ticks;
        ChipLogDetail(DeviceLayer, "Did not get lock yet. Retrying...");
        vTaskDelay(ticks);
    }
    ChipLogError(DeviceLayer, "Could not get lock");
    return LOCK_FAILED;
}

int32_t chip_stack_unlock()
{
    PlatformMgr().UnlockChipStack();
    return -1;
}

} // namespace lock
