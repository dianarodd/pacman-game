#ifndef PACMANGAME_H
#define PACMANGAME_H

#include <Arduino.h>           // ← Para byte, constrain(), millis(), delay(), tone(), noTone(), etc.
#include <LiquidCrystal.h>

// ——— Configuración de pines ———
extern LiquidCrystal lcd;
extern const int buzz;
extern const int btnArribaPin;
extern const int btnAbajoPin;
extern const int btnDerechaPin;
extern const int btnIzquierdaPin;

// ——— EEPROM ———
extern const int EEPROM_ADDR;
extern int maxScore;

// ——— Velocidades y dimensiones ———
#define VITESSE_PAC   150
#define VITESSE_FANT  2000
#define MAXX          15
#define MAXY          1

// ——— Bitmaps personalizados ———
extern byte pacman[8];
extern byte fantome[8];
extern byte point[8];

// ——— Estado del juego ———
extern byte points[MAXX+1][MAXY+1];
extern int  xpac, ypac;
extern int  xfant, yfant;
extern long keystruck, poursuite;
extern bool partieEnCours, vide;
extern byte level;
extern int  score;

// ——— Prototipos ———
void bouge(int dx, int dy);
void poursuis();
void perdu();
void gagne();
void initLevel();
void reproducirMusica();

#endif // PACMANGAME_H
