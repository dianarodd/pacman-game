#include <LiquidCrystal.h>

LiquidCrystal lcd(10, 9, 8, 7, 6, 5);  // RS, E, D4, D5, D6, D7
const int buzzerPin = 11;

// notas de la melodia (prueben si esta bien.. diria q lo cambien a prtir de la implementacion)
#define SOL4 392
#define LA4 440
#define MI4 330
#define RE5 587
#define SI4 494
#define DO5 523
#define SOL5 784
#define NONE 0

// melodia en si
const int melodia[][2] = {
  {SOL4, 400}, {LA4, 200}, {SOL4, 400}, {MI4, 800},
  {SOL4, 400}, {LA4, 200}, {SOL4, 400}, {MI4, 800},
  {RE5, 800}, {RE5, 400}, {SI4, 800},
  {DO5, 800}, {SOL5, 400}, {SOL4, 800},
  {LA4, 800}, {LA4, 400}, {DO5, 400},
  {SI4, 200}, {LA4, 400}, {SOL4, 400}
};

const int numNotas = sizeof(melodia) / sizeof(melodia[0]);

// juego
#define VEL_PAC 300      // velocidad de Pacman (ms)
#define VEL_FANT 500     // velocidad de fantasmas (ms)
#define MAXX 15          // máximo en X (16 columnas)
#define MAXY 1           // máximo en Y (son 2 filas)

// caracteres (lo q aparece en el juego en si)
byte pacman[8] = {B00000,B00000,B01110,B11011,B11100,B01110,B00000,B00000};
byte fantasma[8] = {B00000,B00000,B01110,B10101,B11111,B11111,B10101,B00000};
byte punto[8] = {B00000,B00000,B00000,B01110,B01110,B00000,B00000,B00000};

// variables
int xpac = 0;           // pacman simepre en columna 0
int ypac = 0;           // iniciara en el carril 0 o 1
int xfant[MAXY+1];      // posicion en x de fantasma
int yfant[MAXY+1];      // posicion en y de fantasma
bool jugando = false;
int score = 0;
long t_pac = 0, t_fant = 0;

// posicion inicial de fantasmas
void initFantasmas() {
  for(int i=0; i<=MAXY; i++) {
    xfant[i] = MAXX;    // Aaparecen por derecha
    yfant[i] = i;       // uno por carril
  }
}

// boton (solo arriba y abajo)
int leerBoton() {
  int v = analogRead(A0);
  if (v > 1000) return -1;  // sin puksar
  if (v < 180) return 1;    // arriba
  if (v < 520) return 2;    // abajo
  return -1;
}

// mesaje de inicio
void mostrarInicio() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("PACMAN");
  lcd.setCursor(0, 1); lcd.print("Pulsa para jugar");
  while (leerBoton() == -1);
  delay(500);
}

// fin de juego (pierdes)
void mostrarGameOver() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("GAME OVER");
  lcd.setCursor(0, 1); lcd.print("Score: ");
  lcd.print(score);
  delay(3000);
}

// inicio de nivel
void iniciarNivel() {
  lcd.clear();
  initFantasmas();
  // dibujo pacman
  lcd.setCursor(xpac, ypac); 
  lcd.write(byte(0));
  // cibujo fantasma
  for(int i=0; i<=MAXY; i++) {
    lcd.setCursor(xfant[i], yfant[i]);
    lcd.write(byte(1));
  }
  jugando = true;
}

// movimiento fantasma
void moverFantasmas() {
  for(int i=0; i<=MAXY; i++) {
    // borrar anterior ubicacion
    lcd.setCursor(xfant[i], yfant[i]);
    lcd.print(" ");
    
    // se mueve solo hacia izq
    xfant[i]--;
    
    // si llegfa a limite reaparece por derecha
    if(xfant[i] < 0) {
      xfant[i] = MAXX;
      // probabilidad de cambiar de  carril
      if(random(2) == 0) yfant[i] = !yfant[i]; 
    }
    
    // se dibuja su nueva ubicacion
    lcd.setCursor(xfant[i], yfant[i]);
    lcd.write(byte(1));
  }
}

// movimiento pacman (solo arriba y abajo)
void moverPacman(int dy) {
  // se borra posicion anterior
  lcd.setCursor(xpac, ypac);
  lcd.print(" ");
  
  // cambio de carril (entre el 0 y 1 o carril 1 y 2)
  ypac = (ypac + dy) % 2;
  if(ypac < 0) ypac = 1;
  
  // se dinuja nueva posicion
  lcd.setCursor(xpac, ypac);
  lcd.write(byte(0));
  
  // colisiones
  for(int i=0; i<=MAXY; i++) {
    if(xfant[i] == xpac && yfant[i] == ypac) {
      jugando = false;
      mostrarGameOver();
      setup(); // reinicia juego
      return;
    }
  }
  
  score++; // aumenta la puntuacion
}

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, pacman);
  lcd.createChar(1, fantasma);
  randomSeed(analogRead(A1)); // para aleatoriedad
  
  mostrarInicio();
  iniciarNivel();
  t_pac = millis();
  t_fant = millis();
}

void loop() {
  if(!jugando) return;
  
  // control de pacman
  if(millis() - t_pac > VEL_PAC) {
    int dir = leerBoton();
    if(dir == 1) moverPacman(-1); // arriba
    if(dir == 2) moverPacman(1);  // abajo
    t_pac = millis();
  }
  
  // movimiento de fantasma
  if(millis() - t_fant > VEL_FANT) {
    moverFantasmas();
    t_fant = millis();
  }
}