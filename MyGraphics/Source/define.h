///////////glass door////////////
bool open = false;
float door1Pos = 1023;
float door2Pos = 1365;

//////////gate 1////////////////
float gate1RightRotate = 0;
float gate1LeftRotate = 0;

#define NorthSouthWall Vector3(2252,274,25)
#define EastWestWall Vector3(25,274,1826)

#define leftShelfBounds Vector3(117.5,172,740.5)
#define middleLeftShelfBounds Vector3(235,172,246.5)
#define middleRightShelfBounds Vector3(235,172,246.6)
#define backShelfBounds Vector3(740.5,172,117.5)
#define loneShelfBounds Vector3(247,172,117.5)
#define doorBounds Vector3(400,222.5,225)
#define playerBounds Vector3(50,50,50);
#define freezerBounds Vector3(101,163,552.5);
#define chillerBounds Vector3(117.5,124,608);
#define gateBounds1 Vector3(100,150,50);

#define doorTranslation Vector3(2230,-53,1190);
#define leftShelfTranslate Vector3(-2085,-103.3,-1085);
#define middleLeftShelfTranslate Vector3(-1210,-103.3,-820);
#define middleRightShelfTranslate Vector3(-310,-103.3,-820);
#define backShelfTranslate Vector3(-498,-103.3,-1709);
#define loneShelfTranslate Vector3(600,-103.3,-690);
#define MarketWallNorthSouthTranslate Vector3(0,0,1850);
#define MarketWallEastWestTranslate Vector3(2226,0,0);
#define FreezerTranslate Vector3(1374.4,-110,-1271);
#define ChillerTranslate Vector3(2083,-150.4,-1217.5);
#define gateBounds1Translation Vector3(820, -125, 265);

//AI configurables
#define	passerbySpeed		15
#define	customerSpeed		10
#define	vehicleSpeed		20
#define	vehicleReverseSpeed	10
#define	vehicleOwnerSpeed	10
#define	passerbySpawnRate	1111 // 1 / ___ chance
#define	customerSpawnRate	2222 // of spawning
#define vehiclesSpawnRate	2222 // every update
#define	customerLimit		10
#define passerbyLimit		20
#define cashierTurnDelay	2 // seconds
#define itemRotateSpeed		10


//AI fixed values. DO NOT CHANGE
#define	vehicleLimit		4 
#define	promoterFaceForward	0
#define promoterTurn		45
#define itemsonShelf		36
#define leftShelf1			0
#define leftShelf2			36
#define leftShelf3			72
#define	backShelf1			108
#define	backShelf2			144
#define	backShelf3			180
#define leftPShelf1			216
#define	leftPShelf2			252
#define	middleShelf1		288
#define	middleShelf2		324
#define	rightShelf			360
#define customerForward		90
#define	customerLeft		180
#define	customerRight		0
#define	customerBack		-90
#define	vehicleRight		90
#define vehicleForward		180
#define vehicleLeft			-90
#define	vehicleBack			0
