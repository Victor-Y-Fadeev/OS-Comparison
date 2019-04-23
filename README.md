![logo](https://drive.google.com/uc?export=view&id=1SHy_P1BS9dv5h0lkyDwRzsFg1-hIbg2Z)
=====================================================================================

## Installing environment
To download `esp32` toolchain for [FreeRTOS](https://www.freertos.org/), use the script:
```
$ ./scripts/freeRTOS.sh
```

At the next step install `mos` the [Mongoose OS](https://mongoose-os.com/) tool:
```
$ ./scripts/mongoose.sh
```

Now you can configure `minicom` by yourself as:
 - Serial device `/dev/ttyUSB0`
 - Hardware flow control: `no`
 - Software flow control: `no`
 - Save your configuration as `esp32`

Or use the next line to do this automatically:
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
You can change settings by:
```
$ make menuconfig
```
But then you get incorrect research results.

To build your test use:
```
$ make
```
Or, if you want to flash it at the controller:
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
