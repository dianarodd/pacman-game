#include <Arduino.h>           // ← Igual aquí, para definir todas las funciones Arduino
#include "PacManGame.h"
#include <EEPROM.h>
#include "melodia.h"

// ——— Configuración de pines ———
LiquidCrystal lcd(13, 12, 10, 9, 8, 7);
const int buzz            = 11;
const int btnArribaPin    = 3;
const int btnAbajoPin     = 4;
const int btnDerechaPin   = 5;
const int btnIzquierdaPin = 6;

// ——— EEPROM ———
const int EEPROM_ADDR = 0;
int maxScore = 0;

// ——— Bitmaps personalizados ———
byte pacman[8] = {
  B00000, B00000, B01110, B11011,
  B11100, B01110, B00000, B00000
};
byte fantome[8] = {
  B00000, B00000, B01110, B10101,
  B11111, B11111, B10101, B00000
};
byte point[8] = {
  B00000, B00000, B00000, B01110,
  B01110, B00000, B00000, B00000
};

// ——— Estado del juego ———
byte points[MAXX+1][MAXY+1];
int  xpac = 2, ypac = 1;
int  xfant = 15, yfant = 0;
long keystruck = 0, poursuite = 0;
bool partieEnCours = true, vide = false;
byte level = 0;
int  score = 0;

// ——— Implementación de funciones ———

void bouge(int dx, int dy) {
  int oldx = xpac, oldy = ypac;
  xpac = constrain(xpac + dx, 0, MAXX);
  ypac = constrain(ypac + dy, 0, MAXY);

  lcd.setCursor(xpac, ypac); lcd.write(byte(0));
  lcd.setCursor(oldx, oldy);
  if (xpac != oldx || ypac != oldy) lcd.print(" ");

  if (points[xpac][ypac]) {
    points[xpac][ypac] = false;
    score++;
  }

  // Comprueba si quedan puntos
  vide = true;
  for (int i = 0; i <= MAXX; i++)
    for (int j = 0; j <= MAXY; j++)
      if (points[i][j]) vide = false;
  if (vide && partieEnCours) gagne();
}

void poursuis() {
  int oldx = xfant, oldy = yfant;
  if      (yfant < ypac) yfant++;
  else if (yfant > ypac) yfant--;
  else if (xfant < xpac) xfant++;
  else if (xfant > xpac) xfant--;

  lcd.setCursor(xfant, yfant); lcd.write(byte(1));
  lcd.setCursor(oldx, oldy);
  if (oldx != xfant || oldy != yfant) {
    if (points[oldx][oldy]) lcd.write(byte(2));
    else                    lcd.print(" ");
  }
}

void perdu() {
  partieEnCours = false;
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("GAME OVER");
  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.print(score);
  lcd.print(" Hi:");
  lcd.print(maxScore);

  if (score > maxScore) {
    maxScore = score;
    EEPROM.put(EEPROM_ADDR, maxScore);
    lcd.setCursor(0, 0);
    lcd.print("NEW HIGHSCORE!");
  }

  delay(2000);
  level = 0;
  score = 0;
  initLevel();
}

void gagne() {
  level++;
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("NEXT LEVEL");
  lcd.setCursor(6, 1);
  lcd.print(level);
  delay(1500);
  initLevel();
}

void initLevel() {
  partieEnCours = true;
  for (int i = 0; i <= MAXX; i++) {
    for (int j = 0; j <= MAXY; j++) {
      points[i][j] = true;
      lcd.setCursor(i, j);
      lcd.write(byte(2));
    }
  }
  xpac = 2;  ypac = 1;
  xfant = 15; yfant = 0;
  lcd.setCursor(xpac, ypac); lcd.write(byte(0));
  lcd.setCursor(xfant, yfant); lcd.write(byte(1));
  keystruck = poursuite = millis();
}

void reproducirMusica() {
  static int notaActual = 0;
  static unsigned long tiempoNotaAnterior = 0;
  static bool notaActiva = false;
  unsigned long ahora = millis();

  if (notaActual < numNotas) {
    if (!notaActiva) {
      tone(buzz, melodia[notaActual][0]);
      tiempoNotaAnterior = ahora;
      notaActiva = true;
    }
    if (ahora - tiempoNotaAnterior >= melodia[notaActual][1]) {
      noTone(buzz);
      notaActual++;
      notaActiva = false;
    }
  } else {
    notaActual = 0;
  }
}
