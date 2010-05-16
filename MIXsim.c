#include <stdio.h>
#include <stdlib.h>

#include "mix_assembler.h"
#include "mix_word.h"

int main (int argc, const char * argv[]) {

    
    /* create new machine */

    /* read start line and set Ip */
    /* for each remaining line */
    /* read memory location */
    /* set location in machine */
	
    /* print total locations set and exit */
    
    
    char readbuf[500];
	mix_word *w;
	char *s;
	
	w = mix_word_create();
	
	/* for each line of input */
	while (fgets(readbuf, 500, stdin) != NULL) {
		mix_assemble_line(readbuf, w);
		s = mix_word_tostring(w);
		printf("%s\n", s);
		free(s);
	}
	mix_word_destroy(w);
    return 0;
}
