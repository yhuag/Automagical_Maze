
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct {
    int x;
    int y;
} Point;

struct Node{
    int x;
    int y;
    Node* next;
};

Point pt(int, int);
int visit(int**, Point, Point, Node**);
void print(int**, int);
void print(int**, int, Node*);

void Bias(bool biasRight, int& stepcnt, int valBiasDown){   //Incline Stabilize
	if(biasRight){
		if(stepcnt>50) stepcnt-=1;   //Normal Decrement
		else if(stepcnt<=49) stepcnt=49;   //Initial Offsetting
		else stepcnt=valBiasDown;   //Weighted Incerment
	} else {
		if(stepcnt<50) stepcnt+=1;   //Normal Increment
		else if(stepcnt>=51) stepcnt=51;   //Initial Offsetting
		else stepcnt=0;   //Weighted Decerment
	}
}


int main(void) {
	srand(time(0));
	int SIZE;
	printf("Please Input An Integer\nIt Will Yield An n*n Maze¡G ");
    scanf("%d", &SIZE);


	while(SIZE>=2){

	    int** maze= new int*[SIZE];
	    for(int i=0;i<SIZE;i++){
		 	maze[i] = new int[SIZE];
		 	for(int j=0;j<SIZE;j++){
			 	maze[i][j] = 0;
			 }
		}

		for(int i=0;i<SIZE;i++){
			for(int j=0;j<SIZE;j++){
				if((i==0)||(i==(SIZE-1))||(j==0)||(j==(SIZE-1)))
					maze[i][j] = 2;
				else if(((i==1)&&(j==1))||((i==(SIZE-2))&&(j==(SIZE-2))))
					maze[i][j] = 0;
				else{
					if (rand()%3)  //Obstacles Rate
						maze[i][j] = 2;
				}
			}
		}

		/** Randomize Feasible Route */
		enum Dir { RIGHT, LEFT, DOWN };
		int rightcnt = 0 , downcnt = 0, dircnt;
		int stepcnt = 50;
		bool biasRight = true, megaSize = false;
		int valBiasDown =70;

		megaSize = (SIZE > 12 ? true : false); //Responsive design
		if(megaSize) valBiasDown=90;

	    for(int i=0;i<2*(SIZE-3);i++){
	    	if(rightcnt>=SIZE-3)
	    		downcnt ++ ;
	    	else if	(downcnt>=SIZE-3)
	    		rightcnt ++ ;
	    	else{                        //Randomize Direction
	    		if(megaSize){
		    		if(rightcnt>((SIZE-3)*5/6))
						Bias(false, stepcnt, valBiasDown);
					else if(rightcnt<((SIZE-3)*1/6))
						Bias(true, stepcnt, valBiasDown);
				}

	    		int temp = (rand()%100);
	    		Dir dircnt;
	    		if(temp<stepcnt){ dircnt = Dir(0);}
	    		else if(temp<valBiasDown) dircnt = Dir(1);
	    		else dircnt = Dir(2);

	    		switch(dircnt){
	    			case RIGHT:
	    				rightcnt ++ ;

	    				Bias(true, stepcnt, valBiasDown);

	    				break;
	    			case LEFT:
	    				if((i>=2)&&(rightcnt>0)){
	    					rightcnt -- ;
	    					i-=2;

	    					Bias(false, stepcnt, valBiasDown);

	    					break;
	    				}
	    			case DOWN:
	    				downcnt ++ ;
	    				break;
				}
   				//Downwards acceleration manipulation
				if((valBiasDown>80)&&(megaSize)) valBiasDown = 90 - (downcnt%((SIZE-3)/10) + 1);
			}
			maze[1+downcnt][1+rightcnt] = 0;
		}

		/**  */

		Node* root = NULL;

	    if(!visit(maze, pt(1, 1), pt(SIZE-2, SIZE-2), &root)) {
	        printf("This Is A DEADEND MAZE¡I\nThe Mouse Gonna Be DEAD¡I\n");
	    }
	    print(maze, SIZE);
	    /*
	    Node* cur = root;
	    while(cur){
	    	printf("%d %d\n", cur->x, cur->y);
	    	cur = cur->next;
		}
	    */

	    print(maze, SIZE, root);

	    printf("Please Input An Integer\nIt Will Yield An n*n Maze¡G ");
        scanf("%d", &SIZE);



    }

    return 0;
}

Point pt(int x, int y) {
    Point p = {x, y};
    return p;
}

int visit(int** maze, Point start, Point end, Node** root) {
    if(!maze[start.x][start.y]) {
         maze[start.x][start.y] = 1;

         Node* temp = new Node();
         temp->next = NULL;
         temp->x = start.x;
         temp->y = start.y;
         if(*root==NULL) *root = temp;
         else{
	         Node* cur = *root;
	         while(cur->next) cur=cur->next;
	         cur->next = temp;
     	 }

         //printf("qqq %d %d\n", (temp->x), (temp->y));

         if(!maze[end.x][end.y] &&
            !(visit(maze, pt(start.x, start.y + 1), end, root) ||
              visit(maze, pt(start.x + 1, start.y), end, root) ||
              visit(maze, pt(start.x, start.y - 1), end, root) ||
              visit(maze, pt(start.x - 1, start.y), end, root))) {
                 maze[start.x][start.y] = 0;
         }
    }
    return maze[end.x][end.y];

}

void print(int** maze, int SIZE) {
    int i, j;
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) switch(maze[i][j]) {
            case 0 : printf("  "); break;
            case 1 : printf("¡¹"); break;
            case 2 : printf("¡¼");
        }
        printf("\n");
    }
	printf("\n");
}

void print(int** maze, int SIZE, Node* root) {
    int i, j;
    static bool star = true;
    bool prt = false;
    while(root){
    	for(i = 0; i < SIZE; i++) {
	        for(j = 0; j < SIZE; j++){
	        	if((maze[i][j]==1)&&(root->x==i)&&(root->y==j))
	        		prt = true;
			}
		}
		if(prt){
    		printf("\n\n");
	    	for(i = 0; i < SIZE; i++) {
		        for(j = 0; j < SIZE; j++) switch(maze[i][j]) {
		            case 0 : printf("  "); break;
		            case 1 : if((root->x==i)&&(root->y==j)){
				            	printf("¡¹");
				            	star = true;
				            	maze[i][j] = 3;
							 } else {
							 	printf("  ");
							 }
							 break;
		            case 2 : printf("¡¼"); break;
		            case 3 : printf("¡¹");
		        }
		        printf("\n");
		    }
		    printf("\n");
		}
	    if(star) Sleep(200);
	    star = prt = false;
	    root = root->next;
	}

}
