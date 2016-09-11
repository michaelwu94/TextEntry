#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL); 
#endif

// Assign pins numbers to constant ints
const int up_pin = D9;
const int down_pin = D1;
const int left_pin = D0;
const int right_pin = D2;
const int select_pin = D8;
const int delete_pin = D10;

// Assign button states
// 0 for unpressed, 1 for pressed
bool up_state = 0;
bool down_state = 0;
bool left_state = 0;
bool right_state = 0;
bool select_state = 0;
bool delete_state = 0;

// Assign previous button states to prevent accidental multiple presses
// 0 for unpressed, 1 for pressed
bool up_prev_state = 0;
bool down_prev_state = 0;
bool left_prev_state = 0;
bool right_prev_state = 0;
bool select_prev_state = 0;
bool delete_prev_state = 0;

// Assign keyboards to constant char array
const int keyboard_rows = 5;
const int keyboard_cols = 10;
const char *keyboard[keyboard_rows][keyboard_cols] =
          { {"1234567890", "qwertyuiop", "asdfghjkl<", "^zxcvbnm,.", " SPACEBAR "},
            {"!@#$%&*-+=", "QWERTYUIOP", "ASDFGHJKL<", "^ZXCVBNM/?", " spacebar "} };

// Assign default keyboard parameters
String display_string = "";
int letter_case = 0;    // 0 for lower case, 1 for upper case
int cursor_row = 0;
int cursor_col = 0;

void setup() {
  
  Serial.begin(9600);
  
  // Assign pins to input pulldown
  pinMode(up_pin, INPUT_PULLDOWN);
  pinMode(down_pin, INPUT_PULLDOWN);
  pinMode(left_pin, INPUT_PULLDOWN);
  pinMode(right_pin, INPUT_PULLDOWN);
  pinMode(select_pin, INPUT_PULLDOWN);
  pinMode(delete_pin, INPUT_PULLDOWN);

}

void loop() {
  
  // Update buttons' previous states
  up_prev_state = up_state;
  down_prev_state = down_state;
  left_prev_state = left_state;
  right_prev_state = right_state;
  select_prev_state = select_state;
  delete_prev_state = delete_state;
  
  // Update buttons' current states
  up_state = digitalRead(up_pin);
  down_state = digitalRead(down_pin);
  left_state = digitalRead(left_pin);
  right_state = digitalRead(right_pin);
  select_state = digitalRead(select_pin);
  delete_state = digitalRead(delete_pin);

  // Perform the specified action. To prevent multiple presses, the previous state must be LOW
  if (up_state == HIGH && up_prev_state == LOW && cursor_row != 0) {
    cursor_row--;   // move cursor up
  }
  else if (down_state == HIGH && down_prev_state == LOW && cursor_row != (keyboard_rows-1) ) {
    cursor_row++;   // move cursor down
  }
  else if (left_state == HIGH && left_prev_state == LOW && cursor_col != 0) {
    cursor_col--;   // move cursor left
  }
  else if (right_state == HIGH && right_prev_state == LOW && cursor_col != (keyboard_cols-1) ) {
    cursor_col++;   // move cursor right
  }
  else if (select_state == HIGH && select_prev_state == LOW) {    // perform desired action
    if (keyboard[letter_case][cursor_row][cursor_col] == '^') {   // change letter case
      letter_case = !letter_case;
    }
    else if (keyboard[letter_case][cursor_row][cursor_col] == '<') {  // enter new line
      display_string += "\n";
    }
    else if (cursor_row == keyboard_rows-1) { // append space to display string
      display_string += ' ';
    }
    else {    // append current letter to display string
      display_string += keyboard[letter_case][cursor_row][cursor_col];
    }
  }
  else if (delete_state == HIGH && delete_prev_state == LOW) {  // delete last letter in display string
    display_string.remove(display_string.length()-1);
  }
  
  // Print instructions
  Serial.print("Use ^ to toggle between upper and lower case keyboards.\n");
  Serial.print("Use < to start a new line.\n");
  Serial.print("Your message is shown below.\n");
  Serial.println();
  
  // Print the user's desired sentence
  Serial.print(display_string);
  Serial.print('|');

  // Print some spacing
  Serial.println();
  Serial.println();

  // Print the keyboard
  for (int row = 0; row < keyboard_rows-1; row++) {
    for (int col = 0; col < keyboard_cols; col++) {
      // Check if the cursor is on the current spot
      if ( row == cursor_row && col == cursor_col ) {
        Serial.print('(');    // Print the letter within parenthesis
        Serial.print(keyboard[letter_case][row][col]);
        Serial.print(')');
      }
      else {  // Print the letter normally
        Serial.print(' ');
        Serial.print(keyboard[letter_case][row][col]);
        Serial.print(' ');
      }
    }
    Serial.println();
  }

  // Print the spacebar
  Serial.print("         ");
  if ( cursor_row == keyboard_rows-1 ) {  // Check if the cursor is on the spacebar
    Serial.print('(');    // Print the spacebar within parenthesis
    Serial.print(keyboard[letter_case][keyboard_rows-1]);
    Serial.print(')');
  }
  else {
    Serial.print(' ');    // Print the spacebar normally
    Serial.print(keyboard[letter_case][keyboard_rows-1]);
    Serial.print(' ');    
  }
  Serial.print("         ");
  Serial.println();

  // Refresh rate
  delay(100);

  // Clear the screen
  for (int i = 0; i < 10; i++) {
    Serial.println();
  }

}
