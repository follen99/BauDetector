#include <SPI.h>
#include <SD.h>
#include "TMRpcm.h"
#include "string.h"

//SD READER VARIABLES
File main_file;
//String MAIN_LOG_FILE = "log.txt";

//MICROPHONE SENSOR VARIABLES
#define sensorPin A0
unsigned long lastEvent = 0;


const int SAMPLE_TIME = 10;
unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;

int sampleBufferValue = 0;    //amount of ones in the SAMPLE_TIME

//REAL TIME CLOCK VARIABLES
#include <virtuabotixRTC.h> 
// Creation of the Real Time Clock Object
virtuabotixRTC myRTC(6, 7, 8);
int YEAR;
int MONTH;
int DAY;


//SPEAKER VARIABLES
#define SD_ChipSelectPin 10
TMRpcm tmrpcm;

char* audio_names[] = {"audio1.wav","audio2.wav","audio3.wav","audio4.wav","audio5.wav","audio6.wav","audio7.wav","audio8.wav","audio9.wav","audio10.wav","audio11.wav"};

void setup() {
  

  //######################## SD CARD READER ########################
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  Serial.println("initialization done.");

  //######################## RESET DATE AND TIME ########################

  ArduinoDateToDisplayDate(__DATE__);

  myRTC.setDS1302Time(getSeconds(), getMinutes(), getHours(), NULL, DAY, MONTH, YEAR);
  //######################## MICROPHONE ########################

  pinMode(sensorPin, INPUT);  //microphone sensor

  //######################## SPEAKER ########################
  tmrpcm.speakerPin = 9;
  tmrpcm.setVolume(5);
  tmrpcm.quality(3);


  // ######################## SD STUFF 
  
  //SD.remove("log.txt");
  
  /*
  Serial.println("Checking log.txt");
  if(!SD.exists("log.txt")){
    Serial.println("Creating the main log file...");
    main_file = SD.open("log.txt",FILE_WRITE);
    if(main_file){
      main_file.print("day-month-year,hours-minutes-seconds,intensity,hours,day");
      main_file.print("\n");
    }else {
      Serial.println("Error opening the log file!");
    }
    main_file.close();
    Serial.println("File created!");
  }else{
    Serial.println("Main log file already exists.");
  }
*/
  readLog("log.txt");
}




void loop() {
  checkSoundSensor();
  //readDecibels();
  //sampleAudio();
}



/**this function is the main function of the loop(), it detects the dog's bark and it plays the aduio */
void checkSoundSensor(){
  // Read Sound sensor
	int sensorData = digitalRead(sensorPin);

	// If pin goes LOW, sound is detected
	if (sensorData == LOW) {
		
		// If 25ms have passed since last LOW state, it means that
		// the clap is detected and not due to any spurious sounds
    unsigned long intensity = millis() - lastEvent;
		if (intensity > 2500) {
			Serial.println("Clap detected!");
      Serial.print("Intensity: ");
      Serial.print(intensity);
      writeLog(intensity);

      

      yellToDogs();
      //play the audio
		}
		
		// Remember when last event happened
		lastEvent = millis();
    delay(200);
	}
}

void yellToDogs(){
  //audio_names
  int index = random(0,11);


  Serial.println(audio_names[index]);
  char* fileName = audio_names[index];
  tmrpcm.play(fileName);
  delay(5000);
  tmrpcm.stopPlayback();

  tmrpcm.disable();
}














void readLog(String file){
  main_file = SD.open(file);
  if (main_file) {
    Serial.println(file+": ");

    // read from the file until there's nothing else in it:
    while (main_file.available()) {
      Serial.write(main_file.read());
    }
    // close the file:
    main_file.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening the file ");
  }
}
void writeLog(unsigned long intensity){
  myRTC.updateTime();
  
  main_file = SD.open("log.txt",FILE_WRITE);
  if(main_file){
    //day-month-year,hours-minutes-seconds,intensity,hours,day
    main_file.print(myRTC.dayofmonth);
    main_file.print("-");
    main_file.print(myRTC.month);
    main_file.print("-");
    main_file.print(myRTC.year);
    main_file.print(",");

    main_file.print(myRTC.hours);
    main_file.print("-");
    main_file.print(myRTC.minutes);
    main_file.print("-");
    main_file.print(myRTC.seconds);
    main_file.print(",");

    main_file.print(intensity);
    main_file.print(",");
    
    main_file.print(myRTC.hours);
    main_file.print(",");
    main_file.print(myRTC.dayofmonth);


    main_file.print("\n");
  }else {
    Serial.println("Error opening the log file!");
  }
  main_file.close();
  delay(300);
}


void readDecibels(){
  int sensorData = analogRead(sensorPin);
  //int dB = (sensorData+83.2073) / 11.003; //Convert ADC value to dB using Regression values
  Serial.println(sensorData);
  //delay(300);
}

/**
this function can be used with the arduino serial plotter to visualize the audio*/
void sampleAudio(){
  millisCurrent = millis();
  millisElapsed = millisCurrent-millisLast;

  if(digitalRead(sensorPin)==LOW){
    sampleBufferValue++;
  }

  if (millisElapsed > SAMPLE_TIME) {
    Serial.println(sampleBufferValue);
    sampleBufferValue = 0;  //reset the count
    millisLast = millisCurrent;
  }
}



void ArduinoDateToDisplayDate(char const *time) { 
    char s_month[5];
    int month, day, year;
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time, "%s %d %d", s_month, &day, &year);

    month = (strstr(month_names, s_month)-month_names)/3;

    String monthText = month < 10 ? "0" + String(month) : String(month);
    String dayText = day < 10 ? "0" + String(day) : String(day);
    int date[3];
    YEAR = year;
    MONTH = monthText.toInt();
    DAY = dayText.toInt();

}

int getHours(){
  //10:58:49
  String hour = getValue(__TIME__, ':', 0);
  return hour.toInt();
}
int getMinutes(){
  String minutes = getValue(__TIME__, ':', 1);
  return minutes.toInt();
}
int getSeconds(){
  String seconds = getValue(__TIME__, ':', 2);
  return seconds.toInt();
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
