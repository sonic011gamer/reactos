/* $Id: sh.c,v 1.3 2002/10/29 04:44:59 rex Exp $
 *
 * baresh - Bare Shell for the PSX subsystem.
 * Copyright (c) 2002 Emanuele Aliberti
 * License: GNU GPL v2
 */
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define INPUT_BUFFER_SIZE 128

int run=1;

void cmd_exit(char*buf)
{
  run=0;
}

void cmd_pwd(char * buf)
{
  #if 0
  char pwd[1024];

  getcwd(pwd,sizeof pwd);
  printf("%s\n",pwd);
  #endif
}
#if 0
void cmd_ls(char*buf)
{
  char pwd[1024];
  DIR * dir;
  struct dirent * entry;

  getcwd(pwd,sizeof pwd);
  dir=opendir(pwd);
  while (NULL!=(entry=readdir(dir)))
  {
    printf("%s\n",entry->d_name);
  }
  closedir(dir);
}
#endif
int main()
{
  char buf[INPUT_BUFFER_SIZE];

  while (run)
  {
    printf("# ");
    if (gets(buf))
    {
      if (!strcmp("exit",buf))    cmd_exit(buf);
      else printf("%s: unknown command\n");
    }
  }
  return 0;
}
/* EOF */
