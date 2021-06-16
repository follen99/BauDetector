# Bau Detector
This is simple Arduino projects that detects dogs barks and plays a vocal message to calm them down.
## Features

 - The code allows to play a vocal message picked by a `random()`
   function between 11 files.
   
 - Every time a `Bark` is detected, the system writes to a `log.txt` file with the following format:

> `day-month-year | hours-minutes-seconds | intensity | hours | day`
## Analyze data
To analyze the data just plug the MicroSD into your computer and make a copy of the file `log.txt`.
Open `Excel` or `Google Sheets` and import the data (`file>import`) and separate the columns with the `,` (comma).
At this point you can make everything you want with the data.

## Parts list

 - **Arduino uno** (or every arduino you want, i would use an Arduino Nano) [Ebay](https://www.ebay.it/itm/331535576930?hash=item4d310f6f62:g:W8oAAOSwnbZYDgl1)
 - **Real Time Clock Module** `DS1302` [Ebay](https://www.ebay.it/itm/173861088473?hash=item287aedacd9:g:PgcAAOSw5CZcpGjj) 
 - **SD Card Reader Module** [Ebay](https://www.ebay.it/itm/111773200314?ssPageName=STRK:MEBIDX:IT&_trksid=p2060353.m2749.l2648)
 - **Sound Sensor Module** [Ebay](https://www.ebay.it/itm/111662632326?ssPageName=STRK:MEBIDX:IT&_trksid=p2060353.m2749.l2648)
