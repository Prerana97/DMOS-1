/*
Submission By 

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID: 1217164507

This q.h containg the basic queue functions like

NewItem:  This funciton will create a newnode of type TCB_t and assigns the context and threadid to the new node and address of that node is returned
newQueue: This will create newQueue by pointing headernode to null and will return the address of that header
AddQueue: This function will add the node TCT_t to the end of the circular linked list
DelQueue: This function will delete the TCB_t node for the front of the circular linked list and returns that node address to the calling function
RotateQueue: This function will rotate the queue i.e., will move the header to next element while is similar to deleting the first element and adding it to the end of the queue

*/

#include<stdio.h>
#include<stdlib.h>
#include "TCB.h"

/*struct linked_list{
    int number;
    struct linked_list *next;
    struct linked_list *previous;
};*/

typedef struct TCB_t node;  //giving the alias name to TCB_t struct as node


node* NewItem(node** TCB_data)

{
	node *newnode=(node *)malloc(sizeof(node));
	newnode->context=(*TCB_data)->context;
	newnode->threadid=(*TCB_data)->threadid;
	newnode->next=NULL;
	newnode->previous=NULL;
	return newnode;
}

node* newQueue()
{
	node *head=NULL;
	return head;
}

void AddQueue(node** head,node** item)
{
	node *newnode=*item;
	if(*head==NULL) //This condition will check if there are no elements in the Queue then this will add the first element to queue and points the next and previos to itself making it circular
	{
		*head=newnode;
		(*head)->next=newnode;
		(*head)->previous=newnode;
	}
	else  //otherwise it will traverse till the last element and will add the new node at the end and adjust the previous and next of last but one and first node accordingly
	{
		node *current=*head;
		do
		{
			current=current->next;	
		}while(current->next!=*head);	
		(*head)->previous=newnode;
		newnode->next=*head;
		current->next=newnode;
		newnode->previous=current;
			
	}
}
node* DelQueue(node** head)
{
	node *current=*head;
	if(*head==NULL) return *head;	//This condition will check if there is no elements in the queue then this will return null
	else{  //otherwise that element will be removed from the head and the next and previous of last node and second node are adjusted accordingly
		node *temp=(*head)->next;
		if((*head)==temp){  *head=NULL;  return current;} //if there is only one element in the queue then this again make the queue as empty
		temp->previous=current->previous;
		current->previous->next=temp;
		*head=temp;
	}
	node *tempNode=current;  //The address of element that is deleted will be returned to the calling function
	//free(current);
	return tempNode;
}

void RotateQueue(node** head) //will rotate the queue by moving the header to the next element
{
	if(*head!=NULL)
	{
		*head=(*head)->next;
	}
}
void print_list(node** head) //will print the list of elements in the ReadyQ
{
    if(*head==NULL){ printf("\nEmpty");  return;}
    node *current = *head;
    do
    {
        printf("%d\t", current->threadid);
        current = current->next;
    } while(current != *head);
    printf("\n");
}

/*int main()
{
    return 0;
}*/

