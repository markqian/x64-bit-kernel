#include "screen.h"

int get_screen_offset(int col, int row) {
  return 2*(row*80 + col);
}

int get_cursor() {
  port_byte_out(REG_SCREEN_CTRL, 14);
  int offset = port_byte_in(REG_SCREEN_DATA) << 8;
  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);

  return offset*2;
}

void set_cursor(int offset) {
  offset /= 2;
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8)&0xFF);
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset&0xFF));
}

void print_char(char character, int col, int row, char attribute_byte) {
  unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
  
  if (!attribute_byte) {
    attribute_byte = WHITE_ON_BLACK;
  }

  int offset;

  if (col >= 0 && row >= 0) {
    offset = get_screen_offset(col, row);
  }
  else {
    offset = get_cursor();
  }

  if (character == '\n') {
    int rows = offset / (2*MAX_COLS);
    offset = get_screen_offset(79, rows);
  }
  else {
    vidmem[offset] = character;
    vidmem[offset + 1] = attribute_byte;
  }

  offset += 2;

  offset = handle_scrolling(offset);
  set_cursor(offset);
}

void print_at(char * message, int col, int row) {
  if (col >= 0 && row >= 0) {
    set_cursor(get_screen_offset(col, row));
  }

  int i = 0;
  while (message[i] != 0) {
    print_char(message[i++], col, row, WHITE_ON_BLACK);
  }  
}

void putc(char c) {
  print_char(c, -1, -1, WHITE_ON_BLACK);
}

void putw( long int n, char fc, char *bf ) {
	char ch;
	char *p = bf;

	while( *p++ && n > 0 ) n--;
	while( n-- > 0 ) putc(  fc );
	while( ( ch = *bf++ ) ) putc( ch );
}


void print(char *message) {
  print_at(message, -1, -1);
}

int a2d( char ch ) {
	if( ch >= '0' && ch <= '9' ) return ch - '0';
	if( ch >= 'a' && ch <= 'f' ) return ch - 'a' + 10;
	if( ch >= 'A' && ch <= 'F' ) return ch - 'A' + 10;
	return -1;
}


char a2i( char ch, char **src, long int base, long int *nump ) {
	long int num, digit;
	char *p;

	p = *src; num = 0;
	while( ( digit = a2d( ch ) ) >= 0 ) {
		if ( digit > base ) break;
		num = num*base + digit;
		ch = *p++;
	}
	*src = p; *nump = num;
	return ch;
}

void ui2a( unsigned long int num, unsigned long int base, char *bf ) {
	long int n = 0;
	long int dgt;
	unsigned long int d = 1;
	
	while( (num / d) >= base ) d *= base;
	while( d != 0 ) {
		dgt = num / d;
		num %= d;
		d /= base;
		if( n || dgt > 0 || d == 0 ) {
			*bf++ = dgt + ( dgt < 10 ? '0' : 'a' - 10 );
			++n;
		}
	}
	*bf = 0;
}

void i2a( long int num, char *bf ) {
	if( num < 0 ) {
		num = -num;
		*bf++ = '-';
	}
	ui2a( num, 10, bf );
}

void format ( char *fmt, va_list va ) {
	char bf[48];
	char ch, lz;
	long int w;
	
	while ( ( ch = *(fmt++) ) ) {
		if ( ch != '%' )
		  putc( ch );
		else {
		  lz = 0; w = 0;
		  ch = *(fmt++);
		  switch ( ch ) {
		  case '0':
		    lz = 1; ch = *(fmt++);
		    break;
		  case '1':
		  case '2':
		  case '3':
		  case '4':
		  case '5':
		  case '6':
		  case '7':
		  case '8':
		  case '9':
		    ch = a2i(ch, &fmt, 10, &w );
		    break;
		  }
		  switch( ch ) {
		  case 0: return;
		  case 'c':
		    putc(  va_arg( va, int ) );
		    break;
		  case 's':
		    putw( w, 0, va_arg( va, char* ) );
		    break;
		  case 'u':
		    ui2a( va_arg( va, unsigned long int ), 10, bf );
		    putw( w, lz, bf );
		    break;
		  case 'd':
		    i2a(va_arg( va, long int ), bf );
		    putw(w, lz, bf );
		    break;
		  case 'x':
		    ui2a(va_arg( va, unsigned long int ), 16, bf );
		    putw(w, lz, bf );
		    break;
		  case '%':
		    putc( ch );
		    break;
		  }
		}
	}
}

void printf(char *fmt, ...) {
  va_list va;
  va_start(va,fmt);
  format( fmt, va );
  va_end(va);
}

void clear_screen() {
  int row = 0;
  int col = 0;

  for (row =0; row <MAX_ROWS; row++) {
    for (col=0; col <MAX_COLS; col++) {
      print_char(' ', col, row, WHITE_ON_BLACK);
    }    
  }

  set_cursor(get_screen_offset(0,0));
}


void memory_copy(char * source, char * dest, int no_bytes) {
  int i;
  for (i = 0; i<no_bytes; i++) {
    *(dest + i) = *(source +i);
  }
}

int handle_scrolling(int cursor_offset) {
  if (cursor_offset < MAX_ROWS*MAX_COLS*2) {
    return cursor_offset;
  }

  int i;
  for (i=1; i<MAX_ROWS; i++) {
    memory_copy(
		get_screen_offset(0,i) + (char*)VIDEO_ADDRESS,
		get_screen_offset(0, i-1) + (char*)VIDEO_ADDRESS,
		MAX_COLS*2
		);    
  }
  
  char* last_line = get_screen_offset(0, MAX_ROWS-1) + (char*)VIDEO_ADDRESS;
  for (i = 0; i<MAX_COLS*2; i++) {
    last_line[i] = 0;
  }
  cursor_offset -= 2*MAX_COLS;

  return cursor_offset;
}

char * itoa(int value, char * str, int base) {
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 ) {
      *str = '\0';
      return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 ) {
      *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
      value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr ) {
      char tmp = *low;
      *low++ = *ptr;
      *ptr-- = tmp;
    }
    return rc;
}
