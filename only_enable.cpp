#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include<bits/stdc++.h>

struct termios orig_termios;
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {  
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &=~(ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)!=0){
    return ;
  }
}

int main() {

  enableRawMode();
  
  char c;

  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  atexit(disableRawMode);
  return 0;
}