#include "DriveFunction.h"

void ABaseInit()
{
	BaseLF.resetRotation();
	BaseLB.resetRotation();
	BaseRF.resetRotation();
	BaseRB.resetRotation();
    for(int i=0;i<99;i++)
	{
		BaseA1[i]=0;
		BaseA2[i]=0;
        ABA1[i]=0;
        ABA2[i]=0;
	}
}

void AutoInit()
{
	A_basegoto_p=0.28;//0.28
	A_baseturn_p=0.2;//0.39;
	AStopSpeed=10;
	Brain.resetTimer();
	Stick.resetRotation();
	ABaseInit();
    init();
}

void AStopAll()
{
	BaseLF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    BaseLB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    BaseRF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    BaseRB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    
    LevelL.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    LevelR.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
   
    Intake.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);

    Stick.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
}

int ABaseGoto(float aim)
{
  if(Brain.timer(vex::timeUnits::sec)>=15)
  {
    AStopAll();
  }
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
	float error_l;
	float error_r;

	float lf;
	float lb;
	float rf;
	float rb;
	encoderL=(BaseLF.rotation(vex::rotationUnits::deg));//+BaseLB.rotation(vex::rotationUnits::deg))/2;
	encoderR=(BaseRF.rotation(vex::rotationUnits::deg));//+BaseRB.rotation(vex::rotationUnits::deg))/2;
  error_l=aim-encoderL;
  error_r=-aim-encoderR;
	ABaseSpeedL=(error_l-error_r)*A_basegoto_p/2;
	ABaseSpeedR=-(-error_r+error_l)*A_basegoto_p/2;

	lf=(aim-BaseLF.rotation(vex::rotationUnits::deg));
	lb=(aim-BaseLB.rotation(vex::rotationUnits::deg));
	rf=(-aim-BaseRF.rotation(vex::rotationUnits::deg));
	rb=(-aim-BaseRB.rotation(vex::rotationUnits::deg));

	//cubefilter(lf);
	//cubefilter(lb);
	//cubefilter(rf);
	//cubefilter(rb);
	renewAU(ABA1,rf);
	renewAU(ABA2,rb);
	renewAU(BaseA1,lf);
	renewAU(BaseA2,lb);
	lf=smooth100(BaseA1);
	lb=smooth100(BaseA2);
	rf=smooth100(ABA1);
	rb=smooth100(ABA2);
    lf=lf*A_basegoto_p;
    lb=lb*A_basegoto_p;
    rf=rf*A_basegoto_p;
    rb=rb*A_basegoto_p;

	/*BaseLF.spin(vex::directionType::fwd,(lf+lb-rf-rb)/4,vex::velocityUnits::pct);
    BaseLB.spin(vex::directionType::fwd,(lf+lb-rf-rb)/4,vex::velocityUnits::pct);
    BaseRF.spin(vex::directionType::fwd,(-lf-lb+rf+rb)/4,vex::velocityUnits::pct);
    BaseRB.spin(vex::directionType::fwd,(-lf-lb+rf+rb)/4,vex::velocityUnits::pct);
    */
    BaseLF.spin(vex::directionType::fwd,ABaseSpeedL,vex::velocityUnits::pct);
    BaseLB.spin(vex::directionType::fwd,ABaseSpeedL,vex::velocityUnits::pct);
    BaseRF.spin(vex::directionType::fwd,ABaseSpeedR,vex::velocityUnits::pct);
    BaseRB.spin(vex::directionType::fwd,ABaseSpeedR,vex::velocityUnits::pct);

	if(JDZ(aim-BaseLF.rotation(vex::rotationUnits::deg))<40 && JDZ(-aim-BaseRF.rotation(vex::rotationUnits::deg))<40)
	{
        BaseRB.stop();
        BaseLB.stop();
        BaseLF.stop();
        BaseRF.stop();
    //Intake.spin(vex::directionType::fwd,-100,vex::velocityUnits::pct);    
		//BaseLF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    	//BaseLB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    	//BaseRF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    	//BaseRB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
		ABaseInit();
		return 0;
	}
	else
	{
		return 1;
	}
}

int ABaseTurn(float aim)
{
    if(Brain.timer(vex::timeUnits::sec)>=15)
  {
    AStopAll();
  }
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
	float error_l;
	float error_r;
    
    float lf;
	float lb;
	float rf;
	float rb;
    int a=0;
    int b=0;
	encoderL=(BaseLF.rotation(vex::rotationUnits::deg)+BaseLB.rotation(vex::rotationUnits::deg))/2;
	encoderR=(BaseRF.rotation(vex::rotationUnits::deg)+BaseRB.rotation(vex::rotationUnits::deg))/2;
    error_l=-aim-encoderL;
    error_r=-aim-encoderR;
	ABaseSpeedL=(error_l)*A_baseturn_p;
	ABaseSpeedR=(error_r)*A_baseturn_p;
    
    lf=(-aim-BaseLF.rotation(vex::rotationUnits::deg))*A_baseturn_p;
	lb=(-aim-BaseLB.rotation(vex::rotationUnits::deg))*A_baseturn_p;
	rf=(-aim-BaseRF.rotation(vex::rotationUnits::deg))*A_baseturn_p;
	rb=(-aim-BaseRB.rotation(vex::rotationUnits::deg))*A_baseturn_p;
    
    cubefilter(lf);
	cubefilter(lb);
	cubefilter(rf);
	cubefilter(rb);
	renewAU(ABA1,rf);
	renewAU(ABA2,rb);
	renewAU(BaseA1,lf);
	renewAU(BaseA2,lb);
	lf=smooth100(BaseA1);
	lb=smooth100(BaseA2);
	rf=smooth100(ABA1);
	rb=smooth100(ABA2);

	BaseLF.spin(vex::directionType::fwd,lf,vex::velocityUnits::pct);
    BaseLB.spin(vex::directionType::fwd,lb,vex::velocityUnits::pct);
    BaseRF.spin(vex::directionType::fwd,rf,vex::velocityUnits::pct);
    BaseRB.spin(vex::directionType::fwd,rb,vex::velocityUnits::pct);
	
	if(JDZ(error_l)<40)
	{
		BaseLF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    	BaseLB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
        a=1;
    }
    if(JDZ(error_r)<40)
    {
        BaseRF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    	BaseRB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
        b=1;
    }
    if(a==1 && b==1)
    {
		ABaseInit();
		return 0;
	}
	else
	{
		return 1;
	}
}

void AShoot()
{
    if(Brain.timer(vex::timeUnits::sec)>=15)
  {
    AStopAll();
  }
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
	LevelL.spin(vex::directionType::fwd,100,vex::velocityUnits::pct);
    LevelR.spin(vex::directionType::fwd,-100,vex::velocityUnits::pct);
    vex::task::sleep(600);
   	LevelL.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    LevelR.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    vex::task::sleep(100);

}

void AIntake(float s)
{
    if(Brain.timer(vex::timeUnits::sec)>=15)
  {
    AStopAll();
  }
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
	float ASpeedIntake=100*s;
	Intake.spin(vex::directionType::fwd,-ASpeedIntake,vex::velocityUnits::pct);
}

void AStick(int lv)
{
    if(Brain.timer(vex::timeUnits::sec)>=15)
  {
    AStopAll();
  }
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
    if(lv==1)
 	{
 		SpeedStick=100;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
        vex::task::sleep(400*lv);
        SpeedStick=0;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
 	}
    if(lv==2)
    {
 		SpeedStick=-100;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
        vex::task::sleep(300);
        SpeedStick=0;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
    }
        if(lv==3)
    {
 		SpeedStick=100;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
        vex::task::sleep(600);
        SpeedStick=0;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
    }
}
void ABaseGo(int l,int r, float t, int s)
{
    if(Brain.timer(vex::timeUnits::sec)>=15)
  {
    AStopAll();
  }
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
    BaseLF.spin(vex::directionType::fwd,l,vex::velocityUnits::pct);
    BaseLB.spin(vex::directionType::fwd,l,vex::velocityUnits::pct);
    BaseRF.spin(vex::directionType::fwd,r,vex::velocityUnits::pct);
    BaseRB.spin(vex::directionType::fwd,r,vex::velocityUnits::pct);
    vex::task::sleep(t*1000);
    if(s==1)
    {
    	BaseLF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
	    BaseLB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
	    BaseRF.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
	    BaseRB.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    }
}


void ASleep(float t)
{
    if(Brain.timer(vex::timeUnits::sec)>=15)
  {
    AStopAll();
  }
  if(Controller.ButtonUp.pressing()==1)
  {
    while(1)
    {
      Drive();
    }
  }
	vex::task::sleep(t*1000);
}

void AutoTest(int lv)
{
	if(lv==0)
	{
		return;
	}
	if(lv==1)
	{
    while(1)
    {
      		while(ABaseGoto(-650));
    }

    // while(ABaseTurn(240));
	}

}



