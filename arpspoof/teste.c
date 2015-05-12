#include <string.h>
#include <stdio.h>

int main()
{
   const char str[14] = "aa:bb:cc:dd:ee";
   const char s[2] = ":";
   char *token;

   /* get the first token */
   token = strtok(str, s);

   /* walk through other tokens */
   while(token != NULL)
   {
      printf("%s\n", token);

      token = strtok(NULL, s);
   }

   return(0);
}

