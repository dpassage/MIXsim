#include <stdio.h>
#include <stdlib.h>

#include "mix_instr_decode.h"
#include "mix_machine.h"
#include "mix_word.h"

static void print_instr_callback(int ip, const mix_word *instr) {
    char mix_instr_text[100];
    fprintf(stderr, "Running instr %d: ", ip);
    if (mix_instr_decode(instr, mix_instr_text) == NULL) {
        fprintf(stderr, "Could not decode instruction %s\n", mix_word_tostring(instr)); 
    } else {
        fprintf(stderr, "%s\n", mix_instr_text); 
    }
}

int main (int argc, const char * argv[]) {
    mix_machine *m;
    char readbuf[500];
    int ip;
    int i = 0;
    int loc;
	mix_word w;
    int ret;    
	FILE *infile;
	
	if (argc == 1) {
		infile = stdin;
	} else {
		infile = fopen(argv[1], "r");
	}
    
    /* create new machine */
    m = mix_machine_create();
    mix_machine_device_attach(m, mix_device_printer_create(), 18);
    mix_machine_set_callback_exec(m, print_instr_callback);
    
    /* read start line and set Ip */
    if (fgets(readbuf, 500, infile) != NULL) {
        sscanf(readbuf, "START: %d\n", &ip);
    } else {
        return -1;
    }
    
    mix_machine_set_ip(m, ip);
    
    /* for each remaining line */
	while (fgets(readbuf, 500, infile) != NULL) {
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
        ret = mix_machine_execute(m);
    } while (ret == MIX_M_OK);
    
    if (ret == MIX_M_HALT) {
        printf("Program halted!\n");
    } else { 
        switch (ret) {
            case MIX_M_UNIMPLEMENTED:
                printf("Unimplemented instruction!\n");
                break;
            default:
                printf("Machine aborted!\n");
        }
        printf("Instruction was %s\n", mix_word_tostring(mix_machine_read_mem(m, &w, mix_machine_get_ip(m))));
    }

    printf("Time elapsed = %d\n", mix_machine_get_time(m));               
    return 0;
}
