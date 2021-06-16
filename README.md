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

