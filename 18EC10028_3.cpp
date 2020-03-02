//Roll No : 18EC10028
//Name : Kushal Kedia
//Assignment No. : 3
//running Adaboost for 4 rounds instead of 3 leads to a much improved accuracy on test set

#include<bits/stdc++.h>
#include<fstream>
#include<sstream>
#include<string.h>
using namespace std;

struct Node 
{ 
	string attr_value;
    string attr_name;
    bool leaf_node;
    string ans; 
    vector<Node *>child; 
}; 

Node *newNode() 
{ 
    Node *temp = new Node; 
    return temp; 
}    

void run(vector <vector <string > > data ,vector <string> attr_list,Node *root)
{	
	//a condition for leaf node
	root->leaf_node=false;
	double pos=0;double neg=0;
	if(attr_list.size()==1)
	{
		root->leaf_node=true;
		for(int i=1;i<data.size();i++)
		{
			if(data[i][data[0].size()-1][0]=='y')
				pos++;
			else
				neg++;
		}
		if(pos>=neg)
			root->ans="yes";
		else
			root->ans="no";
		return;
	}

	//to create the domain of different features
	vector <string> row;
	vector <vector <string> > domain;
	bool flag=false;
	for(int j=0;j<data[0].size()-1;j++)
	{
		for(int i=1;i<data.size();i++)
		{
			flag=false;
			for(int k=0;k<row.size();k++)
			{
				if(data[i][j]==row[k])
					flag=true;
			}
			if(!flag)
				row.push_back(data[i][j]);	
		}
		domain.push_back(row);
		row.clear();
	}

	//To calculate entropy of current node
	pos=0;neg=0;
	int iter;double e1=1,e2=1;int best_feature=-1;double e=0;
	for(int i=1;i<data.size();i++)
	{
		if(data[i][data[0].size()-1][0]=='y')
			pos++;
		else
			neg++;
	}
	if(pos==0&&neg==0)
		e1-=0;
	else if(pos==0)
		e1-=neg*log(neg/(pos+neg));
	else if(neg==0)
		e1-=pos*log(pos/(pos+neg));
	else
		e1-=(pos*log(pos/(pos+neg))+neg*log(neg/(pos+neg)));

	//to calculate entropy after splitting and choose best node
	pos=0;neg=0;
	for(int j=0;j<data[0].size()-1;j++)
	{
		vector <pair <int,int> > count;
		for(int k=0;k<domain[j].size();k++)
			count.push_back(make_pair(0,0));
		int index;
		for(int i=1;i<data.size();i++)
		{
			for(int l=0;l<domain[j].size();l++)
			{
				if(data[i][j]==domain[j][l])
					index=l;
			}
			if(data[i][data[0].size()-1][0]=='y')
				count[index].first++;
			else
				count[index].second++;
		}
		for(int k=0;k<domain[j].size();k++)
		{
			pos=count[k].first;
			neg=count[k].second;
			if(pos==0&&neg==0)
				e-=0;
			else if(pos==0)
				e-=neg*log(neg/(pos+neg));
			else if(neg==0)
				e-=pos*log(pos/(pos+neg));
			else
				e-=(pos*log(pos/(pos+neg))+neg*log(neg/(pos+neg)));
		}
		e/=data.size()-1;
		if(best_feature==-1)
		{
			e2=e;
			if(e1-e2>0)
				best_feature=j;
		}
		else if(e1-e>e1-e2)
		{
			best_feature=j;
			e2=e;
		}
		e=0;
	}
	pos=0;neg=0;

	//no feature shows information gain 
	if(best_feature==-1)
	{
		root->leaf_node=true;
		for(int i=1;i<data.size();i++)
		{
			if(data[i][data[0].size()-1][0]=='y')
				pos++;
			else
				neg++;
		}
		if(pos>=neg)
			root->ans="yes";
		else
			root->ans="no";
		return;
	}

	//calling function again on all feature attributes
	root->attr_name=attr_list[best_feature];
	for(int k=0;k<domain[best_feature].size();k++)
	{
		vector <vector <string> > data2;
		for(int j=0;j<data[0].size();j++)
		{
			if(j!=best_feature)
				row.push_back(data[0][j]);
		}
		data2.push_back(row);
		row.clear();
		for(int i=1;i<data.size();i++)
		{
			if(data[i][best_feature]==domain[best_feature][k])
			{
				for(int j=0;j<data[0].size();j++)
				{
					if(j!=best_feature)
						row.push_back(data[i][j]);
				}
				data2.push_back(row);
				row.clear();
			}	
		}
		root->child.push_back(newNode());
		root->child[k]->attr_value=domain[best_feature][k];
		run(data2,data2.front(),root->child[k]);
	}
} 

void display(Node *root,int level)
{
	//normal printing of tree
	for(int i=0;i<root->child.size();i++)
	{
		for(int j=0;j<level;j++)
		{
			cout << "\t";
		}
		cout << root->attr_name << " = " << root->child[i]->attr_value;
		if(root->child[i]->leaf_node)
		{
			cout << " : "<<root->child[i]->ans<<endl;
		}
		else
		{
			cout << endl;
			display(root->child[i],level+1);
		}	
	}
}

string predict(vector <string> test,Node *root)
{
	//traversing till leaf node to predict
	if(root->leaf_node)
	{
		return root->ans;
	}
	for(int i=0;i<root->child.size();i++)
	{
		for(int j=0;j<test.size()-1;j++)
		{
			if(test[j]==root->child[i]->attr_value)
				return predict(test,root->child[i]);
		}
	}
}


int main()	
{
	fstream fin;
	fin.open("data3_19.csv",ios::in);
	vector <vector <string> > data;
	vector <string> row;
	string temp,line,word;

	while(getline(fin,line))
	{
		stringstream s(line);
		
		while(getline(s,word,','))
			row.push_back(word);

		data.push_back(row);
		row.clear();
	}
	fin.close();

	//to store different decision trees
	int n=data.size()-1;
	vector <double> weights(n,(double)1/n);
	vector <Node* > trees;
	vector <double> alphas;

	//rounds of adaboost to be applied is 3
	int rounds=3;

	//adaboost
	for(int i=0;i<rounds;i++)
	{
		//sampling based on weights
		vector <vector <string> > sample;
		sample.push_back(data.front());
		for(int k=0;k<n;k++)
		{
			double cdf=0;
			double r = ((double) rand() / (RAND_MAX));
			for(int j=0;j<n;j++)
			{
				cdf+=weights[j];
				if(cdf>=r)
				{
					sample.push_back(data[j+1]);
					break;
				}				
			}
		}

		//formation of classifier decision tree
		Node *root=new Node;
		run(sample,sample.front(),root);
		trees.push_back(root);

		//calculating if correct output is predicted
		vector <int> predicted(n);
		vector <int> output(n);
		int count=0;
		for(int j=1;j<=n;j++)
		{
			string str1=predict(data[j],root);
			string str2=data[j][data[j].size()-1];
			if(str1[0]=='y')
				predicted[j-1]=1;
			if(str2[0]=='y')
				output[j-1]=1;
			if(predicted[j-1]==output[j-1])
				count++;
		}

		//error and alpha computation
		double error=(double)(n-count)/n;
		// cout << error << endl;
		double alpha=0.5*log((1-error)/error);
		// cout << alpha << endl;
		alphas.push_back(alpha);

		//updation of weights and normalisation
		for(int j=1;j<=n;j++)
		{
			if(predicted[j-1]==output[j-1])
				weights[j-1]*=exp(-alpha);
			else
				weights[j-1]*=exp(alpha);
		}
		double sum=0;
		for(int j=1;j<=n;j++)
		{
			sum+=weights[j-1];
		}
		for(int j=1;j<=n;j++)
		{
			weights[j-1]/=sum;
		}
		// display(trees[i],0);
	}

	//reading of test set
	fin.open("test3_19.csv",ios::in);
	vector <vector <string> > test;
	while(getline(fin,line))
	{
		stringstream s(line);
		
		while(getline(s,word,','))
			row.push_back(word);

		test.push_back(row);
		row.clear();
	}
	fin.close();

	//predicting using the different classifiers and their weights
	n=test.size();
	vector <int> test_output(n);
	int count=0;
	for(int i=0;i<n;i++)
	{
		string str1;
		double pos=0;
		double neg=0;
		for(int j=0;j<rounds;j++)
		{
			str1=predict(test[i],trees[j]);
			if(str1[0]=='y')
				pos+=alphas[j];
			else if(str1[0]=='n')
				neg+=alphas[j];
		}
		string str2=test[i][test[i].size()-1];
		if(pos>=neg&&str2[0]=='y')
			count++;
		else if(neg>=pos&&str2[0]=='n')
			count++;
	}
	
	cout << "No. of correctly classified examples = " << count << endl;
	cout << "Total no. of examples = " << n << endl;
	cout << "Accuracy = " << (double)count*100/n << " %\n";
	return 0;
}