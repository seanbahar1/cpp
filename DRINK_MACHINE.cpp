// DRINK_MACHINE.cpp : This file contains the 'main' function. Program execution begins and ends there.

// thiis project will simulate a drink machine

#define MAX_PER_SLOT 10 // slots will look like a keycode so like A2 B6 up  to ff {doing itlike hexadecimal}
#define PASSWORD "1234"
#define AMOUNT_OF_SLOTS_IN_THE_MACHINE 50
#define NEWLINE std::cout << std::endl

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int moneyInTHeMachine = 0;

class Drink 
{
public:
	//making a new drink
	Drink(int Amount, int price, int slot) 
	{
		this->price = price;
		this->amount = Amount;
		this->slot = slot;
	}

	int returnDrinkSlot() 
	{
		return this->slot;
	}
	int returnDrinkPrice() 
	{
		return this->price;
	}
	int returnAmount() 
	{
		return this->amount;
	}
	void drinkWasGrabbed() 
	{
		this->amount--;
	}
	bool canGetDrink()
	{
		if (this->amount > 0) return true;
		return false;
	}
	void addToAmount(int amount) 
	{
		this->amount += amount;
	}
private:
	int price;
	int amount;
	int slot;

};


std::vector<Drink> inventory;

bool state = false; /// False means its in normal mode and not dev/maintnance mode

/*
commands:
	GLOBAL COMMAND:
	command = return money -> if this command was entered it will put all the money that was entered back down

	going into maintanance Mode:
	command = mtnc -> will initiate second stage to check for password

	inside Maintnance mode:
	command = list -> printing all the drinks and the amount that is left of them
	command = add Drink -> this command will initiate a 3 way command:
		- enter slot -> will expect a name  of the drink -> this will run a check to see if the slot is already taken : if exist ask if they want to add a to the amount else continue to second stage: (if the slot is > 50 than return error)
		- enter price -> this will just take the price for the drink -> check if the price is positive if not return err
		- amount entered to the maching -> this will store the amount that was ewntered into the machine
		
	going back into normal Mode:
	command = Exit -> will take us back to normal mode

	inside normal Mode:
	command = get drink-> this will initiate a 2 step command:
	- enter money -> checking how much money was entered
	- enter Slot -> will check if they got enough money to get the drink: if so gives them the drink -> if the slot is empty say choose another slot
	-> return change if there is a need for it
*/

void addToDrinkAmount(Drink* drk)
{ 
	std::string amount;
	std::cin >> amount;
	NEWLINE;
	while ((drk->returnAmount() + std::stoi(amount)) > MAX_PER_SLOT) 
	{
		// telling the user that they cannot enter more than x
		std::cout << "you can only add " << MAX_PER_SLOT - drk->returnAmount();
		NEWLINE;
		std::cin >> amount;
		NEWLINE;
	}
	drk->addToAmount(std::stoi(amount));
	delete &amount;
}

void newDrink() 
{
	// this is the function that will get the new drink into memory with al lthe neccessary checks
	//checking slots
	std::string slot;
	std::cout << "please enter the required slot";

	std::cin >> slot;
	NEWLINE;
	// checking slot if taken or not
	for (Drink drk : inventory) 
	{
		if (drk.returnDrinkSlot() == std::stoi(slot)) 
		{
			//this means we already have a drink in this slot
			std::cout << "there is already drinks in that slot do you want to add to it?";
			NEWLINE;
			std::string ans;
			std::cin >> ans;
			NEWLINE;
			if (ans == "yes")
			{
				// call the addToDrinkPrice
				addToDrinkAmount(&drk);
			}
			else 
			{
				// wrong command or no both = to no
				// do nothing....
			}
		}
	}

	//getting the price
	std::string price;
	std::cout << "what is the price of the drink?";
	NEWLINE;
	std::cin >> price;
	NEWLINE;

	//getting the amount
	std::string amount;
	std::cout << "how much were entered to the slot?";
	NEWLINE;
	std::cin >> amount;
	NEWLINE;

	//creating the DRINK instance and adding it to the vector
	Drink* drink = new Drink(std::stoi(amount), std::stoi(price),std::stoi(slot));
	inventory.insert(inventory.begin(), *drink);
	delete &slot;
	delete &price;
	delete &amount;
}

void retChange(int paid) 
{
	std::cout << moneyInTHeMachine - paid;
	NEWLINE;
	moneyInTHeMachine = 0;
}

void enteringMoreMoney() 
{
	std::string amount;
	std::cout << "enter amount of money";
	NEWLINE;
	std::cin >> amount;
	if (amount == "cancle") 
	{
		moneyInTHeMachine = 0;
		return;
	}
	NEWLINE;
	moneyInTHeMachine += std::stoi(amount);
	delete &amount;
}

void getDrink(bool ifMoney)
{
	// asking the user to enter the money into the machine
	if (!ifMoney) {
	std::string money;
	std::cout << "please enter money";
	NEWLINE;
	std::cin >> money;
	NEWLINE;
	moneyInTHeMachine = std::stoi(money);
}
	//asking which slot of a drink
	std::string slot;
	std::cout << "please enter slot";
	NEWLINE;
	std::cin >> slot;
	NEWLINE;
	for (int i = 0; i < (int)inventory.size(); i++) 
	{
		if (inventory[i].returnDrinkSlot() == std::stoi(slot)) 
		{
			// if we got here it means the drink exist

			// now we nedto check if there is actualy enough drinks
			if (inventory[i].canGetDrink()) 
			{
				//checking if they entered anough money
				if (inventory[i].returnDrinkPrice() <= moneyInTHeMachine) 
				{
					inventory[i].drinkWasGrabbed();
					//returning change
					retChange(inventory[i].returnDrinkPrice());
					//printing that the drink was given with a ty for buying
					std::cout << "ty for buying";
					NEWLINE;
				}
				else 
				{
					// telling the user they need to enter more money
					enteringMoreMoney();
				}
			}
			else 
			{
				std::cout << "there are no drinks in this slot please try another slot";
				getDrink(false);
			}
		}
	}
	
}

int main()
{
	while (1) 
	{
		std::string command;
		std::cout << "please enter the righto command" << std::endl;
		getline(std::cin, command);
		
		if (command == "return money") 
		{
			moneyInTHeMachine = 0;
		}
		//checking if we are in maintanance state
		else if (state) 
		{
			std::cout << "MTNC MODE";
			NEWLINE;
			// if we got hewre it means we are in maintanance state
			if (command == "list") 
			{
				//if this command is executed we will need to print all the drinks that are in the machine 
				for (Drink dr : inventory) 
				{
					std::cout << "slot -> " << dr.returnDrinkSlot() << " Has" << dr.returnAmount() << " items left for the price of " << dr.returnDrinkPrice() << std::endl;
				}
			}
			else if (command.find("add drink") != std::string::npos) 
			{
				//calling the add drink function
				newDrink();
			}
			else if (command == "exit")
			{
				// this will put us back into normal mode
				state = !state;
			}
		} else { // this will run only when we are not in maint state
			//checking if we need to get into maintanance mode
			if (command == "mtnc")
			{
				std::cout << "you are now in maitanance more -- please enter the password";
				NEWLINE;
				// now we will ask the user to enter a password and then we will check if the password is correct
				std::string password;
				std::cin >> password;
				NEWLINE;
				//now we will check if the password matches our password
				if (password != PASSWORD)
				{
					//now we will tell the used the password is incorrect and return to the main loop again
					std::cout << "the password is incorrect" << std::endl;
				}
				else
				{
					// if we got here it means the password was correct
					//now we will change out bool variable to true
					state = true;
				}
			}
			else if (command == "get drink") 
			{
				// calling the get drink command
				getDrink(true);
			}
		}

	}
	return 0;
}

