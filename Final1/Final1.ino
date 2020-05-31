//Notes about my solution
//-----------------------
// - I assumed that 1212 is a reserved word for changing the password & cannot be a user's password
// - After typing 1212 , I stop delibrately checking on the password sequence and the next four keys pressed will be assigned as the new password
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define col0 0
#define col1 1
#define col2 2
#define col3 3
#define col4 4
#define col5 5
#define col6 6
#define col7 7
#define col8 8
#define col9 9
#define clockPin 15
#define keypadPin 14
#define speakerPin 10

//HighTime for different tones in Microseconds
#define keyZeroToneDuration 200
#define keyOneToneDuration 400
#define keyTwoToneDuration 600
#define keyThreeToneDuration 800
#define keyFourToneDuration 1000
#define keyFiveToneDuration 1400
#define keySixToneDuration 1800
#define keySevenToneDuration 2100
#define keyEightToneDuration 2500
#define keyNineToneDuration 3300

#define emo0ToneDuration 3700
#define emo1ToneDuration 2800
#define emo2ToneDuration 2300
#define emo3ToneDuration 2000
#define emo4ToneDuration 1600
#define emo5ToneDuration 1200
#define emo6ToneDuration 900
#define emo7ToneDuration 700
#define emo8ToneDuration 500
#define emo9ToneDuration 300
#define passwordSequenceToneDuration 4000 

//variables
//time management
unsigned long currentMillis;
unsigned long shiftTime = 0;
unsigned long drawTime = 0;
unsigned long melodyTime = 0;
//led matrix
int currentRow = 0;
int rowTestPattern = B11 * 256 + B11111111;
//Drawing
int currentDrawing[10] = {0,0,0,0,0,0,0,0,0,0};
int * currentPattern;
int patternRowData = -1;
int changingRowIndex = -1;
int bitPositionValue = 512;
boolean drawing = false;
//patterns
//numbers
int zero[10] = {120, 252, 204, 204, 204, 204, 204, 204, 252, 120};
int one[10] = {248, 248, 56, 56, 56, 56, 56, 56, 252, 252};
int two[10] = {248, 508, 396, 12, 24, 48, 96, 192, 508, 508};
int three[10] = {252, 254, 14, 12, 248, 248, 12, 14, 254, 252};
int four[10] = {28, 60, 108, 204, 140, 510, 510, 12, 12, 12};
int five[10] = {252, 252, 192, 216, 252, 28, 12, 28, 252, 248};
int six[10] = {120, 252, 192, 192, 248, 252, 204, 204, 252, 120};
int seven[10] = {252, 204, 12, 28, 24, 48, 48, 96, 224, 192};
int eight[10] = {120, 252, 204, 204, 124, 248, 204, 204, 252, 120};
int nine[10] = {120, 252, 204, 204, 252, 124, 12, 204, 252, 120};
//emotions
int emo0[10] = {252, 258, 513, 645, 585, 513, 561, 585, 258, 252};
int emo1[10] = {252, 510, 1023, 819, 955, 1023, 903, 891, 510, 252};
int emo2[10] = {252, 258, 513, 585, 585, 513, 561, 633, 258, 252};
int emo3[10] = {252, 258, 513, 585, 585, 513, 765, 633, 258, 252};
int emo4[10] = {252, 258, 585, 585, 513, 561, 633, 561, 258, 252};
int emo5[10] = {252, 258, 633, 645, 513, 585, 585, 513, 258, 252};
int emo6[10] = {252, 510, 1015, 823, 1023, 1023, 891, 903, 510, 252}; 
int emo7[10] = {252, 510, 1023, 513, 561, 819, 1023, 891, 390, 252}; 
int emo8[10] = {48, 48, 112, 252, 1022, 1022, 1022, 1022, 0, 0};
int emo9[10] = {252, 510, 951, 951, 1023, 1023, 1023, 1023, 510, 252};
//keypad variables
boolean isKeyRead = false;
char currentkey = ' ';
//password Management
String numToEmoPass = "1234";
String emoToNumPass = "4321";
boolean isNumericMode = true;
//reset password variables
boolean ChPassFirstCheck = true;
boolean ChPassSecondCheck = false;
boolean ChPassThirdCheck = false;
boolean ChPassFourthCheck = false;
boolean resetPassword = false;
int newPasswordIndex = 0;
String newPassword = "";
//numeric to emotions and the opposite
boolean nToeFirstCheck = true;
boolean nToeSecondCheck = false;
boolean nToeThirdCheck = false;
boolean nToeFourthCheck = false;

boolean eTonFirstCheck = true;
boolean eTonSecondCheck = false;
boolean eTonThirdCheck = false;
boolean eTonFourthCheck = false;


void setup() {
  for(int i = col0; i <= col9; i++) pinMode(i, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  for(int i = 16; i <= 19; i++) pinMode(i, OUTPUT);  
  
  //test switching the matrix rows and columns
  columnTesting();
  rowTesting();
  //reset the currentRow to 0
  currentRow = 0;
}

void loop() {
  currentMillis = millis();
  //Read keypad
  if(analogRead(keypadPin)){
    if(!isKeyRead){
      setKeyPressed();
      if(!resetPassword){
        checkPassword(currentkey);
      }
      else{
        setNewPassword(currentkey);
      }
    }
  }
  else{
    isKeyRead = false;
  }
  
  //keep drawing the current pattern
  if(currentMillis - shiftTime >= 1){
     drawCurrent();
     shiftTime = currentMillis;
  }

  //keep track of the number of milliseconds from the start of the program
  
  if(drawing){
    //if a pattern is currently being drawn
    //each millisecond light one Led of the pattern
    if(currentMillis - drawTime >= 100){
      drawTime = currentMillis;
      setCurrentDrawing();
    }
  }
}

//pass a bit string to output on the columns of the matrix 
void columnWrite(int column){
  digitalWrite(col0, column & 1);
  digitalWrite(col1, column & 2);
  digitalWrite(col2, column & 4);
  digitalWrite(col3, column & 8);
  digitalWrite(col4, column & 16);
  digitalWrite(col5, column & 32);
  digitalWrite(col6, column & 64);
  digitalWrite(col7, column & 128);
  digitalWrite(col8, (column / 256) & 1);
  digitalWrite(col9, (column/ 256) & 2);
}

//switches the current row on by making the common cathodes of the Leds connected to 0 
void switchRowOn(int row){
  digitalWrite(16, row & 1);
  digitalWrite(17, row & 2);
  digitalWrite(18, row & 4);
  digitalWrite(19, row & 8);
}

//this function handles row selftesting
void rowTesting(){
    for(int i = 0; i <= 9; i++){
      switchRowOn(currentRow);
      columnWrite(rowTestPattern);
      pulseRow();
      delay(200);
      switchRowOn(currentRow);
      columnWrite(0);
      pulseRow();
      currentRow++;
  }
  //clear the led matrix
  columnWrite(0);
}
void columnTesting(){
    int column = 512;
    while(column != 0){
       columnWrite(column);
       for(int i = 0; i <= 9; i++){
        switchRowOn(i);
        pulseRow();
       }
       delay(200);
       column /= 2;
   }
   clearMatrix();
}

//Draws the current array 
void drawCurrent(){
  //output the current row data in the columns
  columnWrite(currentDrawing[currentRow]);
  switchRowOn(currentRow);
  pulseRow();
  currentRow = (currentRow + 1) % 10;
}

//called to change the current drawing according to the current pattern which is determined by the key pressed
//Function depends that the current pattern being drawn is already set
void setCurrentDrawing(){
  if(changingRowIndex == -1){
    //set the patternRowData to the first row
    changingRowIndex ++;
    patternRowData = currentPattern[changingRowIndex];
    bitPositionValue = 512;
  }

  while(patternRowData < bitPositionValue) bitPositionValue /= 2;
  currentDrawing[changingRowIndex] += bitPositionValue;
  patternRowData -= bitPositionValue;
  bitPositionValue /= 2;
  
  //if there is no more ones in the current row of the current pattern go to the next row
  if(patternRowData == 0){
   changingRowIndex ++;
   if(changingRowIndex > 9){
    //This means that we finished drawing the 10 columns of the currentPattern
    //Reset all the variables
    drawing = false;
    changingRowIndex = -1;
    bitPositionValue = 512; 
   }
   else{
    //Get the next Row in the current Pattern
    patternRowData = currentPattern[changingRowIndex];
    bitPositionValue = 512; 
   }
  }
}

void pulseRow(){
  digitalWrite(clockPin, 1);
  digitalWrite(clockPin, 0);
}


void clearMatrix(){
  columnWrite(0);
  //pulse all rows
  for(int i = 0; i <= 9; i++){
    switchRowOn(i);
    pulseRow();
  }
}

void setKeyPressed(){
  int key = analogRead(keypadPin);
  switch(key){
      case(852):
      if(currentkey != '0'){
        (isNumericMode)?setPattern(zero):setPattern(emo0);
        currentkey = '0';
        drawing = true;
      }
      (isNumericMode)? playMelody(keyZeroToneDuration, 10): playMelody(emo0ToneDuration, 10);
      break;
    case(786):
      if(currentkey != '1'){
        (isNumericMode)?setPattern(one):setPattern(emo1);
        currentkey = '1';
        drawing = true;
      }
      (isNumericMode)? playMelody(keyOneToneDuration, 10): playMelody(emo1ToneDuration, 10);
      break;
    case(730):
      if(currentkey != '2'){
        (isNumericMode)?setPattern(two):setPattern(emo2);
        currentkey = '2';
        drawing = true;
      }
      (isNumericMode)?playMelody(keyTwoToneDuration, 10): playMelody(emo2ToneDuration, 10);
      break;
     case(681):
      if(currentkey != '3'){
        (isNumericMode)?setPattern(three):setPattern(emo3);
        currentkey = '3';
        drawing = true;
      }
      (isNumericMode)?playMelody(keyThreeToneDuration, 10): playMelody(emo3ToneDuration, 10);
      break;
    case(639):
      if(currentkey != '4'){
        (isNumericMode)?setPattern(four):setPattern(emo4);
        currentkey = '4';
        drawing = true;
      }
      (isNumericMode)?playMelody(keyFourToneDuration, 10): playMelody(emo4ToneDuration, 10);
      break;
    case(601):
      if(currentkey != '5'){
        (isNumericMode)?setPattern(five):setPattern(emo5);
        currentkey = '5';
        drawing = true;
      }
      (isNumericMode)?playMelody(keyFiveToneDuration, 10): playMelody(emo5ToneDuration, 10);
      break;
    case(568):
      if(currentkey != '6'){
        (isNumericMode)?setPattern(six):setPattern(emo6);
        currentkey = '6';
        drawing = true;
      }
      (isNumericMode)?playMelody(keySixToneDuration, 10): playMelody(emo6ToneDuration, 10);
      break;
    case(538):
      if(currentkey != '7'){
        (isNumericMode)?setPattern(seven):setPattern(emo7);
        currentkey = '7';
        drawing = true;
      }
      (isNumericMode)?playMelody(keySevenToneDuration, 10): playMelody(emo7ToneDuration, 10);
      break;
    case(511):
      if(currentkey != '8'){
        (isNumericMode)?setPattern(eight):setPattern(emo8);
        currentkey = '8';
        drawing = true;
      }
      (isNumericMode)?playMelody(keyEightToneDuration, 10): playMelody(emo8ToneDuration, 10);
      break;
    case(486):
      if(currentkey != '9'){
        (isNumericMode)?setPattern(nine):setPattern(emo9);
        currentkey = '9';
        drawing = true;
      }
      (isNumericMode)?playMelody(keyNineToneDuration, 10): playMelody(emo9ToneDuration, 10);
  }

  isKeyRead = true;
}

void setPattern(int pattern[]){
  //before drawing clear the matrix
  //reset current Drawing array
  for(int i = 0; i <= 9; i++) currentDrawing[i] = 0;
  //clear the ledMatrix
  clearMatrix();
  currentPattern = pattern;
  changingRowIndex = -1;
  currentRow = 0;
}
//Password Management
String reverseString(String str){
  String rev = String(str[3]) + String(str[2]) + String(str[1]) + String(str[0]);
  return rev;
}

void checkPassword(char key){
  checkChangePasswordSequence(key);
  //if the reset sequence was found don't check the password sequence for this character
  if(!resetPassword){
    if(isNumericMode){
      checknToePasswordSequence(key);
    }
    else{
      checkeTonPasswordSequence(key);
    }    
  }
}

//Function to check the reset password sequence 1212
void checkChangePasswordSequence(char key){
  if(ChPassFirstCheck){
    if(key == '1'){
      ChPassFirstCheck = false;
      ChPassSecondCheck = true;
    }
  }
  else if(ChPassSecondCheck){
    if (key == '2'){ 
      ChPassThirdCheck = true;
      ChPassSecondCheck = false;
      } 
      else{
        (key == '1')? ChPassSecondCheck = true: ChPassFirstCheck = true;
      }
    }
  else if(ChPassThirdCheck){
    (key == '1')? ChPassFourthCheck = true: ChPassFirstCheck = true;
    ChPassThirdCheck = false;
  }
  else if(ChPassFourthCheck){
    if(key == '2'){
       //Store the next four characters as the new password
       resetPassword = true;
       ChPassFirstCheck = true;
       nToeFirstCheck = true;
       eTonFirstCheck = true;
    }
    else{
      (key == '1')? ChPassSecondCheck = true: ChPassFirstCheck = true;
    }

  }
}

//Function to check the numeric to emotions mode password
void checknToePasswordSequence(char key){
  if(nToeFirstCheck){
    if(key == numToEmoPass[0]){
      nToeFirstCheck = false;
      nToeSecondCheck = true;
    }
  }
  else if(nToeSecondCheck){
    if (key == numToEmoPass[1]){
      nToeThirdCheck = true;
      nToeSecondCheck = false; 
    }
    else{
      (key == numToEmoPass[0])? nToeSecondCheck = true :nToeFirstCheck = true;
    }
    }
  else if(nToeThirdCheck){
    if (key == numToEmoPass[2]){
      nToeFourthCheck = true;
      nToeThirdCheck = false; 
    }
    else{
      (key == numToEmoPass[0])? nToeSecondCheck = true: nToeFirstCheck = true;
    }
    
  }
  else if(nToeFourthCheck){
    if(key == numToEmoPass[3]){
         //Go to emotions mode
      isNumericMode = false;
      clearMatrix();
      playMelody(passwordSequenceToneDuration, 1000);
      //reset the current drawing array
      for(int i = 0; i <= 9; i++) currentDrawing[i] = 0;
      drawing = false;
      currentkey = ' ';
      ChPassFirstCheck = true;
      nToeFirstCheck = true;
      eTonFirstCheck = true;
    }
    else{
      (key == numToEmoPass[0])? nToeSecondCheck = true: nToeFirstCheck = true;
    }

  }
}

//Function to check the emotions to numeric mode password
void checkeTonPasswordSequence(char key){
  if(eTonFirstCheck){
    if(key == emoToNumPass[0]){
      eTonFirstCheck = false;
      eTonSecondCheck = true;
    }
  }
  else if(eTonSecondCheck){
    if(key == emoToNumPass[1]){
      eTonThirdCheck = true;
      eTonSecondCheck = false; 
    }
    else{
      (key == emoToNumPass[0])? eTonSecondCheck = true: eTonFirstCheck = true;
    }
  }
  else if(eTonThirdCheck){
    if (key == emoToNumPass[2]){
      eTonFourthCheck = true;
      eTonThirdCheck = false; 
    }
    else{
      (key == emoToNumPass[0])? eTonSecondCheck = true: eTonFirstCheck = true;
    }
  }
  else if(eTonFourthCheck){
    if(key == emoToNumPass[3]){
       //Go to numeric mode
      isNumericMode = true;
      clearMatrix();
      playMelody(passwordSequenceToneDuration, 1000);
      //reset the current drawing array
      for(int i = 0; i <= 9; i++) currentDrawing[i] = 0;
      drawing = false;
      currentkey = ' ';

      ChPassFirstCheck = true;
      nToeFirstCheck = true;
      eTonFirstCheck = true;
    }
    else{
      (key == emoToNumPass[0])? eTonSecondCheck = true: eTonFirstCheck = true;
    }
  }
}

void setNewPassword(char key){
  newPassword += String(key);
  if(newPasswordIndex < 3){
    newPasswordIndex++;
  }
  else{
    //set the new passwords for numeric and emotions modes
    numToEmoPass = newPassword;
    emoToNumPass = reverseString(newPassword);
    //reset the reset password variables
    resetPassword = false;
    newPasswordIndex = 0;
    newPassword = "";
  }
}

void playMelody(int duration, int toneDuration){
  melodyTime = currentMillis;
  while(melodyTime - currentMillis <= toneDuration){
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(duration);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(duration);
    melodyTime = millis();
  }
}
