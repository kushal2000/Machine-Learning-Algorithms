//Roll No : 18EC10028
//Name : Kushal Kedia
//Assignment No. : 1

#include<bits/stdc++.h>
#include<fstream>
#include<sstream>
#include<string.h>
using namespace std;

int main()	
{
	fstream fin;
	fin.open("data2_19.csv",ios::in);
	vector <vector <string> > data;
	vector <vector <int> > dataset;
	vector <vector <string> > test;
	vector <vector <int> > testset;
	vector <string> row;
	vector <int> rowset;
	string line,word,temp;
	int x;int no_of_classes=2;

	while(getline(fin,line))
	{
		stringstream s(line);
		
		while(getline(s,word,','))
			row.push_back(word);

		data.push_back(row);
		row.clear();
	}
	fin.close();
	fin.open("test2_19.csv",ios::in);

	while(getline(fin,line))
	{
		stringstream s(line);
		
		while(getline(s,word,','))
			row.push_back(word);

		test.push_back(row);
		row.clear();
	}
	cout << data[0][0]<< endl;
	for(int i=1;i<data.size();i++)
	{
		for(int j=0;j<data[0].size();j++)
		{
			if(j==0)
			{
				temp=data[i][j][1];
				stringstream geek(temp); 
				geek >> x;
			}
			else
			{
				stringstream geek(data[i][j]); 
				geek >> x;
			}
			
			rowset.push_back(x);
		}
		dataset.push_back(rowset);
		rowset.clear();
	}

	for(int i=1;i<test.size();i++)
	{
		for(int j=0;j<test[0].size();j++)
		{
			if(j==0)
			{
				temp=data[i][j][1];
				stringstream geek(temp);
				geek >> x;
			}
			else
			{
				stringstream geek(test[i][j]);
				geek >> x;
			}
			rowset.push_back(x);
		}
		testset.push_back(rowset);
		rowset.clear();
	}
	float pos_probabilities[6][5];
	float neg_probabilities[6][5];
	float probability[2];
	probability[0]=0;
	probability[1]=0;
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<5;j++)
		{
			pos_probabilities[i][j]=0;
			neg_probabilities[i][j]=0;
		}
	}
	float pos=0;
	cout << dataset.size() << endl;
	for(int i=0;i<dataset.size();i++)
	{
		cout << dataset[i][0];
		if(dataset[i][0]==1)
		{
			probability[1]++;
			for(int j=1;j<dataset[0].size();j++)
			{
				pos_probabilities[j-1][dataset[i][j]-1]++;
			}
		}
		else
		{
			probability[0]++;
			for(int j=1;j<dataset[0].size();j++)
			{
				neg_probabilities[j-1][dataset[i][j]-1]++;
			}
		}		
	}
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<5;j++)
		{
			pos_probabilities[i][j]/=probability[1];
			neg_probabilities[i][j]/=probability[0];
		}
	}

	int pred;float acc_pred=0;
	float pos_prob=1,neg_prob=1;

	for(int i=1;i<testset.size();i++)
	{
		pos_prob=probability[1];
		neg_prob=probability[0];
		for(int j=1;j<testset[0].size();j++)
		{
			pos_prob*=pos_probabilities[j-1][testset[i][j]-1];
			neg_prob*=neg_probabilities[j-1][testset[i][j]-1];
		}
		if(pos_prob>neg_prob)
			pred=1;
		else
			pred=0;
		if(pred==testset[i][0])
			acc_pred++;
	}

	cout << "ACCURACY = " << 100*(float)(acc_pred/testset.size()) << endl;

	return 0;
}