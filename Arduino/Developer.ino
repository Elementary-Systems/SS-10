/*
This program is created and distributed at Elementary systems and is used to Drive Barebone Motors 1.0 with all of its functionalities.

Email:engineering@elementarysystems.in (Reply within 8 Hours  guarenteed)
Contact:+91-8200186144 ( 10AM-6PM, Monday-Saturday). Sundays we go out on a trip.

Documentation and code available at:
Video Tutorials available at:
*/

#include <SoftwareSerial.h>     						// Creating Software based Serial to communicate with Motors. Native Serial is still available to users.
SoftwareSerial mySerial(2, 3);  						// (2 pin) connects to RX pin of breakout board, (3 pin) connects to TX pin of breakout board.

void setup(){
	mySerial.begin(57600); 						// serial used in Motor communication.
  Serial.begin(57600); 
}

void loop(){
  _write_angle(1,0);
  delay(2000);
  _write_angle(1,180);
  delay(2000);
}

void Ready_to_use_examples(){
	_reset_motors();                      		        	// This command resets the motor id to #1.
	uint8_t returns =_ping(1);                             		// This example is for checking motor id 1 connected or not.
	_write_torque(1,0);                   		        	// This example will disable power on motor #1. To enable power, _write_torque(1,1).
	_write_angle(1,90);                   		        	// This example will command motor #1 to move to 90. (Min-0 Max-180 with of resolution 5 degrees).
	uint8_t angle =read_angle(1);                       		// This command returns the current angle of the motor is #1.
	
	uint8_t MotorIds[]={1,2,3,4,5,6,7,8,9,10};
	uint8_t MotorAngles[]={0,90,0,0,90,90,0,45,135,180};
	uint8_t No_of_Motors=10;
	_syncwrite(MotorIds,MotorAngles,No_of_Motors); 	        	// This example is for set angle for servo motor ids 1 to 10 in sync mode
 	_set_id(1,2);                                                 	// This command sets id of the motor from 1 to 2.
}

uint8_t read_angle(uint8_t MotorID){
	  uint8_t _return_packet[7]={0,0,0,0,0,0,0};
	  uint8_t arrays[]={255,253,2,4,2,MotorID,16,251};
	  mySerial.write(arrays,sizeof(arrays));
	  delay(100); 								//Don't change these numbers.
	  if(mySerial.available() > 0){
		    mySerial.readBytes(_return_packet,7);
	  }

	  return _return_packet[4];
}

void _reset_motors(){
	  uint8_t _return_packet[7]={0,0,0,0,0,0,0};
	  uint8_t arrays[]={255,253,5};
	  mySerial.write(arrays,sizeof(arrays));
	  delay(100); 								//Don't change these numbers.
}

uint8_t _ping(uint8_t motorid){
	  uint8_t _return_packet[7]={0,0,0,0,0,0,0};     					// Packet structure for packets from the motor
	  uint8_t arrays[]={255,253,1,2,motorid,251};
	  mySerial.write(arrays,sizeof(arrays));
	  delay(100); 								//Don't change these numbers.
	  if(mySerial.available() >0){
		    mySerial.readBytes(_return_packet,7);
	  }
	  if(_return_packet[0] == 255 && _return_packet[1] == 253 && 
	  _return_packet[3] == 0 && _return_packet[4] == 0 && 
	  _return_packet[5] == 0 && _return_packet[6] == 251){
	    	return _return_packet[2];
	  }
	  else{
	    	return 0;
	  }
}

void _set_id(uint8_t motorid,uint8_t value){
	  uint8_t arrays[]={255,253,1,6,3,motorid,2,value,0,251};
	  mySerial.write(arrays,sizeof(arrays));
	  delay(100); 								//Don't change these numbers.
}
void _write_angle(uint8_t motorid,uint8_t value){
	  uint8_t arrays[]={255,253,1,6,3,motorid,13,value,0,251};
	  mySerial.write(arrays,sizeof(arrays));
	  delay(100); 								//Don't change these numbers.
}
void _write_torque(uint8_t motorid,uint8_t value){
	  uint8_t arrays[]={255,253,1,6,3,motorid,11,value,0,251};
	  mySerial.write(arrays,sizeof(arrays));
	  delay(100); 								//Don't change these numbers.
}

void _syncwrite(uint8_t motorids[],uint8_t motor_angless[],uint8_t idsize){
	uint8_t arrays[4+1+(idsize*4)+1];
	uint8_t p;
	uint8_t L_motor_angless=0;
	uint8_t R_motor_angless=0;

	arrays[0]=255;
	arrays[1]=253;
	arrays[2]=4;
	arrays[3]=(1+4*idsize+1);
	arrays[4]=4;
	for(p=0;p<idsize;p=p+1){
		arrays[5+(p*4)]=motorids[p];
		arrays[5+(p*4)+1]=1;

		if(motor_angless[p] > 127){
			L_motor_angless=127;
			R_motor_angless=motor_angless[p]-127;
		}
		else{
			L_motor_angless=motor_angless[p];
			R_motor_angless=0;
		}
		arrays[5+(p*4)+2]=L_motor_angless;
		arrays[5+(p*4)+3]=R_motor_angless;
	    }
	arrays[sizeof(arrays)-1]=251;
	mySerial.write(arrays,sizeof(arrays));
	delay(100); 								//Don't change these numbers.
}
