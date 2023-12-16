#include "Arduino_LED_Matrix.h"

#define SW     A2

uint8_t DEATH_SCREEN[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};


ArduinoLEDMatrix matrix;

uint8_t frame[8][12];

bool sw_state;
int line_pos, line_height, player_pos;

/***********************
 *******Fonctions*******
 ***********************/
void draw_line(int length, int x) {
  if(x > 11)
    return;

  for(int i = 0; i < length; i++) {
    if(7-i <= 0)
      return;

    frame[7-i][x] = 1;
  }
}

/***********************
 ********Code***********
 ***********************/
void setup() {
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  matrix.begin();

  line_pos = 11;
  line_height = random(1, 6);
}

void loop() {
  sw_state = !digitalRead(SW);

  if(line_pos <= 0) {
    line_pos = 11;
    line_height = random(1, 6);
  } else {
    line_pos--;
  }

  frame[0][line_pos] = 1; // Draw top pixel
  draw_line(line_height, line_pos);

  if(sw_state) {
    if(player_pos > 0)
      player_pos--;
  } else {
    if(player_pos < 7)
      player_pos++;
  }

  // Detect player's death
  if(line_pos == 2 && ((7-line_height) < player_pos || player_pos == 0)) {
    while(HIGH) {
      matrix.renderBitmap(DEATH_SCREEN, 8, 12);
    }
  }

  frame[player_pos][2] = 1; // Draw the player

  matrix.renderBitmap(frame, 8, 12);
  memset(frame, 0, sizeof(frame));
  delay(100);
}
