#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<set>
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
int num=0;//交易的总数
double rate=0.05;
int support=0;
set<int >deal[90000];
map<set<int >, int >FrequentItem[80000];//key值对应项集的集合，value值对应支持度
map<set<int >, int >CandidateItem[80000];
map<set<int >, int >DeletdItem[80000];//删除集，存放从候选集中被删除的项
//初始化数据库
void init_database()
{
    set<int>temp;
    ifstream fin("data1.txt");
    string s;
    while(getline(fin,s))
    {
        //把读入的一行字符串中的整数解析出来
        istringstream iss(s);
        int item;
        while(iss >> item)
           {
               deal[num].insert(item);
               temp.insert(item);
               CandidateItem[1][temp]++;
               temp.clear();
           }
         num++;
    }
}
//得到单项频繁集
void getL1()
{
    int i,j;
    int sum=0;
    map<set<int>,int >::iterator it;
    map<set<int>,int >::iterator p;
    for(it=CandidateItem[1].begin();it!=CandidateItem[1].end();it++)
    {
        if(it->second>=support)
            {
                FrequentItem[1].insert(pair<set<int >,int >(it->first,it->second));
            }
    }
    cout<<"一项集的个数"<<FrequentItem[1].size()<<endl;
    cout<<"频繁1项集为:"<<endl;
    for(p=FrequentItem[1].begin();p!=FrequentItem[1].end();p++)
    {
        vector<set<int> >a;
        a.push_back(p->first);
        set<int>::iterator s;
        for(int i=0;i<a.size();i++)
        {
            for(s=a[i].begin();s!=a[i].end();s++)
            {
                cout<<*s<<" ";
            }
        }
        a.clear();
    }
    cout<<endl;
}
//利用k-1项频繁项集得到k项候选集
void getKcandidate(int k)
{
    map<set<int>,int >::iterator i;
    map<set<int>,int >::iterator j;
    set<int>temp;
    vector<set<int> >temp1;
    vector<set<int> >temp2;
    set<int>::iterator p1;
    set<int>::iterator p2;
    //使用嵌套循环得到k项候选集，自连接
        for(i=FrequentItem[k-1].begin();i!=FrequentItem[k-1].end();i++)
        {
            for(j=FrequentItem[k-1].begin();j!=FrequentItem[k-1].end();j++)
            {
                //使用set容器可以减少排序和比较这一步
                   temp1.push_back(i->first);
                   temp2.push_back(j->first);
                   for(int x=0;x!=temp1.size();x++)
                   {
                       for(p1=temp1[x].begin();p1!=temp1[x].end();p1++)
                            temp.insert(*p1);
                   }
                   for(int y=0;y!=temp2.size();y++)
                   {
                       for(p2=temp2[y].begin();p2!=temp2[y].end();p2++)
                            temp.insert(*p2);
                   }
                   if(temp.size()==k)
                       CandidateItem[k].insert(pair<set<int >,int >(temp,0));
                   temp1.clear();
                   temp2.clear();
                   temp.clear();
            }
        }
}
//剪枝函数，思路：如果对于一个候选集在存删除的项的集合中有其子集出现，那么该项删除
void jianzhi(int a,int b)
{
    set<int >temp;
    map<set<int>,int >::iterator p;
    map<set<int>,int >::iterator q;
    if(a==2)//对于二项集，不用剪枝
        return;
     int flag=1;
     for(p=CandidateItem[a].begin();p!=CandidateItem[a].end();)
     {
         for(q=DeletdItem[b].begin();q!=DeletdItem[b].end();q++)
         {
             set_difference((q->first).begin(),(q->first).end(),(p->first).begin(),(p->first).end(),inserter(temp,temp.begin()));
             if(temp.size()==0)
                flag=0;
             temp.clear();
         }
         if(flag==0)
        {
            CandidateItem[a].erase(p++);
        }
        else
            p++;
        flag=1;
     }
}
//得到k项频繁集
void  getKFrenquent(int k)
{
    set<int > data;//存放数据库中的每一行
    set<int > result;//求差集的结果
    set<int >::iterator j;
    map<set<int>,int  >::iterator it;
    map<set<int>,int  >::iterator iter;
    map<set<int>,int  >::iterator p1;
    for(int i=0;i<num;i++)
            {
               for(j=deal[i].begin();j!=deal[i].end();j++)
                     data.insert(*j);
               for(it=CandidateItem[k].begin();it!=CandidateItem[k].end();it++)
                {
                    set_difference((it->first).begin(),(it->first).end(),data.begin(),data.end(),inserter(result,result.begin()));
                   // cout<<result.size()<<endl;
                    if(result.size()==0)//求差集结果为0，表明该项在数据库的这一行中
                    {
                        it->second++;
                    }
                    result.clear();
                }
                data.clear();
            }
    for(iter=CandidateItem[k].begin();iter!=CandidateItem[k].end();iter++)
            {
                if(iter->second>=support)
                    {
                        FrequentItem[k].insert(pair<set<int >,int >(iter->first,iter->second));
                    }
                else//表明该项要删除，那么把这一项放在删除集里面
                    {
                        DeletdItem[k].insert(pair<set<int >,int >(iter->first,iter->second));
                    }
            }
      cout<<"频繁"<<k<<"项集个数"<<FrequentItem[k].size()<<endl;
      cout<<"频繁"<<k<<"项集为:"<<endl;
      for(p1=FrequentItem[k].begin();p1!=FrequentItem[k].end();p1++)
            {
                vector<set<int> >a;
                a.push_back(p1->first);
                set<int>::iterator s;
                for(int i=0;i<a.size();i++)
                {
                    for(s=a[i].begin();s!=a[i].end();s++)
                    {
                        cout<<*s<<" ";
                    }
                }
                 cout<<"   ";
                 a.clear();
             }
                cout<<endl;
}
void Apriori()
{
    int k;
    for(k=2;FrequentItem[k-1].size()>=1;k++)
    {
        getKcandidate(k);
        jianzhi(k,k-1);
        getKFrenquent(k);
    }
}
int main()
{
    init_database();
    support=num*rate;
    getL1();
    Apriori();
}
