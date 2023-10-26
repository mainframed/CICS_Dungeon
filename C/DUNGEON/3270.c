//////////////////////////////////////////////////////////////////////////
// 3270 Functions
// 3270.c
//////////////////////////////////////////////////////////////////////////


short encode_sba (int y, int x) {
    unsigned short loc;
    // x is columns, y is rows
    // ((Row -1) * 80) + (Col - 1)
    loc = ((y - 1) * 80 ) + (x - 1);

    // sprintf(wtomsg,"loc = %dencode x (COL) %d y (ROW) %d",loc,x, y);
    // write_op();

     return(code_table[((loc) >> 6) & 0x3F] * 0x100 \
     + code_table[(loc) & 0x3F]);
  }
  
void append_screen(const char *chars, int length) {
  int i;    
  // Check if the index is within the bounds of the array.
  if (location < 0 || location >= sizeof(screen)) {

    // sprintf(wtomsg,"CANNOT APPEND TO screen location=%d",location);
    // write_op();
    return;
  }

  for(i=0;i<length;i++){
    // sprintf(wtomsg,"appending: 0x%.02x",chars[i]);
    // write_op();
  }

  // Copy the characters to the array.
  memcpy(&screen[location], chars, length);

  // Increment the index by the number of characters inserted.
  location += length;
}

void sba(int y, int x) {
    // Don't forget x is columns, y is rows
    // sprintf(wtomsg, SBA_MESSAGE,y, x);
    // write_op();
    short enc;
    append_screen("\x11",1);
    enc = encode_sba(y, x);
  // Check if the index is within the bounds of the array.
  if (location +2 >= sizeof(screen)) {
    return;
  }
    memcpy(&screen[location], (unsigned char*)&enc, 2);
    location += 2;
}


void change_color(char color) {
  // x'28' - SF
  // x'41' - Extended Highlighting
  // x'00' - Default
  // x'28' - SF
  // x'42' - Color
  append_screen("\x28\x41\x00\x28\x42",5);
  // printf("Changing color to 0x%.02hhx\n",color);
  append_screen(&color,1);
}

int size_of_screen() {
  // Screen contains nulls so we can't use
  // strlen so we use this
  // Find the last non-null character.
  int end_index = MAXSCREEN;
  while (screen[end_index] == '\0') {
    end_index--;
  }

  return(end_index);
}


void exit_kicks() {
  sprintf(wtomsg,"Error Allocating Memory, exiting KICKS");
  write_op();
  EXEC KICKS SIGNOFF END-EXEC;
  EXEC KICKS DELAY INTERVAL(2) END-EXEC;
  EXEC KICKS SEND CONTROL ERASE ALTERNATE FREEKB END-EXEC;
}