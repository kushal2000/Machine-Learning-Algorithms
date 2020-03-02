//Roll No : 18EC10028
//Name : Kushal Kedia
//Assignment No. : 1

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
			root->ans="YES";
		else
			root->ans="NO";
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
			root->ans="YES";
		else
			root->ans="NO";
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
	//The following two flags check if there is a redundancy in children (all children are leaf nodes of same value)

	int leaf_flag=0;int redundancy_flag=1;
	for(int i=0;i<root->child.size();i++)
	{
		if(!root->child[i]->leaf_node)
			leaf_flag=1;
	}
	if(!leaf_flag)
	{
		for(int i=1;i<root->child.size();i++)
		{
			if(root->child[i]->ans!=root->child[i-1]->ans)
			{
				redundancy_flag=0;
				break;
			}
		}
	}


	if(redundancy_flag&&(!leaf_flag))
	{
		for(int j=0;j<level;j++)
		{
			cout << "\t";
		}
		cout << ": "<<root->child[0]->ans<<endl;
	}

	//normal printing of tree otherwise
	else
	{
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

	//top node 
	Node *root=new Node;
	run(data,data.front(),root);

	//display tree
	display(root,0);
	return 0;
}