///////////glass door////////////
bool open = false;
float door1Pos = 1023;
float door2Pos = 1365;

//////////Taking Item////////////
bool isHolding = false;
int holding = -1;

#define NorthSouthWall Vector3(2252,274,25)
#define EastWestWall Vector3(25,274,1826)

#define leftShelfBounds Vector3(117.5,172,740.5)
#define middleLeftShelfBounds Vector3(235,172,246.5)
#define middleRightShelfBounds Vector3(235,172,246.6)
#define backShelfBounds Vector3(740.5,172,117.5)
#define loneShelfBounds Vector3(247,172,117.5)

#define doorBounds Vector3(400,222.5,225)
#define playerBounds Vector3(50,50,50);

#define doorTranslation Vector3(2230,-53,1190);
#define leftShelfTranslate Vector3(-2085,-103.3,-1085);
#define middleLeftShelfTranslate Vector3(-1210,-103.3,-820);
#define middleRightShelfTranslate Vector3(-310,-103.3,-820);
#define backShelfTranslate Vector3(-498,-103.3,-1709);
#define loneShelfTranslate Vector3(600,-103.3,-690);
#define MarketWallNorthSouthTranslate Vector3(0,0,1850);
#define MarketWallEastWestTranslate Vector3(2226,0,0);


//AI configurables
#define	passerbySpeed		15
#define	customerSpeed		10
#define	vehicleSpeed		20
#define	vehicleOwnerSpeed	10
#define	passerbySpawnRate	1111 // 1 / ___ chance
#define	customerSpawnRate	2222 // of spawning
#define vehiclesSpawnRate	2222 // every update
#define	customerLimit		10
#define passerbyLimit		20
#define cashierTurnDelay	2 // seconds
#define itemRotateSpeed		10
#define	promoterFaceForward	0
#define promoterTurn		45

//AI fixed values. DO NOT CHANGE
#define	vehicleLimit		4 