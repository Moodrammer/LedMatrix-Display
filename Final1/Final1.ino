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

//variables
//time management
unsigned long currentMillis;
unsigned long shiftTime = 0;
unsigned long drawTime = 0;
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
boolean endOfPattern;
//patterns
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
//keypad variables
boolean isKeyRead = false;
char currentkey = ' ';



void setup() {
  for(int i = col0; i <= col9; i++) pinMode(i, OUTPUT);
  pinMode(clockPin, OUTPUT);
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
      getKeyPressed();
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
      if(endOfPattern){
        drawing = false;
        endOfPattern = false;
      }
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
    endOfPattern = true;
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

void getKeyPressed(){
  int key = analogRead(keypadPin);
  switch(key){
      case(852):
      if(currentkey != '0'){
        setPattern(zero);
        currentkey = '0';
        drawing = true;
      }
      break;
    case(786):
      if(currentkey != '1'){
        setPattern(one);
        currentkey = '1';
        drawing = true;
      }
      break;
    case(730):
      if(currentkey != '2'){
        setPattern(two);
        currentkey = '2';
        drawing = true;
      }
      break;
     case(681):
      if(currentkey != '3'){
        setPattern(three);
        currentkey = '3';
        drawing = true;
      }
      break;
    case(639):
      if(currentkey != '4'){
        setPattern(four);
        currentkey = '4';
        drawing = true;
      }
      break;
    case(601):
      if(currentkey != '5'){
        setPattern(five);
        currentkey = '5';
        drawing = true;
      }
      break;
    case(568):
      if(currentkey != '6'){
        setPattern(six);
        currentkey = '6';
        drawing = true;
      }
      break;
    case(538):
      if(currentkey != '7'){
        setPattern(seven);
        currentkey = '7';
        drawing = true;
      }
      break;
    case(511):
      if(currentkey != '8'){
        setPattern(eight);
        currentkey = '8';
        drawing = true;
      }
      break;
    case(486):
      if(currentkey != '9'){
        setPattern(nine);
        currentkey = '9';
        drawing = true;
      }
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
