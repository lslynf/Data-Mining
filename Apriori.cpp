/*�����ھ� Apriori�㷨*/
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
int num=0;//���׵�����
int support=1500;
vector<int>deal[90000];//��¼����
map<set<int >, int >FrequentItem[80000];//keyֵ��Ӧ��ļ��ϣ�valueֵ��Ӧ֧�ֶ�
map<set<int >, int >CandidateItem[80000];
map<set<int >, int >DeletdItem[80000];//ɾ��������ŴӺ�ѡ���б�ɾ������
//��ʼ�����ݿ�
void init_database()
{
    set<int>temp;
    ifstream fin("data1.txt");
    string s;
    while(getline(fin,s))
    {
        //�Ѷ����һ���ַ����е�������������
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
//�õ�����Ƶ����
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
    cout<<"Ƶ��1�Ϊ:"<<endl;
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
//����k-1��Ƶ����õ�k���ѡ��
void getKcandidate(int k)
{
    map<set<int>,int >::iterator i;
    map<set<int>,int >::iterator j;
    set<int>temp;
    vector<set<int> >temp1;
    vector<set<int> >temp2;
    set<int>::iterator p1;
    set<int>::iterator p2;
    //ʹ��Ƕ��ѭ���õ�k���ѡ����������
        for(i=FrequentItem[k-1].begin();i!=FrequentItem[k-1].end();i++)
        {
            for(j=FrequentItem[k-1].begin();j!=FrequentItem[k-1].end();j++)
            {
                //ʹ��set�������Լ�������ͱȽ���һ��
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
//��֦������˼·���������һ����ѡ���ڴ�ɾ������ļ����������Ӽ����֣���ô����ɾ��
void jianzhi(int a,int b)
{
    //DeletdItem[b]�е�Ԫ�����ں�ѡ��CandidateItem[a]�г��֣���ô��CandidateItem[a]��ɾ����Ԫ��
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
    if(a==2)//���ڶ�������ü�֦
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
       //cout<<"����";
          // for(r1=temp.begin();r1!=temp.end();r1++)
           //cout<<*r1<<" ";
           //cout<<endl;
        for(q=DeletdItem[b].begin();q!=DeletdItem[b].end();q++)
        {
          //  cout<<"����ѭ����"<<endl;
            vec1.push_back(q->first);
            for(j=0;j!=vec1.size();j++)
            {
                for(iter1=vec1[j].begin();iter1!=vec1[j].end();iter1++)
                    temp1.insert(*iter1);
            }
          // cout<<"�����";
             //     for(r2=temp1.begin();r2!=temp1.end();r2++)
                //     cout<<*r2<<" ";
                   //   cout<<endl;
            if(temp1.size()==csize)
            {
                 flag=0;
                 //CandidateItem[a].erase(p++);
            }
            else//��֮�����������ݱ��ˣ��ָ����ݣ���һ�ֱȽ�
            {
                temp1.clear();
                temp1=temp;
            }
            vec1.clear();
        }
        //p++;//���⣬һ��ɾ��������Ԫ��,����flag�����
        if(flag==0)
        {
            CandidateItem[a].erase(p++);
        }
        else
            p++;
        vec.clear();
        temp1.clear();
        temp.clear();
        flag=1;//�ָ�
    }
}
//�õ�k��Ƶ���,�Ѿ�����֦���������ĺ�ѡ���õ����ݿ���ȥɨһ��
//˼·�������ݿ��ÿһ�зŵ�һ����ʱ��set�����K���ѡ����ÿһ��Ҳ�ŵ����set����set��size���ˣ���ô
//˵������û�У����set��sizeû�䣬��valueֵ++
void  getKFrenquent(int k)
{
    set<int>temp;//������ݿ��ÿһ��
    set<int>temp1;
    vector<set<int> >vec;//���k�����
    set<int>::iterator  p;
    set<int>::iterator  r1;
    set<int>::iterator  r2;
    map<set<int>,int  >::iterator i;
    map<set<int>,int  >::iterator it;
    map<set<int>,int  >::iterator p1;
    int sum_size;
        //ɨ�����ݿ⣬�õ�֧�ֶ�
        for(int x=0;x<num;x++)
        {
            for(int j=0;j!=deal[x].size();j++)
            {
                temp.insert(deal[x][j]);
                temp1.insert(deal[x][j]);
            }
            //cout<<k<<"�"<<endl;
            //cout<<"����";
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
                    //cout<<"�����";
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
                       // cout<<"�仯��";
                        //for(r2=temp1.begin();r2!=temp1.end();r2++)
                         //cout<<*r2<<" ";
                         //cout<<endl;
                        vec.clear();
             }
              temp1.clear();//�ڽ���ÿһ��֮��Ҳ�������գ���ʱ�����һ����������ʾ����һ��
              temp.clear();
        }
               for(it=CandidateItem[k].begin();it!=CandidateItem[k].end();it++)
              {
                    if(it->second>=support)
                        {
                            FrequentItem[k].insert(pair<set<int >,int >(it->first,it->second));
                        }
                    else//��������Ҫɾ������ô����һ�����ɾ��������
                        {
                            DeletdItem[k].insert(pair<set<int >,int >(it->first,it->second));
                        }
              }
            cout<<"Ƶ��"<<k<<"�Ϊ:"<<endl;
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
            /*cout<<"��ɾ����Ԫ����:"<<endl;
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
//���Լ�֦����
void show(int k)
{
    map<set<int>,int >::iterator  p;
    vector<set<int > >temp;
    set<int>::iterator  q;
    cout<<k<<"���ѡ��Ϊ"<<endl;
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
