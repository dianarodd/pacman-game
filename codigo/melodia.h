// melodia.h
#pragma once
#include "notas.h"

// Melodía de introducción de Pac-Man (105 BPM) en pares {nota, duración_ms}
const int melodia[][2] = {
  { SI4, 143 }, { SI5, 143 }, { FAS5, 143 }, { RES5, 143 },
  { SI5,  71 }, { FAS5, 214 }, { RES5, 286 }, { DO5, 143 },
  { DO6, 143 }, { SOL6,143 }, { MI6, 143 }, { DO6,  71 },
  { SOL6,214 }, { MI6, 286 },

  { SI4, 143 }, { SI5, 143 }, { FAS5,143 }, { RES5, 143 },
  { SI5,  71 },

  { FAS5,214 }, { RES5,286 }, { RES5, 71 }, { MI5,  71 },
  { FA5,  71 }, { FA5,  71 }, { FAS5, 71 }, { SOL5, 71 },
  { SOL5, 71 }, { SOLS5,71 }, { LA5, 143 }, { SI5, 286 }
};

const int numNotas = sizeof(melodia) / sizeof(melodia[0]);
