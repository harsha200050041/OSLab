#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int c_score;
int t_score;
int h_score;

int main(int argc, char *argv[])
{
	if(argc != 2) return (EXIT_FAILURE);
	int total_players = atoi(argv[1]);
	int matches = total_players/22;
	int wins = 0;
	int losses = 0;
	int ties = 0;
	int tmax_score = 0;
	int hmax_score = 0;
	for(int i = 0; i < matches; i++){
        
                
        
	    printf("----------MATCH : ( %d ) Summary----------\n\n", i);
	    printf("SCORE: Capitals : %d :: Titans : %d\n", c_score, t_score);
	    printf("Highest Individual Score : %d\n", h_score);
	    if(h_score > hmax_score) hmax_score = h_score;
	    if(c_score > tmax_score) tmax_score = c_score;
	    if(t_score > tmax_score) tmax_score = t_score;
	    if(c_score > t_score) {
	        printf("Result : Capitals won by %d runs\n\n", c_score-t_score);
	        losses+=1;
	    }
	    else if(t_score > c_score) {
	        printf("Result : Titans won by %d runs\n\n", t_score-c_score);
	        wins+=1;
	    }
	    else {
	        printf("Result : Match tied\n\n");
	        ties+=1;
	    }
	}
    printf("----------SUMMARY OF THE DAY---------\n");
    printf("Matches Played : %d\n", matches);
    printf("Titans   :: Won : %d || Lost : %d || Tied : %d\n", wins, losses, ties);
    printf("Capitals :: Won : %d || Lost : %d || Tied : %d\n", matches-ties-losses, matches-ties-wins, ties);
	printf("Highest Team Score           : %d\n", tmax_score);
	printf("Highest Individual Score     : %d\n", hmax_score);
    printf("-------------------\n\n");
    
    
    return (EXIT_SUCCESS);
}
