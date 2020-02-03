#include "my402list.h"


//lenght of the list
 int  My402ListLength(My402List* list)
{
	return list->num_members;
}



//check if the list is empty
int  My402ListEmpty(My402List*list)
{
	int num = list->num_members;
	if (num == 0)
	{
		return TRUE;
	}
	return FALSE;
}



/*
	Append to the end of the list or the first element if the list is empty. 
	Returns true if the appending the new element to the end was successful.
	
	PRE: list must be a valid pointed for the append to work, If not, append returns FALSE.
*/
int  My402ListAppend(My402List* list, void* object)
{
	if(list != NULL)
	{
		My402ListElem * newElement = malloc(sizeof(My402ListElem));
		if(newElement == NULL)//Malloc failure check;
		{
			printf("Malloc Falied to allocate memory\n");
			exit(1);
		}

		if(My402ListEmpty(list)) //list is empty
		{
			list->anchor.next = newElement;
			newElement-> prev = &list->anchor;
		}
		else 
		{
			My402ListElem *last = My402ListLast(list);//getting the last element
			last->next = newElement;
			newElement->prev = last;
		}
		list->anchor.prev = newElement;
		newElement->next = &list->anchor;
		newElement->obj = object;
		list->num_members++;
		return TRUE;
	}
	return FALSE;
}




int  My402ListPrepend(My402List* list, void* object)
{
	if(list != NULL) //valid list
	{
		My402ListElem * newElement = malloc(sizeof(My402ListElem));
		if(newElement == NULL)//Malloc failure check;
		{
			printf("Malloc Falied to allocate memory\n");
			exit(1);
		}

		if(My402ListEmpty(list)) //list is empty, prepend is same as append.
		{
			My402ListAppend(list, object);
		}
		else 
		{
			My402ListElem * firstElementPtr = My402ListFirst(list);
			list->anchor.next = newElement;
			newElement->prev = &list->anchor;
			newElement->next = firstElementPtr;
			firstElementPtr->prev = newElement;
			newElement->obj = object;
			list->num_members++;
		}
		return TRUE;
	}
	return FALSE;
}



/*
	Unlinks the given elem from the list. If the list has 1 element, it emtpies the list.

	PRE: elem must be a valid My402ListElem pointer in the list.
*/
void My402ListUnlink(My402List* list, My402ListElem* elem)
{
	My402ListElem* next = My402ListNext(list, elem);
	My402ListElem* previous = My402ListPrev(list, elem);
	if(next == NULL && previous == NULL) //elem is the only element in the list
	{
		list->anchor.next = NULL;
		list->anchor.prev = NULL;
	}
	else if(next == NULL)
	{
		previous->next = &list->anchor;
		list->anchor.prev = previous;
	}
	else if(previous == NULL)
	{
		list->anchor.next = next;
		next->prev = &list->anchor;
	}
	else
	{
		previous->next = next;
		next->prev = previous;
	}
	free(elem);
	list->num_members--;
}


void My402ListUnlinkAll(My402List* list)
{
	My402ListElem* elem;
	My402ListElem* toUnlink;
	int increm = 0;
	for(elem = My402ListFirst(list); elem != NULL; elem = My402ListNext(list, elem))
	{
		toUnlink = elem;
		toUnlink->obj = NULL;
		free(toUnlink);
		increm++;
	}
	list->anchor.next = NULL;
	list->anchor.prev = NULL;
	list->num_members = 0;
	toUnlink = NULL;
}


/*
	Creates a new My402ListElem and inserts it after the elem in the list. Assigns the obj of the 
	new element to the object.
*/
int  My402ListInsertAfter(My402List* list, void* object, My402ListElem* elem)
{
	My402ListElem * newElement = malloc(sizeof(My402ListElem));
	if(newElement == NULL)//Malloc failure check;
	{
		printf("Malloc Falied to allocate memory\n");
		exit(1);
	}

	My402ListElem* nextElem = elem->next;
	if(nextElem != NULL)
	{
		elem->next = newElement;
		newElement->prev = elem;
		newElement->next = nextElem;
		nextElem->prev = newElement;
		newElement->obj = object;
		list->num_members ++;
		return TRUE;
	}
	return FALSE;
}


/*
	Creates a new My402ListElem and inserts it before the elem in the list. Assigns the obj of the 
	new element to the object.
*/
int  My402ListInsertBefore(My402List* list, void* object, My402ListElem* elem)
{
	My402ListElem * newElement = malloc(sizeof(My402ListElem));
	if(newElement == NULL)//Malloc failure check;
	{
		printf("Malloc Falied to allocate memory l176\n");
		exit(1);
	}
	My402ListElem* prevElem = elem->prev;

	if(prevElem != NULL)
	{
		elem->prev = newElement;
		newElement->next=elem;
		newElement->prev = prevElem;
		prevElem->next = newElement;
		newElement->obj = object;
		list->num_members++;
		return TRUE;
	}
	return FALSE;
}


/*
	Returns the first element in the list or NULL if the list is empty

	PRE: The list has to be valid list
*/
My402ListElem *My402ListFirst(My402List* list)
{
	if (list->anchor.next == NULL) //list is empty
	{
		return NULL;
	}
	return list->anchor.next;
}

/*
	Returns the last element in the list or NULL if the list is empty.
	
	PRE: The list has to be a valid list.
*/
My402ListElem *My402ListLast(My402List* list)
{
	if (list->anchor.prev == NULL) //list is empty
	{
		return NULL;
	}
	return list->anchor.prev;
}



/*
	Returns the next element in the list or NULL if the list is empty or if the
	next element is the anchor.

	PRE: elem should be a valid pointer(not NULL)
*/
My402ListElem *My402ListNext(My402List* list, My402ListElem* elem)
{
	if(elem->next == &list->anchor)
	{
		return NULL;
	}
	else return elem->next;	
}

/*
	Returns the previous element in the list or NULL if the list is empty or the 
	previous element is the list anchor.

	PRE: The elem shoud be a valid pointer.
*/
My402ListElem *My402ListPrev(My402List* list, My402ListElem* elem)
{
	if(elem->prev == &list->anchor)
	{
		return NULL;
	}
	return elem->prev;
}




//Finds and returns a My402ListElem pointer if the obj pointer is found.
My402ListElem *My402ListFind(My402List* list, void* object)
{
	if (list->anchor.next == NULL) //check if the list is empty
	{
		return NULL;
	}

	My402ListElem * elem;

	for(elem = My402ListFirst(list); elem != NULL; elem = My402ListNext(list, elem))
	{
		if (elem->obj == object)
		{
			return elem->obj;
		}
	}
	return NULL; //The element was not found.
}





//Initializing an empty list. The prev and next pointers are null and obj is pointing to the list.
int My402ListInit(My402List* list)
{
	if(list == NULL)//Malloc failure check;
	{
		printf("Malloc Falied to allocate memory\n");
		exit(1);
	}

	
	My402ListElem * pAnchor = malloc(sizeof(My402ListElem));
	if(pAnchor == NULL) //Malloc failure check.
	{
		printf("Malloc Falied to allocate memory\n");
		exit(1);
	}
	list->anchor = *(pAnchor);
	list->anchor.next = NULL;
	list->anchor.prev = NULL;
	list->anchor.obj = list;
	list->num_members = 0;
	return TRUE;
}

