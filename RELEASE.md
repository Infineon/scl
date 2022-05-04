# Subsystems Communication Layer(SCL)

## What's Included?
See the [README.md](./README.md) for a brief description on the SCL.

## Changelog
### v1.1.1
* Minor change to increase throughput

### v1.1.0
* Added the deepsleep related APIs

* Modified the IPC wait method to use semaphores

* Protected the IPC Register access with Mutex

### v1.0.2
* Updated the mbedtls configuration.

### v1.0.1
* Updated folder structure

* Added LWIP, FreeRTOS and mbedtls configurations

### v1.0.0
* Initial release for scl.

* Includes support for Wi-Fi STA Mode.

* Exposes Wi-Fi APIs to join, communicate and leave a Wi-Fi network.

* Connection monitoring: Monitor active connections and link events. Provides a mechanism to register for event notification.

### Supported Software and Tools
This version of the library was validated for compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 2.2     |
| PSoC 6 Peripheral Driver Library (PDL)                  | 1.6.1   |
| GCC Compiler                                            | 9.2.1   |
| IAR Compiler                                            | 8.32    |
