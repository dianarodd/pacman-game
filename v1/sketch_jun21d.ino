#include <LiquidCrystal.h>

// configuracion del lcd
LiquidCrystal lcd(10, 8, 7, 6, 5, 4);

// direcciones en pines
const int botonIzquierda = 13;   // pin 13 - Izquierda
const int botonDerecha = 12;     // pin 12 - Derecha
const int botonArriba = 3;       // pin 3 - Arriba
const int botonAbajo = 2;        // pin 2 - Abajo

// pin de sonido
const int buzzerMusica = 9;      // pin 9 - Melodías
const int buzzerEfectos = 11;    // pin 11 - Efectos 

// notas
const int Do = 261, Do2 = 523, Do3 = 1046;
const int Re = 293, Re2 = 587, Re3 = 1174;
const int Mi = 329, Ree = 331;
const int Fa = 349, Fa2 = 698;
const int Sol = 392, Soll = 415, Sol2 = 784, Soll2 = 830;
const int La = 440, Laa = 466, La2 = 880, Laa2 = 932;
const int Si = 493, Si1 = 247;

const int prim = 100, segun = 200; // duracion de notas

// personajes
byte pacman[8] = {B00000,B00000,B01110,B11011,B11100,B01110,B00000,B00000};
byte fantasma[8] = {B00000,B00000,B01110,B10101,B11111,B11111,B10101,B00000};
byte punto[8] = {B00000,B00000,B00000,B01110,B01110,B00000,B00000,B00000};

// variables
const int MAXX = 15;
const int MAXY = 1;

int xpac = 2, ypac = 1;
int xfant = 15, yfant = 0;
bool puntos[MAXX + 1][MAXY + 1];
bool jugando = false;
int score = 0;
long t_fant = 0;
int VEL_FANT = 1000;

// deteccion de botones
bool lastIzquierdaState = HIGH;
bool lastDerechaState = HIGH;
bool lastArribaState = HIGH;
bool lastAbajoState = HIGH;

// sonido

void nota(int freq, int dur) {
  tone(buzzerMusica, freq, dur);
  delay(dur);
  noTone(buzzerMusica);
  delay(20); // pausa entre notas
}

void reproducirMelodia() {
  nota(Re, prim); nota(Re, prim); nota(Re2, prim); nota(La, prim); nota(Soll, prim);
  nota(Sol, prim); nota(Fa, segun); nota(Re, prim); nota(Fa, prim); nota(Sol, prim);
  nota(Do, prim); nota(Do, prim); nota(Re2, prim); nota(La, prim); nota(Soll, prim);
  nota(Sol, prim); nota(Fa, segun); nota(Re, prim); nota(Fa, prim); nota(Sol, prim);
  nota(Si1, prim); nota(Si1, prim); nota(Re2, prim); nota(La, prim); nota(Soll, prim);
  nota(Sol, prim); nota(Fa, segun); nota(Re, prim); nota(Fa, prim); nota(Sol, prim);
}

void sonidoMuerte() {
  noTone(buzzerMusica); // detiene la musica al sonar efectos
  tone(buzzerEfectos, 400, 300); delay(300);
  tone(buzzerEfectos, 300, 300); delay(300);
  tone(buzzerEfectos, 200, 500); delay(500);
  noTone(buzzerEfectos);
}

// juego 

void moverPacman(int dx, int dy) {
  int ox = xpac, oy = ypac;
  xpac = constrain(xpac + dx, 0, MAXX);
  ypac = constrain(ypac + dy, 0, MAXY);

  lcd.setCursor(ox, oy);
  if (puntos[ox][oy]) lcd.write(2); else lcd.print(" ");

  lcd.setCursor(xpac, ypac);
  lcd.write((byte)0);

  if (puntos[xpac][ypac]) {
    puntos[xpac][ypac] = false;
    score++;
    tone(buzzerEfectos, 1000, 50); // efecto al comer
  }

  bool vacio = true;
  for (int i = 0; i <= MAXX; i++) {
    for (int j = 0; j <= MAXY; j++) {
      if (puntos[i][j]) vacio = false;
    }
  }
  if (vacio) iniciarNivel();
}

void moverFantasma() {
  int ox = xfant, oy = yfant;
  if (xfant < xpac) xfant++;
  else if (xfant > xpac) xfant--;
  else if (yfant < ypac) yfant++;
  else if (yfant > ypac) yfant--;

  lcd.setCursor(ox, oy);
  if (puntos[ox][oy]) lcd.write(2); else lcd.print(" ");

  lcd.setCursor(xfant, yfant);
  lcd.write(1);
}

void pantallaInicio() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("PACMAN :v");
  lcd.setCursor(0, 1); lcd.print("accion:Izq/Der");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Arriba/Abajo");
  lcd.setCursor(0, 1); lcd.print("Pulse para mover");
  delay(2500);
  reproducirMelodia(); // sonido de inicio
}

void pantallaGameOver() {
  sonidoMuerte(); // efecto al morir
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("GAME OVER      ");
  lcd.setCursor(0, 1); lcd.print("Puntaje: "); lcd.print(score);
  delay(3000);
  iniciarNivel();
}

void iniciarNivel() {
  lcd.clear();
  for (int i = 0; i <= MAXX; i++) {
    for (int j = 0; j <= MAXY; j++) {
      puntos[i][j] = true;
      lcd.setCursor(i, j); lcd.write(2);
    }
  }
  xpac = 2; ypac = 1;
  xfant = 15; yfant = 0;
  score = 0;
  jugando = true;
  t_fant = millis();

  lcd.setCursor(xpac, ypac); lcd.write((byte)0);
  lcd.setCursor(xfant, yfant); lcd.write((byte)1);
}



void setup() {
  pinMode(botonIzquierda, INPUT_PULLUP);
  pinMode(botonDerecha, INPUT_PULLUP);
  pinMode(botonArriba, INPUT_PULLUP);
  pinMode(botonAbajo, INPUT_PULLUP);
  pinMode(buzzerMusica, OUTPUT);
  pinMode(buzzerEfectos, OUTPUT);

  lcd.begin(16, 2);
  lcd.createChar(0, pacman);
  lcd.createChar(1, fantasma);
  lcd.createChar(2, punto);

  pantallaInicio();
  iniciarNivel();
}

void loop() {
  if (!jugando) return;

  bool currentIzquierdaState = digitalRead(botonIzquierda);
  bool currentDerechaState = digitalRead(botonDerecha);
  bool currentArribaState = digitalRead(botonArriba);
  bool currentAbajoState = digitalRead(botonAbajo);

  if (currentIzquierdaState == LOW && lastIzquierdaState == HIGH) moverPacman(-1, 0);
  if (currentDerechaState == LOW && lastDerechaState == HIGH) moverPacman(1, 0);
  if (currentArribaState == LOW && lastArribaState == HIGH) moverPacman(0, -1);
  if (currentAbajoState == LOW && lastAbajoState == HIGH) moverPacman(0, 1);

  lastIzquierdaState = currentIzquierdaState;
  lastDerechaState = currentDerechaState;
  lastArribaState = currentArribaState;
  lastAbajoState = currentAbajoState;

  if (millis() - t_fant > VEL_FANT) {
    moverFantasma();
    t_fant = millis();
  }

  if (xpac == xfant && ypac == yfant) {
    jugando = false;
    pantallaGameOver();
  }
}
