/******************************************************************************/
/*!
\file	AIDefine.h
\author Group 7
\brief
Header file for AIDefine
*/
/******************************************************************************/
//AI configurables
#define	passerbySpeed		15
#define	customerSpeed		10
#define	vehicleSpeed		20
#define	vehicleReverseSpeed	10
#define	vehicleOwnerSpeed	10
#define	passerbySpawnRate	555 // 1 / ___ chance
#define	customerSpawnRate	1111 // of spawning
#define vehicleSpawnRate	2222 // every update
#define	customerLimit		10
#define passerbyLimit		20
#define cashierTurnDelay	2 // seconds
#define itemRotateSpeed		10
#define	itemPickupChance	2 // 1 / __ chance of picking up
#define	itemLookatTime		2 // seconds
#define	itemsHeldtoLeave	3
#define	checkoutTime		20
#define	abyss				Vector3(99999, 99999, 99999) //location to store NPC held items


//AI fixed values. Change at own risk
#define cashierLimit		4
#define	vehicleLimit		4 
#define	promoterLimit		1
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
#define passerbyForward		90
#define	cashierLeft			180
#define	cashierRight		0
#define	cashierForward		90
#define	cashierBack			-90

#define	PasserbySpawn1		Vector3(5000, -275, 7100)
#define	PasserbySpawn2		Vector3(5000, -275, 7200)
#define PromoterSpawn		Vector3(6000, -275, 0)
#define	CustomerSpawn		Vector3(5200, -275, 7000)
#define	VehicleSpawn		Vector3(6400, -275, 6000)
#define	Carpark1Spawnpoint	Vector3(3000, -275, 1700)
#define	Carpark2Spawnpoint	Vector3(3500, -275, 1700)
#define	Carpark3Spawnpoint	Vector3(3900, -275, 1700)
#define	Carpark4Spawnpoint	Vector3(4300, -275, 1700)

#define	StandingGuard		Vector3(1100, -275, 480)
#define	SittingGuard		Vector3(1100, -240, 400)
#define	GuardDirection		-90

#define Cashier1Spawnpoint	Vector3(-1200, -275, 350)
#define Cashier2Spawnpoint	Vector3(-500, -275, 350)
#define Cashier3Spawnpoint	Vector3(-250, -275, 350)
#define Cashier4Spawnpoint	Vector3(500, -275, 350)

#define PasserbyEndWaypoint	Vector3(7000, 0, -7000)

#define CarparkEntranceWaypoint		Vector3(5200, -275, 1200)
#define	ShopEntranceWaypoint		Vector3(850, -275, 1200)
#define	BasketsWaypoint				Vector3(0, 0, 0)
#define	SupermarketGatesWaypoint	Vector3(1000, -275, -200)
#define	TrolleyWaypoint				Vector3(0, 0, 0)
#define	DrinksWaypoint				Vector3(1700, -275, -1700)
#define	RightShelfWaypoint			Vector3(600, -275, -1000)
#define	MiddleRightShelfWaypoint	Vector3(100, -275, -800)
#define	BackShelfWaypoint			Vector3(-400, -275, -1500)
#define	LeftRightShelfWaypoint		Vector3(-875, -275, -800)
#define	LeftShelfWaypoint			Vector3(-1850, -275, -1500)
#define	FrontLeftShelfWaypoint		Vector3(-1850, -275, -600)
#define	CustomerServiceWaypoint		Vector3(-1850, -275, -200)
#define	Checkout1Waypoint			Vector3(-900, -275, -200)
#define	Checkout2Waypoint			Vector3(-800, -275, -200)
#define	Checkout3Waypoint			Vector3(100, -275, -200)
#define	Checkout4Waypoint			Vector3(200, -275, -200)
#define	CheckingoutWaypoint			Vector3(0, -275, 350)
#define	CheckedoutWaypoint			Vector3(0, -275, 1200)
#define	DespawnWaypoint				Vector3(5000, -275, -7000)
#define VehicleDespawnWaypoint		Vector3(6400, -275, -7000)
#define	VehicleEntranceWaypoint		Vector3(6000, -275, 1400)

#define CarparkSlot1Waypoint		Vector3(3040, -275, 2050)
#define CarparkSlot2Waypoint		Vector3(3500, -275, 2050)
#define CarparkSlot3Waypoint		Vector3(3900, -275, 2050)
#define CarparkSlot4Waypoint		Vector3(4300, -275, 2050)
