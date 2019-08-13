# Can-Logger
# Teamprojekt Ostfalia 2019

* [Einleitung](#einleitung)
* [Pinbelegung](#pinbelegung)
    * [Display-Pins](#displayPins)
    * [SD-Karten-Pins](#sdKartenPins)
    * [Encoder-Pins](#encoderPins)
    * [Taster-Pins](#tasterPins)
    * [CAN-Controller-Pins](#canControllerPins)
* [Quellen und Hilfen](#QandH)

<a name="einleitung"></a>
## Einleitung

Bei diesem Projekt handelt es sich um eine studentische Gruppenarbeit an der [Ostfalia Hochschule für angewandte Wissenschaften](https://www.ostfalia.de/cms/de/) in Wolfenbüttel. Betreut wird die Arbeit von Prof. Dr.-Ing. Karl-Dieter Tieste von der Fakultät Elektrotechnik.
Ziel des Projektes ist die Programmierung eines [Can Loggers](https://de.wikipedia.org/wiki/Datenlogger). Dieser soll eingehende [CAN-Bus-Nachrichten](https://de.wikipedia.org/wiki/Controller_Area_Network) über ein (Display) grafisch darstellen und zusätzlich die Möglichkeit bieten diese auf einer [SD Speicherkarte](https://de.wikipedia.org/wiki/SD-Karte) abzuspeichern.  
Als Hardwearegrundlage dient das [STM32 Nucleo-64 development board](https://www.st.com/en/evaluation-tools/nucleo-f303re.html#) mit der [STM32F303RE](STM32F303RE) MCU. Dieses Bord wird durch eine zusätzlich erstellte Platine ergänzt. Hier sind unter anderem ein [Drehdrücksteller](), ein [Taster](), ein [CAN-Controller]() und ein [QVAG 2.2 TFT SPI 240X230]() Farbdisplay verbaut. Das Display verfügt auf der Unterseite über einen SD-Speicherkarten-Sockel.  
Programmiert wird in der Entwicklungsumgebung [Microsoft Visual Studio Code](https://code.visualstudio.com/) mit [C++](https://de.wikipedia.org/wiki/C%2B%2B).


<a name="pinbelegung"></a>
## Pinbelegung

<a name="displayPins"></a>
### Display-Pins

| Display   | Board-Pin | Morpho    | 
| :-------- | :-------- | :-------- |
| SDOI/MISO | NC        | NC        |
| LED       | CN7\|6    | E5V       |
| SCK       | CN10\|29  | PB5       |
| SDI/MOSI  | CN10\|33  | PA10      |
| DC/RS     | CN10\|23  | PA8       |
| RESET     | CN10\|2   | PC8       |
| CS        | CN10\|1   | PC9       |
| GND       | GND       | GND       |
| VCC       | CN7\|5    | VCC       |

*Der LED-Pin des Displays ist über einen 47 Ohm Widerstand mit CN7\|6 verbunden.*

<a name="sdKartenPins"></a>
### SD-Karten-Pins

| Sockel    | Bord-Pin  | Morpho    | 
| :-------- | :-------- | :-------- |
| SD_CS     | CN10\|17  | PB6       | 
| SD_MOSI   | CN10\|15  | PA7       | 
| SD_MISO   | CN10\|13  | PA6       | 
| SD_SCK    | CN10\|11  | PA5       | 
| F_CS      | NC        | NC        | 

<a name="encoderPins"></a>
### Encoder-Pins

| Encoder   | Bord-Pin  | Morpho    | 
| :-------- | :-------- | :-------- |
| A         | CN10\|19  | PC7       | 
| B         | CN10\|4   | PC6       | 
| C         | GND       | GND       | 
| SW        | CN10\|27  | PB4       | 
| GND       | GND       | GND       | 

*SW wird beim Drücken des Drehdrückstellers auf Masse gelegt.*

<a name="tasterPins"></a>
### Taster-Pins

| Taster    | Bord-Pin     | Morpho    | 
| :-------- | :--------    | :-------- |
| 1         | VCC          | VCC       | 
| 2         | CN10\|34     | PC4       | 
| GND       | GND          | GND       | 

*1 und 2 liegen auf dem selben Potenzial. Wird der NO-Taster betätigt werden beide Pins auf Masse gelegt. Dies führt zu einem Spannungsabfall an CN10\|34 PCB.*

<a name="canControllerPins"></a>
### CAN-Controller-Pins

|CAN-Controller| Bord-Pin   | Morpho     |
| :----------- | :--------  | :--------  |
| TXD          | CN10\|5    | PB9        |
| GND          | GND        | GND        |
| VCC          | VCC        | VCC        |
| RXD          | CN10\|3    | PB8        |
| RS           | NC         | NC         |
| CANH         |*CAN-SOCKED*|*CAN-SOCKED*|
| CANL         |*CAN-SOCKED*|*CAN-SOCKED*|
| Vref         | NC         | NC         |


<a name="QandH"></a>
## Quellen und Hilfen

* [Einführung Marcdown](https://www.markdownguide.org/basic-syntax/#overview)
