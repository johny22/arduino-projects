const int yellow = 2,
          green = 4,
          red = 7;

const int choose_list[] = {yellow, green, red};

const int HIT = 1, MISSED = 0;

int sequence[100];
int counter = 1;

void blink(int color, int times=1, int on_time=800, int off_time=400) {
  int test = 0, i;
  for(i = 0; i < 3; i++){
    if(color == choose_list[i])
      test = 1;
  }
  for(int i = 0; i<times; i++){
    if(test == 1){
      digitalWrite(color, HIGH);
      delay(on_time);
      digitalWrite(color, LOW);
      delay(off_time);
    }
  }
}

void correct_answer(){
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH);
  delay(500);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  delay(500);
}

void wrong_answer(){
  digitalWrite(red, HIGH);
  digitalWrite(yellow, HIGH);
  delay(500);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  delay(500);
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
      pins_state[i] = digitalRead(choose_list[i]);
      if(pins_state[i] == HIGH){
        while(pins_state[i] == HIGH){
          pins_state[i] = digitalRead(choose_list[i]);
        }
        sequence2check[keypress_count] = choose_list[i];
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
  pinMode(yellow, OUTPUT);     
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
}

void setup_read() {
  pinMode(yellow, INPUT);
  pinMode(red, INPUT);
  pinMode(green, INPUT);
}

void loop() {
  int index, answer;
  
  randomSeed(analogRead(0));
  //Adds a random blink at the end
  int choice = random(3);

  sequence[counter - 1] = choose_list[choice];
  
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
