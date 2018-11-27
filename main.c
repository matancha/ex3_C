#include <stdio.h>
#include <errno.h>
#include "stack.h"

int main()
{
    char infix[100];

    while (fgets(infix, sizeof(infix), stdin) != EOF)
    {
        printf("Infix: %s", infix);

        char *endptr;
        int variable = (int)strtol(str, &endptr, 10);
    }
}

/*int main()
{
  int i, num = 10;
  printf("Generating list with %d ints\n", num);
  Stack *stack = stackAlloc(sizeof(int)); 
  for(i = 1; i <= num; i++) {
    push(stack,&i);
  }
  for(i = 1; i <= num-2; i++) {
    int headData;
    pop(stack,&headData);
    printf("top value is: %d\n",headData);
  }
  freeStack(&stack);
  return 0;
}*/
