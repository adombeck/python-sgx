#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH "/tests/foo/bar"

int main(){
	unsigned int i = 0;
  char line[256];

  FILE* f = fopen(PATH, "r");
  if (f == NULL) {
	  f = fopen(PATH, "w");
		if (f == NULL)
			return 1;	
	} else {
    while (fgets(line, sizeof(line), f))
        printf("%s", line); 

		i = atoi(line);
		printf("i: %d\n", i);
	}

	fclose(f);

	sprintf(line, "%d\n", ++i);
	printf("line: %s", line);

	f = fopen(PATH, "a");
  fputs(line, f);
	fflush(f);
  fclose(f);
	sync();

	f = fopen(PATH, "r");
	if (f == NULL)
    return 1;
	while (fgets(line, sizeof(line), f))
    printf("%s", line);
  fclose(f);

	return 0;
}

