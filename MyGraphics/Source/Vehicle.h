/******************************************************************************/
/*!
\file	Vehicle.h
\author Group 7
\brief
Header file for Vehicle
*/
/******************************************************************************/
#ifndef VEHICLE_H
#define VEHICLE_H

#include "Customer.h"

#pragma once

class Vehicle :	public Customer
{
public:
	Vehicle(void);
	~Vehicle(void);

	void updateAI(void);
	void spawnAI(void);
	void interactWith(void);

	void takeItem();

	vector<Vector3> vehicleCoordinates;
	vector<float> vehicleRotateY;
	vector<int> renderOwner, parkingLocation;
	vector<Vector3> V_Waypoints;

	int carparkSlot[vehicleLimit];
};

enum VehiclePath
{
	V_SpawntoCarparkEntrance,
	V_SpawntoVehicleEntrance,
	V_VehicleEntrancetoCarparkSlot,
	V_CarparkEntrancetoSupermarketEntrance,
	V_EntrancetoBaskets,
	V_BasketstoEntrance,
	V_EntrancetoGates,
	V_GatestoTrolley,
	V_TrolleytoGates,
	V_GatestoDrinks,
	V_DrinkstoGates,
	V_GatestoRightShelf,
	V_RightShelftoMiddleRightShelf,
	V_RightShelftoBackShelf,
	V_MiddleRightShelftoBackShelf,
	V_BackShelftoLeftShelf,
	V_BackShelftoLeftRightShelf,
	V_LeftRightShelftoCheckout2,
	V_LeftShelftoFrontLeftShelf,
	V_LeftFrontShelftoCustomerService,
	V_CustomerServicetoCheckout1,
	V_CustomerServicetoCheckout2,
	V_CustomerServicetoCheckout3,
	V_CustomerServicetoCheckout4,
	V_CustomerServicetoGates,
	V_LeftRightShelftoGates,
	V_CheckingOut,
	V_CheckedOuttoExit,
};

enum VehicleWaypoint
{
	V_CarparkEntrance,
	V_VehicleEntrance,
	V_CarparkSlot1,
	V_CarparkSlot2,
	V_CarparkSlot3,
	V_CarparkSlot4,
	V_ShopEntrance,
	V_Baskets,
	V_SupermarketGates,
	V_Trolley,
	V_Drinks,
	V_RightShelf,
	V_MiddleRightShelf,
	V_BackShelf,
	V_LeftRightShelf,
	V_LeftShelf,
	V_FrontLeftShelf,
	V_CustomerService,
	V_Checkout1,
	V_Checkout2,
	V_Checkout3,
	V_Checkout4,
	V_Checkingout,
	V_Checkedout,
	V_Despawn,
};

enum OwnerState
{
	DontRenderOwner,
	RenderOwner,
};

enum SlotAvailability
{
	isEmpty,
	isFilled,
};

#endif