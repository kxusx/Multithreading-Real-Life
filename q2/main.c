#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

void input(){
    scanf("%lld %lld %lld",&Hzone_capacity,&Azone_capacity,&Nzone_capacity);
    scanf("%lld",&X);
    scanf("%lld",&num_groups);
    for(i = 0; i < num_groups ; i++){
        scanf("%lld",&groups[i].k);
        for(j = 0; j < groups[i].k; j++){
            scanf("%s %c %lld %lld %lld",groups[i].names[j],&groups[i].support[j],&groups[i].time_reached[j],&groups[i].patience[j],groups[i].enragedScore[j]);
        }
    }
    scanf("%lld",&G);
    for(i=0;i<G;i++){
        scanf("%c %lld %lld",goals[i].team,goals[i].time,goals[i].probability);
    }

}

int main(){
    input();
    
}
