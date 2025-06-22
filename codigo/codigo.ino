#include <EEPROM.h>
#include "PacManGame.h"

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  // Carga el high score desde EEPROM
  EEPROM.get(EEPROM_ADDR, maxScore);
  
  // Crea los caracteres
  lcd.createChar(0, pacman);
  lcd.createChar(1, fantome);
  lcd.createChar(2, point);

  pinMode(btnArribaPin,    INPUT_PULLUP);
  pinMode(btnAbajoPin,     INPUT_PULLUP);
  pinMode(btnDerechaPin,   INPUT_PULLUP);
  pinMode(btnIzquierdaPin, INPUT_PULLUP);

  pinMode(buzz, OUTPUT);
  noTone(buzz);

  // Mensaje de bienvenida
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PAC-MAN!");
  lcd.setCursor(0, 1);
  delay(1500);

  initLevel();
}

void loop() {
  reproducirMusica();

  // Control de Pac-Man
  if (millis() - keystruck > VITESSE_PAC) {
    bool moved = false;
    if (digitalRead(btnArribaPin) == LOW)   { bouge(0, -1); moved = true; Serial.println("ARRIBA"); }
    if (digitalRead(btnAbajoPin) == LOW)    { bouge(0,  1); moved = true; Serial.println("ABAJO"); }
    if (digitalRead(btnDerechaPin) == LOW)  { bouge(1,  0); moved = true; Serial.println("DERECHA"); }
    if (digitalRead(btnIzquierdaPin) == LOW){ bouge(-1, 0); moved = true; Serial.println("IZQUIERDA"); }
    if (moved) keystruck = millis();
  }

  // Fantasma
  if (millis() - poursuite > VITESSE_FANT / (level + 1) + 10) {
    poursuis();
    poursuite = millis();
  }

  // Colisión
  if (partieEnCours && xpac == xfant && ypac == yfant) {
    perdu();
  }
}
