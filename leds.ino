/* Pin setup:
 *
 * col1   = 13, 12, 11, 10
 * col2   = 9,  8,  7,  6
 * col3   = 5,  4,  3,  2
 * col4   = 1,  0,  A5, A4
 * layers = A3, A2, A1, A0
 */

#define ON   1
#define OFF  0

#define TRUE  1
#define FALSE 0

#define NUMBER_OF_PATTERNS 9

/* define pins which represent each floor/layer */
int layer[4] =   {   A3, A2, A1, A0   };

/* define pins which represent each column */
int base[4][4] = {  {13, 12, 11, 10},
                    {9,  8,   7,  6},
                    {5,  4,   3,  2},
                    {1,  0,  A5, A4}  };

/********************** SETUP **********************/
void setup() {
  randomSeed(analogRead(0));
  
  //set all lights as initially outputs on off mode
  for(int i = 0; i<4; i++){
    for(int j = 0; j<4; j++){
      pinMode(base[i][j], OUTPUT);
    }
  }

  for(int i=0;i<4;i++){
    pinMode(layer[i], OUTPUT);
  }

  all(OFF);
}

/********************** LOOP **********************/
void loop() {
  int randomNumber = (int)random(NUMBER_OF_PATTERNS);

  switch(randomNumber){
    case 0: upDownUp((int)random(3,6));
            break;
    case 1:
    case 2: leftRight();
            break;
    case 3: followMe(1);
            break;
    case 4: followMe(0);
            break;
    case 5:
    case 6: tickTok((int)random(5,7));
            break;
    case 7: cycle((int)random(2,5), 1);
            break;
    case 8: cycle((int)random(2,5), 0);
            break;
    case 9: upAndAround();
            break;
    
  }
  
}

/********************** Utilities **********************/
int toBeOrNotToBe(){
  return random(0,1) > 0.5;
}

/* Switches on/off (according to x) all pins with given X axis */
void switchCol(int c, int x){
  for(int i=0;i<4;i++){
    switchBase(c, i, x);
  }
}

/* Switches on/off (according to x) all pins with given Z axis */
void switchRow(int r, int x){
  for(int i=0;i<4;i++){
    switchBase(i, r, x);
  }
}

/* Switches on/off (according to x) all pins with given Y axis */
void switchLayer(int i, int x){
  if(i < 0 || i >= 4){
    return;
  }
  
  digitalWrite(layer[i], x);
}

/* Switches all floors on/off */
void allLayers(int x){
  for(int i = 0; i<4; i++){
    switchLayer(i, x);
  }
}

/* Switches all columns on/off */
void allBases(int x){
  for(int i = 0; i<4; i++){
    for(int j=0; j<4; j++){
      switchBase(i, j, x);
    }
  }
}

/* Switches a specific column on/off */
void switchBase(int i, int j, int x){
  if(i < 0 || i >= 4 || j < 0 || j >= 4){
    return;
  }
  
  digitalWrite(base[i][j], x);
}

/* Switches every single led on/off */
void all(int x){
  allLayers(x);
  allBases(x);
}

/********************** Patterns **********************/
//1
void upAndAround(){
  int d = 1000;
  all(OFF);
  switchBase(1,2,ON);

  for(int i=0;i<4;i++){
    switchLayer(i-1, OFF);
    switchLayer(i, ON);
    delay(d);
  }

  allBases(ON);
  delay(d);
  all(OFF);

  for(int i=0;i<4;i++){
    switchBase(0,i,ON);
    switchBase(3,i,ON);
    switchBase(i,3,ON);
    switchBase(0,i,ON);
  }

  for(int i=3;i>=0;i--){
    switchLayer(i, ON);
    delay(d);
  }
}

//2
void cycle(int aroundTimes, int trace){

  all(OFF);
  for(int i=3;i>=0;i--){
    if(!trace){
      switchLayer(i+1, OFF);
    }
    switchLayer(i, ON);
    for(int t=0;t<aroundTimes;t++){
      aroundOutside();
    }
  }
}

//3
void aroundOutside(){
  //TODO make more generic and reuse methods

  int d = 120;

  for(int i=0;i<4;i++){
    allBases(OFF);
    switchBase(0,i,ON);
    switchBase(i,3,ON);
    switchBase(3,3-i,ON);
    switchBase(3-i,0,ON);
    delay(d);
  }
}

//4
void followMe(int trace){
  //TODO make more generic and reuse methods
  all(OFF);
  switchLayer(3, ON);
  
  forthBack(trace);

  switchLayer(3, OFF);
  switchLayer(2, ON);

  backForth(trace);

  switchLayer(2, OFF);
  switchLayer(1, ON);

  forthBack(trace);

  switchLayer(1, OFF);
  switchLayer(0, ON);

  backForth(trace);

}

//5
void forthBack(int trace){
  //TODO make more generic and reuse methods
  int d = 80;

  allBases(OFF);

  for(int i=0;i<4;i++){
    if(!trace){
      switchBase(0, i-1, OFF);
    }
    
    switchBase(0, i, ON);
    delay(d);
  }

  if(!trace){
    allBases(OFF);
  }

  for(int i=3;i>=0;i--){
    if(!trace){
      switchBase(1, i+1, OFF);
    }
    switchBase(1, i, ON);
    delay(d);
  }

  if(!trace){
    allBases(OFF);
  }

  for(int i=0;i<4;i++){
    if(!trace){
      switchBase(2, i-1, OFF);
    }
    switchBase(2, i, ON);
    delay(d);
  }

  if(!trace){
    allBases(OFF);
  }

  for(int i=3;i>=0;i--){
    if(!trace){
      switchBase(3, i+1, OFF);
    }
    switchBase(3, i, ON);
    delay(d);
  }
}

//6
void backForth(int trace){
  //TODO make more generic and reuse methods
  int d = 80;
  allBases(OFF);
  
  for(int i=0;i<4;i++){
    if(!trace){
      switchBase(3, i-1, OFF);
    }
    switchBase(3, i, ON);
    delay(d);
  }

  if(!trace){
    allBases(OFF);
  }

  for(int i=3;i>=0;i--){
    if(!trace){
      switchBase(2, i+1, OFF);
    }
    switchBase(2, i, ON);
    delay(d);
  }

  if(!trace){
    allBases(OFF);
  }
  
  for(int i=0;i<4;i++){
    if(!trace){
      switchBase(1, i-1, OFF);
    }
    switchBase(1, i, ON);
    delay(d);
  }

  if(!trace){
    allBases(OFF);
  }

  for(int i=3;i>=0;i--){
    if(!trace){
      switchBase(0, i+1, OFF);
    }
    switchBase(0, i, ON);
    delay(d);
  }
}

//7
void tickTok(int times){
  //TODO make more generic and reuse methods

  int d = 200;
  
  all(OFF);
  allLayers(ON);

  for(int t=0;t<times;t++){
    switchBase(0, 3, ON);
    switchBase(1, 2, ON);
    switchBase(2, 1, ON);
    switchBase(3, 0, ON);
  
    delay(d);
    allBases(OFF);
  
    switchBase(0, 2, ON);
    switchBase(1, 2, ON);
    switchBase(2, 1, ON);
    switchBase(3, 1, ON);
  
    delay(d);
    allBases(OFF);
    
    switchBase(0, 1, ON);
    switchBase(1, 1, ON);
    switchBase(2, 2, ON);
    switchBase(3, 2, ON);
    
    delay(d);
    allBases(OFF);
    
    switchBase(0, 0, ON);
    switchBase(1, 1, ON);
    switchBase(2, 2, ON);
    switchBase(3, 3, ON);  
    delay(d);
    
    allBases(OFF);
    
    switchBase(1, 0, ON);
    switchBase(1, 1, ON);
    switchBase(2, 2, ON);
    switchBase(2, 3, ON);  
    delay(d);
  
    allBases(OFF);
    
    switchBase(1, 2, ON);
    switchBase(1, 3, ON);
    switchBase(2, 0, ON);
    switchBase(2, 1, ON);  
    delay(d);
  }
  
}

//8
void leftRight(){
  //TODO make more generic and reuse methods

  all(OFF);
  allLayers(ON);

  for(int i=3;i>=0;i--){
    switchRow(i, ON);
    delay(150);
    switchRow(i, OFF);
  }

  for(int i=3;i>=0;i--){
    switchCol(i, ON);
    delay(150);
    switchCol(i, OFF);
  }

  for(int i=0;i<4;i++){
    switchRow(i, ON);
    delay(150);
    switchRow(i, OFF);
  }

  for(int i=0;i<4;i++){
    switchCol(i, ON);
    delay(150);
    switchCol(i, OFF);
  }
}

//9
void upDownUp(int times){
  //TODO make more generic and reuse methods
  int d = 150;

  all(OFF);
  allBases(ON);

  for(int t=0;t<times;t++){
    for(int i=3;i>0;i--){   
      switchLayer(i, ON);
      delay(d);
      switchLayer(i, OFF);
    }
    for(int i=0;i<3;i++){   
      switchLayer(i, ON);
      delay(d);
      switchLayer(i, OFF);
    }
  }
}


