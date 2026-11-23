#include <Arduino.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Wire.h>
MAX30105 max30102;
const byte rozmiar = 20;
float pomiary[rozmiar];
byte indeks = 0;
byte ileZebrano = 0;
long ostatniPuls = 0; // kiedy był ostatni puls
float tetno = 0;      // BPM
void setup()
{
  Serial.begin(9600);
  max30102.begin();
  max30102.setup(60, 4, 2, 100, 411, 4096);
  Serial.println("Przyloz palec do czujnika");
}

void loop()
{

  long odczyt = max30102.getIR();

  if (odczyt < 50000)
  {
    ileZebrano = 0;
    indeks = 0;
    ostatniPuls = 0;
    return;
  }
  if (checkForBeat(odczyt))
  {
    long teraz = millis();
    long delta = teraz - ostatniPuls;
    ostatniPuls = teraz;
    tetno = 60.0 / (delta / 1000.0);

    if (tetno > 40 && tetno < 180)
    {
      pomiary[indeks] = tetno;
      indeks = (indeks + 1) % rozmiar;
      if (ileZebrano < rozmiar)
        ileZebrano++;
      float suma = 0;
      for (byte i = 0; i < ileZebrano; i++)
        suma += pomiary[i];
      suma = suma / ileZebrano;
      Serial.println("Tetno");
      Serial.print(suma);
      Serial.println(" BPM");
    }
  }
}
