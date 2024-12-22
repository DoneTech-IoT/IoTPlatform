#ifndef MATTER_EVENT_INFO_H
#define MATTER_EVENT_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Public Event Types
 *
 * Enumerates event types that are visible to the application and common across
 * all platforms.
 */
typedef enum
{
    /**
     * WiFi Connectivity Change
     *
     * Signals a change in connectivity of the device's WiFi station interface.
     */
    kWiFiConnectivityChange,

    /**
     * Thread Connectivity Change
     *
     * Signals a change in connectivity of the device's Thread interface.
     */
    kThreadConnectivityChange,

    /**
     * Internet Connectivity Change
     *
     * Signals a change in the device's ability to communicate via the Internet.
     */
    kInternetConnectivityChange,

    /**
     * Service Connectivity Change
     *
     * Signals a change in the device's ability to communicate with a chip-enabled service.
     */
    kServiceConnectivityChange,

    /**
     * Service Provisioning Change
     *
     * Signals a change to the device's service provisioning state.
     */
    kServiceProvisioningChange,

    /**
     * Time Sync Change
     *
     * Signals a change to the device's real time clock synchronization state.
     */
    kTimeSyncChange,

    /**
     * CHIPoBLE Connection Established
     *
     * Signals that an external entity has established a new CHIPoBLE connection with the device.
     */
    kCHIPoBLEConnectionEstablished,

    /**
     * CHIPoBLE Connection Closed
     *
     * Signals that an external entity has closed existing CHIPoBLE connection with the device.
     */
    kCHIPoBLEConnectionClosed,

    /**
     * Request BLE connections to be closed.
     * This is used in the supportsConcurrentConnection = False case.
     */
    kCloseAllBleConnections,

    /**
     * When supportsConcurrentConnection = False, the ConnectNetwork command cannot start until
     * the BLE device is closed and the Operation Network device (e.g. WiFi) has been started.
     */
    kWiFiDeviceAvailable,
    kOperationalNetworkStarted,

    /**
     * Thread State Change
     *
     * Signals that a state change has occurred in the Thread stack.
     */
    kThreadStateChange,

    /**
     * Thread Interface State Change
     *
     * Signals that the state of the Thread network interface has changed.
     */
    kThreadInterfaceStateChange,

    /**
     * chip-over-BLE (CHIPoBLE) Advertising Change
     *
     * Signals that the state of CHIPoBLE advertising has changed.
     */
    kCHIPoBLEAdvertisingChange,

    /**
     * IP address availability - either ipv4 or ipv6 addresses assigned to the underlying
     * wifi/ethernet interface.
     */
    kInterfaceIpAddressChanged,

    /**
     * Commissioning has completed by a call to the general commissioning cluster command.
     */
    kCommissioningComplete,

    /**
     * Signals that the fail-safe timer expired before the CommissioningComplete command was
     * successfully invoked.
     */
    kFailSafeTimerExpired,

    /**
     *
     */
    kOperationalNetworkEnabled,

    /**
     * Signals that DNS-SD has been initialized and is ready to operate.
     */
    kDnssdInitialized,

    /**
     * Signals that DNS-SD backend was restarted and services must be published again.
     */
    kDnssdRestartNeeded,

    /**
     * Signals that bindings were updated.
     */
    kBindingsChangedViaCluster,

    /**
     * Signals that the state of the OTA engine changed.
     */
    kOtaStateChanged,

    /**
     * Server initialization has completed.
     *
     * Signals that all server components have been initialized and the node is ready to establish
     * connections with other nodes. This event can be used to trigger on-boot actions that require
     * sending messages to other nodes.
     */
    kServerReady,

    /**
     * Signals that BLE is deinitialized.
     */
    kBLEDeinitialized,
    
    //
    //this signals belong to platform specific events (esp32)
    //
    /** Signals that Commissioning session has started */
    kCommissioningSessionStarted,
    /** Signals that Commissioning session has stopped */
    kCommissioningSessionStopped,
    /** Signals that Commissioning window is now opend */
    kCommissioningWindowOpened,
    /** Signals that Commissioning window is now closed */
    kCommissioningWindowClosed,
    /** Signals that a fabric is about to be deleted. This allows actions to be taken that need the
    fabric to still be around before we delete it */
    kFabricWillBeRemoved,
    /** Signals that a fabric is deleted */
    kFabricRemoved,
    /** Signals that a fabric in Fabric Table is persisted to storage, by CommitPendingFabricData */
    kFabricCommitted,
    /** Signals that operational credentials are changed, which may not be persistent.
    Can be used to affect what is needed for UpdateNOC prior to commit */
    kFabricUpdated,
} PublicEventTypes;

typedef struct 
{    
    PublicEventTypes PublicEventTypes;    
} MatterEventPacket;

#ifdef __cplusplus
}
#endif

#endif //MATTER_EVENT_INFO_H