/**
 * Modul 242 - Mikroprozessoranwendungen realisieren
 * 
 * Leistungsbeurteilung 2
 * Schaltung 2
 * 
 * Marina Stucki
 * IABM16c
 * 09.04.2019
 * 
 * Diese Schaltung dient dem persönlichen Vergnügen. 
 * 
 * Diese Schaltung repräsentiert ein Spiel. 
 * Es gibt den User Blau und Rot. 
 * Beim Start läuchten alle 3 Led Lampen auf. 
 * Nach einer zufälligen Anzahl Sekunden (Beschränkt auf 15) leuchtet die weisse LED Lampe in der Mitte auf. 
 * Nun müssen die Benutzer so schnell wie möglich den entsprechenden Button (Blau oder Rot) drücken. 
 * Der schnellere Benutzer gewinnd. Dies wird mit dem Aufleuchten der Grünen Lampe auf der enstrechenden Seite gezeigt. 
 * Um das Spiel neu zu starten, muss der weisse Button gedrückt werden. 
 * 
 */
#include <time.h>
#include <stdlib.h>

// Variablen deklarieren & ggf. initialisieren
int randomTimeLed;  // in Sekunden
int timerWhiteLed = 100;    // Je höcher die Nummer, desto langsamer das Timing
int timerGreenLeds = 500;    // Je höcher die Nummer, desto langsamer das Timing

int buttonStateBlue;    // Kann LOW oder HIGH sein. Auslesung vom Input Pin (Button)
int lastButtonStateBlue = LOW;  // Der zuvor gelesene Wert vom Input Pin (Button) -> vorheriger ButtonState
int buttonReadingBlue;
unsigned long startTimeBlue; 
unsigned long buttonPressedTimeBlue = 0;   
unsigned long timeDifferenceUserBlue;

int buttonStateRed;    // Kann LOW oder HIGH sein. Auslesung vom Input Pin (Button)
int lastButtonStateRed = LOW;  // Der zuvor gelesene Wert vom Input Pin (Button) -> vorheriger ButtonState
int buttonReadingRed;
unsigned long startTimeRed;  
unsigned long buttonPressedTimeRed = 0; 
unsigned long timeDifferenceUserRed; 

// Die Variablen haben den Datentyp unsigned long, weil die Zeit in millisekunden 
// gemessen wird und die Zahl schnell zu gross wird für den Datentyp int. 
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int buttonReadingWhite;

// Konstonanten, da die PinNr. nicht verändert werden soll.
const int buttonPinUserBlue = 2;
const int buttonPinUserRed = 3;
const int buttonPinWhite = 4;
const int whiteLedPin = 11;
const int greenLedPinUserBlue = 12;
const int greenLedPinUserRed = 13;

void setup() {

  Serial.begin(9600);
  // Initialisierung der Pins
  pinMode(buttonPinUserBlue, INPUT);
  pinMode(buttonPinUserRed, INPUT);
  pinMode(buttonPinWhite, INPUT);

  pinMode(whiteLedPin, OUTPUT);
  pinMode(greenLedPinUserBlue, OUTPUT);
  pinMode(greenLedPinUserRed, OUTPUT);
}

void loop() {

  // Initialisierung
  buttonReadingBlue = LOW;
  buttonReadingRed = LOW;
  buttonReadingWhite = LOW;


  digitalWrite(greenLedPinUserRed, HIGH);  
  digitalWrite(greenLedPinUserBlue, HIGH);  
  digitalWrite(whiteLedPin, HIGH);   
  delay(timerWhiteLed);
  digitalWrite(greenLedPinUserRed, LOW);
  digitalWrite(greenLedPinUserBlue, LOW);
  digitalWrite(whiteLedPin, LOW);
  delay(timerWhiteLed);

  // Zufällige Wartezeit bis LED aufleuchtet
  srand(time(NULL));
  randomTimeLed = rand() % 6;
  if (randomTimeLed != 0){
    delay(randomTimeLed * 1000);
  }
  else{
    delay(1000);
  }

  // Timer start, User Rot und Blau auf gleiche Zeit setzen. 
  startTimeBlue = millis();
  startTimeRed = startTimeBlue;

  digitalWrite(whiteLedPin, HIGH);   
  delay(timerWhiteLed);
  digitalWrite(whiteLedPin, LOW);

  do {
    buttonReadingBlue = digitalRead(buttonPinUserBlue);
    buttonReadingRed = digitalRead(buttonPinUserRed);
    delay(40);
  } while(buttonReadingBlue == LOW && buttonReadingRed == LOW);

  // Überprüft, ob der Button gedrückt wurde. 
  if (buttonReadingBlue != lastButtonStateBlue) {
    // Wenn der Button gedrück wurde, wird die lastDebounceTime zurückgesetzt. 
    buttonPressedTimeBlue = millis();
  }

  // Überprüft, ob der Button gedrückt wurde. 
  if (buttonReadingRed != lastButtonStateRed) {
    // Wenn der Button gedrück wurde, wird die lastDebounceTime zurückgesetzt. 
    buttonPressedTimeRed = millis();
  }
  
  timeDifferenceUserBlue = buttonPressedTimeBlue - startTimeBlue;
  timeDifferenceUserRed = buttonPressedTimeRed - startTimeRed;

  Serial.println("Blue - Start: " + String(startTimeBlue, DEC));
  Serial.println("Red - Start: " + String(startTimeRed, DEC));
  Serial.println("Blue - ButtonPressed: " + String(buttonPressedTimeBlue, DEC));
  Serial.println("Red - ButtonPressed: " + String(buttonPressedTimeRed, DEC));
  Serial.println("Blue - Zeitunterschied: " + String(timeDifferenceUserBlue, DEC));
  Serial.println("Red - Zeitunterschied: " + String(timeDifferenceUserRed, DEC));

  // Wenn der Benutzer Blau zuerst den Button gedrückt hat, erleuchtet die grüne Lampe links
  if(timeDifferenceUserBlue < timeDifferenceUserRed){
    digitalWrite(greenLedPinUserBlue, HIGH);   
    delay(timerGreenLeds);
    digitalWrite(greenLedPinUserBlue, LOW);
  }
  // Wenn der Benutzer Rot zuerst den Button gedrückt hat, erleuchtet die grüne Lampe rechts
  else if (timeDifferenceUserBlue > timeDifferenceUserRed){
    digitalWrite(greenLedPinUserRed, HIGH);   
    delay(timerGreenLeds);
    digitalWrite(greenLedPinUserRed, LOW);
  }
  // Wenn beide Benutzer die Buttons gleichzeitig gedrückt haben, erläuchten alle 3 LED's in kurzem Abstand hintereinander. 
  else {
    for (int i = 0; i < 4; i++){
      digitalWrite(greenLedPinUserRed, HIGH);  
      digitalWrite(greenLedPinUserBlue, HIGH);  
      digitalWrite(whiteLedPin, HIGH);   
      delay(timerWhiteLed);
      digitalWrite(greenLedPinUserRed, LOW);
      digitalWrite(greenLedPinUserBlue, LOW);
      digitalWrite(whiteLedPin, LOW);
      delay(timerWhiteLed);
    }
  }

  // Neues Spiel starten durch Klick auf Button Weiss
  do {
    buttonReadingWhite = digitalRead(buttonPinWhite);
    delay(40);
  } while(buttonReadingWhite == LOW);

}
