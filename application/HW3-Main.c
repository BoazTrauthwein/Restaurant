#include "HW3-Header.h"

void main()
{
	int i, funcNum, tableNumber, quantity;
	char *productName[50];
	FILE *in;
	List MainMenue;
	Table Tables[TABLE_NUMBERS];

	in = fopen("Instructions.txt", "rt");
	if (in == NULL)
		Error_Msg("Could not open file!");

	MainMenue.head = NULL;//Initializing main menue
	MainMenue.size = 0;
	for (i = 0; i < TABLE_NUMBERS; i++)//Initializing all Tables.
	{
		Tables[i].head = NULL;
		Tables[i].TotalPrice = 0;
	}
	printf("Functions Menue");
	printf("\n1 - CreateProducts: Recieves fom file Manot.txt dishes, and saves it to the menue list");
	printf("\n2 - AddItems: Finds a specific dish and rises the asked quantity.");
	printf("\n3 - OrderItem: Adding a dish with its quantity to a table.");
	printf("\n4 - RemoveItem: Removes part or whole order from a table.");
	printf("\n5 - RemoveTable: A report of what the table ordered and the price that needs to be paid.");
	GetFromUserZeroToContinue();//Getting from user '0'
	printf("\n");
	while (fscanf(in, "%d", &funcNum) != EOF)
	{
		switch (funcNum)
		{
		case 1:
			CreateProducts(&MainMenue);
			break;
		case 2:
			if (fscanf(in, "%s%d", productName, &quantity) == 2)
				AddItems(productName, quantity, &MainMenue);
			break;
		case 3:
			if (fscanf(in, "%d%s%d", &tableNumber, productName, &quantity) == 3)
				OrderItem(tableNumber, productName, quantity, &MainMenue, Tables);
			break;
		case 4:
			if (fscanf(in, "%d%s%d", &tableNumber, productName, &quantity) == 3)
				RemoveItem(tableNumber, productName, quantity, &MainMenue, Tables);
			break;
		case 5:
			if (fscanf(in, "%d", &tableNumber) == 1)
				RemoveTable(tableNumber, Tables);
			break;
		default:
			break;
		}
		GetFromUserZeroToContinue();//Getting from user '0'
	}
	printf("\nEnd of the Program\n");
	FreeList(&MainMenue);
	FreeTables(Tables);
}