#include <stdio.h>
#include <stdlib.h>

#include "mix_machine.h"
#include "mix_word.h"

int main (int argc, const char * argv[]) {
    mix_machine *m;
    char readbuf[500];
    int ip;
    int i = 0;
    int loc;
	mix_word w;
    int ret;    
    
    /* create new machine */
    m = mix_machine_create();
    
    /* read start line and set Ip */
    if (fgets(readbuf, 500, stdin) != NULL) {
        sscanf(readbuf, "START: %d\n", &ip);
    }
    
    mix_machine_set_ip(m, ip);
    
    /* for each remaining line */
	while (fgets(readbuf, 500, stdin) != NULL) {
        if (readbuf[4] != ':') {
            continue;
        }
        readbuf[4] = '\0';
        loc = atoi(readbuf);
        mix_word_set(&w, &(readbuf[6]));
        mix_machine_load_mem(m, &w, loc);
        i++;
    }
    
    printf("Locations loaded: %d\n", i);
    printf("Starting instruction: %d\n", ip);
    
    do {
        printf("Running instr %d\n", mix_machine_get_ip(m));
        ret = mix_machine_execute(m);
    } while (ret == MIX_M_OK);
    
    if (ret == MIX_M_HALT) {
        printf("Program halted!\n");
        printf("Time elapsed = %d\n", mix_machine_get_time(m));               
    } else {
        printf("Machine aborted!\n");
        printf("Instruction was %s\n", mix_word_tostring(mix_machine_read_mem(m, &w, mix_machine_get_ip(m))));
    }
                   
    return 0;
}
