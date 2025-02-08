//abdallah kokash_1220116_sec3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 50
//definitions
typedef struct nodeQ* queue;
typedef struct nodeS* stack;
typedef struct nodeS* nodeSt;
typedef struct nodeQ* nodeQu;
///////////////////////////////////////////
//structures
struct nodeS {
    struct nodeS* next;
    int index;
    char operation[30];
    char token[30];
};
struct nodeQ {
    struct nodeQ* next;
    char String[30];
};
////////////////////////////
//function
stack createStack();
int isEmptyS();
stack pop(stack s);
void push(stack s,char String[30],char operation[30],int index);
queue makeQueue();
int isEmptyQ(queue q);
char* dequeue(queue q);
void enqueue(queue q,char String[30]);
void insert(queue addQueue,stack undoStack,stack redoStack);
int find(char loc[30]);
void shiftDown(int index);
void Remove(stack undoStack,stack redoStack);
void shifyUp(int index);
int exist(char line[100]);
void pirntStack(stack undoStack,stack redoStack);
void shiftAll();
void clear(stack redoStack);
void save();
void redo(stack undoStack,stack redoStack);
void undo(stack undoStack,stack redoStack);
void print();
void load();
/////////////////////////////////////
//global variables
char sentence [size][size];
int realsize =0;
struct nodeQ* Front;
struct nodeQ* rear;

/////////////////////////////////
int main() {
    //here we define the two stack and the queue that will be used in the whole project
    stack undoStack=createStack();
    stack redoStack=createStack();
    queue addQueue=makeQueue();
    //menu
    int op;
    do {
        printf("\nmenu\n");
        printf("1.Load the input file which contains the initial text.\n");
        printf("2.Print the loaded text.\n");
        printf("3.Insert strings to the text.\n");
        printf("4.Remove strings from the text.\n");
        printf("5.Perform Undo operation.\n");
        printf("6.Perform Redo operation.\n");
        printf("7.Print the Undo Stack and the Redo stack.\n");
        printf("8.Save the updated text to the output file.\n");
        printf("9.exit\n");
        printf("please enter operation: ");
        //here we check if the input entered by the user is not a character
        while(scanf("%d",&op) != 1) {
            while(getchar() != '\n');
            printf("invalid input\n");
        }
        switch(op) {
        case 1:
            load();
            break;
        case 2:
            print();
            break;
        case 3:
            insert(addQueue,undoStack,redoStack);
            break;
        case 4:
            Remove(undoStack,redoStack);
            break;
        case 5:
            undo(undoStack,redoStack);
            break;
        case 6:
            redo(undoStack,redoStack);
            break;
        case 7:
            pirntStack(undoStack,redoStack);
            break;
        case 8:
            save();
            break;
        case 9:
            printf("exiting system.....\n");
            break;
        default:
            printf("not a correct operation!!...please enter a valid operation\n");
            break;

        }

    } while(op != 9);

}
//load function that will load the text from the file
void load() {
    printf("\n");
    char line[100];
    FILE *input_file;
    input_file = fopen("input.txt","r");
    if(input_file == NULL) {
        printf("error opening file\n");
    } else {
        while(fgets(line,200,input_file)!=NULL) {
            //to change the the new line character into an '\0' so it sorest them  in a one big line
            if(line[strlen(line)-1] == '\n')
                line[strlen(line)-1]='\0';
            //here we split each word and move them in the 2d array
            char *ptr =strtok(line," ");
            while(ptr !=NULL) {
                strcpy(sentence[realsize],ptr);
                realsize++;
                ptr = strtok(NULL," ");
            }

        }

        printf("file loaded successfully\n");
    }
}
void shiftDown(int index) {
    for(int i=realsize-1; i>index; i--) {
        strcpy(sentence[i+1],sentence[i]);
    }
}
//the shift function the shift the rows upward
void shiftUp(int index) {
    for(int i=index; i<=realsize-1; i++) {
        strcpy(sentence[i],sentence[i+1]);
    }
}
//function to print the loaded text
void print() {
    printf("\n");
    if(sentence[0][0] == '\000')
        printf("empty file\n");
    else {
        for(int i=0; i<realsize; i++)
            printf("%s ",sentence[i]);
        printf("\n");
    }
}
//function to insert in the text
void insert(queue addQueue,stack undoStack,stack redoStack) {
    printf("\n");
    int op;
    char* word;
    char line[100];
    int index;
    if(sentence[0][0] == '\000')
        printf("empty file\n");
    else {
        printf("enter 1 if you want to insert after a word or enter 2 if you want to insert last and 3 if you want to insert first \n");
        while(scanf("%d",&op) != 1) {
            while(getchar() != '\n');
            printf("invalid input\n");
        }
        if(op ==1) {
            char loc[30];
            printf("enter word you want to insert after\n");
            scanf("%s",loc);
            //here we call function find to find the row the contains the word the the user inserted
            index = find(loc);
            if(index == -2) {
                printf("the word you inserted does not exist\n");
                return;
            }
            printf("enter what you want to insert\n");
            //here we empty the input buffer before loading the newly inserted text to the line array
            while(getchar() != '\n');
            gets(line);
            char *ptr =strtok(line," ");
            while(ptr !=NULL) {
                //here we put each word in a queue which later will be Dequeued
                enqueue(addQueue,ptr);
                ptr = strtok(NULL," ");
            }
            while(!isEmptyQ(addQueue)) {
                word = dequeue(addQueue);
                //here we call shift function to open a new row for inserting
                shiftDown(index);
                //after shift the word will be inserted in the correct location and pushed into the undoStack
                strcpy(sentence[index+1],word);
                realsize++;
                index++;
                push(undoStack,word,"insert",index);
                free(word);
            }
            printf("insert was successfully\n");
            clear(redoStack);
        }
        //here if the op is 2 which means we insert last which is the same as above except we do not need to call find and the index will be the last row
        else if(op==2) {
            index =realsize-1;
            printf("enter what you want to insert\n");
            while(getchar() != '\n');
            gets(line);
            char *ptr =strtok(line," ");
            while(ptr !=NULL) {
                enqueue(addQueue,ptr);
                ptr = strtok(NULL," ");
            }
            while(!isEmptyQ(addQueue)) {
                word = dequeue(addQueue);
                shiftDown(index);
                strcpy(sentence[index+1],word);
                realsize++;
                index++;
                push(undoStack,word,"insert",-1);
                free(word);
            }
            printf("insert was successfully\n");
            clear(redoStack);
        }
        //here we are adding first
        else if(op ==3) {
            index=0;
            printf("enter what you want to insert\n");
            while(getchar() != '\n');
            gets(line);
            char *ptr =strtok(line," ");
            while(ptr !=NULL) {
                enqueue(addQueue,ptr);
                ptr = strtok(NULL," ");
            }
            //here we shift all the element by 1 row
            shiftAll();
            while(!isEmptyQ(addQueue)) {
                word = dequeue(addQueue);
                strcpy(sentence[index],word);
                realsize++;
                push(undoStack,word,"insert",index);
                free(word);
                if(isEmptyQ(addQueue))
                    break;
                shiftDown(index);
                index++;
            }
            printf("insert was successfully\n");
            clear(redoStack);
        } else
            printf("not a correct operation\n");

    }
}
//shift function the will push all the rows down by 1 row
void shiftAll() {
    for(int i=realsize-1; i>=0; i--) {
        strcpy(sentence[i+1],sentence[i]);
    }
}
//remove function
void Remove(stack undoStack,stack redoStack) {
    printf("\n");
    char line[100];
    char temp[100];
    char loc[30];
    int index;
    int point;
    if(sentence[0][0] == '\000')
        printf("empty file\n");
    else {
        printf("enter what you want to delete\n");
        while(getchar() != '\n');
        gets(line);
        //here we make a copy of the inserted text which will be given to the exist function
        strcpy(temp,line);
        //to check if the given text is in the loaded text
        int ch = exist(temp);
        if( ch== 1) {
            printf("what you have entered does not exist\n");
            return;
        }
        char *ptr =strtok(line," ");
        point=find(ptr);
        index=point;
        int counter=0;
        while(ptr !=NULL) {
            //here we shift the rows up so that the text will be deleted and then pushed to the undoStack
            shiftUp(point);
            push(undoStack,ptr,"remove",index-counter);
            index++;
            realsize--;
            ptr = strtok(NULL," ");
            counter++;
        }
        printf("delete was successful\n");
        clear(redoStack);
    }

}
//undo function that will reverse an action
void undo(stack undoStack,stack redoStack) {
    printf("\n");
    if(isEmptyS(undoStack)) {
        printf("undoStack is empty\n");
        return ;
    } else {
        nodeSt info =pop(undoStack);
        //if the operation is insert it will remove the word and if it is remove it will insert
        if(strcasecmp(info->operation,"insert")==0) {
            if(info->index == -1) {
                shiftUp(realsize-1);
                realsize--;
                push(redoStack,info->token,info->operation,info->index);
                free(info);

            } else {
                shiftUp(info->index);
                realsize--;
                push(redoStack,info->token,info->operation,info->index);
                free(info);
            }
        } else {
            if(info->index==0) {
                shiftAll();
                strcpy(sentence[info->index],info->token);
                realsize++;
                push(redoStack,info->token,info->operation,info->index);
                free(info);
            } else {
                shiftDown(info->index-1);
                strcpy(sentence[info->index],info->token);
                realsize++;
                push(redoStack,info->token,info->operation,info->index);
                free(info);
            }
        }
    }
    printf("undo was successful\n");
}
//redo function that will reverse the undo action
void redo(stack undoStack,stack redoStack) {
    printf("\n");
    if(isEmptyS(redoStack)) {
        printf("redoStack is empty\n");
        return ;
    } else {
        nodeSt info =pop(redoStack);
        //exactly the opposite of undo
        if(strcasecmp(info->operation,"insert")==0) {
            if(info->index == -1) {
                shiftDown(realsize-1);
                strcpy(sentence[realsize],info->token);
                realsize++;
                push(undoStack,info->token,info->operation,info->index);
                free(info);

            } else if(info->index==0) {
                shiftAll();
                strcpy(sentence[info->index],info->token);
                realsize++;
                push(undoStack,info->token,info->operation,info->index);
                free(info);
            } else {
                shiftDown(info->index-1);
                strcpy(sentence[info->index],info->token);
                realsize++;
                push(undoStack,info->token,info->operation,info->index);
                free(info);
            }
        } else {
            shiftUp(info->index);
            realsize--;
            push(undoStack,info->token,info->operation,info->index);
            free(info);
        }
    }
    printf("redo was successful\n");
}
//function to clear the redo stack
void clear(stack redoStack) {
    nodeSt info;
    while(!isEmptyS(redoStack)) {
        info = pop(redoStack);
        free(info);
    }
}
//the find function the will search in every row for the given word and return its index and if not it is not found is returns -2
int find(char loc[30]) {
    int index =-2;

    for(int i=0; i<realsize; i++) {
        if(strcasecmp(sentence[i],loc)==0) {
            index=i;
            break;
        }

    }
    return index;
}
//the shift function the shift the rows downward

//the check function the will check if a given word or sentence exist in the text we have
int exist(char line[100]) {
    int index;
    //here we split the text to check each word with the sentence array and if there is an error it returns 1
    char *ptr =strtok(line," ");
    index=find(ptr);
    while(ptr !=NULL) {
        if(strcasecmp(sentence[index],ptr)!=0)
            return 1;
        index++;
        ptr = strtok(NULL," ");
    }
}
//here we want to print the two stacks by poping each node and printing its information on the screen and then pushing it into
//a temporary stack and then moving the node of the temporary stack to the undo or redo stack again to keep the arrangement
void pirntStack(stack undoStack,stack redoStack) {
    printf("\n");
    if(isEmptyS(undoStack))
        printf("undoStack is empty\n");
    else {
        stack temp = createStack();
        printf("undoStack\n");
        printf("%-20s%-15s%-6s\n","Token","Operation","Index");
        while(!isEmptyS(undoStack)) {
            nodeSt info=pop(undoStack);
            printf("%-20s%-15s%-6d\n",info->token,info->operation,info->index);
            push(temp,info->token,info->operation,info->index);
            free(info);
        }
        while(!isEmptyS(temp)) {
            nodeSt info1=pop(temp);
            push(undoStack,info1->token,info1->operation,info1->index);
            free(info1);
        }
        free(temp);
    }
    printf("\n");
    if(isEmptyS(redoStack))
        printf("redoStack is empty\n");
    else {
        stack temp = createStack();
        printf("redoStack\n");
        printf("%-20s%-15s%-6s\n","Token","Operation","Index");
        while(!isEmptyS(redoStack)) {
            nodeSt info=pop(redoStack);
            printf("%-20s%-15s%-6d\n",info->token,info->operation,info->index);
            push(temp,info->token,info->operation,info->index);
            free(info);
        }
        stack temp1 = createStack();
        while(!isEmptyS(temp)) {
            nodeSt info1=pop(temp);
            push(redoStack,info1->token,info1->operation,info1->index);
            free(info1);
        }
        free(temp);
    }

}
void save() {
    printf("\n");
    FILE *out_file;
    out_file = fopen("output.txt","w");
    if(out_file == NULL) {
        printf("error opening file");
    } else {
        for(int i=0; i<realsize; i++)
            fprintf(out_file,"%s ",sentence[i]);
    }
    printf("save was successfull\n");
    fclose(out_file);
}
//function to create the stack
stack createStack() {
    stack s;
    s=(stack)malloc(sizeof(struct nodeS));
    if(s==NULL)
        printf("memory is full");
    else
        s->next=NULL;
    return s;
}
//to check if the stack is empty
int isEmptyS(stack s) {
    return s->next==NULL;
}
//pop function the will pop the node from the top of the stack and return a pointer to it
stack pop(stack s) {
    nodeSt firstcell;
    if(isEmptyS(s))
        printf("stack is empty");
    else {
        firstcell=s->next;
        s->next=s->next->next;
    }
    return firstcell;
}
//push function the will inert in the stack
void push(stack s,char String[30],char operation[30],int index) {
    nodeSt temp;
    temp=(nodeSt)malloc(sizeof(struct nodeS));
    if(temp == NULL)
        printf("memory is full");
    else {
        strcpy(temp->token,String);
        strcpy(temp->operation,operation);
        temp->index=index;
        temp->next=s->next;
        s->next=temp;
    }
}
//function to make the queue
queue makeQueue() {
    nodeQu q;
    q=(queue)malloc(sizeof(struct nodeQ));
    if(q == NULL)
        printf("memory is full");
    else {
        q->next=NULL;
        Front=q;
        rear=q;
    }
    return q;
}
//to check if the queue is empty
int isEmptyQ(queue q) {
    return q->next==NULL;
}
//dequeue function the will remove the node at the front of the queue and return the word that was in it
char* dequeue(queue q) {
    nodeQu temp;
    char* word;
    if(isEmptyQ(q))
        printf("queue is empty\n");
    else {
        temp =q->next;
        word = (char *)malloc(sizeof(char));
        strcpy(word,temp->String);
        if(q->next ==rear) {
            temp=q->next;
            q->next =NULL;
            free(temp);
            rear=q;
            Front=q;
        } else {
            temp=q->next;
            q->next=Front->next;
            Front=Front->next;
            free(temp);
        }

    }
    return word;
}
// function to insert in the queue at the last position
void enqueue(queue q,char String[30]) {
    nodeQu temp;
    temp=(nodeQu)malloc(sizeof(struct nodeQ));
    if(temp == NULL)
        printf("memory is full");
    else {
        if(q->next==NULL) {
            strcpy(temp->String,String);
            temp->next=q->next;
            q->next=temp;
            rear=temp;
            Front=temp;

        } else {
            strcpy(temp->String,String);
            temp->next=rear->next;
            rear->next=temp;
            rear=temp;
        }

    }

}


