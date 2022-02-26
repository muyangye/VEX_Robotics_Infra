#include "AutoFunction.h"
void AutoRedNear()
{
	AShoot();
  while(ABaseTurn(-260));
  AIntake(1);
  while(ABaseGoto(900));
  ABaseGo(25,-25,0.5,1);
  ASleep(0.2);
  while(ABaseGoto(-800));
  while(ABaseTurn(240));
	while(ABaseGoto(900));
	AShoot();
  while(ABaseTurn(80));
	AStick(1);
	ASleep(0.3);
	AStick(2);
  
  while(ABaseGoto(-200));
  while(ABaseTurn(-400));
  AIntake(1);
   while(ABaseGoto(-200));
  AStick(1);
  while(ABaseGoto(300));
  AStick(2);

  while(ABaseTurn(120));
   while(ABaseGoto(1200));
  AStick(1);
  AStick(2);
  while(ABaseGoto(-500));
  AStopAll();
}

void AutoBlueNear()
{
	AShoot();
  while(ABaseTurn(240));
  AIntake(1);
  while(ABaseGoto(900));
  ABaseGo(25,-25,0.5,1);
  ASleep(0.2);
  while(ABaseGoto(-800));
  while(ABaseTurn(-260));
	while(ABaseGoto(900));
	AShoot();
  while(ABaseTurn(-80));
	AStick(1);
	ASleep(0.3);
	AStick(2);
  
  while(ABaseGoto(-200));
  while(ABaseTurn(400));
  AIntake(1);
   while(ABaseGoto(-200));
  AStick(1);
  while(ABaseGoto(300));
  AStick(2);

  while(ABaseTurn(-150));
   while(ABaseGoto(1200));
  AStick(1);
  AStick(2);
  while(ABaseGoto(-500));
  AStopAll();
}

void AutoRedFar()
{
	AShoot();

	while(ABaseTurn(-190));//turn to first cap and intake second ball
    
	AIntake(1);
  ABaseGo(-20,20,0.4,1);// go back to wall to make sure;
	while(ABaseGoto(1000));// go to first cap
  ABaseGo(30,-30,0.5,1);
	vex::task::sleep(700);
    while(ABaseGoto(-50));//go back a little to turn
    while(ABaseTurn(120));//turn to enemy top flag
    AShoot();
    
    while(ABaseTurn(-300));
    AStick(1);
    while(ABaseGoto(600));
    AStick(2);
    while(ABaseGoto(-100));
    while(ABaseTurn(400));

    
    //go to platform
    ABaseGo(60,-60,0.2,0);//接近己方平台
    //ABaseGo(-20,20,0.4,0);//贴近已方平台边缘
    ABaseGo(30,-30,0.3,1);
    ABaseGo(100,-100,0.9,1);
    ABaseGo(40,-40,0.01,1);
  
    AStopAll();
}

void AutoBlueFar()
{
	AShoot();

	while(ABaseTurn(190));//turn to first cap and intake second ball
    
	AIntake(1);
  ABaseGo(-20,20,0.6,1);// go back to wall to make sure;
	while(ABaseGoto(950));// go to first cap
  ABaseGo(30,-30,0.5,1);
	vex::task::sleep(700);
    while(ABaseGoto(-50));//go back a little to turn
    while(ABaseTurn(-150));//turn to enemy top flag
    AShoot();
    
    while(ABaseTurn(260));
    AStick(1);
    while(ABaseGoto(400));
    AStick(2);
    while(ABaseGoto(-200));
    while(ABaseTurn(-400));

    
    //go to platform
    ABaseGo(60,-60,0.2,0);//接近己方平台
    //ABaseGo(-20,20,0.4,0);//贴近已方平台边缘
    ABaseGo(30,-30,0.3,1);
    ABaseGo(100,-100,0.9,1);
    ABaseGo(40,-40,0.01,1);
  
    AStopAll();
}

void AutoRun(void)
{
	AutoInit();
  AutoBlueFar();
}

void AutoStart()
{
  while(Bumper.pressing()==0)
  {
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
  }
  AutoRun();
}
