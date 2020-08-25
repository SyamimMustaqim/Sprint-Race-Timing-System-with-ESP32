#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

//WIFI Credential
const char* ssid = ""; //wifi id
const char* password = ""; //wifi password 

String GOOGLE_SCRIPT_ID = "-"; // Replace by your GAS service id

//updated 04.12.2019
const char * root_ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n" \
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n" \
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n" \
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n" \
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n" \
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n" \
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n" \
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n" \
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n" \
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n" \
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n" \
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n" \
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n" \
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n" \
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n" \
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n" \
"-----END CERTIFICATE-----\n";

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

//set pin 
const int trigPin = 25;
const int echoPin = 26;
const int buzzerPin = 18;
const int savePin = 5;
const int navPin = 23;
const int enterPin = 19;
const int ledPin = 27;

//set variable
unsigned long runTimeStart;
unsigned long runTimeMs;
double times = 0;
float duration;
float distance;
int counter = 0;
const int a = 0;
int countdown = 0;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


WiFiClientSecure client;


void setup(){
	Serial.begin(115200);
	
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(buzzerPin, OUTPUT);
	pinMode(savePin, INPUT_PULLUP);
	pinMode(navPin, INPUT_PULLUP);
	pinMode(enterPin, INPUT_PULLUP);
	pinMode(ledPin, OUTPUT);

	// Set device as a Wi-Fi Station
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	
	Serial.println("Started");
	Serial.print("Connecting");
	  while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	  }
	  
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	

	lcd.init();			// initialize LCD                    
	lcd.backlight();	// turn on LCD backlight 
	lcd.print("Press to start");
	
}



void loop(){
	
	//loop for timer selection
	if(digitalRead(navPin)==LOW){
		
		digitalWrite(ledPin, LOW);
		if(counter == 0){
			lcd.clear();
			countdown = 3;
			lcd.print("Select timer: 3");
			counter++;
		}
		
		else if(counter == 1){
			lcd.clear();
			countdown = 10;
			lcd.print("Select timer: 10");
			counter++;
		}
		
		else if(counter == 2){
			lcd.clear();
			countdown = 30;
			lcd.print("Select timer: 30");
			counter = 0;			
		}
		
	}
	
	if(digitalRead(enterPin) == LOW){ //select timer button
    digitalWrite(ledPin, LOW);
		MeasureRunTime();
	}
	
	if(digitalRead(savePin) == LOW){ //save data button 
    digitalWrite(ledPin, LOW);
		sendData("tag=" + fakeFunc1() +"&value="+String(times));
	}
	delay(100);
}

String fakeFunc1() //function data tag string 
{
  return "somedata";
}


void MeasureRunTime(){ 
	
	for(int i = countdown; i>0; i--){ //start countdown
		lcd.clear();
		lcd.print(i);
		if(i<=3){
			digitalWrite(buzzerPin, HIGH);
			delay(500);
			digitalWrite(buzzerPin, LOW);			
		}
		delay(1000);
	}
	lcd.clear();
	lcd.print("Go!");
	digitalWrite(buzzerPin, HIGH);
	runTimeStart = millis(); //initialize timer 
	delay(1000);
	digitalWrite(buzzerPin, LOW);	
	runTimeStart = runTimeStart + 1000;
	lcd.clear();
	
	
	while(measureDistance() > 150){
		runTimeMs = millis() - runTimeStart;
		times = (double)runTimeMs/1000;
		lcd.setCursor(0, 0);
		lcd.print(times);
		delay(10);
	}
	
	lcd.clear();
	lcd.print(times);
	digitalWrite(ledPin, HIGH);
}

float measureDistance(){ //measure ultra sonic sensor 
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);

	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);

	duration = pulseIn(echoPin,HIGH);
	distance = duration*0.0343/2;

	Serial.print("Distance = ");
	Serial.println(distance);
	return distance;
}

void sendData(String params) { //function sending data to google excel sheet
	lcd.clear();
	lcd.print("Sending Data..");
    HTTPClient http;
    String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
    Serial.print(url);
    Serial.println("Making a request");
    http.begin(url, root_ca); //Specify the URL and certificate
    int httpCode = http.GET();  
    http.end();
	lcd.clear();
    lcd.print("Data Sent!");
}
