//Roll No : 18EC10028
//Name : Kushal Kedia
//Assignment No. : 4
/*Have not normalised features because input is of same dimension and normalisation is not explicitly asked - 
also used boost library to convert string values to float - other option was to use atof but requires c++11 compiler*/

#include<bits/stdc++.h>
#include<fstream>
#include<sstream>
#include<string.h>
#include <boost/lexical_cast.hpp>
using namespace std;
using boost::lexical_cast;

int main()	
{
	//No of k-means and no. of iterations
	int k=3;
	int iter=100;

	//Reading of datset
	fstream fin;
	fin.open("data4_19.csv",ios::in);
	vector <vector <string> > data;
	vector <vector <float> > dataset;
	vector <string> row;
	vector <float> rowset;
	string line,word,temp;
	float x;

	while(getline(fin,line))
	{
		stringstream s(line);
		
		while(getline(s,word,','))
			row.push_back(word);

		data.push_back(row);
		row.clear();
	}
	fin.close();

	//Hardcoded this as .csv seems to have an extra blank line which is causing problems
	int points=150;

	//Convert values to float
	for(int i=0;i<points;i++)
	{
		for(int j=0;j<data[0].size()-1;j++)
		{
			temp=data[i][j];
			x = boost::lexical_cast<float>(temp);
			rowset.push_back(x);
		}
		dataset.push_back(rowset);
		rowset.clear();
	}
	
	//Initalise k random points as initial centroids
	int d=data[0].size()-1;
	vector < vector < float> > centroids;
	vector <int> indexs;
	int index=0;
	int flag=0;
	for(int i=0;i<k;i++)
	{
		//checking to see that we don't choose same random point again - symmetry will cause problems
		do
		{
			flag=0;
			index = rand()%points;
			for(int j=0;j<indexs.size();j++)
			{
				if(index==indexs[j])
					flag=1;
			}
		} while(flag);
		indexs.push_back(index);
		centroids.push_back(dataset[index]);
	}

	vector < vector <int> > clusters;
	vector <int> v;


	//run iter=10 iterations of centroid updation
	for(int i=0;i<iter;i++)
	{
		//initalisation of clusters in each iteration
		clusters.clear();
		for(int i=0;i<k;i++)
		{
			clusters.push_back(v);
		}

		//classify each point to closest centroid
		for(int j=0;j<points;j++)
		{
			int cluster=-1;
			float min=-1;
			for(int m=0;m<k;m++)
			{
				float sum=0;
				for(int n=0;n<d;n++)
				{
					sum+=pow(dataset[j][n]-centroids[m][n],2);
				}
				if(min==-1||sum<min)
				{
					cluster=m;
					min=sum;
				}
			}
			clusters[cluster].push_back(j);
		}

		//update centroid to mean of each cluster
		for(int m=0;m<clusters.size();m++)
		{
			vector <float> vec(d);
			for(int j=0;j<clusters[m].size();j++)
			{
				for(int n=0;n<d;n++)
				{
					vec[n]+=dataset[clusters[m][j]][n];
				}
			}
			for(int n=0;n<d;n++)
			{
				vec[n]/=clusters[m].size();
			}
			centroids[m]=vec;
		}
	}

	//print the means of each cluster
	for(int i=0;i<clusters.size();i++)
	{
		cout << "Mean of Cluster "<<i+1<<":\n";
		for(int n=0;n<d;n++)
		{
			cout << centroids[i][n] << "\t";
		}
		cout << endl;
	}

	//extract the different ground truth labels
	vector <string> labels;
	for(int i=0;i<points;i++)
	{
		flag=0;
		for(int j=0;j<labels.size();j++)
		{
			if(data[i][d]==labels[j])
				flag=1;
		}
		if(!flag)
			labels.push_back(data[i][d]);
	}

	int c=labels.size();

	//computer jacquard distances of each cluster with each ground truth label
	for(int i=0;i<clusters.size();i++)
	{
		cout << endl;
		cout << "Jacquard Distances of Cluster "<<i+1<<":\n";
		vector <float> intersection(c);
		vector <float> unions(c);
		float min=-1;
		int index=-1;
		for(int j=0;j<c;j++)
		{
			//find corresponding labels in data set
			for(int m=0;m<points;m++)
			{
				if(data[m][d]==labels[j])
				{
					unions[j]++;
				}
			}
			//find unions and intersections
			for(int m=0;m<clusters[i].size();m++)
			{
				unions[j]++;
				if(data[clusters[i][m]][d]==labels[j])
					intersection[j]++;
			}	
			//subtract double counts
			unions[j]-=intersection[j];

			//compute smallest Jacquard Distance
			if(min==-1)
			{
				min=1-intersection[j]/unions[j];
				index=j;
			}
			else if(1-intersection[j]/unions[j]<min)
			{
				min=1-intersection[j]/unions[j];
				index=j;
			}
			cout << "\t Distance from " << labels[j] <<" = " << 1-intersection[j]/unions[j] << endl;
		}
		cout << "------------------------------------------------------------------------\n";
		cout << "Cluster probably corresponds to -> " << labels[index] << endl;
		cout << "------------------------------------------------------------------------\n";
	}
	return 0;
}