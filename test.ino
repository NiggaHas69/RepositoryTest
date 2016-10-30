#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

int CS_PIN = 10;
File file;
int logz[10];
int tail = 0;
int getFingerprintIDez();

// pin #2 is IN from sensor (RED wire)
// pin #3 is OUT from arduino  (YELLOW wire)

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  
  Serial.begin(9600);

  initializeSD();

  Serial.println("R305 finger detection test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
  //  Serial.println("Found fingerprint sensor!");
  } else {
  //  Serial.println("Did not find fingerprint sensor");
    while (1);
  }
 // Serial.println("Place finger on the sensor");
}
//End setup

void loop()                     
{
int a = getFingerprintIDez();
//Serial.print("Returned ID is \t");
//Serial.println(a);
logz[0] = -1;


int flag = 0;                                   //************************************
for(int i=0; i<20; i++)
  { if (a == logz[i] && a!= -1)
    {flag = 1;}
   }
   if (flag==1)
   { //Serial.println("ID LOGGED ALREADY");
   }
if (flag==0)
{
  if (a!=-1)
    {
  //  Serial.print("0 1 2 are ");
   // Serial.println(logz[0]);
   // Serial.println(logz[1]);
    //Serial.println(logz[2]);
    tail++;
        logz [tail] = a;
        file = SD.open("test2.txt");
      //  Serial.println("File opened");    //************************************
        file.println(a);
       // Serial.println("Writing to file: ");
       // Serial.println(a);
        file.close();
       // Serial.println("File closed");
        
      createFile("test2.txt");
      writeToFile("a");
      closeFile();

     // Serial.println("Logged attendance"); 
    }
  }



 

  delay(1000);            //don't need to run this at full speed.
}

void initializeSD()
{
 // Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
    //Serial.println("SD is ready");
  } else
  {
   // Serial.println("SD initialization failed");
    return;
  }
}

int createFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
   // Serial.println("File created successfully.");
    return 1;
  } else
  {
   // Serial.println("Error while creating file.");
    return 0;
  }
}

int writeToFile(char text[])
{
  if (file)
  {
    file.println(text);
   // Serial.println("Writing to file: ");
   // Serial.println(text);
    return 1;
  } else
  {
   // Serial.println("Couldn't write to file");
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
  //  Serial.println("File closed");
  }
}

int openFile(char filename[])
{
  file = SD.open(filename);
  if (file)
  {
   // Serial.println("File opened");
    return 1;
  } else
  {
   // Serial.println("Error opening file...");
    return 0;
  }
}

String readLine()
{
  String received = "";
  char ch;
  while (file.available())
  {
    ch = file.read();
    if (ch == '\n')
    {
      return String(received);
    }
    else
    {
      received += ch;
    }
  }
  return "";
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
 // Serial.println("p id is");
//  Serial.println(p);

  switch (p) {
    case FINGERPRINT_OK:
    //  Serial.println("Captured image of finger");
      break;
    case FINGERPRINT_NOFINGER:
    //  Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
   //   Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
    //  Serial.println("Imaging error");
      return p;
    default:
   //   Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
 // Serial.println(p);
 // Serial.println("p is template id");
  switch (p) {
    case FINGERPRINT_OK:
    //  Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
   //  Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
   //   Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
   //   Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
    //  Serial.println("Could not find fingerprint features");
      return p;
    default:
  //    Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
   // Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  //  Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
   // Serial.println("Did not find a match");
    return p;
  } else {
   // Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {Serial.println("Invalid Finger!"); return -1;}
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  // Serial.println(finger.fingerID);
  return finger.fingerID; 
}
