const int PIN_LED_YELLOW = 2,
          TONE_YELLOW = 2640,

          PIN_LED_GREEN = 4,
          TONE_GREEN = 2970,

          PIN_LED_RED = 7,
          TONE_RED = 3300;

const int PIN_BUZZER = 9;
const int CHOOSE_LIST[] = {PIN_LED_YELLOW, PIN_LED_GREEN, PIN_LED_RED};
const int LED_TONES[] = {TONE_YELLOW, TONE_GREEN, TONE_RED};

const int HIT = 1, MISSED = 0;

int sequence[100];
int counter = 1;

void blink(int color, int on_time=800, int off_time=500) {
  int test = 0, i;
  int freq_buzzer = 1000;

  for(i = 0; i < 3; i++){
    if(color == CHOOSE_LIST[i]) {
      freq_buzzer = LED_TONES[i];
      test = 1;
    }
  }
  if(test == 1){
    digitalWrite(color, HIGH);
    tone(PIN_BUZZER, freq_buzzer, on_time);
    delay(on_time);
    digitalWrite(color, LOW);
    delay(off_time);
  }
}

void correct_answer(){
  tone(PIN_BUZZER, 400, 200);
  delay(400);
  tone(PIN_BUZZER, 600, 300);
  delay(1000);
}

void wrong_answer(){
  tone(PIN_BUZZER, 300, 600);
  tone(PIN_BUZZER, 100, 300);
  tone(PIN_BUZZER, 200, 600);
  delay(1400);
}

int play(int length) {
  int pins_state[] = {LOW, LOW, LOW},
      sequence2check[length],
      keypress_count = 0;

  int answer = HIT;

  setup_read();
  while(keypress_count < length) {
    delay(100);
    for(int i = 0; i < 3; i++) {
      pins_state[i] = digitalRead(CHOOSE_LIST[i]);
      if(pins_state[i] == HIGH){
        while(pins_state[i] == HIGH){
          pins_state[i] = digitalRead(CHOOSE_LIST[i]);
          tone(PIN_BUZZER, LED_TONES[i], 100);
        }
        sequence2check[keypress_count] = CHOOSE_LIST[i];
        keypress_count += 1;
        break;
      }
    }
  }
  
  for(int i = 0; i < length; i++) {
    if(sequence[i] != sequence2check[i]){
      answer = MISSED;
      break;
    }
  }
  
  setup();
  delay(1000);
  return answer;
}

void setup() { 
  Serial.begin(9600);
  pinMode(PIN_LED_YELLOW, OUTPUT);     
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
}

void setup_read() {
  pinMode(PIN_LED_YELLOW, INPUT);
  pinMode(PIN_LED_RED, INPUT);
  pinMode(PIN_LED_GREEN, INPUT);
}

void loop() {
  int index, answer;
  
  randomSeed(analogRead(0));
  //Adds a random blink at the end
  int choice = random(3);

  sequence[counter - 1] = CHOOSE_LIST[choice];
  
  for(index = 0; index < counter; index++)
    blink(sequence[index]);
    
  answer = play(counter);
  if(answer == HIT){
    correct_answer();
  }
  else{
    wrong_answer();
    counter = 0;
    for(int i = 0; i < 100; i++){
      if(sequence[i] == 0)
        break;
      sequence[i] = 0;
    }
  }
  
  counter += 1;
  if(counter >= 100)
    counter = 1;
}
