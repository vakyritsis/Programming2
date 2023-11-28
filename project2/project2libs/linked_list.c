#include<stdio.h>
#include<stdlib.h>
#include"linked_list.h"



void list_init(list **head){ //init the list
    *head = NULL;
}

int list_find(list *head, unsigned short COURSE_NR){ //search the list for a course number returns 0=1, not found- found
    list *curr;
    
    for(curr = head; (curr != NULL) && (curr->course_nr != COURSE_NR); curr = curr->next);
    
    return (curr != NULL);
}

list *newNode(unsigned short COURSE_NR){ //this function's job is to allocate memory for a node for the course list and to return the adress of the pointer.
    list *curr;
    
    curr = (list*)malloc(sizeof(list));
    
    curr->course_nr = COURSE_NR;
    curr->next = NULL;
    
    return curr;
}

void list_add(list **head, list *new_node){ //inserts the course's number to the students list with increasing order
    list *curr;
    
    if(*head == NULL || (*head)->course_nr >= new_node->course_nr){
        new_node->next = *head; 
        *head = new_node; 
    }
    else{
        curr = *head;
        while(curr->next != NULL && curr->next->course_nr < new_node->course_nr){
            curr = curr->next;
        }
        new_node->next = curr->next; 
        curr->next = new_node; 
    }
}

void list_rmv(list **head, unsigned short COURSE_NR){ //removes the course number from students list
    list *curr, *prev;
    
    for(curr = *head; (curr != NULL) && (curr->course_nr != COURSE_NR); prev = curr, curr = curr->next);
    
    if(curr != NULL){
        if(curr == *head)
            *head = curr->next;
        else
            prev->next = curr->next;
        free(curr);
    }
}

void list_print(list *head){ //for a given student it prints the lessons that he registered for
    list *curr;
    
    curr = head;
    while(curr != NULL){
        printf("%hu\n", curr->course_nr);
        curr = curr->next;
    }
}

void list_clear(list **head){ //clears the list of subjects for a student
    list *curr, *next;
    
    curr = *head;
    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    *head = NULL;
} 
