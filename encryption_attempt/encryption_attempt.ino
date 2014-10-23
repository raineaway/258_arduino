#include <MD5.h>
char *question = "";
char *key = "12345";
String devices[3] = {"Raine", "", ""};
unsigned long time;

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  String string = "";
  if(Serial.available())
  {
    delay(100);
    while (Serial.available()) {
      string = string + char(Serial.read());
    }
  }
  
  if (string == "request") {
    time = millis();
    Serial.println(time); //question
  } else {
    if (string != "") {
      if (string.substring(0,10) == "add_device") {
        boolean check = match(string.substring(11,43));
        Serial.println("add device");
        delay(100);
    
        if (check) {
          for (int i=0; i<5; i++)  {
            if (devices[i] == "") {
              String device = string.substring(44);
              //device.toCharArray(devices[i], sizeof(device));
              devices[i] = device;
              Serial.println("ok");
              //return;
            }
          }
          //Serial.println("Devices full");
        } else {
          Serial.println("Unauthorized!"); 
        }
      } else {
        boolean check = match(string);
        if (check) {
          Serial.println("Match!");
          //rotate
        } else {
          Serial.println("Invalid!");
        }
      }
    }
  }
}

boolean match(String check) {
  String temp = String(time);
  temp.toCharArray(question, sizeof(temp));
  for (int i=0; i<3; i++) {
    char *raw = strcat(key,question);
    String final = raw + devices[i];
    int length = final.length()+1;
    char *make = (char *)malloc(length);
    final.toCharArray(make, length);
    unsigned char* hash = MD5::make_hash(make);
    String md5str = MD5::make_digest(hash, 16);
    
    if (md5str.equals(check)) {
      return true;
    }
  }
  return false;
}
