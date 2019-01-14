/*数据挖掘 Apriori算法*/
#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<set>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
int num=0;//交易的总数
int support=1500;
vector<int>deal[90000];//记录交易
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
               deal[num].push_back(item);
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
    //set<set<int> >temp;
    for(it=CandidateItem[1].begin();it!=CandidateItem[1].end();it++)
    {
        if(it->second>=support)
            {
                //temp.insert(it->first);
                FrequentItem[1].insert(pair<set<int >,int >(it->first,it->second));
            }
    }
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
    //cout<<sum;
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
    //DeletdItem[b]中的元素若在候选集CandidateItem[a]中出现，那么从CandidateItem[a]中删除该元素
    set<int>temp;
    set<int>temp1;
    vector<set<int> >vec;
    vector<set<int> >vec1;
    map<set<int>,int >::iterator p;
    map<set<int>,int >::iterator q;
    set<int >::iterator iter;
    set<int >::iterator iter1;
    set<int>::iterator  r1;
    set<int>::iterator  r2;
    int i,j;
    int csize;
    if(a==2)//对于二项集，不用剪枝
        return;
    int flag=1;
    for(p=CandidateItem[a].begin();p!=CandidateItem[a].end();)
    {
        vec.push_back(p->first);
        for(i=0;i!=vec.size();i++)
        {
            for(iter=vec[i].begin();iter!=vec[i].end();iter++)
                {
                    temp.insert(*iter);
                    temp1.insert(*iter);
                }
                csize=temp.size();
        }
       //cout<<"数据";
          // for(r1=temp.begin();r1!=temp.end();r1++)
           //cout<<*r1<<" ";
           //cout<<endl;
        for(q=DeletdItem[b].begin();q!=DeletdItem[b].end();q++)
        {
          //  cout<<"进入循环了"<<endl;
            vec1.push_back(q->first);
            for(j=0;j!=vec1.size();j++)
            {
                for(iter1=vec1[j].begin();iter1!=vec1[j].end();iter1++)
                    temp1.insert(*iter1);
            }
          // cout<<"插入后";
             //     for(r2=temp1.begin();r2!=temp1.end();r2++)
                //     cout<<*r2<<" ";
                   //   cout<<endl;
            if(temp1.size()==csize)
            {
                 flag=0;
                 //CandidateItem[a].erase(p++);
            }
            else//反之，容器的内容变了，恢复内容，下一轮比较
            {
                temp1.clear();
                temp1=temp;
            }
            vec1.clear();
        }
        //p++;//问题，一次删掉了两个元素,利用flag来解决
        if(flag==0)
        {
            CandidateItem[a].erase(p++);
        }
        else
            p++;
        vec.clear();
        temp1.clear();
        temp.clear();
        flag=1;//恢复
    }
}
//得到k项频繁项集,把经过剪枝函数处理后的候选集拿到数据库中去扫一遍
//思路：把数据库的每一行放到一个临时的set里，对于K项候选集的每一项也放到这个set里，如果set的size变了，那么
//说明该项没有，如果set的size没变，则value值++
void  getKFrenquent(int k)
{
    set<int>temp;//存放数据库的每一行
    set<int>temp1;
    vector<set<int> >vec;//存放k项集的项
    set<int>::iterator  p;
    set<int>::iterator  r1;
    set<int>::iterator  r2;
    map<set<int>,int  >::iterator i;
    map<set<int>,int  >::iterator it;
    map<set<int>,int  >::iterator p1;
    int sum_size;
        //扫描数据库，得到支持度
        for(int x=0;x<num;x++)
        {
            for(int j=0;j!=deal[x].size();j++)
            {
                temp.insert(deal[x][j]);
                temp1.insert(deal[x][j]);
            }
            //cout<<k<<"项集"<<endl;
            //cout<<"数据";
            //for(r1=temp.begin();r1!=temp.end();r1++)
            //cout<<*r1<<" ";
            //cout<<endl;
            sum_size=temp.size();
            for(i=CandidateItem[k].begin();i!=CandidateItem[k].end();i++)
            {
                    vec.push_back(i->first);
                    for(int x=0;x!=vec.size();x++)
                    {
                        for(p=vec[x].begin();p!=vec[x].end();p++)
                        {
                             temp1.insert(*p);
                        }
                    }
                    //cout<<"插入后";
                    //for(r2=temp1.begin();r2!=temp1.end();r2++)
                       // cout<<*r2<<" ";
                        //cout<<endl;
                    if(temp1.size()==sum_size)
                        {
                            i->second++;
                           // cout<<i->second<<"  ";
                            //CandidateItem[k].erase(i++);
                        }
                    else
                        {
                            //i++;
                            temp1.clear();
                            temp1=temp;
                        }
                       // temp1.clear();
                        //temp1=temp;
                       // cout<<"变化后";
                        //for(r2=temp1.begin();r2!=temp1.end();r2++)
                         //cout<<*r2<<" ";
                         //cout<<endl;
                        vec.clear();
             }
              temp1.clear();//在结束每一次之后，也必须得清空，有时候个数一样，并不表示内容一样
              temp.clear();
        }
               for(it=CandidateItem[k].begin();it!=CandidateItem[k].end();it++)
              {
                    if(it->second>=support)
                        {
                            FrequentItem[k].insert(pair<set<int >,int >(it->first,it->second));
                        }
                    else//表明该项要删除，那么把这一项放在删除集里面
                        {
                            DeletdItem[k].insert(pair<set<int >,int >(it->first,it->second));
                        }
              }
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
            /*cout<<"所删除的元素是:"<<endl;
            for(p1=DeletdItem[k].begin();p1!=DeletdItem[k].end();p1++)
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
                    cout<<endl;*/
}
//测试剪枝函数
void show(int k)
{
    map<set<int>,int >::iterator  p;
    vector<set<int > >temp;
    set<int>::iterator  q;
    cout<<k<<"项候选集为"<<endl;
    for(p=CandidateItem[k].begin();p!=CandidateItem[k].end();p++)
    {
         temp.push_back(p->first);
         for(int i=0;i<temp.size();i++)
         {
            for(q=temp[i].begin();q!=temp[i].end();q++)
              cout<<*q<<" ";
         }
         cout<<"  ";
         temp.clear();
    }
    cout<<endl;
}
void Apriori()
{
    int k;
    for(k=2;FrequentItem[k-1].size()>=1;k++)
    {
        //cout<<FrequentItem[k-1].size();
        getKcandidate(k);
       // show(k);
        jianzhi(k,k-1);
       // show(k);
        getKFrenquent(k);
    }
}
int main()
{
    init_database();
    //cout<<num<<endl;
    getL1();
    Apriori();
    //show();
}
