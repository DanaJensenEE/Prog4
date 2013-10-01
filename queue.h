/**************************************************************************//**
* @file
 *****************************************************************************/
/**************************************************************************//**
Author: Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
Date: March 25, 2013
Teacher: Roger Schrader
Program Description
 *****************************************************************************/
#include <iostream>
#include <list>
#include <iomanip>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include <io.h>

/******************************************************************************
 *							User defined Structures							  *
 *****************************************************************************/
/*!
 * @brief structure that holds information about the documents added to the 
 *	print queue
 */
struct document
{
	int pages;              /*!<Number of page in Document*/
	int time_arrived;       /*!<Number of seconds from start of day*/
	int time_dequed;	/*!<Time in seconds document was removed from queue*/
	int time_started_print; /*!<Time when document has started printing */
	int time_end_printing;  /*!<Time when document has finished printing*/
};

/*!
 * @brief structure for nodes in the linked list.
 */
template <class _TY>
struct node
{
	_TY item;		/*!< The object in the node */
	node *next;		/*!< Pointer to the next node in the list */
};



/*!
 * @brief queue class
 *
 */
template <class _TY>
class queue
{
	public:
		queue();
		~queue();

		bool enque ( _TY item );
		bool deque ( _TY &item );
		bool front ( _TY &item );

		bool is_empty();

	private:
		node<_TY> *headptr;
		node<_TY> *tailptr;
};
/*!
 * @brief queue class constructor
 *
 */
template <class _TY>
queue<_TY>::queue()
{
	headptr=NULL;
	tailptr=NULL;
}
/*!
 * @brief queue class destructor
 *
 */
template <class _TY>
queue<_TY>::~queue()
{
	node<_TY> *temp;
	temp=headptr;
	while(temp!=NULL)
	{
		headptr=headptr->next;
		delete(temp);
		temp=headptr;
	}

}
/*!
 * @brief queue class enque - adds items to the end of the queue
 *
 */
template <class _TY>
bool queue<_TY>::enque ( _TY item )
{
	  node<_TY> *temp;

	  temp = new(nothrow) node<_TY>;
	  if(temp == NULL)
		 return 0;
	  temp->item = item;
	  temp->next = NULL;
	  if (headptr == NULL)
	  {
		headptr = temp;
		tailptr = temp;
	  }
	  else
	  {
		tailptr->next = temp;
		tailptr = temp;
	  }
	  return 1;
}
/*!
 * @brief queue class deque - removes items from the front of the queue
 *
 */
template <class _TY>
bool queue<_TY>::deque ( _TY &item )
{
	node<_TY> *temp;

  if (is_empty())
	return 0;
  else
  {
	temp = headptr;
	item = temp->item;
	headptr = headptr->next;
	delete(temp);
	if(headptr == NULL)
		tailptr = NULL;

  }  
  return 1;
}
/*!
 * @brief finds the value of the first item in the queue
 *
 */
template <class _TY>
bool queue<_TY>::front ( _TY &item )
{
	if (headptr == NULL)
		return 0;
	else
	{
		item = headptr->item;
		return 1;
	}
	return 0;
}
/*!
 * @brief checks if the queue is empty
 *
 */
template <class _TY>
bool queue<_TY>::is_empty()
{
  if (headptr == NULL)
	return(1);
  else
	return(0);
}
