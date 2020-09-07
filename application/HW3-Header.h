#ifndef  _HW3-Header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_NUMBERS 15

typedef struct Manot
{
	char *ProductName;
	unsigned int Price;
	unsigned int Quantity;
	char Premium;
	struct Manot *next;
}Manot;

typedef struct List
{
	Manot *head;
	int size;
}List, *PList;

typedef struct Table
{
	Manot *head;
	unsigned int TotalPrice;
}Table;


void GetFromUserZeroToContinue();
void CreateProducts(PList MainMenue);
int CheckIfProductExists(PList MainMenue, char *name);
void AddItems(char *ProductName, int Quantity, PList MainMenue);
void OrderItem(int TableNumber, char *ProductName, unsigned int Quantity, PList MainMenue, Table *Tables);
void RemoveItem(int TableNumber, char *ProductName, unsigned int Quantity, PList MainMenue, Table *Tables);
void RemoveTable(int TableNumber, Table *Tables);

void CalculateTotalPrice(int TableNumber, Table *Tables);
void FreeList(PList MainMenue);
void FreeTables(Table* Tables);
void FreeOneTable(Manot* Orders);
void Error_Msg(char *txt);

#endif 
