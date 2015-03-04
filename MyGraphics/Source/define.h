///////////glass door////////////
bool open = false;
float door1Pos = 1023;
float door2Pos = 1365;

//////////gate 1////////////////
float gate1RightRotate = 0;
float gate1LeftRotate = 0;

//////////gate 2////////////////
float gate2RightRotate = 180;
float gate2LeftRotate = -180;

/////////UI bools////////
bool pickUpText = false;
bool putBackText = false;

#define NorthSouthWall Vector3(2252,274,25)
#define EastWestWall Vector3(25,274,1826)

#define leftShelfBounds Vector3(117.5,172,740.5)
#define middleLeftShelfBounds Vector3(235,172,246.5)
#define middleRightShelfBounds Vector3(235,172,246.6)
#define backShelfBounds Vector3(740.5,172,117.5)
#define loneShelfBounds Vector3(247,172,117.5)
#define doorBounds Vector3(400,222.5,225)
#define playerBounds Vector3(50,50,50)
#define freezerBounds Vector3(101,163,552.5)
#define chillerBounds Vector3(117.5,124,608)
#define gateBoundsRight Vector3(180,150,100)
#define gateBoundsLeft Vector3(170,150,100)
#define cashierBounds Vector3(167,150,338)
#define middleCashierBounds Vector3(337,150,338)

#define doorTranslation Vector3(2230,-53,1190)
#define leftShelfTranslate Vector3(-2085,-103.3,-1085)
#define middleLeftShelfTranslate Vector3(-1210,-103.3,-820)
#define middleRightShelfTranslate Vector3(-310,-103.3,-820)
#define backShelfTranslate Vector3(-498,-103.3,-1709)
#define loneShelfTranslate Vector3(600,-103.3,-690)
#define MarketWallNorthSouthTranslate Vector3(0,0,1850)
#define MarketWallEastWestTranslate Vector3(2226,0,0)
#define FreezerTranslate Vector3(1374.4,-110,-1271)
#define ChillerTranslate Vector3(2083,-150.4,-1217.5)
#define gateBounds1Translation Vector3(820, -125, 302)
#define gateBounds2Translation Vector3(-1490, -125, 238)
#define rightCashierTranslate Vector3(463.8, -125, 385.3)
#define middleCashierTranslate Vector3(-367.5, -125, 385.3)
#define leftCashierTranslate Vector3(-1148.2, -125, 385.3)