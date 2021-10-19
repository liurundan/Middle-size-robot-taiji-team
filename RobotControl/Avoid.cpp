#include "StdAfx.h"
#include "Avoid.h"
#include "parameters.h"
#include "basicfunctions.h"
#include "OffenceParameter.h"
#include "InPlayParameters.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include <cmath>

using namespace std;

 
Avoid::Avoid()
{
	best_angle = 0.0;
}

void Avoid::EditObstacles()
{
	MWM.avoidData.Num = 0;
    int num = MWM.obstacles_num;
	MWM.avoidData.MaxAngle.resize(num);
	MWM.avoidData.MinAngle.resize(num);
	MWM.avoidData.Distance.resize(num);
    
	if( MWM.obstacles_num > 0 )
	{
		for(int i=0; i < MWM.obstacles_num; i++)
		{
			MWM.avoidData.MinAngle[i]= MWM.obstacle[i].minAngle;
			MWM.avoidData.MaxAngle[i]= MWM.obstacle[i].maxAngle;
			MWM.avoidData.Distance[i]= MWM.obstacle[i].distance;
		}
		MWM.avoidData.Num = MWM.obstacles_num;
		for( i=0; i < MWM.obstacles_num; i++ )
		{
			if( MWM.avoidData.MinAngle[i] > MWM.avoidData.MaxAngle[i] )
			{
				MWM.avoidData.MaxAngle[i] += 2.0*M_PI;
			}
		}
		if( MWM.obstacles_num > 1 )
		{
			ConnectObstacles();
		}
		for( i=0; i< MWM.avoidData.Num; i++)
		{
			if(MWM.avoidData.MaxAngle[i] > 2.0*M_PI)
			{
				MWM.avoidData.MaxAngle[i] -= 2.0*M_PI;
			}
		}
	}
}
 
void Avoid::ConnectObstacles()
{
	double tmp; // 按角度的大小升序排列障碍物
	for( int i=0; i <= MWM.avoidData.Num-2; i++)
	{
		for( int j = MWM.avoidData.Num - 1; j >= i+1; j--)
		{
			if(MWM.avoidData.MinAngle[j] < MWM.avoidData.MinAngle[j-1])
			{
				tmp = MWM.avoidData.MinAngle[j];
				MWM.avoidData.MinAngle[j] = MWM.avoidData.MinAngle[j-1];
				MWM.avoidData.MinAngle[j-1] = tmp;
				tmp = MWM.avoidData.MaxAngle[j];
				MWM.avoidData.MaxAngle[j] = MWM.avoidData.MaxAngle[j-1];
				MWM.avoidData.MaxAngle[j-1]=tmp;
				tmp=MWM.avoidData.Distance[j];
				MWM.avoidData.Distance[j]=MWM.avoidData.Distance[j-1];
				MWM.avoidData.Distance[j-1]=tmp;
			}
		}
	}
	// 
	for( i = MWM.avoidData.Num-2; i >= 0; i--)
	{
		double tm =  2e-3*Min2( MWM.avoidData.Distance[i],MWM.avoidData.Distance[i+1] ) +
			         sin(fabs(MWM.avoidData.MaxAngle[i]-MWM.avoidData.MinAngle[i] ) / 2 );

		if( ( MWM.avoidData.MinAngle[i+1] > MWM.avoidData.MaxAngle[i] ) && 
			( 2e-3*Min2( MWM.avoidData.Distance[i],MWM.avoidData.Distance[i+1] ) +
			sin(fabs(MWM.avoidData.MaxAngle[i]-MWM.avoidData.MinAngle[i] ) / 2 )
			< DISTANCE_UNIT_OBSTACLES ) )
		{	
			MWM.avoidData.MaxAngle[i] = MWM.avoidData.MaxAngle[i+1];
			MWM.avoidData.Distance[i] =
				Min2(MWM.avoidData.Distance[i],MWM.avoidData.Distance[i+1]);
			for(int j=i+1; j<MWM.avoidData.Num; j++)
			{
				MWM.avoidData.MinAngle[j]=MWM.avoidData.MinAngle[j+1];
				MWM.avoidData.MaxAngle[j]=MWM.avoidData.MaxAngle[j+1];
				MWM.avoidData.Distance[j]=MWM.avoidData.Distance[j+1];
			}
			MWM.avoidData.Num--;
		}
		else if((MWM.avoidData.MinAngle[i+1] < MWM.avoidData.MaxAngle[i]) && 
			(MWM.avoidData.MaxAngle[i+1]>MWM.avoidData.MaxAngle[i]))
		{	
			MWM.avoidData.MaxAngle[i] = MWM.avoidData.MaxAngle[i+1];
			MWM.avoidData.Distance[i] = Min2(MWM.avoidData.Distance[i],MWM.avoidData.Distance[i+1]);
			for(int j=i+1;j<MWM.avoidData.Num;j++)
			{
				MWM.avoidData.MinAngle[j] = MWM.avoidData.MinAngle[j+1];
				MWM.avoidData.MaxAngle[j] = MWM.avoidData.MaxAngle[j+1];
				MWM.avoidData.Distance[j] = MWM.avoidData.Distance[j+1];
			}
			MWM.avoidData.Num--;
		}
		else if((MWM.avoidData.MinAngle[i+1] > MWM.avoidData.MinAngle[i]) && 
			(MWM.avoidData.MaxAngle[i] > MWM.avoidData.MaxAngle[i+1]))
		{
			MWM.avoidData.Distance[i] = 
				Min2(MWM.avoidData.Distance[i],MWM.avoidData.Distance[i+1]);
			for(int j=i+1; j<MWM.avoidData.Num; j++)
			{
				MWM.avoidData.MinAngle[j]=MWM.avoidData.MinAngle[j+1];
				MWM.avoidData.MaxAngle[j]=MWM.avoidData.MaxAngle[j+1];
				MWM.avoidData.Distance[j]=MWM.avoidData.Distance[j+1];
			}
			MWM.avoidData.Num--;
		}
	}
	for( i=0; i <= MWM.avoidData.Num-2; i++)
	{
		for(int j = MWM.avoidData.Num - 1; j >= i+1; j--)
		{
			if(MWM.avoidData.Distance[j] < MWM.avoidData.Distance[j-1])
			{
				tmp = MWM.avoidData.MinAngle[j];
				MWM.avoidData.MinAngle[j] = MWM.avoidData.MinAngle[j-1];
				MWM.avoidData.MinAngle[j-1] = tmp;
				tmp = MWM.avoidData.MaxAngle[j];
				MWM.avoidData.MaxAngle[j] = MWM.avoidData.MaxAngle[j-1];
				MWM.avoidData.MaxAngle[j-1]=tmp;
				tmp=MWM.avoidData.Distance[j];
				MWM.avoidData.Distance[j]=MWM.avoidData.Distance[j-1];
				MWM.avoidData.Distance[j-1]=tmp;
			}
		}
	}
}

void Avoid::DecideVAR(double *Velocity, double *GoAngle, double *ROme )
{
	Time t;
	double tmp;	
	static double countTime = myGetTickCount() ;
	static int count=0;
	int have = 0, ob2 = 0;

	if(MWM.avoidData.Num > 0)
	{
		for(int i=0; i<MWM.avoidData.Num; i++)
		{
			tmp = atan(AVOID_DISTANCE_FOR_MY_BODY / MWM.avoidData.Distance[i]);
			if(MWM.avoidData.Distance[i] < 2000.0 && 
				(MWM.avoidData.MinAngle[i] - tmp < *GoAngle) && 
				(MWM.avoidData.MaxAngle[i] + tmp > *GoAngle))
			{
				if( WBOARD->doPossesBall(t) == true && 
					MWM.avoidData.Distance[i] < AVOID_STOP_DISTANCE && 
					MWM.avoidData.MaxAngle[i] > 0. && 
					MWM.avoidData.MinAngle[i] < 0.)
				{
					*Velocity = 0.0; 

#if AVOID_NUKASANAI
					if(MWM.avoidData.Distance[i] < 700 && 
						fabs(MWM.avoidData.MaxAngle[i]+MWM.avoidData.MinAngle[i])/2 < 30*DegreeToRad)
					{
						if(MWM.ball2Robot.angle > 10*DegreeToRad)
							*GoAngle = 75*DegreeToRad;//MWM.goal2Robot.angle + M_PI/2;
						else if(MWM.ball2Robot.angle < -10*DegreeToRad)
							*GoAngle = -75*DegreeToRad;//MWM.goal2Robot.angle - M_PI/2;
					}
#else
					if(MWM.ball2Robot.angle > MWM.avoidData.MinAngle[i] && 
						MWM.ball2Robot.angle < MWM.avoidData.MaxAngle[i])
					{
						if( myGetTickCount() - countTime> 100.0)
						{
							count++;
							countTime = myGetTickCount();
						}
						if(fabs(MWM.goal2Robot.angle) > 20*DegreeToRad)	
							*ROme = 5.0*MWM.goal2Robot.angle;
						else if(count%2==0)	
							*ROme = 2.0;
						else			
							*ROme = -2.0;
					}
#endif
				}
				else
				{
					for(int j=i; j<MWM.avoidData.Num; j++)
					{
						if(MWM.avoidData.Distance[j] < 2000.0 && 
							(MWM.avoidData.MinAngle[j] - tmp < *GoAngle) && 
							(MWM.avoidData.MaxAngle[j] + tmp > *GoAngle))
						{
								ob2 = j;
								break;
						}
					}
					if(ob2 == 0)
					{
						if(fabs(MWM.avoidData.MinAngle[i] - *GoAngle) < 
							fabs(MWM.avoidData.MaxAngle[i] - *GoAngle))
						{
							*GoAngle = Limit2PI(MWM.avoidData.MinAngle[i] - tmp);
						}
						else
						{	
							*GoAngle = Limit2PI(MWM.avoidData.MaxAngle[i] + tmp);
						}
					}
					else
					{
						if(MWM.avoidData.MaxAngle[i] > MWM.avoidData.MaxAngle[ob2])
						{
							*GoAngle = Limit2PI(MWM.avoidData.MinAngle[i] - tmp); // + 
						}
						else
						{	
							*GoAngle = Limit2PI(MWM.avoidData.MaxAngle[i] + tmp); // -
						}
					}
				}
				break;	
			}
		}
	}
}


void Avoid::Avoid2007(double *Velocity, double *GoAngle, double *ROme )
{
	double Velocity_tmp, GoAngle_tmp, ROme_tmp;

	Velocity_tmp = *Velocity;
	GoAngle_tmp = *GoAngle;
	ROme_tmp = *ROme;

	EditObstacles();
	DecideVAR(&Velocity_tmp, &GoAngle_tmp, &ROme_tmp );

	*GoAngle = GoAngle_tmp;
	*Velocity = Velocity_tmp;
	*ROme = ROme_tmp;
}


double Avoid::AvoidTurn(double GoAngle)
{
	double CmbDis;			
	double MgnDis;
	int num = MWM.obstacles_num;

	vector<double> minAngle;
	vector<double> maxAngle;
	vector<double> Distance;
	minAngle.resize(num);
	maxAngle.resize(num);
	Distance.resize(num);

	//double minAngle[MAX_OBSTACLES];	
	//double maxAngle[MAX_OBSTACLES];
	//double Distance[MAX_OBSTACLES];	
	double tmp;				
	int obs = 0;		
	int i,j;

	CmbDis = 2.0;	
	MgnDis = 1500;	
	double obsDistance = 4000.0;

	if(MWM.obstacles_num > 0)
	{	
		for(i=0; i<MWM.obstacles_num; i++)
		{
			minAngle[obs]=MWM.obstacle[i].minAngle;
			maxAngle[obs]=MWM.obstacle[i].maxAngle;
			Distance[obs]=MWM.obstacle[i].distance;
			obs++;
		}
		for(i=0;i<obs;i++)
		{
			if(minAngle[i]>maxAngle[i])
			{	
				maxAngle[i] += 2.0*M_PI;
			}	
		}

		if(obs>1)
		{
			for (i=0;i<=obs-2;i++)
			{
				for(j=obs-1;j>=i+1;j--)
				{	
					if(minAngle[j]<minAngle[j-1])
					{	
						tmp=minAngle[j];
						minAngle[j]=minAngle[j-1];
						minAngle[j-1]=tmp;
						tmp=maxAngle[j];
						maxAngle[j]=maxAngle[j-1];
						maxAngle[j-1]=tmp;
						tmp=Distance[j];
						Distance[j]=Distance[j-1];
						Distance[j-1]=tmp;
					}	
				}
			}	

			for(i=obs-2;i>=0;i--)
			{
				if((minAngle[i+1]>maxAngle[i]) && 
					(2e-3*Min2(Distance[i],Distance[i+1])+
					sin(fabs(maxAngle[i]-minAngle[i])/2)<CmbDis))
				{	
					maxAngle[i]=maxAngle[i+1];
					Distance[i]=Min2(Distance[i],Distance[i+1]);
					for(j=i+1;j<obs;j++)
					{
						minAngle[j]=minAngle[j+1];
						maxAngle[j]=maxAngle[j+1];
						Distance[j]=Distance[j+1];
					}
					obs--;
				}
				else if((minAngle[i+1]<maxAngle[i]) && 
					(maxAngle[i+1]>maxAngle[i]))
				{	
					maxAngle[i]=maxAngle[i+1];
					Distance[i]=Min2(Distance[i],Distance[i+1]);
					for(j=i+1;j<obs;j++)
					{
						minAngle[j]=minAngle[j+1];
						maxAngle[j]=maxAngle[j+1];
						Distance[j]=Distance[j+1];
					}	
					obs--;
				}
				else if((minAngle[i+1]>minAngle[i]) && 
					(maxAngle[i]>maxAngle[i+1]))
				{
					Distance[i]=Min2(Distance[i],Distance[i+1]);
					for(j=i+1;j<obs;j++)
					{	
						minAngle[j]=minAngle[j+1];
						maxAngle[j]=maxAngle[j+1];
						Distance[j]=Distance[j+1];
					}	
					obs--;
				}	
			}	
		}

		for(i=0;i<obs;i++)
		{	
			if(maxAngle[i]>2.0*M_PI)
			{
				maxAngle[i]-=2.0*M_PI;
			}
		}
		for(i=0;i<obs;i++)
		{
			if(Distance[i] < 2000.0 && Distance[i] > 0.0)
			{
				tmp=30*2.0*DegreeToRad;
			}
			else
			{
				tmp=30*DegreeToRad;
			}
			if((minAngle[i]-tmp<GoAngle) && 
				(maxAngle[i]+tmp>GoAngle) && (Distance[i] < obsDistance))
			{
				if(fabs(minAngle[i]-GoAngle)<fabs(maxAngle[i]-GoAngle))
				{
					GoAngle=Limit2PI(minAngle[i]-tmp);
				}	
				else
				{
					GoAngle=Limit2PI(maxAngle[i]+tmp);
				}
			}	
		}
	}
	return(GoAngle);
}


double Avoid::Avoid2006(double GoAngle )
{

	double CmbDis;		
	double MgnDis;		
	double minAngle[MAX_OBSTACLES];	
	double maxAngle[MAX_OBSTACLES];
	double Distance[MAX_OBSTACLES];	
	double tmp;			
	int obs = MWM.obstacles_num;		
	int i,j;

	CmbDis = 1.5;
	MgnDis = 2000.0;	

#if 0
printf("                                            \n");
printf("                                                  \n");
printf("obs=%d                                            \n",MWM.obstacles_num);
for(i=0;i<MWM.obstacles_num;i++){
	printf("min=%f max=%f dis=%f\n",MWM.obstacle[i].minAngle,MWM.obstacle[i].maxAngle,MWM.obstacle[i].distance);
}
#endif

	if(obs>0  && obs<MAX_OBSTACLES)
	{
		for(i=0;i<MWM.obstacles_num;i++)
		{
			minAngle[i]=MWM.obstacle[i].minAngle;
			maxAngle[i]=MWM.obstacle[i].maxAngle;
			Distance[i]=MWM.obstacle[i].distance;
			if(Distance[i]<0.)	Distance[i]=0.1;
		}
		for (i=0;i<obs;i++)
		{
			if(minAngle[i]>maxAngle[i])
			{
				maxAngle[i] += 2.0*M_PI;
			}
		}	
		if(obs>1)
		{	
			for (i=0;i<=obs-2;i++)
			{	
				for(j=obs-1;j>=i+1;j--)
				{
					if(minAngle[j]<minAngle[j-1])
					{	
						tmp=minAngle[j];
						minAngle[j]=minAngle[j-1];
						minAngle[j-1]=tmp;
						tmp=maxAngle[j];
						maxAngle[j]=maxAngle[j-1];
						maxAngle[j-1]=tmp;
						tmp=Distance[j];
						Distance[j]=Distance[j-1];
						Distance[j-1]=tmp;
					}	
				}	
			}	
			for(i=obs-2;i>=0;i--)
			{	
				if((minAngle[i+1]>maxAngle[i]) && 
					(2e-3*Min2(Distance[i],Distance[i+1])+
					sin(fabs(maxAngle[i]-minAngle[i])/2)<CmbDis))
				{	
					maxAngle[i]=maxAngle[i+1];
					Distance[i]=Min2(Distance[i],Distance[i+1]);
					for(j=i+1;j<obs;j++)
					{	
						minAngle[j]=minAngle[j+1];
						maxAngle[j]=maxAngle[j+1];
						Distance[j]=Distance[j+1];
					}	
					obs--;
				}
				else if((minAngle[i+1]<maxAngle[i]) && 
					(maxAngle[i+1]>maxAngle[i]))
				{
					maxAngle[i]=maxAngle[i+1];
					Distance[i]=Min2(Distance[i],Distance[i+1]);
					for(j=i+1;j<obs;j++)
					{
						minAngle[j]=minAngle[j+1];
						maxAngle[j]=maxAngle[j+1];
						Distance[j]=Distance[j+1];
					}
					obs--;
				}
				else if((minAngle[i+1]>minAngle[i]) && 
					(maxAngle[i]>maxAngle[i+1]))
				{
					Distance[i]=Min2(Distance[i],Distance[i+1]);
					for(j=i+1;j<obs;j++)
					{
						minAngle[j]=minAngle[j+1];
						maxAngle[j]=maxAngle[j+1];
						Distance[j]=Distance[j+1];
					}	
					obs--;
				}
			}	
		}	
		for(i=0;i<obs;i++)
		{	
			if(maxAngle[i]>2.0*M_PI)
			{	
				maxAngle[i]-=2.0*M_PI;
			}
		}
		for(i=0;i<obs;i++)
		{
			tmp=atan(MgnDis/Distance[i]);
			if((minAngle[i]-tmp<GoAngle) && (maxAngle[i]+tmp>GoAngle))
			{
				if(fabs(minAngle[i]-GoAngle)<fabs(maxAngle[i]-GoAngle))
				{
					GoAngle=Limit2PI(minAngle[i]-tmp);
				}
				else
				{
					GoAngle=Limit2PI(maxAngle[i]+tmp);
				}	
			}	
		}	
	}

	return(GoAngle);
}

double Avoid::Avoid2006Ball(double GoAngle)
{
	double CmbDis;		
	double MgnDis;		
	double BalAng;		
	double BalDis;		
	double minAngle[10];	
	double maxAngle[10];
	double Distance[10];
	double BallAngle;	
	double BallDistance;	
	double tmp;			
	int obs = 0;			
	int i,j;

	CmbDis = 1.0;		
	MgnDis = 1000.0;		
	BalAng = 0.6;	
	BalDis = 300;	

	BallAngle = MWM.ball2Robot.angle;
	BallDistance = MWM.ball2Robot.distance;

    obs = 0;
	if(MWM.obstacles_num>0)
	{	
		for(i=0;i<MWM.obstacles_num;i++)
		{
			minAngle[obs]=MWM.obstacle[i].minAngle;
			maxAngle[obs]=MWM.obstacle[i].maxAngle;
			Distance[obs]=MWM.obstacle[i].distance;
			obs++;
		}
		for (i=0;i<obs;i++)
		{
			if(minAngle[i]>maxAngle[i])
			{
				maxAngle[i] += 2.0*M_PI;
			}	
		}
		
		if(obs>1)
		{	
			for (i=0;i<=obs-2;i++)
			{	
				for(j=obs-1;j>=i+1;j--)
				{	
					if(minAngle[j]<minAngle[j-1])
					{	
						tmp=minAngle[j];
						minAngle[j]=minAngle[j-1];
						minAngle[j-1]=tmp;
						tmp=maxAngle[j];
						maxAngle[j]=maxAngle[j-1];
						maxAngle[j-1]=tmp;
						tmp=Distance[j];
						Distance[j]=Distance[j-1];
						Distance[j-1]=tmp;
					}	
				}	
			}	
		} 
		
		for(i=obs-2;i>=0;i--)
		{	
			if((minAngle[i+1]>maxAngle[i]) && 
				(2e-3*Min2(Distance[i],Distance[i+1])+
				sin(fabs(maxAngle[i]-minAngle[i])/2)<CmbDis))
			{	
				maxAngle[i]=maxAngle[i+1];
				Distance[i]=Min2(Distance[i],Distance[i+1]);
				for(j=i+1;j<obs;j++)
				{
					minAngle[j]=minAngle[j+1];
					maxAngle[j]=maxAngle[j+1];
					Distance[j]=Distance[j+1];
				}	
				obs--;
			}
			else if((minAngle[i+1]<maxAngle[i]) && 
				(maxAngle[i+1]>maxAngle[i]))
			{
				maxAngle[i]=maxAngle[i+1];
				Distance[i]=Min2(Distance[i],Distance[i+1]);
				for(j=i+1;j<obs;j++)
				{	
					minAngle[j]=minAngle[j+1];
					maxAngle[j]=maxAngle[j+1];
					Distance[j]=Distance[j+1];
				}
				obs--;
			}	
			else if((minAngle[i+1]>minAngle[i]) && 
				(maxAngle[i]>maxAngle[i+1]))
			{	
				Distance[i]=Min2(Distance[i],Distance[i+1]);
				for(j=i+1;j<obs;j++)
				{	
					minAngle[j]=minAngle[j+1];
					maxAngle[j]=maxAngle[j+1];
					Distance[j]=Distance[j+1];
				}	
				obs--;
			}	
		}	
	}	
	
	for(i=0;i<obs;i++)
	{	
		if(maxAngle[i]>2*M_PI)
		{	
			maxAngle[i]-=2*M_PI;
		}
	}	
	
	for(i=0;i<obs;i++)
	{	
		tmp=atan(MgnDis/Distance[i]);
		if((minAngle[i]-tmp<GoAngle) && (maxAngle[i]+tmp>GoAngle))
		{	
			if ((fabs(GoAngle-BallAngle)< BalAng) && (BallDistance-BalDis < Distance[i]))
			{;
			}
			else
			{
				if(fabs(minAngle[i]-GoAngle)<fabs(maxAngle[i]-GoAngle))
				{
					GoAngle=Limit2PI(minAngle[i]-tmp);
				}	
				else
				{
					GoAngle=Limit2PI(maxAngle[i]+tmp);
				}	
			}	
		}	
	}	
 
	return (GoAngle);

}


double Avoid::Min2(double AA,double BB)
{
	if(AA<=BB)	return(AA);
	else		return(BB);
}

double Avoid::Avoid2005(double GoAngle)
{
	double K;      
	int i=0, j=0;
	int obNum=0, ob[22]={0};  
	double obMinDis=0.0;  
	int minNum=0;   
	double Velocity = 4.0;

	K=0.55+0.03*Velocity;

	for(i=0;i<MWM.obstacles_num;i++)
	{
		if(GoAngle-0.3 < MWM.obstacle[i].angle && MWM.obstacle[i].angle < GoAngle+0.3 ||
			GoAngle-0.3 < MWM.obstacle[i].maxAngle && MWM.obstacle[i].maxAngle < GoAngle+0.3 ||
			GoAngle-0.3 < MWM.obstacle[i].minAngle && MWM.obstacle[i].minAngle < GoAngle+0.3){
			ob[obNum]=i;
			obNum++;
		}
	}

	if(obNum!=0)
	{
		obMinDis=MWM.obstacle[ob[0]].distance;
		minNum=ob[0];
		for(i=1;i<obNum;i++)
		{
			if(obMinDis > MWM.obstacle[ob[i]].distance)
			{
				obMinDis=MWM.obstacle[ob[i]].distance;
				minNum=ob[i];
			}
		}
	}

	if(obNum)
	{          
		if(GoAngle-MWM.obstacle[minNum].angle > 0) GoAngle = Limit2PI(GoAngle+K/obMinDis);
		else GoAngle = Limit2PI(GoAngle-K/obMinDis);
	}
	return(GoAngle);
}

