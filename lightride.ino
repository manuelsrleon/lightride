#include <FastLED.h>
#include <time.h>

//from FastLED example
#define LED_PIN 5
#define NUM_LEDS 28
#define BRIGHTNESS 32
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define TURNLIGHT_TIME_MS 500

//lightride-specific
#define F_SIZE 14
CRGB f_leds[F_SIZE];

#define B_SIZE 7

CRGB bl_leds[B_SIZE];
CRGB br_leds[B_SIZE];

CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 1000


//input
#include <EasyButton.h>
#define L_PIN 3
#define R_PIN 2
#define F_PIN 4
EasyButton R_button(R_PIN);
EasyButton L_button(L_PIN);
EasyButton F_button(F_PIN);
char mode;

//time
unsigned long _time;
int current_time;

void setup() {
  Serial.begin(9600);
  delay(3000);
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  //compute led strip positions
  fill(f_leds,  F_SIZE, CRGB::White);
  fill(bl_leds, B_SIZE, CRGB::Yellow);
  fill(br_leds, B_SIZE, CRGB::Red);

  R_button.begin();
  R_button.onPressed(onPressed_R);
  L_button.begin();
  L_button.onPressed(onPressed_R);
  F_button.begin();
  F_button.onPressed(onPressed_F);
}

void loop() {
  R_button.read();
  L_button.read();
  F_button.read();
  _time = millis() % 500;
  anim_func_turnlights(bl_leds,B_SIZE, _time, 1);
  //fill(bl_leds,B_SIZE,CRGB::Red);


  switch(mode)
  {
    case 'L':  //Turn left
      fill(f_leds, F_SIZE, CRGB::White);
      anim_func_turnlights(bl_leds,B_SIZE, _time, 1);
      fill(br_leds, B_SIZE, CRGB::Red);    
      break;
      
    //Turn right
    case 'R':
      fill(f_leds, F_SIZE, CRGB::White);
      fill(bl_leds, B_SIZE, CRGB::Red); 
      anim_func_turnlights(br_leds,B_SIZE, _time, 0);
      break;
    //Hazards
    case 'H':
      fill(f_leds, F_SIZE, CRGB::White);
      anim_func_turnlights(br_leds,B_SIZE, _time, 0);
      anim_func_turnlights(bl_leds,B_SIZE, _time, 1);
      break;
    default:
      fill(f_leds, F_SIZE, CRGB::White);
      fill(bl_leds, B_SIZE, CRGB::Red);
      fill(br_leds, B_SIZE, CRGB::Red);
  }
  //anim_func_turnlights(br_leds,B_SIZE, _time, 0);
  update_all();
  
  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND);
}

void fill(CRGB leds[], int n, CRGB color){
  for(int i = 0; i < n ;i++){
    leds[i] = color;
  }
}

void update_all(){
  
  int f_end = F_SIZE;
  int bl_end = F_SIZE+B_SIZE;
  int br_end = F_SIZE+B_SIZE*2;
  
  for(int i = 0; i < f_end; i++){
    leds[i] = f_leds[i];
  }
  for(int i = f_end; i < bl_end ;i++){
    leds[i] = bl_leds[i-f_end];
  }
  for(int i = bl_end; i < br_end ;i++){
    leds[i] = br_leds[i-bl_end];
  }
}

void printLEDState(){
   for(int i = 0; i < sizeof(leds);i++){
    Serial.println(leds[i]);
   }
}

int anim_func_turnlights(CRGB this_leds[], int n, float current_time, int reverse){
  
  int current_leds = 0;
  current_leds = (current_time/TURNLIGHT_TIME_MS)*n;
  fill(this_leds, n, CRGB::Black);

  if(reverse){
    for(int i= n; i >= B_SIZE-current_leds;i--){
      this_leds[i-1] = CRGB::Yellow;
    }  
  }else{
    for(int i=0; i <= current_leds;i++){
      this_leds[i] = CRGB::Yellow;
    }  
  }
  return 0;
}

//INPUTS
void onPressed_R(){
  if(mode == 'R'){
    mode = 'D';
  }else{
    mode = 'R';
  }
} 
void onPressed_L(){
  if(mode == 'L'){
    mode = 'D';
  }else{
    mode = 'L';
  }
} 
void onPressed_F(){
  
} 
