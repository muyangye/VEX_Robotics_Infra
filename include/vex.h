#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"
//
// Creates a competition object that allows access to Competition methods.
// vex::competition    Competition;

vex::brain Brain;
vex::controller Controller = vex::controller();

vex::motor BaseLF = vex::motor(vex::PORT1,vex::gearSetting::ratio18_1);
vex::motor BaseLB = vex::motor(vex::PORT2,vex::gearSetting::ratio18_1);
vex::motor BaseRF = vex::motor(vex::PORT3,vex::gearSetting::ratio18_1);
vex::motor BaseRB = vex::motor(vex::PORT4,vex::gearSetting::ratio18_1);
vex::motor LevelL = vex::motor(vex::PORT5,vex::gearSetting::ratio18_1);
vex::motor LevelR = vex::motor(vex::PORT6,vex::gearSetting::ratio18_1);
vex::motor Intake = vex::motor(vex::PORT7,vex::gearSetting::ratio6_1);
vex::motor Stick = vex::motor(vex::PORT8,vex::gearSetting::ratio18_1);
vex::bumper Bumper = vex::bumper(Brain.ThreeWirePort.A);



//var for base
float BaseL;
float BaseR;
float BaseA1[100];
float BaseA2[100];
float ABA1[100];
float ABA2[100];
int StateBase;
int StopFlag;

//var for intake
int VarIntake;
float StateIntake;
float SpeedIntake;
int CountIntake;

//var for lever;
int VarLever;
int StateLever;
float lever_load;
float SpeedLever;
float p_lever_load;
float d_lever_load;
float lever_stop;
float p_lever_stop;
float lever_error_l;
float lever_error_r;
float pre_lever_error;
int LockLever;


//var for claw
int VarClaw;
int StateClaw;
int SpeedClaw;
int KeepClaw;

//var for stick
int VarStick;
int SpeedStick;
int StateStick;
int HoldStick;
int Stick_lv1;
int Stick_lv2;
int Stick_kick;
float p_stick;
float p_push;

//auto init
float A_basegoto_p;
float A_baseturn_p;
float A_basegoto_d;

//test
float test_lever;
float test_arm;
float test_base;
int test_10;
int test_12;
//var for system
int joystick_choose;
int p_lever_choose;

//var for auto
int color_choose;
int dis_choose;

float APre_error;

float encoderL;
float encoderR;
float ABaseSpeedL;
float ABaseSpeedR;
float AStopSpeed;
int timeswitch1;
int timeswitch2;
int AAcount;
float ABaseSpeed;

//var for slow drive
float SlowBase;
float VarSlowBase;

void init(void)
{
	//var for system pre
	lever_load=150;//2800
	lever_stop=350;
	//var for system pid
	p_lever_load=0.6;
	d_lever_load=0;
	p_lever_stop=0.6;
	//var for arm PID
	p_stick=3;
    p_push=1;
	//Var for System choose
	joystick_choose=1;
	p_lever_choose=1;
	//Var for keep
	KeepClaw=25;
	//var for stick place
	Stick_lv1=320;//push enemy
	Stick_lv2=463;//bottom
	Stick_kick=150;


	// test
	test_lever=0;



	//Var for base init
	BaseL=0;
	BaseR=0;
	StateBase=1;
    StopFlag=0;
	//Var for intake init
	VarIntake=0;
	StateIntake=0;
	SpeedIntake=0;
	CountIntake=0;
	//Var for lever init
	VarLever=0;
	StateLever=0;
	SpeedLever=0;
	lever_error_l=0;
    lever_error_r=0;
	pre_lever_error=0;
	LockLever=0;
	//var for Stick init
	VarStick=0;
	SpeedStick=0;
	StateStick=0;
	HoldStick=0;
	//var for auto
	color_choose=0;
	dis_choose=0;
	// var for encoder
	encoderL=0;
	encoderR=0;
	ABaseSpeedL=0;
	ABaseSpeedR=0;
	ABaseSpeed=1;
	AStopSpeed=0;
	timeswitch1=1;
	timeswitch2=1;
	AAcount=0;

	//Au init
	for(int i=0;i<99;i++)
	{
		BaseA1[i]=0;
		BaseA2[i]=0;
        ABA1[i]=0;
        ABA2[i]=0;
	}

	//new
	SlowBase=0.6;
	VarSlowBase=0;
    
    /*LevelL.setMaxTorque(1,vex::percentUnits::pct);
    while(!Controller.ButtonX.pressing())
    {   
        LevelL.spin(vex::directionType::fwd,-1,vex::velocityUnits::pct);
    }
    LevelL.spin(vex::directionType::fwd,0,vex::velocityUnits::pct);
    LevelL.setMaxTorque(100,vex::percentUnits::pct);
    LevelL.resetRotation();
    LevelR.resetRotation();
    //LevelL.setRotation(0,vex::rotationUnits::deg);
    //LevelR.setRotation(0,vex::rotationUnits::deg);*/
    Stick.resetRotation();

}


void check(float num, float limit, float sign)
{
	if(sign == 1)
	{
		if(num > limit)
		{
			num = limit;
		}
	}
	if(sign == -1)
	{
		if(num < limit)
		{
			num = limit;
		}
	}
	if(sign == 0)
	{
		if(num > limit)
		{
			num = limit;
		}
		if(num < -limit)
		{
			num = -limit;
		}
	}
}



float JDZ(float x)
{
	if(x<=0)
	{
		x=-x;
	}
	else
	{
		x=x;
	}
	return x;
}

/*---------------------------------------------------------------------------*/
/*                function of filter of bottom number                        */
/*---------------------------------------------------------------------------*/
float bottomfilter(float limit, float num)
{
	if(JDZ(num)<limit)
	{
		num=0;
	}
	return num;
}




/*---------------------------------------------------------------------------*/
/*                 function of square filter                                 */
/*---------------------------------------------------------------------------*/
void cubefilter(float num)
{
	num=1/(10000+(126-JDZ(num)))*num*num*num;
}




/*---------------------------------------------------------------------------*/
/*                 function of 10 smooth filter                              */
/*---------------------------------------------------------------------------*/
float smooth100(float* a)
{
	float ans;
	for(int i=0;i<100;i++)
	{
		ans=a[i]+ans;
	}
	ans=ans/100;
	// aerage filter
	return ans;
}




/*---------------------------------------------------------------------------*/
/*                 function of AU Renew                                      */
/*---------------------------------------------------------------------------*/
void renewAU(float* a,float num)
{
	float f=0;
	for(int i=0;i<99;i++)
	{
		a[i+1]=a[i];
	}
	f=4*a[1]-6*a[2]+4*a[3]-a[4]-num;
	if(JDZ(f)<10)
	{
	a[0]=4*a[1]-6*a[2]+4*a[3]-a[4];
	}
	else
	{
		a[0]=num+3*a[1]-6*a[2]+4*a[3]-a[4];
	}
}
/*---------------------------------------------------------------------------*/
/*                 function of difference filter                             */
/*---------------------------------------------------------------------------*/
float differencefilter(float left,float right,float differencestandar)
{
	float difference;
	difference=left-right;
	if(difference<differencestandar && difference>-differencestandar)
	{
		difference=0;
	}
	return difference;
}
/*---------------------------------------------------------------------------*/
/*                 function of Cycle filter                                  */
/*---------------------------------------------------------------------------*/
float cyclefilter(float a)
{
    while(a>360)
    {
        a=a-360;
    }
    return a;
}
