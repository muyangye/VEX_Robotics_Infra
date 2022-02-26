#include "vex.h"
void JoystickInit(void)
{
	if(joystick_choose==1)
	{
		BaseL=Controller.Axis4.value()+Controller.Axis3.value()*StateBase;
		BaseR=Controller.Axis4.value()-Controller.Axis3.value()*StateBase;
		VarIntake=Controller.ButtonR1.pressing()-Controller.ButtonR2.pressing();
		VarLever=-Controller.ButtonB.pressing();
		VarSlowBase=Controller.ButtonL1.pressing()*SlowBase;
		VarStick=-Controller.Axis2.value();
		StateStick=Controller.ButtonX.pressing()-Controller.ButtonY.pressing();
        StopFlag=Controller.ButtonL2.pressing();
	}
}

void DriveBase()
{
    BaseL=((BaseL+BaseR)+differencefilter(BaseL,BaseR,50))/2;
	BaseR=((BaseL+BaseR)-differencefilter(BaseL,BaseR,50))/2;
	BaseL=BaseL*(1-VarSlowBase);
	BaseR=BaseR*(1-VarSlowBase);
	BaseL=bottomfilter(15,BaseL);
	BaseR=bottomfilter(15,BaseR);
	cubefilter(BaseL);
	cubefilter(BaseR);
	renewAU(BaseA1,BaseL);
	renewAU(BaseA2,BaseR);
	BaseL=smooth100(BaseA1);
	BaseR=smooth100(BaseA2);
	check(BaseL,100,0);
	check(BaseR,100,0);
   
}

void DriveIntake(void)
{
		if(VarIntake==1)
		{
			SpeedIntake=100;
			StateIntake=1;
		}
		else if(VarIntake==-1)
		{
			SpeedIntake=-100;
			StateIntake=-1;
		}
		else
		{
			if(StateIntake==1)
			{
				SpeedIntake=100;
			}
			else
			{
				SpeedIntake=0;
			}
		}
}

void DriveLever(void)
{
   /* if(VarLever==1)
    {
        StateLever=1;
    }
    else if(VarLever==-1)
    {
        StateLever=-1;
    }
    if(StateLever==1)
    {
        lever_error_l=lever_load-(JDZ(LevelL.rotation(vex::rotationUnits::deg)));
        lever_error_r=lever_load-(JDZ(LevelR.rotation(vex::rotationUnits::deg)));
        SpeedLever=p_lever_load*(lever_error_l+lever_error_r)/2;
        //while(LevelL.rotateTo(-150,vex::rotationUnits::deg,100,vex::velocityUnits::pct));
        //LevelL.rotateTo(-150,vex::rotationUnits::deg,100,vex::velocityUnits::pct);
        //LevelR.rotateTo(150,vex::rotationUnits::deg,100,vex::velocityUnits::pct);
    }
    else if(StateLever==-1)
    {
        lever_error_l=lever_stop-(JDZ(LevelL.rotation(vex::rotationUnits::deg)));
        lever_error_r=lever_stop-(JDZ(LevelR.rotation(vex::rotationUnits::deg)));
        SpeedLever=p_lever_stop*(lever_error_l+lever_error_r)/2;
        if(JDZ(lever_error_l+lever_error_r)/2<=10)
        {
            LevelL.resetRotation();
            LevelR.resetRotation();
            //StateLever=0;
        }
    }*/
    SpeedLever=VarLever*100;
 }

 void DriveStick()
 {
 	if(StateStick==1)
 	{
 		HoldStick=1;
 	}
 	else if(StateStick==-1)
 	{
 		HoldStick=-1;
 	}
 	if(JDZ(VarStick)>30)
 	{
 		HoldStick=0;
 	}

 	if(HoldStick==1)
 	{
 		SpeedStick=(Stick_lv1-Stick.rotation(vex::rotationUnits::deg))*p_push;
 	}
 	else if(HoldStick==-1)
 	{
 		/*SpeedStick=-100;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
 		int a=5;
 		while(a>0)
 		{
 			vex::task::sleep(60);
 			a--;
 			if(Stick.rotation(vex::rotationUnits::deg)<220)
 			{
 				SpeedStick=0;
 			}
 		}

 		SpeedStick=100;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
 		vex::task::sleep(100);
 		 SpeedStick=0;
 		Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);
 		HoldStick=0;*/
 		SpeedStick=(Stick_lv2-Stick_kick-Stick.rotation(vex::rotationUnits::deg))*p_stick;
 		if(JDZ(Stick_lv2-Stick_kick-Stick.rotation(vex::rotationUnits::deg))<10)
 		{
 			SpeedStick=(Stick_lv2-Stick.rotation(vex::rotationUnits::deg))*p_stick;
 			HoldStick=0;
 		}
 	}
 	else
 	{
 		 SpeedStick=VarStick;
 	}

 }

void Run()
{
    BaseLF.spin(vex::directionType::fwd,BaseL,vex::velocityUnits::pct);
    BaseLB.spin(vex::directionType::fwd,BaseL,vex::velocityUnits::pct);
    BaseRF.spin(vex::directionType::fwd,BaseR,vex::velocityUnits::pct);
    BaseRB.spin(vex::directionType::fwd,BaseR,vex::velocityUnits::pct);
    
    if(StopFlag==1)
    {
        BaseLF.stop();
        BaseLB.stop();
        BaseRF.stop();
        BaseRB.stop();
    }

    LevelL.spin(vex::directionType::fwd,-SpeedLever,vex::velocityUnits::pct);
    LevelR.spin(vex::directionType::fwd,SpeedLever,vex::velocityUnits::pct);
   
    Intake.spin(vex::directionType::fwd,-SpeedIntake,vex::velocityUnits::pct);

    Stick.spin(vex::directionType::fwd,SpeedStick,vex::velocityUnits::pct);

}

void Drive(void)
{
	while(true)
	{
		JoystickInit();
		DriveBase();
		DriveIntake();
		DriveLever();
		DriveStick();
		Run();
	}
}
