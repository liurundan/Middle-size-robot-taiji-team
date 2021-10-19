#include "StdAfx.h"
#include "DistanceCalibration.h"
#include "Journal.h"
#include <fstream>
#include <cstdio>
#include <cmath>

using namespace std;

DistanceCalibration::DistanceCalibration ( Vec p) : image_center (p) {;}

DistanceCalibration::~DistanceCalibration () {;}

double DistanceCalibration::search_min_transition_index (const std::deque<MarkerInfo>& m, double dmin) 
{
	double min=1e300;
	for (unsigned int i=0; i<m.size(); i++)
		if (m[i].image_distance>dmin && m[i].image_distance<min) 
		{
			min = m[i].image_distance;
		}
		return min;
}

void DistanceCalibration::search_markers (const  ScanResultList& scan_result,  Angle dir) 
{
	const Angle viergrad = Angle::deg_angle (4);
	
	deque<MarkerInfo> blue_white;
	deque<MarkerInfo> white_blue;
	bool dir_okay=false;
	vector<Transition>::const_iterator it = scan_result.results[COLOR_BLUE]->transitions.begin();
	vector<Transition>::const_iterator itend = scan_result.results[COLOR_BLUE]->transitions.end();
	while(it<itend) 
	{
		Angle ta = (it->toPos-image_center).angle();
		if (ta.in_between (dir-viergrad, dir+viergrad))
		{
			if (it->type==Transition::START && it->twoStep==COLOR_LINE)  
			{
				MarkerInfo mi;
				mi.angle = ta;
				mi.true_distance = 0;
				mi.image_distance = (it->toPos-image_center).length();
				white_blue.push_back (mi);
			} 
			else if (it->type==Transition::END && it->twoStep==COLOR_LINE) // COLOR_FIELD
			{
				MarkerInfo mi;
				mi.angle = ta;
				mi.true_distance = 0;
				mi.image_distance = (it->toPos-image_center).length();
				blue_white.push_back(mi);
			}
			
			double imagedist4000, imagedist3000, imagedist2000, imagedist1000, imagedist500;
			double imagedist3700, imagedist2700, imagedist1700, imagedist700;
			imagedist500  = search_min_transition_index (blue_white, 20);
			imagedist700  = search_min_transition_index (white_blue, imagedist500);
			imagedist1000 = search_min_transition_index (blue_white, imagedist700);
			imagedist1700 = search_min_transition_index (white_blue, imagedist1000);
			imagedist2000 = search_min_transition_index (blue_white, imagedist1700);
			imagedist2700 = search_min_transition_index (white_blue, imagedist2000);
			imagedist3000 = search_min_transition_index (blue_white, imagedist2700);
			imagedist3700 = search_min_transition_index (white_blue, imagedist3000);
			imagedist4000 = search_min_transition_index (blue_white, imagedist3700);
			
			if (imagedist4000<1e100) 
			{ 
				dir_okay=true;
				for (unsigned int i=0; i<blue_white.size(); i++) 
				{
					if (blue_white[i].image_distance<imagedist700) 
					{
						blue_white[i].true_distance=1;
						markers.push_back (blue_white[i]);
					} 
					else if (blue_white[i].image_distance>imagedist700 && blue_white[i].image_distance<imagedist1700) 
					{
						blue_white[i].true_distance=2;
						markers.push_back (blue_white[i]);
					} 
					else if (blue_white[i].image_distance>imagedist1700 && blue_white[i].image_distance<imagedist2700) 
					{
						blue_white[i].true_distance=3;
						markers.push_back (blue_white[i]);
					} 
					else if (blue_white[i].image_distance>imagedist2700 && blue_white[i].image_distance<imagedist3700)
					{
						blue_white[i].true_distance=4;
						markers.push_back (blue_white[i]);
					} 
					else if (blue_white[i].image_distance>imagedist3700 && blue_white[i].image_distance<imagedist4000+1) 
					{
						blue_white[i].true_distance=5;
						markers.push_back (blue_white[i]);
					}
				}
			}
		}
		it++;
	}
	
	if (dir_okay) 
	{
		it    = scan_result.results[COLOR_BALL]->transitions.begin();
		itend = scan_result.results[COLOR_BALL]->transitions.end();
		while (it<itend) 
		{
			Angle ta = (it->toPos-image_center).angle();
			if (ta.in_between (dir-viergrad, dir+viergrad)) 
			{
				if (it->type==Transition::START && it->twoStep==COLOR_LINE) 
				{
					MarkerInfo mi;
					mi.angle = ta;
					mi.true_distance = 6;
					mi.image_distance = (it->toPos-image_center).length();
					markers.push_back (mi);
				} 
				else if (it->type==Transition::END) 
				{
					MarkerInfo mi;
					mi.angle = ta;
					mi.true_distance = 7;
					mi.image_distance = (it->toPos-image_center).length();
					markers.push_back (mi);
				}
			}
			it++;
		}
	}
}

void DistanceCalibration::writeMarkerFile (const std::string& filename) 
{
	//double true_distances [] = { 0, 500, 1000, 2000, 3000, 4000, 8000, 16000 };
	double true_distances [] = { 0, 1000, 2000, 3000, 4000, 6000, 8000, 16000 };
	const unsigned int num_markers = 8;
	
	string filenamebak = filename+".bak";
	if (rename (filename.c_str(), filenamebak.c_str())!=0)
		JERROR("Could not rename marker file!");
	ofstream dest (filename.c_str());
	if( !dest )
		JERROR("Could not open marker file! ");
	
	dest << 640 << ' ' << 480 << ' ' << static_cast<int>(image_center.x) << ' ' <<
		static_cast<int>(image_center.y) << '\n';
	dest << 3 << ' ' << num_markers+1 << '\n';
	for (unsigned int i=0; i<num_markers; i++)
		dest << true_distances[i] << ' ';
	dest << "1000000" << '\n';
	
	vector<double> values (num_markers);
	vector<double> values2 (num_markers);
	vector<unsigned int> value_count (num_markers);
	vector<unsigned int> iter_wo_assignments (num_markers);
	vector<double> prototypes (num_markers);
	prototypes[0] = 0;
	prototypes[1] = 40;
	prototypes[2] = 45;
	prototypes[3] = 50;
	prototypes[4] = 55;
	prototypes[5] = 60;
	prototypes[6] = 180;
	prototypes[7] = 200;
	for( unsigned int j=0; j<num_markers; j++)
		iter_wo_assignments[j]=0;
	
	for (int angleiter=-45; angleiter<45; angleiter++) 
	{  
		
		Angle cangle = Angle::deg_angle (angleiter*8);
		Angle leftangle = Angle::deg_angle (angleiter*8+10);
		Angle rightangle = Angle::deg_angle (angleiter*8-10);
		for (unsigned int i3=0; i3<num_markers; i3++) 
		{
			values[i3]=6*prototypes[i3];   
			value_count[i3]=6;
		}
		
		unsigned int trial=0;
		bool clusters_okay=false;
		while (!clusters_okay && trial<3) 
		{
			trial++;
			for (unsigned int kmeansiter=0; kmeansiter<10; kmeansiter++) 
			{
				for (unsigned int i1=1; i1<=5; i1++) 
				{
					values[i1]=6*prototypes[i1];
					values2[i1]=6*prototypes[i1]*prototypes[i1];
					value_count[i1]=6;
				}
				
				deque<MarkerInfo>::const_iterator it = markers.begin();
				deque<MarkerInfo>::const_iterator itend = markers.end();
				while (it<itend) 
				{
					if (it->true_distance<=5 && it->angle.in_between (rightangle, leftangle)) 
					{
						unsigned int bestprototype=0;
						double bestdist=1e300;
						for (unsigned int i=1; i<=5; i++) 
						{
							double d = fabs(prototypes[i]-it->image_distance);
							if (d<bestdist) 
							{
								bestdist=d;
								bestprototype=i;
							}
						}
						values[bestprototype]+=it->image_distance;
						values2[bestprototype]+=it->image_distance*it->image_distance;
						value_count[bestprototype]++;
						iter_wo_assignments[bestprototype]=0;
					}
					it++;
				}
				for (unsigned int i2=1; i2<=5; i2++) 
				{
					prototypes[i2]=values[i2]/=(static_cast<double>(value_count[i2])+1e-20);  //1e-20表示10的负20次方 
					values2[i2]=values2[i2]/(static_cast<double>(value_count[i2])+1e-20)-values[i2]*values[i2];
				}
				
			}
			clusters_okay=true;
			unsigned int similar_clusters=0;
			unsigned int cluster_wo_assignments=0;
			
			for (unsigned int k=1; k<5; k++) 
			{
				if( fabs(values[k]-values[k+1])<=5) 
				{
					clusters_okay=false;
					similar_clusters=k+1;
				}
			}
			for(unsigned int k1=1; k1<=5; k1++) 
			{
				iter_wo_assignments[k1]++;
				if (iter_wo_assignments[k1]>10) 
				{
					clusters_okay=false;
					cluster_wo_assignments=k1;
				}
			}
			if (angleiter>=0)
				clusters_okay=true;
			if (!clusters_okay) 
			{
				unsigned int broad_cluster=0;
				double max_var=0;
				for (unsigned int i=1; i<=5; i++) 
				{
					if (values2[i]>max_var && i!=similar_clusters && i!=cluster_wo_assignments) 
					{
						max_var = values2[i];
						broad_cluster=i;
					}
				}
				
				unsigned int min_cluster = (similar_clusters>0 ? similar_clusters : cluster_wo_assignments);
				if (min_cluster<broad_cluster) 
				{
					for (unsigned int i=min_cluster; i<broad_cluster; i++)
						prototypes[i]=prototypes[i+1];
					prototypes[broad_cluster-1]-=2;
					prototypes[broad_cluster]+=2;
				} 
				else if (min_cluster>broad_cluster) 
				{
					for (unsigned int i=min_cluster; i>broad_cluster; i--)
						prototypes[i]=prototypes[i-1];
					prototypes[broad_cluster]-=2;
					prototypes[broad_cluster+1]+=2;
				}
			}
			
		}
		
		if (angleiter>=0) 
		{
			deque<MarkerInfo>::const_iterator it = markers.begin();
			deque<MarkerInfo>::const_iterator itend = markers.end();
			while (it<itend) 
			{
				if (it->angle.in_between (rightangle, leftangle)) 
				{
					if (it->true_distance>=6) 
					{
						values[it->true_distance]+=it->image_distance;
						value_count[it->true_distance]++;
					}
				}
				it++;
			}
			prototypes[6]=values[6]/=(static_cast<double>(value_count[6])+1e-20);
			prototypes[7]=values[7]/=(static_cast<double>(value_count[7])+1e-20);
			
			
			dest << angleiter << " 0";   
			for (unsigned int j=0; j<num_markers-1; j++)
				dest << ' ' << values[j];
			dest << ' ' << 0.5*(values[6]+values[7]) << ' ' << values[7] << '\n';
		}
  }
  
}