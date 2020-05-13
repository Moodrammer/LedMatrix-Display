#define col0 0
#define col1 1
#define col2 2
#define col3 3
#define col4 4
#define col5 5
#define col6 7
#define col7 8
#define col8 9
#define col9 10

//variables
//time management
unsigned long currentMillis;
unsigned long shiftTime;
//led matrix
int currentRow = 0;
bool columnTestPattern[10][10] = {{1,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,0,0,0,0,0},{0,0,1,0,0,0,0,0,0,0},{0,0,0,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,0},
{0,0,0,0,0,0,1,0,0,0},{0,0,0,0,0,0,0,1,0,0},{0,0,0,0,0,0,0,0,1,0},{0,0,0,0,0,0,0,0,0,1}};

bool out[] = {1,1,1,1,1,1,1,1,1,1};

void setup() {
  for(int i = col0; i <= col5; i++) pinMode(i, OUTPUT);
  for(int i = col6; i <= col9; i++) pinMode(i, OUTPUT);
  for(int i = 16; i <= 19; i++) pinMode(i, OUTPUT);  
  
  //test switching the matrix rows and columns
  columnTesting();
  rowTesting();
}

void loop() {
  //keep track of the number of milliseconds from the start of the program
  currentMillis = millis();

}

//pass a bit string to output on the columns of the matrix 
void columnWrite(bool column[]){
  digitalWrite(col0, column[9]);
  digitalWrite(col1, column[8]);
  digitalWrite(col2, column[7]);
  digitalWrite(col3, column[6]);
  digitalWrite(col4, column[5]);
  digitalWrite(col5, column[4]);
  digitalWrite(col6, column[3]);
  digitalWrite(col7, column[2]);
  digitalWrite(col8, column[1]);
  digitalWrite(col9, column[0]);
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
      columnWrite(out);
      currentRow++;
      delay(200);
  }
}
void columnTesting(){
    int row = 0;
    for(int i = 0; i <= 9; i++){
      columnWrite(columnTestPattern[i]);
      unsigned long start = millis();
      unsigned long current = millis();
      while(current - start <= 200){
        switchRowOn(row);
        delay(1);
        row = (row + 1) % 10;
        current = millis();
      }
    }
}
