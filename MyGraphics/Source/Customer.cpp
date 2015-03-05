/******************************************************************************/
/*!
\file	Customer.cpp
\author Group 7
\brief
CPP file for Customer
*/
/******************************************************************************/
#include "Customer.h"
#include "Shelfslot.h"
#include "Item.h"

Customer::Customer(void)
{
	this->Waypoints.push_back(CarparkEntranceWaypoint);
	this->Waypoints.push_back(ShopEntranceWaypoint);
	this->Waypoints.push_back(BasketsWaypoint);
	this->Waypoints.push_back(SupermarketGatesWaypoint);
	this->Waypoints.push_back(TrolleyWaypoint);
	this->Waypoints.push_back(DrinksWaypoint);
	this->Waypoints.push_back(RightShelfWaypoint);
	this->Waypoints.push_back(MiddleRightShelfWaypoint);
	this->Waypoints.push_back(BackShelfWaypoint);
	this->Waypoints.push_back(LeftRightShelfWaypoint);
	this->Waypoints.push_back(LeftShelfWaypoint);
	this->Waypoints.push_back(FrontLeftShelfWaypoint);
	this->Waypoints.push_back(CustomerServiceWaypoint);
	this->Waypoints.push_back(Checkout1Waypoint);
	this->Waypoints.push_back(Checkout2Waypoint);
	this->Waypoints.push_back(Checkout3Waypoint);
	this->Waypoints.push_back(Checkout4Waypoint);
	this->Waypoints.push_back(CheckingoutWaypoint);
	this->Waypoints.push_back(CheckedoutWaypoint);
	this->Waypoints.push_back(DespawnWaypoint);
}

Customer::~Customer(void)
{
}

void Customer::updateAI(void)
{
	for ( int i = 0; i < this->AICurrent; i++ )
	{
		//std::cout << "AICurrent: " << this->AICurrent << std::endl;
		//std::cout << "Coordinates: " << this->Coordinates[i].x << ", " << this->Coordinates[i].y << ", " << this->Coordinates[i].z << std::endl;
		//std::cout << "Waypoints: " << this->Waypoints[Destination].x << ", " << this->Waypoints[Destination].y << ", " << this->Waypoints[Destination].z << std::endl;

		if ( this->State[i] == SpawntoCarpark )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->Waypoints[CarparkEntrance].z )
				this->Coordinates[i].z -= customerSpeed;
			else
				this->State[i] = CarparktoEntrance;
		}
		else if ( this->State[i] == CarparktoEntrance )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->Waypoints[ShopEntrance].x )
				this->Coordinates[i].x -= customerSpeed;
			else
			{
				int goWhere = rand() % 2;

				if ( goWhere == 0 )
					this->State[i] = EntrancetoBaskets;
				else
					this->State[i] = EntrancetoGates;
			}
		}
		else if ( this->State[i] == EntrancetoBaskets )
		{
			//Not coded yet.
			this->State[i] = EntrancetoGates;
		}
		else if ( this->State[i] == BasketstoEntrance )
		{
			//Not coded yet.
			this->State[i] = EntrancetoGates;
		}
		else if ( this->State[i] == EntrancetoGates )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->Waypoints[SupermarketGates].z )
				this->Coordinates[i].z -= customerSpeed;
			else
			{
				int goWhere = rand() % 2;

				if ( goWhere == 0 )
					this->State[i] = GatestoTrolley;
				else
				{
					int goWhereNext = rand() % 2;

					if ( goWhereNext == 0 )
						this->State[i] = GatestoDrinks;
					else
						this->State[i] = GatestoRightShelf;
				}
			}
		}
		else if ( this->State[i] == GatestoTrolley )
		{
			//Not coded yet.
			this->State[i] = TrolleytoGates;
		}
		else if ( this->State[i] == TrolleytoGates )
		{
			//Not coded yet.
			this->State[i] = GatestoDrinks;
		}
		else if ( this->State[i] == GatestoDrinks )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->Waypoints[Drinks].x )
				this->Coordinates[i].x += customerSpeed;
			else
			{
				this->rotateY[i] = customerForward;
				if ( this->Coordinates[i].z > this->Waypoints[Drinks].z )
					this->Coordinates[i].z -= customerSpeed;
				else
					this->State[i] = DrinkstoGates;
			}
		}
		else if ( this->State[i] == DrinkstoGates )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->Waypoints[SupermarketGates].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				this->rotateY[i] = customerLeft;
				if ( this->Coordinates[i].x > this->Waypoints[SupermarketGates].x )
					this->Coordinates[i].x -= customerSpeed;
				else
					this->State[i] = GatestoRightShelf;
			}
		}
		else if ( this->State[i] == GatestoRightShelf )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->Waypoints[SupermarketGates].x && this->Coordinates[i].z > this->Waypoints[RightShelf].z )
				this->Coordinates[i].x += customerSpeed;
			else
			{
				this->rotateY[i] = customerForward;
				if ( this->Coordinates[i].z > this->Waypoints[RightShelf].z )
					this->Coordinates[i].z -= customerSpeed;
				else
				{
					this->rotateY[i] = customerLeft;
					if ( this->Coordinates[i].x > this->Waypoints[RightShelf].x )
						this->Coordinates[i].x -= customerSpeed;
					else
					{
						int goWhere = rand() % 2;
						if ( goWhere == 0 )
							this->State[i] = RightShelftoMiddleRightShelf;
						else
							this->State[i] = RightShelftoBackShelf;
					}
				}
			}
		}
		else if ( this->State[i] == RightShelftoMiddleRightShelf )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->Waypoints[MiddleRightShelf].x )
				this->Coordinates[i].x -= customerSpeed;
			else
			{
				this->rotateY[i] = customerBack;
				if ( this->Coordinates[i].z < this->Waypoints[MiddleRightShelf].z )
					this->Coordinates[i].z += customerSpeed;
				else
				{
					this->State[i] = MiddleRightShelftoBackShelf;
				}
			}
		}
		else if ( this->State[i] == RightShelftoBackShelf )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->Waypoints[BackShelf].z )
				this->Coordinates[i].z -= customerSpeed;
			else
			{
				this->rotateY[i] = customerLeft;
				if ( this->Coordinates[i].x > this->Waypoints[BackShelf].x )
					this->Coordinates[i].x -= customerSpeed;
				else
				{
					int goWhere = rand() % 2;

					if ( goWhere == 0 )
						this->State[i] = BackShelftoLeftShelf;
					else
						this->State[i] = BackShelftoLeftRightShelf;
				}
			}
		}
		else if ( this->State[i] == MiddleRightShelftoBackShelf )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->Waypoints[BackShelf].z )
				this->Coordinates[i].z -= customerSpeed;
			else
			{
				this->rotateY[i] = customerLeft;
				if ( this->Coordinates[i].x > this->Waypoints[BackShelf].x )
					this->Coordinates[i].x -= customerSpeed;
				else
				{
					int goWhere = rand() % 2;

					if ( goWhere == 0 )
						this->State[i] = BackShelftoLeftShelf;
					else
						this->State[i] = BackShelftoLeftRightShelf;
				}
			}
		}
		else if ( this->State[i] == BackShelftoLeftShelf )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->Waypoints[LeftShelf].x )
					this->Coordinates[i].x -= customerSpeed;
			else
			{
				this->State[i] = LeftShelftoFrontLeftShelf;
			}
		}
		else if ( this->State[i] == BackShelftoLeftRightShelf )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->Waypoints[LeftRightShelf].x )
				this->Coordinates[i].x -= customerSpeed;
			else
			{
				this->rotateY[i] = customerBack;
				if ( this->Coordinates[i].z < this->Waypoints[LeftRightShelf].z )
					this->Coordinates[i].z += customerSpeed;
				else
				{
					int goWhere = rand() % 2;

					if ( goWhere == 0 )
						this->State[i] = LeftRightShelftoCheckout2;
					else
						this->State[i] = LeftRightShelftoGates;
				}
			}
		}
		else if ( this->State[i] == LeftRightShelftoCheckout2 )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->Waypoints[Checkout2].x )
				this->Coordinates[i].x -= customerSpeed;
			else
				this->State[i] = CheckingOut;
		}
		else if ( this->State[i] == LeftShelftoFrontLeftShelf )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->Waypoints[FrontLeftShelf].z )
				this->Coordinates[i].z += customerSpeed;
			else
				this->State[i] = LeftFrontShelftoCustomerService;
		}
		else if ( this->State[i] == LeftFrontShelftoCustomerService )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->Waypoints[CustomerService].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				if ( this->itemsHeld[i] == itemsHeldtoLeave )
				{
					int goWhere = rand() % 4;

					if ( goWhere == 0 )
						this->State[i] = CustomerServicetoCheckout1;
					else if ( goWhere == 1 )
						this->State[i] = CustomerServicetoCheckout2;
					else if ( goWhere == 2 )
						this->State[i] = CustomerServicetoCheckout3;
					else if ( goWhere == 3 )
						this->State[i] = CustomerServicetoCheckout4;
				}
				else
					this->State[i] = CustomerServicetoGates;
			}
		}
		else if ( this->State[i] == CustomerServicetoCheckout1 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->Waypoints[Checkout1].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = CheckingOut;
		}
		else if ( this->State[i] == CustomerServicetoCheckout2 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->Waypoints[Checkout2].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = CheckingOut;
		}
		else if ( this->State[i] == CustomerServicetoCheckout3 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->Waypoints[Checkout3].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = CheckingOut;
		}
		else if ( this->State[i] == CustomerServicetoCheckout4 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->Waypoints[Checkout4].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = CheckingOut;
		}
		else if ( this->State[i] == CustomerServicetoGates )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->Waypoints[SupermarketGates].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = EntrancetoGates;
		}
		else if ( this->State[i] == LeftRightShelftoGates )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->Waypoints[SupermarketGates].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				this->rotateY[i] = customerRight;
				if ( this->Coordinates[i].x < this->Waypoints[SupermarketGates].x )
					this->Coordinates[i].x += customerSpeed;
				else
					this->State[i] = EntrancetoGates;
			}
		}
		else if ( this->State[i] == CheckingOut )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->Waypoints[Checkingout].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				if ( this->Coordinates[i].x == this->Waypoints[Checkout1].x || this->Coordinates[i].x == this->Waypoints[Checkout3].x )
					this->rotateY[i] = customerLeft;
				else
					this-> rotateY[i] = customerRight;
				
				if ( this->cashierDelay[i] > checkoutTime )
					this->State[i] = CheckedOuttoExit;
				else
					this->cashierDelay[i] += 0.2f;
			}
		}
		else if ( this->State[i] == CheckedOuttoExit )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->Waypoints[Checkedout].z && this->Coordinates[i].x != this->Waypoints[Despawn].x )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				this->rotateY[i] = customerRight;
				if ( this->Coordinates[i].x < this->Waypoints[Despawn].x )
					this->Coordinates[i].x += customerSpeed;
				else
				{
					this->rotateY[i] = customerForward;
					if ( this->Coordinates[i].z > this->Waypoints[Despawn].z )
						this->Coordinates[i].z -= customerSpeed;
					else
					{
						this->Coordinates.erase(this->Coordinates.begin() + i);
						this->rotateY.erase(this->rotateY.begin() + i);
						this->State.erase(this->State.begin() + i);
						this->itemsHeld.erase(this->itemsHeld.begin() + i);
						this->cashierDelay.erase(this->cashierDelay.begin() + i);
						this->AICurrent -= 1;
					}
				}
			}
		}
	}
}

void Customer::spawnAI(void)
{
	if ( this->AICurrent < this->AILimit )
	{
		int SpawnChance = rand() % this->spawnRate;
		if ( SpawnChance == 1 )
		{
			this->Coordinates.push_back(CustomerSpawn);
			this->rotateY.push_back(customerForward);
			this->State.push_back(SpawntoCarpark);
			this->cashierDelay.push_back(0);
			this->itemsHeld.push_back(0);
			this->Max.push_back(CustomerBounds);
			this->Min.push_back(-CustomerBounds);
			this->AICurrent += 1;
		}
	}
}

void Customer::interactWith(void)
{
}

void Customer::takeItem(void)
{
	int canTakeItem = rand() % itemPickupChance;

	if ( canTakeItem == 1 )
	{
		//int takeWhich = rand() % itemsonShelf + rightShelf;

		//itemVector[shelfVector[takeWhich]->itemid]->takeItem(abyss);
		//shelfVector[takeWhich]->isempty = true;
	}
}