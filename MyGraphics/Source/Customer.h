/******************************************************************************/
/*!
\file	Customer.h
\author Group 7
\brief
Header file for Customer
*/
/******************************************************************************/
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "AI.h"
#include "Shelfslot.h"
#include "Item.h"

#pragma once

class Customer :	public AI
{
public:
	Customer(void);
	~Customer(void);

	int spawnRate;

	void updateAI(void);
	void spawnAI(void);
	void interactWith(void);

	vector<int> itemsHeld;
	vector<float> cashierDelay;
};

enum CustomerPath
{
	SpawntoCarpark,
	CarparktoEntrance,
	EntrancetoBaskets,
	BasketstoEntrance,
	EntrancetoGates,
	GatestoTrolley,
	TrolleytoGates,
	GatestoDrinks,
	DrinkstoGates,
	GatestoRightShelf,
	RightShelftoMiddleRightShelf,
	RightShelftoBackShelf,
	MiddleRightShelftoBackShelf,
	BackShelftoLeftShelf,
	BackShelftoLeftRightShelf,
	LeftRightShelftoCheckout2,
	LeftShelftoFrontLeftShelf,
	LeftFrontShelftoCustomerService,
	CustomerServicetoCheckout1,
	CustomerServicetoCheckout2,
	CustomerServicetoCheckout3,
	CustomerServicetoCheckout4,
	CustomerServicetoGates,
	LeftRightShelftoGates,
	CheckingOut,
	CheckedOuttoExit,
};

enum CustomerWaypoint
{
	CarparkEntrance,
	ShopEntrance,
	Baskets,
	SupermarketGates,
	Trolley,
	Drinks,
	RightShelf,
	MiddleRightShelf,
	BackShelf,
	LeftRightShelf,
	LeftShelf,
	FrontLeftShelf,
	CustomerService,
	Checkout1,
	Checkout2,
	Checkout3,
	Checkout4,
	Checkingout,
	Checkedout,
	Despawn,
};

#endif