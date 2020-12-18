# Subsystems Communication Layer(SCL)

Subsystems Communication Layer(SCL) is a library built for using wifi application on CM4 core while CM0 core runs Subsystems firmware. SCL is portable to Mbed-OS and FreeRTOS. SCL is dependent on CY-RTOS Abstraction layer to create, start and end thread. SCL runs on a single thread, data being sent from CM4 to CM0 runs on the context of calling thread and the data received from CM0 to CM4 is sent to the application in the context of SCL Thread.
The [release notes](./RELEASE.md) detail the current release.

## Supported Platforms
This library and its features are supported on the following Cypress platforms:
* CYSBSYSKIT-01 Rapid IoT Connect Platform RP01 Feather Kit


## Dependent Libraries
This library depends on the following:
* Abstraction-rtos (https://github.com/cypresssemiconductorco/abstraction-rtos/#latest-v1.X)
* clib-support (https://github.com/cypresssemiconductorco/clib-support/#latest-v1.X)
* freertos (https://github.com/cypresssemiconductorco/freertos/#latest-v10.X)
* lwip (https://git.savannah.nongnu.org/git/lwip/#STABLE-2_1_2_RELEASE)
* retarget-io (https://github.com/cypresssemiconductorco/retarget-io/#latest-v1.X)
* wifi-mw-core (https://github.com/cypresssemiconductorco/wifi-mw-core/#latest-v2.X)