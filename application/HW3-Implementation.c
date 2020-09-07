#include "HW3-Header.h"

void GetFromUserZeroToContinue()
//Function requieres from user to enter '0'.
{
	char input;
	printf("\nTo continue type '0': ");
	input = getch();
	putch(input);
	while (input != '0')//while '0' was not entered.
	{
		printf("\nERROR! To continue type '0': ");
		input = getch();
		putch(input);
	}
	return;
}

void CreateProducts(PList MainMenue)
//Recieves fom file Manot.txt dishes, and saves it to the menue list.
{
	Manot *temp;
	FILE *in;
	char name[225], premium[2];
	int price, quantity, scanResault, errorFlag = 0, errorCounter = 0;

	in = fopen("Manot.txt", "rt");
	if (in == NULL)//If opening file failed.
		Error_Msg("Could not open file!");
	if ((temp = (Manot*)malloc(sizeof(Manot))) == NULL)//Allocates memory for one dish.
	{
		fclose(in);
		Error_Msg("Allocation of memory failed");
	}
	scanResault = fscanf(in, "%s%d%d%s", name, &quantity, &price, premium);//Scans the data from file, name, quantity, price, and premium ('N','Y').
	while (scanResault != EOF&&scanResault == 4)
	{
		if (CheckIfProductExists(MainMenue, name) == 1)//Checks if the product alredy is n the list, name is the indicator.
		{
			errorFlag = 1;
			printf("The dish alredy exists in stock.\n");
		}
		if (price < 0)//Checks if price is a negative number.
		{
			errorFlag = 1;
			printf("Price can not be a negative number.\n");
		}
		if (quantity < 0)//Checks if quantity is a negative number.
		{
			errorFlag = 1;
			printf("Quantity can not be a negative number.\n");
		}
		if (errorFlag == 0)//If there are no flaws in the data create a junction in the list.
		{
			if ((temp->ProductName = (char*)malloc((strlen(name) + 1) * sizeof(char))) == NULL)//Allocate memory for name.
			{
				fclose(in);
				FreeList(MainMenue);
				Error_Msg("Allocation of memory failed");
			}
			strcpy(temp->ProductName, name);//Copys the name to ProductName in the list.
			temp->Premium = premium[0];//Defines if the dish is premium or not.

			temp->next = MainMenue->head;//The list is defined one way, dishes are added to the top(head).
			temp->Quantity = quantity;
			temp->Price = price;
			MainMenue->head = temp;
			MainMenue->size++;

			if ((temp = (Manot*)malloc(sizeof(Manot))) == NULL)//Allocates memory for one dish.
			{
				fclose(in);
				FreeList(MainMenue);
				Error_Msg("Allocation of memory failed");
			}
		}
		else
			errorCounter++;
		scanResault = fscanf(in, "%s%d%d%s", name, &quantity, &price, premium);//Scans the data from file, name, quantity, price, and premium ('N','Y').
		errorFlag = 0;
	}
	free(temp);
	if (scanResault != EOF)
	{
		fclose(in);
		FreeList(MainMenue);
		Error_Msg("Error! Did not recieve dishes from file correctly\nThe file may be corrupt.");
	}
	fclose(in);
	if (errorCounter > 0)
		printf("The system created all dishes from Manot.txt accept of %d.\n", errorCounter);
	else
		printf("The system created all dishes from Manot.txt successfully\n");
}

int CheckIfProductExists(PList MainMenue, char *name)
//Checks if the product exists in the menue.
{
	Manot *temp;
	temp = MainMenue->head;
	while (temp != NULL)
	{
		if (strcmp(temp->ProductName, name) == 0)//The name eixsts in the list so return 1.
			return 1;
		temp = temp->next;
	}
	return 0;//The name dos not exists in the list return 0;
}

void AddItems(char *ProductName, int Quantity, PList MainMenue)
//Finds a specific dish and rises the asked quantity.
{
	int counter = 0, flag = 0;
	Manot *temp;
	if (Quantity < 0)//Checks if quantity is a negative number.
	{
		printf("\nError! Quantity can not be a negative number.\n"); return;
	}
	temp = MainMenue->head;
	while (temp != NULL&&flag == 0)//While we didnt got to the end of the list, and we still didnt found the dish.
	{
		if (strcmp(temp->ProductName, ProductName) == 0)//Checks if the product names are equal.
		{
			temp->Quantity += Quantity;//Rise the quantity.
			flag = 1;
		}
		else
			counter++;
		temp = temp->next;
	}
	if (counter == MainMenue->size)//If the counter got to the menue size that means there is no such a dish.
		printf("\n\"%s\" is not a dish in the Menue\n", ProductName);
	else
		printf("\n%d %s are been added to the stock successfully\n", Quantity, ProductName);
}

void OrderItem(int TableNumber, char *ProductName, unsigned int Quantity, PList MainMenue, Table *Tables)
//Adding a dish with its quantity to a table.
{
	int counterDishMenue = 0, flag = 0;
	Manot *temp, *tempTable, *newDishTable;

	if (TableNumber > 15 || TableNumber < 1)//Checks if the table number is between 1 - 15.
	{
		printf("\nTable %d doesn't exist in the restaurant.\n", TableNumber);
		return;
	}
	TableNumber--;//In order to work with the array;
	if (Quantity < 0)//Checks if quantity is a negative number.
	{
		printf("\nError! Quantity can not be a negative number.\n"); return;
	}
	temp = MainMenue->head;
	while (temp != NULL&&flag == 0)//While we didnt got to the end of the list, and we still didnt found the dish.
	{
		if (strcmp(temp->ProductName, ProductName) == 0)//Checks if the product names are equal.
		{
			if (temp->Quantity < Quantity)//Checks if the kitchen has enough dishes for the order.
			{
				printf("\nWe can not supply %d %s we just have %d in stock\n", Quantity, ProductName, temp->Quantity);
				return;
			}
			else
			{
				tempTable = Tables[TableNumber].head;
				while (tempTable != NULL&&strcmp(tempTable->ProductName, ProductName) != 0)//Checks if the dish (junction) alredy exists on the table.
					tempTable = tempTable->next;
				if (tempTable == NULL)//If there is no such a dish create a new junction.
				{
					if ((newDishTable = (Manot*)malloc(sizeof(Manot))) == NULL)//Allocates memory for the dish to the table.
					{
						FreeList(MainMenue);
						FreeTables(Tables);
						Error_Msg("Allocation of memory failed");
					}
					if ((newDishTable->ProductName = (char*)malloc((strlen(ProductName) + 1) * sizeof(char))) == NULL)//Allocates the name of the product for the table.
					{
						FreeList(MainMenue);
						FreeTables(Tables);
						Error_Msg("Allocation of memory failed");
					}
					strcpy(newDishTable->ProductName, ProductName);//Copys all the data of the dish.
					newDishTable->Price = temp->Price;
					newDishTable->Quantity = Quantity;
					newDishTable->Premium = temp->Premium;

					newDishTable->next = Tables[TableNumber].head;//Moves the head of the list upwoards.
					Tables[TableNumber].head = newDishTable;
				}
				else//If the dish already exists raise just the quantity.
					tempTable->Quantity += Quantity;
				temp->Quantity -= Quantity;//Dicrease quanity from the main menue.
				Tables[TableNumber].TotalPrice += (temp->Price)*Quantity;//Updating total price.
			}
			flag = 1;
		}
		else
			counterDishMenue++;
		temp = temp->next;
	}
	if (counterDishMenue == MainMenue->size)//If the counter got to the menue size that means there is no such a dish.
		printf("\n\"%s\" is not a dish in the Menue.\n", ProductName);
	else
		printf("\n%d %s was added to table %d seccessfully.\n", Quantity, ProductName, TableNumber + 1);

}

void RemoveItem(int TableNumber, char *ProductName, unsigned int Quantity, PList MainMenue, Table *Tables)
//Removes part or whole order from a table.
{
	Manot *temp, *prev;
	if (TableNumber > 15 || TableNumber < 1)//Checks if the table number is between 1 - 15.
	{
		printf("\nThe Table %d doesn't exist in the restaurant.\n", TableNumber); return;
	}
	TableNumber--;//In order to work with the array;
	if (Quantity < 0)//Checks if quantity is a negative number.
	{
		printf("Quantity needs to be a positive number.\n"); return;
	}
	if (CheckIfProductExists(MainMenue, ProductName) == 0)
	{
		printf("\n\"%s\" is not a dish in the Menue.\n", ProductName); return;
	}
	temp = Tables[TableNumber].head;
	while (temp != NULL&&strcmp(temp->ProductName, ProductName) != 0)//Checks if the Table ordered the dish.
		temp = temp->next;
	if (temp == NULL)
	{
		printf("\nTable %d did not order %s.\n", TableNumber + 1, ProductName);
		return;
	}
	else
	{
		if (temp->Quantity < Quantity)//Checks the input dish quantity if it is not greater to the table order.
		{
			printf("Table %d did not order so much %s.\n", TableNumber, ProductName); return;
		}
		if (temp->Quantity == Quantity)//If the quantities are equal then free the junction.
		{
			if (Tables[TableNumber].head == temp)
				Tables[TableNumber].head = Tables[TableNumber].head->next;
			else
			{
				prev = Tables[TableNumber].head;
				while (prev->next != temp)
					prev = prev->next;
				prev->next = temp->next;
			}
			free(temp);
			return;
		}
		else
			temp->Quantity -= Quantity;//Else just subtract between the quantities.
		CalculateTotalPrice(TableNumber, Tables);
	}
	printf("\n%d %s on table %d where removed successfully.\n", Quantity, ProductName, TableNumber + 1);
}

void RemoveTable(int TableNumber, Table *Tables)
//A report of what the table ordered and the price that needs to be paid.
{
	Manot *temp;
	int premium = 0;
	if (TableNumber > 15 || TableNumber < 1)//Checks if the table number is between 1 - 15.
	{
		printf("\nThe Table %d doesn't exist in the restaurant.\n", TableNumber);
		return;
	}
	TableNumber--;//In order to work with the array;
	temp = Tables[TableNumber].head;
	printf("\nTable %d Bill:\n", TableNumber + 1);
	while (temp != NULL)//Prints every dish that the Table ordered.
	{
		printf("Dish: %s, Quantity: %d, Price: %d\$\n", temp->ProductName, temp->Quantity, temp->Price);
		if (temp->Premium == 'Y')//Checks if the Table was a premium Table.
			premium = 1;
		temp = temp->next;
	}
	printf("------------------------------\n");
	if (premium == 1)
		printf("Your Table is a Premium Table that means 20%% for special Wines and personal Waiter.\n------------------------------\nTotal Cost: %.2f \$\n", Tables[TableNumber].TotalPrice + Tables[TableNumber].TotalPrice*0.2);
	else
		printf("Total Cost: %d \$\n	", Tables[TableNumber].TotalPrice);
	FreeOneTable(Tables[TableNumber].head);//Freeing the table.
	Tables[TableNumber].head = NULL;//Initializing the table if knew guest are coming.
	Tables[TableNumber].TotalPrice = 0;
}

void CalculateTotalPrice(int TableNumber, Table *Tables)
//Calculates and updates the total price of a table.
{
	Manot *temp;
	temp = Tables[TableNumber].head;
	Tables[TableNumber].TotalPrice = 0;
	while (temp != NULL)
	{
		Tables[TableNumber].TotalPrice += (temp->Quantity)*(temp->Price);
		temp = temp->next;
	}
}

void FreeList(PList MainMenue)
//Freeing the main menue list.
{
	Manot *temp;
	while (MainMenue->head != NULL)
	{
		temp = MainMenue->head;
		MainMenue->head = MainMenue->head->next;
		free(temp);
	}

}

void FreeTables(Table* Tables)
//Freeing al Tables.
{
	int i;
	for (i = 1; i < TABLE_NUMBERS; i++)
		FreeOneTable(Tables[i].head);
}

void FreeOneTable(Manot* Orders)
//Free just one all orders from a specific table.
{
	Manot *temp;
	while (Orders != NULL)
	{
		temp = Orders;
		Orders = Orders->next;
		free(temp);
	}
}

void Error_Msg(char *txt)
//Prints a message and exits th program.
{
	printf(txt);
	exit(1);
}


