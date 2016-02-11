#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x; 
    int y;
} Point;

Point pt(int, int);
int visit(int**, Point, Point);
void print(int**, int);

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

				if((valBiasDown>80)&&(megaSize)) valBiasDown = 90 - (downcnt%((SIZE-3)/10) + 1); //Downwards acceleration manipulation
			}
			maze[1+downcnt][1+rightcnt] = 0;   
		}
	
		/**  */
	    if(!visit(maze, pt(1, 1), pt(SIZE-2, SIZE-2))) {
	        printf("This Is A DEADEND MAZE¡I\nThe Mouse Gonna Be DEAD¡I\n"); 
	    }
	    print(maze, SIZE);
	    
	    printf("Please Input An Integer\nIt Will Yield An n*n Maze¡G ");
        scanf("%d", &SIZE);
    
    
    } 
    
    return 0; 
}

Point pt(int x, int y) {
    Point p = {x, y};
    return p;
}

int visit(int** maze, Point start, Point end) {
    if(!maze[start.x][start.y]) {
         maze[start.x][start.y] = 1;
         if(!maze[end.x][end.y] && 
            !(visit(maze, pt(start.x, start.y + 1), end) || 
              visit(maze, pt(start.x + 1, start.y), end) ||
              visit(maze, pt(start.x, start.y - 1), end) || 
              visit(maze, pt(start.x - 1, start.y), end))) {
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
}
