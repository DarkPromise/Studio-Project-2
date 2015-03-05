/******************************************************************************/
/*!
\file	Vehicle.cpp
\author Group 7
\brief
CPP file for Vehicle
*/
/******************************************************************************/
#include "Vehicle.h"


Vehicle::Vehicle(void)
{
	this->V_Waypoints.push_back(CarparkEntranceWaypoint);
	this->V_Waypoints.push_back(VehicleEntranceWaypoint);
	this->V_Waypoints.push_back(CarparkSlot1Waypoint);
	this->V_Waypoints.push_back(CarparkSlot2Waypoint);
	this->V_Waypoints.push_back(CarparkSlot3Waypoint);
	this->V_Waypoints.push_back(CarparkSlot4Waypoint);
	this->V_Waypoints.push_back(ShopEntranceWaypoint);
	this->V_Waypoints.push_back(BasketsWaypoint);
	this->V_Waypoints.push_back(SupermarketGatesWaypoint);
	this->V_Waypoints.push_back(TrolleyWaypoint);
	this->V_Waypoints.push_back(DrinksWaypoint);
	this->V_Waypoints.push_back(RightShelfWaypoint);
	this->V_Waypoints.push_back(MiddleRightShelfWaypoint);
	this->V_Waypoints.push_back(BackShelfWaypoint);
	this->V_Waypoints.push_back(LeftRightShelfWaypoint);
	this->V_Waypoints.push_back(LeftShelfWaypoint);
	this->V_Waypoints.push_back(FrontLeftShelfWaypoint);
	this->V_Waypoints.push_back(CustomerServiceWaypoint);
	this->V_Waypoints.push_back(Checkout1Waypoint);
	this->V_Waypoints.push_back(Checkout2Waypoint);
	this->V_Waypoints.push_back(Checkout3Waypoint);
	this->V_Waypoints.push_back(Checkout4Waypoint);
	this->V_Waypoints.push_back(CheckingoutWaypoint);
	this->V_Waypoints.push_back(CheckedoutWaypoint);
	this->V_Waypoints.push_back(VehicleDespawnWaypoint);

	for ( int i = 0; i < vehicleLimit; i++ )
		carparkSlot[i] = isEmpty;
}

Vehicle::~Vehicle(void)
{
}

void Vehicle::updateAI(void)
{
	/*std::cout <<"this->vehicleCoordinates.size();"<<this->vehicleCoordinates.size()<< std::endl;
	std::cout <<"this->vehicleRotateY.size();"<<this->vehicleRotateY.size()<< std::endl;
	std::cout <<"this->State.size();"<<this->State.size()<< std::endl;
	std::cout <<"this->parkingLocation.size();"<<this->parkingLocation.size()<< std::endl;
	std::cout <<"this->Coordinates.size();"<<this->Coordinates.size()<< std::endl;
	std::cout <<"this->rotateY.size();"<<this->rotateY.size()<< std::endl;
	std::cout <<"this->renderOwner.size();"<<this->renderOwner.size()<< std::endl;
	std::cout <<"this->itemsHeld.size();"<<	this->itemsHeld.size()<< std::endl;*/

	for ( int i = 0; i < this->AICurrent; i++ )
	{
		if ( this->State[i] == V_SpawntoCarparkEntrance )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->V_Waypoints[V_CarparkEntrance].z )
				this->Coordinates[i].z -= customerSpeed;
			else
			{
				this->State[i] = V_CarparkEntrancetoSupermarketEntrance;
			}
		}
		else if ( this->State[i] == V_SpawntoVehicleEntrance )
		{
			this->vehicleRotateY[i] = vehicleForward;
			if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_VehicleEntrance].z )
				this->vehicleCoordinates[i].z -= vehicleSpeed;
			else
			{
				this->State[i] = V_VehicleEntrancetoCarparkSlot;
			}
		}
		else if ( this->State[i] == V_VehicleEntrancetoCarparkSlot )
		{
			this->vehicleRotateY[i] = vehicleLeft;
			if ( this->carparkSlot[0] == isEmpty )
			{
				if ( this->vehicleCoordinates[i].x > this->V_Waypoints[V_CarparkSlot1].x )
					this->vehicleCoordinates[i].x -= vehicleSpeed;
				else
				{
					this->vehicleRotateY[i] = vehicleBack;
					if ( this->vehicleCoordinates[i].z < this->V_Waypoints[V_CarparkSlot1].z )
						this->vehicleCoordinates[i].z += vehicleSpeed;
					else
					{
						this->carparkSlot[0] = isFilled;
						this->Coordinates.push_back(Carpark1Spawnpoint);
						this->rotateY.push_back(customerForward);
						this->parkingLocation.push_back(0);
						this->renderOwner[i] = RenderOwner;
						this->Max.push_back(CustomerBounds);
						this->Min.push_back(-CustomerBounds);
						this->State[i] = V_SpawntoCarparkEntrance;
					}
				}
			}
			else if ( this->carparkSlot[1] == isEmpty )
			{
				if ( this->vehicleCoordinates[i].x > this->V_Waypoints[V_CarparkSlot2].x )
					this->vehicleCoordinates[i].x -= vehicleSpeed;
				else
				{
					this->vehicleRotateY[i] = vehicleBack;
					if ( this->vehicleCoordinates[i].z < this->V_Waypoints[V_CarparkSlot2].z )
						this->vehicleCoordinates[i].z += vehicleSpeed;
					else
					{
						this->carparkSlot[1] = isFilled;
						this->Coordinates.push_back(Carpark2Spawnpoint);
						this->rotateY.push_back(customerForward);
						this->parkingLocation.push_back(1);
						this->renderOwner[i] = RenderOwner;
						this->Max.push_back(CustomerBounds);
						this->Min.push_back(-CustomerBounds);
						this->State[i] = V_SpawntoCarparkEntrance;
					}
				}
			}
			else if ( this->carparkSlot[2] == isEmpty )
			{
				if ( this->vehicleCoordinates[i].x > this->V_Waypoints[V_CarparkSlot3].x )
					this->vehicleCoordinates[i].x -= vehicleSpeed;
				else
				{
					this->vehicleRotateY[i] = vehicleBack;
					if ( this->vehicleCoordinates[i].z < this->V_Waypoints[V_CarparkSlot3].z )
						this->vehicleCoordinates[i].z += vehicleSpeed;
					else
					{
						this->carparkSlot[2] = isFilled;
						this->Coordinates.push_back(Carpark3Spawnpoint);
						this->rotateY.push_back(customerForward);
						this->parkingLocation.push_back(2);
						this->renderOwner[i] = RenderOwner;
						this->Max.push_back(CustomerBounds);
						this->Min.push_back(-CustomerBounds);
						this->State[i] = V_SpawntoCarparkEntrance;
					}
				}
			}
			else if ( this->carparkSlot[3] == isEmpty )
			{
				if ( this->vehicleCoordinates[i].x > this->V_Waypoints[V_CarparkSlot4].x )
					this->vehicleCoordinates[i].x -= vehicleSpeed;
				else
				{
					this->vehicleRotateY[i] = vehicleBack;
					if ( this->vehicleCoordinates[i].z < this->V_Waypoints[V_CarparkSlot4].z )
						this->vehicleCoordinates[i].z += vehicleSpeed;
					else
					{
						this->carparkSlot[3] = isFilled;
						this->Coordinates.push_back(Carpark4Spawnpoint);
						this->rotateY.push_back(customerForward);
						this->parkingLocation.push_back(3);
						this->Max.push_back(CustomerBounds);
						this->Min.push_back(-CustomerBounds);
						this->renderOwner[i] = RenderOwner;
						this->State[i] = V_SpawntoCarparkEntrance;
					}
				}
			}
		}
		else if ( this->State[i] == V_CarparkEntrancetoSupermarketEntrance )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->V_Waypoints[V_ShopEntrance].x )
				this->Coordinates[i].x -= customerSpeed;
			else
			{
				int goWhere = rand() % 2;

				if ( goWhere == 0 )
					this->State[i] = V_EntrancetoBaskets;
				else
					this->State[i] = V_EntrancetoGates;
			}
		}
		else if ( this->State[i] == V_EntrancetoBaskets )
		{
			//Not coded yet.
			this->State[i] = V_EntrancetoGates;
		}
		else if ( this->State[i] == V_BasketstoEntrance )
		{
			//Not coded yet.
			this->State[i] = V_EntrancetoGates;
		}
		else if ( this->State[i] == V_EntrancetoGates )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->V_Waypoints[V_SupermarketGates].z )
				this->Coordinates[i].z -= customerSpeed;
			else
			{
				int goWhere = rand() % 2;

				if ( goWhere == 0 )
					this->State[i] = V_GatestoTrolley;
				else
				{
					int goWhereNext = rand() % 2;

					if ( goWhereNext == 0 )
						this->State[i] = V_GatestoDrinks;
					else
						this->State[i] = V_GatestoRightShelf;
				}
			}
		}
		else if ( this->State[i] == V_GatestoTrolley )
		{
			//Not coded yet.
			this->State[i] = V_TrolleytoGates;
		}
		else if ( this->State[i] == V_TrolleytoGates )
		{
			//Not coded yet.
			this->State[i] = V_GatestoDrinks;
		}
		else if ( this->State[i] == V_GatestoDrinks )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->V_Waypoints[V_Drinks].x )
				this->Coordinates[i].x += customerSpeed;
			else
			{
				this->rotateY[i] = customerForward;
				if ( this->Coordinates[i].z > this->V_Waypoints[V_Drinks].z )
					this->Coordinates[i].z -= customerSpeed;
				else
					this->State[i] = V_DrinkstoGates;
			}
		}
		else if ( this->State[i] == V_DrinkstoGates )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->V_Waypoints[V_SupermarketGates].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				this->rotateY[i] = customerLeft;
				if ( this->Coordinates[i].x > this->V_Waypoints[V_SupermarketGates].x )
					this->Coordinates[i].x -= customerSpeed;
				else
					this->State[i] = V_GatestoRightShelf;
			}
		}
		else if ( this->State[i] == V_GatestoRightShelf )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->V_Waypoints[V_SupermarketGates].x && this->Coordinates[i].z > this->V_Waypoints[V_RightShelf].z )
				this->Coordinates[i].x += customerSpeed;
			else
			{
				this->rotateY[i] = customerForward;
				if ( this->Coordinates[i].z > this->V_Waypoints[V_RightShelf].z )
					this->Coordinates[i].z -= customerSpeed;
				else
				{
					this->rotateY[i] = customerLeft;
					if ( this->Coordinates[i].x > this->V_Waypoints[V_RightShelf].x )
						this->Coordinates[i].x -= customerSpeed;
					else
					{
						int goWhere = rand() % 2;
						if ( goWhere == 0 )
							this->State[i] = V_RightShelftoMiddleRightShelf;
						else
							this->State[i] = V_RightShelftoBackShelf;
					}
				}
			}
		}
		else if ( this->State[i] == V_RightShelftoMiddleRightShelf )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->V_Waypoints[V_MiddleRightShelf].x )
				this->Coordinates[i].x -= customerSpeed;
			else
			{
				this->rotateY[i] = customerBack;
				if ( this->Coordinates[i].z < this->V_Waypoints[V_MiddleRightShelf].z )
					this->Coordinates[i].z += customerSpeed;
				else
				{
					this->State[i] = V_MiddleRightShelftoBackShelf;
				}
			}
		}
		else if ( this->State[i] == V_RightShelftoBackShelf )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->V_Waypoints[V_BackShelf].z )
				this->Coordinates[i].z -= customerSpeed;
			else
			{
				this->rotateY[i] = customerLeft;
				if ( this->Coordinates[i].x > this->V_Waypoints[V_BackShelf].x )
					this->Coordinates[i].x -= customerSpeed;
				else
				{
					int goWhere = rand() % 2;

					if ( goWhere == 0 )
						this->State[i] = V_BackShelftoLeftShelf;
					else
						this->State[i] = V_BackShelftoLeftRightShelf;
				}
			}
		}
		else if ( this->State[i] == V_MiddleRightShelftoBackShelf )
		{
			this->rotateY[i] = customerForward;
			if ( this->Coordinates[i].z > this->V_Waypoints[V_BackShelf].z )
				this->Coordinates[i].z -= customerSpeed;
			else
			{
				this->rotateY[i] = customerLeft;
				if ( this->Coordinates[i].x > this->V_Waypoints[V_BackShelf].x )
					this->Coordinates[i].x -= customerSpeed;
				else
				{
					int goWhere = rand() % 2;

					if ( goWhere == 0 )
						this->State[i] = V_BackShelftoLeftShelf;
					else
						this->State[i] = V_BackShelftoLeftRightShelf;
				}
			}
		}
		else if ( this->State[i] == V_BackShelftoLeftShelf )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->V_Waypoints[V_LeftShelf].x )
					this->Coordinates[i].x -= customerSpeed;
			else
			{
				this->State[i] = V_LeftShelftoFrontLeftShelf;
			}
		}
		else if ( this->State[i] == V_BackShelftoLeftRightShelf )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->V_Waypoints[V_LeftRightShelf].x )
				this->Coordinates[i].x -= customerSpeed;
			else
			{
				this->rotateY[i] = customerBack;
				if ( this->Coordinates[i].z < this->V_Waypoints[V_LeftRightShelf].z )
					this->Coordinates[i].z += customerSpeed;
				else
				{
					if ( this->itemsHeld[i] == itemsHeldtoLeave )
						this->State[i] = V_LeftRightShelftoCheckout2;
					else
						this->State[i] = V_LeftRightShelftoGates;
				}
			}
		}
		else if ( this->State[i] == V_LeftRightShelftoCheckout2 )
		{
			this->rotateY[i] = customerLeft;
			if ( this->Coordinates[i].x > this->V_Waypoints[V_Checkout2].x )
				this->Coordinates[i].x -= customerSpeed;
			else
				this->State[i] = V_CheckingOut;
		}
		else if ( this->State[i] == V_LeftShelftoFrontLeftShelf )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->V_Waypoints[V_FrontLeftShelf].z )
				this->Coordinates[i].z += customerSpeed;
			else
				this->State[i] = V_LeftFrontShelftoCustomerService;
		}
		else if ( this->State[i] == V_LeftFrontShelftoCustomerService )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->V_Waypoints[V_CustomerService].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				if ( this->itemsHeld[i] == itemsHeldtoLeave )
				{
					int goWhere = rand() % 4;

					if ( goWhere == 0 )
						this->State[i] = V_CustomerServicetoCheckout1;
					else if ( goWhere == 1 )
						this->State[i] = V_CustomerServicetoCheckout2;
					else if ( goWhere == 2 )
						this->State[i] = V_CustomerServicetoCheckout3;
					else if ( goWhere == 3 )
						this->State[i] = V_CustomerServicetoCheckout4;
				}
				else
					this->State[i] = V_CustomerServicetoGates;
			}
		}
		else if ( this->State[i] == V_CustomerServicetoCheckout1 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->V_Waypoints[V_Checkout1].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = V_CheckingOut;
		}
		else if ( this->State[i] == V_CustomerServicetoCheckout2 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->V_Waypoints[V_Checkout2].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = V_CheckingOut;
		}
		else if ( this->State[i] == V_CustomerServicetoCheckout3 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->V_Waypoints[V_Checkout3].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = V_CheckingOut;
		}
		else if ( this->State[i] == V_CustomerServicetoCheckout4 )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->V_Waypoints[V_Checkout4].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = V_CheckingOut;
		}
		else if ( this->State[i] == V_CustomerServicetoGates )
		{
			this->rotateY[i] = customerRight;
			if ( this->Coordinates[i].x < this->V_Waypoints[V_SupermarketGates].x )
				this->Coordinates[i].x += customerSpeed;
			else
				this->State[i] = V_EntrancetoGates;
		}
		else if ( this->State[i] == V_LeftRightShelftoGates )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->V_Waypoints[V_SupermarketGates].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				this->rotateY[i] = customerRight;
				if ( this->Coordinates[i].x < this->V_Waypoints[V_SupermarketGates].x )
					this->Coordinates[i].x += customerSpeed;
				else
					this->State[i] = V_EntrancetoGates;
			}
		}
		else if ( this->State[i] == V_CheckingOut )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->V_Waypoints[V_Checkingout].z )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				if ( this->Coordinates[i].x == this->V_Waypoints[V_Checkout1].x || this->Coordinates[i].x == this->V_Waypoints[V_Checkout3].x )
					this->rotateY[i] = customerLeft;
				else
					this-> rotateY[i] = customerRight;
				
				if ( this->cashierDelay[i] > checkoutTime )
					this->State[i] = V_CheckedOuttoExit;
				else
					this->cashierDelay[i] += 0.2f;
			}
		}
		else if ( this->State[i] == V_CheckedOuttoExit )
		{
			this->rotateY[i] = customerBack;
			if ( this->Coordinates[i].z < this->V_Waypoints[V_Checkedout].z && this->Coordinates[i].x != this->V_Waypoints[V_Despawn].x )
				this->Coordinates[i].z += customerSpeed;
			else
			{
				if ( this->parkingLocation[i] == 0 )
				{
					this->rotateY[i] = customerRight;
					if ( this->Coordinates[i].x < this->V_Waypoints[V_CarparkSlot1].x )
						this->Coordinates[i].x += customerSpeed;
					else
					{
						this->renderOwner[i] = DontRenderOwner;
						if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_CarparkEntrance].z )
							this->vehicleCoordinates[i].z -= vehicleReverseSpeed;
						else
						{
							this->vehicleRotateY[i] = vehicleRight;
							if ( this->vehicleCoordinates[i].x < this->V_Waypoints[V_Despawn].x )
								this->vehicleCoordinates[i].x += vehicleSpeed;
							else
							{
								this->vehicleRotateY[i] = vehicleForward;
								if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_Despawn].z )
									this->vehicleCoordinates[i].z -= vehicleSpeed;
								else
								{
									this->carparkSlot[0] = isEmpty;
									this->vehicleCoordinates.erase(this->vehicleCoordinates.begin() + i);
									this->vehicleRotateY.erase(this->vehicleRotateY.begin() + i);
									this->State.erase(this->State.begin() + i);
									this->parkingLocation.erase(this->parkingLocation.begin() + i);
									this->Coordinates.erase(this->Coordinates.begin() + i);
									this->rotateY.erase(this->rotateY.begin() + i );
									this->renderOwner.erase(this->renderOwner.begin() + i);
									this->itemsHeld.erase(this->itemsHeld.begin() + i);
									this->AICurrent -= 1;
								}
							}
						}
					}
				}
				else if ( this->parkingLocation[i] == 1 )
				{
					this->rotateY[i] = customerRight;
					if ( this->Coordinates[i].x < this->V_Waypoints[V_CarparkSlot2].x )
						this->Coordinates[i].x += customerSpeed;
					else
					{
						this->renderOwner[i] = DontRenderOwner;
						if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_CarparkEntrance].z )
							this->vehicleCoordinates[i].z -= vehicleReverseSpeed;
						else
						{
							this->vehicleRotateY[i] = vehicleRight;
							if ( this->vehicleCoordinates[i].x < this->V_Waypoints[V_Despawn].x )
								this->vehicleCoordinates[i].x += vehicleSpeed;
							else
							{
								this->vehicleRotateY[i] = vehicleForward;
								if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_Despawn].z )
									this->vehicleCoordinates[i].z -= vehicleSpeed;
								else
								{
									this->carparkSlot[1] = isEmpty;
									this->vehicleCoordinates.erase(this->vehicleCoordinates.begin() + i);
									this->vehicleRotateY.erase(this->vehicleRotateY.begin() + i);
									this->State.erase(this->State.begin() + i);
									this->parkingLocation.erase(this->parkingLocation.begin() + i);
									this->Coordinates.erase(this->Coordinates.begin() + i);
									this->rotateY.erase(this->rotateY.begin() + i );
									this->renderOwner.erase(this->renderOwner.begin() + i);
									this->itemsHeld.erase(this->itemsHeld.begin() + i);
									this->AICurrent -= 1;
								}
							}
						}
					}
				}
				else if ( this->parkingLocation[i] == 2 )
				{
					this->rotateY[i] = customerRight;
					if ( this->Coordinates[i].x < this->V_Waypoints[V_CarparkSlot3].x )
						this->Coordinates[i].x += customerSpeed;
					else
					{
						this->renderOwner[i] = DontRenderOwner;
						if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_CarparkEntrance].z )
							this->vehicleCoordinates[i].z -= vehicleReverseSpeed;
						else
						{
							this->vehicleRotateY[i] = vehicleRight;
							if ( this->vehicleCoordinates[i].x < this->V_Waypoints[V_Despawn].x )
								this->vehicleCoordinates[i].x += vehicleSpeed;
							else
							{
								this->vehicleRotateY[i] = vehicleForward;
								if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_Despawn].z )
									this->vehicleCoordinates[i].z -= vehicleSpeed;
								else
								{
									this->carparkSlot[2] = isEmpty;
									this->vehicleCoordinates.erase(this->vehicleCoordinates.begin() + i);
									this->vehicleRotateY.erase(this->vehicleRotateY.begin() + i);
									this->State.erase(this->State.begin() + i);
									this->parkingLocation.erase(this->parkingLocation.begin() + i);
									this->Coordinates.erase(this->Coordinates.begin() + i);
									this->rotateY.erase(this->rotateY.begin() + i );
									this->renderOwner.erase(this->renderOwner.begin() + i);
									this->itemsHeld.erase(this->itemsHeld.begin() + i);
									this->AICurrent -= 1;
								}
							}
						}
					}
				}
				else if ( this->parkingLocation[i] == 4 )
				{
					this->rotateY[i] = customerRight;
					if ( this->Coordinates[i].x < this->V_Waypoints[V_CarparkSlot4].x )
						this->Coordinates[i].x += customerSpeed;
					else
					{
						this->renderOwner[i] = DontRenderOwner;
						if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_CarparkEntrance].z )
							this->vehicleCoordinates[i].z -= vehicleReverseSpeed;
						else
						{
							this->vehicleRotateY[i] = vehicleRight;
							if ( this->vehicleCoordinates[i].x < this->V_Waypoints[V_Despawn].x )
								this->vehicleCoordinates[i].x += vehicleSpeed;
							else
							{
								this->vehicleRotateY[i] = vehicleForward;
								if ( this->vehicleCoordinates[i].z > this->V_Waypoints[V_Despawn].z )
									this->vehicleCoordinates[i].z -= vehicleSpeed;
								else
								{
									this->carparkSlot[3] = isEmpty;
									this->vehicleCoordinates.erase(this->vehicleCoordinates.begin() + i);
									this->vehicleRotateY.erase(this->vehicleRotateY.begin() + i);
									this->State.erase(this->State.begin() + i);
									this->parkingLocation.erase(this->parkingLocation.begin() + i);
									this->Coordinates.erase(this->Coordinates.begin() + i);
									this->rotateY.erase(this->rotateY.begin() + i );
									this->renderOwner.erase(this->renderOwner.begin() + i);
									this->itemsHeld.erase(this->itemsHeld.begin() + i);
									this->AICurrent -= 1;
								}
							}
						}
					}
				}
			}
		}
	}
}

void Vehicle::spawnAI(void)
{
	if ( this->AICurrent < this->AILimit && this->Coordinates.size() == this->AICurrent )
	{
		int SpawnChance = rand() % this->spawnRate;
		if ( SpawnChance == 1 )
		{
			this->vehicleCoordinates.push_back(VehicleSpawn);
			this->vehicleRotateY.push_back(vehicleForward);
			this->State.push_back(V_SpawntoVehicleEntrance);
			this->renderOwner.push_back(DontRenderOwner);
			this->cashierDelay.push_back(0);
			this->itemsHeld.push_back(0);
			this->AICurrent += 1;
		}
	}
}

void Vehicle::interactWith(void)
{
}