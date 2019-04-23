![logo](https://drive.google.com/uc?export=view&id=1SHy_P1BS9dv5h0lkyDwRzsFg1-hIbg2Z)
# OS Comparison
This repository contain the list of tests for comparing real time operating systems.

At the research comparing [FreeRTOS](https://www.freertos.org/) and [Mongoose OS](https://mongoose-os.com/) by this parameters:
 - Processes speed
 - Interrupts
 - Semaphores
 - Messages
 - Firmware size

As a hardware using ESP32 microprocessor. Testing board was IoTik 32A v2.0 of [MGBot](https://mgbot.ru/) company.

## Installing environment
To download `esp32` [toolchain](https://demo-dijiudu.readthedocs.io/en/latest/) for FreeRTOS, use the script:
```
$ ./scripts/freeRTOS.sh
```
At the next step install `mos` the Mongoose OS [tool](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md):
```
$ ./scripts/mongoose.sh
```
Now you can automatically configure `minicom`, use the line:
```
$ sudo echo -e 'pu port /dev/ttyUSB0\npu rtscts No' > ~/.minirc.esp32
```
After this you should restart your user session to complete installation. You can use this command:
```
$ pkill -KILL -u $USER
```

## Compiling & Start testing
If you want to check research results follow the next instructions to build & flash tests.

### FreeRTOS:
Enter at the `./freeRTOS` folder first time and set sdkconfig as a defaults by:
```
$ cd freeRTOS
$ make defconfig
```
To build & flash your test at the controller use:
```
$ make flash
```

### Mongoose OS:
Here you should first build your firmware form `./mongoose` folder:
```
$ cd mongoose
$ mos build --platform esp32
```
And then flash it at the board:
```
$ mos flash
```
