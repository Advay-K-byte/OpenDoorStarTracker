//OpenDoor Star Tracker CODE
//GITHUB: https://github.com/Advay-K-byte/OpenDoorStarTracker/tree/main
//This is a very open source and highly changeable project, you can change many things to fit whatever resources you have available.
// Good luck, and clear skies!


#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>

Preferences prefs;  //makes preferences object

// set Access Point network name and password
const char* AP_SSID = "OpenDoor Star Tracker";
const char* AP_PASSWORD = "MoreThanAstrophotography";

//starts webserver at port 80 of wifi
WebServer server(80);

const int IN1 = 33;
const int IN2 = 25;
const int IN3 = 26;
const int IN4 = 32;

const int stepSequence[4][4] = {
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1}
};

int stepIndex = 0;


const int stepsPerRevolution = 2038;


float pos = 0;
const double mmPerStep = 0.000124632;
const int stepsPermm = 8024;

bool trackingState = false;
bool resetState = false;
bool slewState = false;

int slewDir = -1;
int slewmm = 5;
float targetPos = 0;

float trackingSpeed = 5.9;    //set the speed for now in rpm
unsigned long lastStepTime = 0;    // the last time it took a step
unsigned long stepIntervalMicros = 5400;                                     //speed setting basically
unsigned long stepIntervalResetMicros = 1500; //speed setting of the resetting speedbasically
unsigned long stepIntervalSlewing = 1500; //sets slewing speed

                 //full html page that is sent over the wifi connection to the client
const char* htmlPage = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
      
  <head><title>OpenDoor Star Tracker Controller</title></head>
  <style>
      body {
          background-color:#f25c5c;
          text-align:center;
      }
      h1 {
          /*header size one css */
          text-align:center;
          font-family: Andale Mono, monospace;
      }
      h2 {
          /*header size two css */
          text-align:center;
          font-family: Andale Mono, monospace;
      }
      h3 {
          /*header size three css */
          text-align:center;
          font-family: Andale Mono, monospace;
      }
      h4 {
          /*header size four css */
          text-align:center;
          font-family: Andale Mono, monospace;
      }
      h5 {
          /*header size five css */
          text-align:center;
          font-family: Andale Mono, monospace;
      }
      button {
          /*header size button css */
          text-align:center;
          font-family: Andale Mono, monospace;
          border-radius: 20px;
      }
      
      
  </style>
  <body>
    <h1>OpenDoor Star Tracker Controller</h1>
    <hr style="width:70%;"> <!--divider horizontal rule-->
    <h2>TRACKING:</h2>                                                      <!--TRACKING ONOFF-->
    <button style="padding:25px; color:red ; background-color:black; font-weight:bold;font-size:60px;" id="tracking">OFF</button>
    
    
    <h3>Current Tracking Speed</h3>
    <div style="display: flex; gap:50px; justify-content: center;">           <!--TRACKING SPEED-->
        <button style="padding:25px;" onclick="addSpeed()">+10</button>
        <h1 id="trackingSpeed">0</h1>
        <button style="padding:25px;" onclick="minusSpeed()">-10</button>
    </div>
    
    <hr style="width:30%;">
    
    <h3>Slew Position</h3>
    <div style="display: flex; gap:30px; justify-content: center;">           <!--SLEW POSITION-->
        <button style="padding:25px;" onclick="slewOne()">+1mm</button>
        <button style="padding:25px;" onclick="slewFive()">+5mm</button>
        <h1 id="motorPos">0</h1>
        <button style="padding:25px;" onclick="slewMinusFive()">-5mm</button>
        <button style="padding:25px;" onclick="slewMinusOne()">-1mm</button>
    </div>
    
    <hr style="width:30%;">
    
    <h3>Settings controls:</h3>                                                <!--SETTINGS CONTROLS-->
    <div style="display: flex; gap:20px; justify-content: center;">
        <button style="padding:15px;" onclick="nightMode()">NIGHT MODE</button>
        <button style="padding:15px;" onclick="resetPosition()">RESET TRACKER</button>
        <button style="padding:15px;" onclick="setZeroPos()">SET POSITION 0</button>
    </div>
    </div>
    
    
    
    <script>                                                              <!-- JAVASCRIPT CODE STARTS HERE -->
      
      
      const trackbutton = document.getElementById("tracking");    //tracking button code to change color + send fetch
                                                                  //this is for elements that have to be changed, hence ID
      const posLabel = document.getElementById("motorPos");
      
      const speedLabel = document.getElementById("trackingSpeed");
      
      
      
                      //SINGLE PRESS BUTTONS FUNCTIONS
      function addSpeed(){
          fetch('/addSpeed')
              .then(response => response.text())
              .then(newSpeed =>{
              speedLabel.textContent = newSpeed;   
              });
      }
      function minusSpeed(){
          fetch('/minusSpeed')
              .then(response => response.text())
              .then(newSpeed =>{
              speedLabel.textContent = newSpeed;   
              });
      }
      function slewFive() {
          if (trackbutton.textContent == "OFF"){
              fetch('/slewFive');
          }

      }
      function slewMinusFive() {
          if (trackbutton.textContent == "OFF"){
              fetch('/slewMinusFive');
          }
      }
      function slewOne() {
          if (trackbutton.textContent == "OFF"){
              fetch('/slewOne');
          }
      }
      function slewMinusOne() {
          if (trackbutton.textContent == "OFF"){
              fetch('/slewMinusOne');
          }
      }
      function updatePosition(){
          fetch("/getPosition")
              .then(response => response.text()) //get a simple text back
              .then(data => {
                  posLabel.textContent = data;
              })
              .catch(error => console.error("ERROR FETCHING POSITION:", error ));
          
      }
      
      function setZeroPos(){
          fetch("/setZeroPos");
      }
      function resetPosition(){
          if (trackbutton.textContent == "OFF"){
              fetch("/resetPosition");
          }
          
      }
      setInterval(updatePosition, 200);
      
                        //MAINTRACKING OFF/ON RELOADING
                        
      window.addEventListener("DOMContentLoaded", function(){    //does this right when the tab is loaded up
          fetch("/getTrackingState")             //sends fetch command
              .then(response =>{                 //then the esp does the fetch command, and send reply
                if (response.ok){              // if that reply is ok, then we are good no error
                    return response.text();
                } 
                throw new Error("Response was not OK");
                  
              })
              .then(data =>{                   // now we see if the data is equal to tracking has been turned on
                  if (data === "stateOn"){
                      trackbutton.textContent = "ON";           //change colors
                      trackbutton.style.backgroundColor = "#ffffff";
                      trackbutton.style.color = "#000000";
                  }
                  else if (data === "stateOff"){
                          trackbutton.textContent = "OFF";
                          trackbutton.style.backgroundColor = "#000000";
                          trackbutton.style.color = "#ff0000";
                      }
                  else{
                      trackbutton.textContent = "ERROR";
                  }
              })
          fetch("/getSpeed")
              .then(response => response.text())
              .then(data => {
                  speedLabel.textContent = data;
              });
          }); //ends window.eventlistner
          
          
          
          
                    //IF THE TRACKBUTTON IS PRESSED
                    
      trackbutton.addEventListener('click', function(){
          if (trackbutton.textContent == "OFF"){     //if the text in the button is OFF,
              fetch('/trackingOn')                   // send the fetch command
                  .then(response =>{                 //then the esp does the fetch command, and send reply
                    if (response.ok){              // if that reply is ok, then we are good no error
                        return response.text();
                    } 
                    throw new Error("Response was not OK");
                      
                  })
                  .then(data =>{                   // now we see if the data is equal to tracking has been turned on
                      if (data === "trackingHasBeenTurnedOn"){
                          trackbutton.textContent = "ON";           //change colors
                          trackbutton.style.backgroundColor = "#ffffff";
                          trackbutton.style.color = "#000000";
                      }
                      else{
                          trackbutton.textContent = "ERROR";         //sends error if the function in esp does not spin motor
                      }
                  })
          }
          else if (trackbutton.textContent == "ON"){  //same code to turn off
              fetch('/trackingOff')
                  .then(response =>{
                    if (response.ok){
                        return response.text();
                    } 
                    throw new Error("Response was not OK");
                      
                  })
                  .then(data =>{
                      if (data === "trackingHasBeenTurnedOff"){
                          trackbutton.textContent = "OFF";
                          trackbutton.style.backgroundColor = "#000000";
                          trackbutton.style.color = "#ff0000";
                      }
                      else{
                          trackbutton.textContent = "ERROR";
                      }
                  })
          }
      });
      
      
      
      
    </script>
  </body>
  </html>
)rawliteral";

void stepMotorDirect(int direction){
  if (direction > 0){
    stepIndex = (stepIndex + 1) % 4;
  } else {
    stepIndex = (stepIndex - 1 + 4) % 4;
  }
  digitalWrite(IN1, stepSequence[stepIndex][0]);
  digitalWrite(IN2, stepSequence[stepIndex][1]);
  digitalWrite(IN3, stepSequence[stepIndex][2]);
  digitalWrite(IN4, stepSequence[stepIndex][3]);
}
//functions that the esp32 does after webpage sends fetch command

void handleRoot(){
  server.send(200,"text/html", htmlPage);           //Handleroot is the main command when the webpage is opened, 
                                                    // it sends the whole webpage through http, displaying the html port 200
}

void trackingOn(){
  Serial.println("Tracking Turned On...");
  trackingState = true;
  server.send(200, "text/plain","trackingHasBeenTurnedOn");
}

void trackingOff(){
  Serial.println("Tracking Turned Off...");
  trackingState = false;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  server.send(200, "text/plain","trackingHasBeenTurnedOff");
}

void getTrackingState(){
  Serial.println(trackingState);
  if (trackingState){
    server.send(200,"text/plain","stateOn");
  }
  else {
    server.send(200,"text/plain","stateOff");
  }
}

int slewspeed = 2;   //Sets slewing speed from 2 is fast, 8 is slow

void slewFive(){
  Serial.print("Slewing +5mm");
  slewState = true;
  slewDir = -1;
  slewmm = 5;
  targetPos = pos + slewmm;
  server.send(200, "text/plain","OK");
}

void slewMinusFive(){

  Serial.print("Slewing -5mm");
  slewState = true;
  slewDir = 1;
  slewmm = 5;
  targetPos = pos - slewmm;
  server.send(200, "text/plain","OK");


}

void slewOne(){

  Serial.print("Slewing +1mm");
  slewState = true;
  slewDir = -1;
  slewmm = 1;
  targetPos = pos + slewmm;
  server.send(200, "text/plain","OK");

}

void slewMinusOne(){

  Serial.print("Slewing -1mm");
  slewState = true;
  slewDir = 1;
  slewmm = 1;
  targetPos = pos - slewmm;
  server.send(200, "text/plain","OK");

}

void getPosition(){
  server.send(200, "text/plain", String(pos) );
}

void setZeroPos(){
  pos = 0;
}

void resetPosition(){
  if (pos > 0){
    float stepsToMove = pos * stepsPermm;
    Serial.print("Resetting started");
    resetState = true;


  }
}

void slewStep(int dir, int mm){
  unsigned long currentMicros = micros(); //micros() is the onboard clock in microseconds

  if (dir == -1){

    //targetPos = pos + mm;

    if (pos < targetPos){
      if (currentMicros - lastStepTime >= stepIntervalSlewing){
        lastStepTime = currentMicros;                              
        stepMotorDirect(-1);
        Serial.print(".");

        pos = pos + mmPerStep;
      }
    }
    if (pos >= targetPos){
      Serial.println(" ");
      Serial.println("FINISHED SLEWING");
      slewState = false;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
  }

  else if (dir == 1){
    //targetPos = pos - mm;

    if (pos > targetPos){
      if (currentMicros - lastStepTime >= stepIntervalSlewing){
        lastStepTime = currentMicros;                              
        stepMotorDirect(1);
        Serial.print(".");

        pos = pos - mmPerStep;
      }
    }
    if (pos <= targetPos){
      Serial.println(" ");
      Serial.println("FINISHED SLEWING");
      slewState = false;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    } 
  }
}

void addSpeed(){
  stepIntervalMicros += 10;

  prefs.begin("tracker",false);
  prefs.putULong("speed", stepIntervalMicros);
  prefs.end();
  
  server.send(200, "text/plain", String(stepIntervalMicros));
}

void minusSpeed(){
  stepIntervalMicros -= 10;

  prefs.begin("tracker",false);
  prefs.putULong("speed", stepIntervalMicros);
  prefs.end();
  
  server.send(200, "text/plain", String(stepIntervalMicros));
}

void getSpeed(){
  server.send(200, "text/plain", String(stepIntervalMicros));
}


void setup() {


  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(115200);

  prefs.begin("tracker", false);
  //prefs.clear();
  stepIntervalMicros = prefs.getULong("speed", 5400);  //read the stores speed or default to 5400
  prefs.end(); // ends prefereences handle


  //starts access point
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.println("Access point started");
  Serial.print("IP Adress: ");
  Serial.println(WiFi.softAPIP()); // normally 192.168.4.1

  server.on("/", handleRoot);
  server.on("/trackingOn",trackingOn);
  server.on("/trackingOff",trackingOff);
  server.on("/getTrackingState",getTrackingState);
  server.on("/slewFive",slewFive);
  server.on("/slewMinusFive", slewMinusFive);
  server.on("/slewOne",slewOne);
  server.on("/slewMinusOne", slewMinusOne);
  server.on("/getPosition", getPosition);
  server.on("/setZeroPos", setZeroPos);
  server.on("/resetPosition", resetPosition);
  server.on("/addSpeed", addSpeed);
  server.on("/minusSpeed", minusSpeed);
  server.on("/getSpeed", getSpeed);

  server.begin();

  Serial.print("Interval: ");
  Serial.println(stepIntervalMicros);
}

void loop() {

  server.handleClient();

  if (trackingState && !resetState && !slewState){
    unsigned long currentMicros = micros(); //micros() is the onboard clock in microseconds
  
    if (currentMicros - lastStepTime >= stepIntervalMicros){
      lastStepTime = currentMicros;                              // if the current micro second on the clock time - the last time a step was taken,
                                                                 // is greater than the number of time per step, then take set the last time a step
                                                                 // was taken to current time, and step by 1
      stepMotorDirect(-1);

      pos = pos + mmPerStep;
    }
  }
  if (resetState && !trackingState && !slewState ){
    unsigned long currentMicros = micros(); //micros() is the onboard clock in microseconds
    
    if (pos > 0){
      
      if (currentMicros - lastStepTime >= stepIntervalResetMicros){
      lastStepTime = currentMicros;                              // if the current micro second on the clock time - the last time a step was taken,
                                                                 // is greater than the number of time per step, then take set the last time a step
                                                                 // was taken to current time, and step by 1
      stepMotorDirect(1);
      Serial.print(".");

      pos = pos - mmPerStep;
      }
    }
    if (pos <= 0){
      Serial.println(" ");
      Serial.println("FINISHED RESETTING");
      resetState = false;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    
  }
  if (slewState && !trackingState && !resetState){
    slewStep(slewDir, slewmm);
  }

}
