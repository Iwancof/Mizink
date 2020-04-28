
int global_x = 0;


void _start() {
	// Mizink main entry point
	
	while(1) {
	  global_x += 1;
	  if(0x10000 <= global_x) {
	    return;
	  }
	}
}
