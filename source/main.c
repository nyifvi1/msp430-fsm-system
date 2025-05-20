#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){
	  case state0:
        enterLPM(lpm_mode);
		break;
		 
	  case state1:
		freqCounter();
		break;
		 
	  case state2:
		stopWatch();
		break;

	  case state3:
		buzzer();
	  	break;
	}
  }
}
  
  
  
  
  
  