# Getting started with Sequana RGB LED example on mbed OS

This guide reviews the steps required to get exmple program working on Sequana platform.

Please install [mbed CLI](https://github.com/ARMmbed/mbed-cli#installing-mbed-cli).

## Import the example application

From the command-line, import the example:

```
mbed import https://github.com/lrusinowicz/sequana-rgb-led-example
cd sequana-rgb-led-example
```

### Now compile

Invoke `mbed compile`, and specify the name of your platform and your favorite toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for GCC:

```
mbed target FUTURE_SEQUANA
mbed toolchain GCC_ARM
mbed compile
```

Your PC may take a few minutes to compile your code. At the end, you see the following result:

```
[snip]
| Module                     |     .text |    .data |     .bss |
|----------------------------|-----------|----------|----------|
| [fill]                     |   176(+0) |   13(+0) |   33(+0) |
| [lib]\c.a                  | 22147(+0) | 2472(+0) |   89(+0) |
| [lib]\gcc.a                |  3112(+0) |    0(+0) |    0(+0) |
| [lib]\misc                 |   208(+0) |   12(+0) |   28(+0) |
| [misc]                     |    48(+0) |   56(+0) |    0(+0) |
| bd2808-driver\BD2808.o     |   270(+0) |   46(+0) |    4(+0) |
| bd2808-driver\bd2808_hal.o |  1435(+0) | 1444(+0) |   24(+0) |
| main.o                     |   602(+0) |   24(+0) |  202(+0) |
| mbed-os\cmsis              |  1033(+0) |    0(+0) |   84(+0) |
| mbed-os\drivers            |  1245(+0) |    0(+0) |    0(+0) |
| mbed-os\hal                |  2094(+0) |    4(+0) |   68(+0) |
| mbed-os\platform           |  5005(+0) |  260(+0) |  242(+0) |
| mbed-os\rtos               |  8667(+0) |  168(+0) | 5969(+0) |
| mbed-os\targets            | 11169(+0) |  953(+0) | 1125(+0) |
| Subtotals                  | 57211(+0) | 5452(+0) | 7868(+0) |
Total Static RAM memory (data + bss): 13320(+0) bytes
Total Flash memory (text + data): 62663(+0) bytes

Image: .\BUILD\FUTURE_SEQUANA\GCC_ARM\sequana-rgb-led-example.hex
```

### Program your board

1. Connect your Sequana board to the computer over USB.
2. Copy the hex file to the mbed device (DAPLINK removable storage device).
3. Your program will start automaticaly after programming has fihished.

The LEDs on your platform will blink in a rotating pattern.

## Troubleshooting

1. Make sure `mbed-cli` is working correctly and its version is `>=1.8.2`

 ```
 mbed --version
 ```

 If not, you can update it:

 ```
 pip install mbed-cli --upgrade
 ```
