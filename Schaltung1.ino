/**
 * Modul 242 - Mikroprozessoranwendungen realisieren
 * 
 * Leistungsbeurteilung 2
 * Schaltung 1
 * 
 * Marina Stucki
 * IABM16c
 * 09.04.2019
 * 
 * Diese Schaltung dient dem persönlichen Vergnügen. 
 * Die 6 LED-Lampen erleuchten der Reihe nach hin und zurück in den Farben rot, gelb und weiss.
 * Wenn man den Knopf gedrückt hält, leuchten die LED-Lichter in einer anderen Reihenfolge auf. 
 * Sobald sie das Ende der Reihe ereicht haben, beginnen sie wieder von vorne zu leuchten. 
 * 
 * Diese Schaltung ist vorallem für kleine Kinder ein Vergnügen. Sie erkennen, was ein kleiner
 * Knopf alles bewirken kann. Diese Schaltung kann als ein kleiner Partyspiel verwendet werden. 
 * 
 */

// Variablen deklarieren & ggf. initialisieren
int timer = 100;    // Je höcher die Nummer, desto langsamer das Timing
int buttonState;    // Kann LOW oder HIGH sein. Auslesung vom Input Pin (Button)
int lastButtonState = LOW;  // Der zuvor gelesene Wert vom Input Pin (Button) -> vorheriger ButtonState
int currentLEDModus = 0;  // LED-Modus 0: hin und zurück / LED-Modus 1: wenn am Ende angekommen, von neu beginnen

// Die Variablen haben den Datentyp unsigned long, weil die Zeit in millisekunden 
// gemessen wird und die Zahl schnell zu gross wird für den Datentyp int. 
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Konstonanten, da die PinNr. nicht verändert werden soll.
const int buttonPin = 8;
const int firstLedPin = 2;
const int lastLedPin = 7;

void setup() {

  // mit diesre Schleife werden alle Pins für die LEDs als output initialisiert. 
  for (int thisPin = firstLedPin; thisPin <= lastLedPin; thisPin++)  {
    pinMode(thisPin, OUTPUT);      
  }
  pinMode(buttonPin, INPUT);
}

void loop() {
  
  // Liest den Status des Buttons und speichert ihn in die Variable buttonReading
  int buttonReading = digitalRead(buttonPin);

  // Überprüft, ob der Button gedrückt wurde. 
  if (buttonReading != lastButtonState) {
    // Wenn der Button gedrück wurde, wird die lastDebounceTime zurückgesetzt. 
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // wenn der Button gedrückt wurde (überprüfen ob Status geändert wurde)
    if (buttonReading != buttonState) {
      buttonState = buttonReading;

      // LEDModus nur wechseln, wenn der Button gedrückt wurde. 
      if(buttonState == HIGH){
        // Wenn momentan der LEDModus 0 aktiv ist, den Modus auf 1 setzen.
        if (currentLEDModus == 0) {
          currentLEDModus = 1;
        }
        else {
          currentLEDModus = 0;
        }
        // Alle LEDs kurz abschalten, damit der Wechsel bemerkbar ist. 
        for (int thisPin = firstLedPin; thisPin <= lastLedPin; thisPin++) { 
            digitalWrite(thisPin, LOW); 
            delay(50);
        }
      }
    }
  }

  // Schleife vom ersten zum letzen LED-Licht
  // Diese Schleife wird bei jedem Modus aufgerufen, 
  // daher ist ihr keine Bedingung vorangestellt
  for (int thisPin = firstLedPin; thisPin <= lastLedPin; thisPin++) { 
    // LED-Licht anschalten
    digitalWrite(thisPin, HIGH);   
    delay(timer);                  
    // LED-Licht ausschalten
    digitalWrite(thisPin, LOW);    
  }

  // Wenn der LED-Modus den Wert 0 hat, wird der Inhalt dieser Methode ausgefüht. 
  // Modus 0 hat ein anderes LED-Muster. 
  if(currentLEDModus == 0){
    // Schleife vom letzten zum ersten LED-Licht
    for (int thisPin = lastLedPin; thisPin >= firstLedPin; thisPin--) { 
      // LED-Licht anschalten
      digitalWrite(thisPin, HIGH);
      delay(timer);
      //LED-Licht ausschalten
      digitalWrite(thisPin, LOW);
    }
  }

  // Der letzte Button-Status wird gesetzt.
  lastButtonState = buttonReading;

}



  
