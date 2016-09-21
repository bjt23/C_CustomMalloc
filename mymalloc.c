/* Ben Tomasulo
 * CS0499
 * Project 3 
 * Implementation File */

#include <stdio.h>
#include "mymalloc.h"

//node for linked lists of memory
struct mnode {
	//size of memory chunk
	int msize;
	//free or used flag, '1' for used, '0' for free
	char used;
	//pointer to next node
	struct mnode *next;
	//pointer to previous node
	struct mnode *prev;
	//pointer to data (used for return vals in malloc)
	void *mdata;
};

//global root struct
struct mnode *mroot;
//current node for next fit
struct mnode *nf;
//end node in list struct
struct mnode *mend;

void *my_nextfit_malloc(int size){

	struct mnode *curr;
	//check for no instantiated root
	if(!mroot){
		//set new pointer location
		mroot=(struct mnode *)(sbrk(0));
		
		//resize brk
		sbrk((size+sizeof(struct mnode))*(size+sizeof(struct mnode)));
		
		mroot->msize=size;
		mroot->used='1';
		//set return value
		mroot->mdata=(void *)(mroot+sizeof(struct mnode));

		//sets initial end
		curr=mroot;
		mend=curr;
		//sets initial nf
		nf=curr;
		
		//sets nulls
		mroot->prev=NULL;
		mroot->next=NULL;
		
		return mroot->mdata;
	}
	
	//next fit loop
	curr=nf;
	while(1){
		
		//check for unused and correct size
		if(((curr->used)=='0')&&((curr->msize)>=size)){
			//set memory as used
			curr->used='1';
			//increment next fit tracker
			nf=nf->next;
			//return pointer after struct
			return curr->mdata;
		}
		//check for wraparound
		else if(!(curr->next)){
			curr=mroot;
		}
		//else go to next value in list
		else{
			curr=curr->next;
		}
		
		//check for end of list and increment brk if no fits found
		if(curr==nf){
			//save old end
			struct mnode *oldend=mend;
			
			//resize brk
			sbrk((size+sizeof(struct mnode))*(size+sizeof(struct mnode)));
			
			//set new pointer location
			curr=(struct mnode *)(mend+sizeof(struct mnode)+mend->msize);
			
			curr->msize=size;
			curr->used='1';
			curr->prev=oldend;
			//set return value
			curr->mdata=(void *)(curr+sizeof(struct mnode));
			void *rval=curr->mdata;
			
			//sets old end next
			oldend->next=curr;
			//changes end
			mend=curr;
			
			//sets null
			curr->next=NULL;
			
			//printf("data:%p\n",rval);
			return rval;
		}
	}
	
	//should never throw
	return (void *)('0');
}

void my_free(void *ptr){
	//access node to be freed
	struct mnode *curr=(struct mnode *)(ptr-sizeof(struct mnode));
	//set free
	curr->used='0';
	
	//check if next node free for coalescing
	if((curr->next)&&(((curr->next)->used)=='0')){
		curr->msize=((curr->msize)+((curr->next)->msize)+sizeof(struct mnode));
		curr->next=(curr->next)->next;
	}
	//check if previous node free for coalescing
	if((curr->prev)&&(((curr->prev)->used)=='0')){
		
		//move vals back to coalesce
		(curr->prev)->msize=((curr->msize)+sizeof(struct mnode)+((curr->prev)->msize));
		(curr->prev)->next=curr->next;
		//don't have to change prev, data, or used
	}
}