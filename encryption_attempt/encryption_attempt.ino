#include <MD5.h>
#include <Servo.h>
Servo servoMain;
String key = "12345";
String main_device = "Raine";
String devices[3] = {"", "", ""};
unsigned long time;
String str;

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  str = "";
  if(Serial.available())
  {
    delay(100);
    while (Serial.available()) {
      str = str + char(Serial.read());
    }
  }

  if (str == "request") {
    time = millis();
    Serial.println(time); //question
  } 
  else {
    if (str != "") {
      if (str.length() > 44 && str.substring(0,10) == "add_device") {
        String result = addRemove("add");
        Serial.println(result);
        time = 0;
      }
      else if (str.length() > 46 && str.substring(0,13) == "remove_device") {
        String result = addRemove("remove");
        Serial.println(result);
        time = 0;
      }
      else {
        boolean check = match(main_device, str);
        if (check) {
          Serial.println("Match!");
          time = 0;
          servoMain.attach(9);
          servoMain.write(0);
          servoMain.write(90);
          delay(5000);
          servoMain.write(0);
        } 
        else {
          for (int i=0; i<3; i++) {
            check = match(devices[i], str);
            if (check) {
              Serial.println("Match!");
              time = 0;
              //rotate
              break;
            }
          }
          if (!check) {
            Serial.println("Invalid!");
          }
        }
        time = 0;
      }
    }
  }
}

boolean match(String device, String check) {
  String question = String(time);
  String final = key + question + device;
  Serial.println(final);
  char *make = makeChar(final);
  unsigned char* hash = MD5::make_hash(make);
  char *md5str = MD5::make_digest(hash, 16);
  free(make);

  String md = String(md5str);
  Serial.println(md);
  if (md.equals(check)) {
    return true;
  }
  return false;
}

char *makeChar(String a) {
  int length = a.length()+1;
  char *result = (char *)malloc(length);
  a.toCharArray(result, length);
  return result;
}

String addRemove(String type) {
  boolean check = false;
  if (type == "add") {
    check = match(main_device, str.substring(11,43));
  } 
  else {
    check = match(main_device, str.substring(14,46));
  }
  if (!check) {
    return "Unauthorized.";
  } else {
    for (int i=0; i<3; i++)  {
      if (type == "add") {
        if (devices[i].length() == 0) {
          String device = str.substring(44); 
          devices[i] = device;
          return "Device added.";
        }
      } 
      else {
        if (devices[i] == str.substring(47)) {
          devices[i] = "";
          return "Device removed.";
        }
      }
    }
    if (type == "add") {
      return "Devices full.";
    } else {
      return "Device not found.";
    }
  }
}

