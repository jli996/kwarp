# KWarp

KWarp emulates a HID device (keyboard) and sends the text it receives via Bluetooth as keystrokes to the connected computer. It helps sending passwords managed by a password manager from mobile to a computer that doesn't have direct access to the manager.

The hardware used by this project is T-Display-S3. It's an esp32s3 development board with a display. Check out their [repo](https://github.com/Xinyuan-LilyGO/T-Display-S3) for more information.

## Quick Start with PlatformIO

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `kwarp` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click the (✔) symbol in the lower left corner to compile
7. Connect the board to the computer USB
8. Click (→) to upload firmware

## FAQ

- PlatformIO no longer finds my development board after I run the project.
  - KWarp uses the USB port to emulate a keyboard. PlatformIO cannot write to the device when it's running. Press Boot and then RST and release them to put the device into development mode and PlatformIO should see it again. Press RST again after writting the program to the board.
