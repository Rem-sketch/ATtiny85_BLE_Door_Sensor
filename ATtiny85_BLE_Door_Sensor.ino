#include <avr/sleep.h> //set this library for device's sleep
#include <avr/interrupt.h>  //set this library for device's interruption
#include <SoftwareSerial.h> //for bluetooth's connection

SoftwareSerial mySerial(0, 1); // RX, TX
const int switchPin                     = 2;    //Reed Switch
const int statusLED                     = 4;    //LED indicator

void setup() {

    pinMode(switchPin, INPUT);      //Reed Switch
    digitalWrite(switchPin, HIGH);  //Reed Switch
    pinMode(statusLED, OUTPUT);     //LED indicator
    pinMode(0,INPUT);               // BLUETOOTH TX PIN
  pinMode(1,OUTPUT);                // BLUETOOTH RX PIN
  pinMode(A3,INPUT);                // LDR SENSOR PIN
  
  mySerial.begin(9600);
  mySerial.println("AT+NAMEATTINY85_BLE");      //set the device's name
  mySerial.println("AT+PWRM0");           //set the module to auto sleep

  mySerial.println("AT+RESET");       //set the ble to reset at first
  mySerial.println("AT+PIO11");       //set the ble's system led to switch off when unconnected and only on when connected
  mySerial.println("AT+IMME1");       //set the ble to peripheral role
    delay(500);
    }

void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT2);                   // Use PB2 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT2);                  // Turn off PB2 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
    } // sleep

ISR(PCINT2_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
    }

void loop() {
    sleep();
    digitalWrite(statusLED, HIGH);      //when interrupt start,led will blink 0.5s
    delay(500);
    digitalWrite(statusLED, LOW);

    if(mySerial.available()){
    int a = digitalRead(2);
    int b = analogRead(A3);
if (a==HIGH){
  mySerial.println("Open\n");
}
else{
  mySerial.println("Close\n");
}
delay(1000);
if (b<=500){
  mySerial.println("Light\n");
}
else {
  mySerial.println("Dark\n");
    }
    }

}
