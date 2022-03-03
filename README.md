# SuplaRest Library for Arduino IDE

SuplaRest is a library for [Arduino IDE](https://www.arduino.cc/en/software) that makes the communication using [Supla REST Api](https://app.swaggerhub.com/apis/supla/supla-cloud-api/) easier
The library is on the very beginning phase of development. The basic structure is almoast given, but the implementation of many actions is not done yet.

## Actions
The list of already implemented actions:

### Opening roller shutter
Use _SuplaRest.openRollerShutter()_ to open your roller shutter.

### Closing roller shutter
Use _SuplaRest.closeRollerShutter()_ to close your roller shutter.

## Requirements
As the library is on very beginning phase it is currently tested only with **ES32**. It may happen, that it will not work properly with other hardware.

### Importing to Arduino IDE
If you download the repository as a zip file, you can import it as a library. For that please go *Sketch > Include Library > Add .ZIP Library*. Find the download zip file and that's it! Detailed instruction you can find on [Arduino IDE - Installing Libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries).

## Examples
Take a look on [examples](examples/) directory to check how to use the library.
