#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<set>
#include<string>
#include<string.h>
#include<vector>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<sstream>
using namespace std;
int num=0;//���׵�����
double rate=0.05;
int support=0;
int ItemSum=0;//ͳ�Ƹ���
//������ͷ��Ľṹ�壬����ʱ������ֵ����,��Ϊint�ͣ�ֱ�Ӹ�����ֵ
struct ItemNode
{
    int id;
    int cnt;
    ItemNode(int a,int b)
    {
        id=a;
        cnt=b;
    }
};
//˼·�������ж���int���͵ģ������ڽṹ����ֻ���int����
//������ڵ�Ľṹ��
struct  TreeNode
{
    int  id;
    int  cnt;
    int parent;
    vector<int >children;
    TreeNode()
     {
         id=0;
         cnt=0;
         parent=0;
         children.clear();
     }
     TreeNode(int a,int b,int c)
     {
         id=a;
         cnt=b;
         parent=c;
         children.clear();
     }
};
/* һ�����⣺������vector�游�ӹ�ϵʱ��һ��ʼ���ü�ֵ��Ÿ��׽ڵ��id��
���ڴ������Ĺ����У��������ĸ��׽ڵ�ı�Ų�Ψһ���ڼ��뺢�ӽڵ�ʱ���
�����ְ����浽һ���������棬�������±���Ψһ�ģ������±�Ψһ��ʶһ���ڵ㡣
*/
vector< vector<int> >fptree(10000,vector<int>(20000,0));//�������ӽڵ�Ĺ�ϵ
set<int >sum;//����set�õ��ж��ٲ��ظ�����
vector<ItemNode>deal[90000];//������ݿ������
int CountNum[20000]={0};//��¼ÿһ����ֵĴ���
int CutOrder[10000];//��¼Ƶ��һ���˳�򣬶����������
bool cmp(ItemNode a,ItemNode b)
{
    return a.cnt>b.cnt;//����֧�ֶ�����
}
bool CutCmp(ItemNode a,ItemNode b)//ɨ�����ݿ�ʱ�õ�Ƶ�����˳��
{
     return CutOrder[a.id]<CutOrder[b.id];
}
//��ʼ�����ݿ�
void init_database()
{
    ifstream fin("data1.txt");
    string s;
    while(getline(fin,s))
    {
        istringstream iss(s);
        int  item;
        while(iss >> item)
           {
               deal[num].push_back(ItemNode(item,1));//ÿһ���һ���ڵ����
               sum.insert(item);
           }
         num++;
    }
}
//�õ�ɾ�������ݿ��ͷ��
vector<ItemNode> GetHeadTable(vector<ItemNode>*deal,int num)
{
    //ÿһ������գ������µ����ݿ�
    memset(CountNum,0,sizeof(CountNum));
    memset(CutOrder,0,sizeof(CutOrder));
   // sum.clear();
    vector<ItemNode>HeadTable;
    //ɨ�����ݿ⣬�õ�Ƶ��һ�
    for(int i=0;i<num;i++)
    {
        for(int j=0;j!=deal[i].size();j++)
         {
               CountNum[deal[i][j].id]+=deal[i][j].cnt;
         }
    }
    vector<ItemNode>::iterator it;
    for(int i=0;i<=sum.size();i++)
    {
        if (CountNum[i]>=support)
            HeadTable.push_back(ItemNode(i,CountNum[i]));
    }
    sort(HeadTable.begin(),HeadTable.end(),cmp);
    /*vector<ItemNode>::iterator itw;
    cout<<"��ͷ���Ԫ���ǣ�"<<endl;
    for(itw=HeadTable.begin();itw!=HeadTable.end();itw++)
        cout<<(*itw).id<<"  "<<(*itw).cnt<<endl;*/
    for(int i=0;i<HeadTable.size();i++)//����vector���±�����
    {
        CutOrder[HeadTable[i].id]=i;
    }
    for(int i=0;i<num;i++)
        {
             for(it=deal[i].begin();it!=deal[i].end();)
             {
                  if(CountNum[it->id]>=support)
                      ++it;
                  else
                      deal[i].erase(it);//ɾ����Ƶ����
             }
             sort(deal[i].begin(),deal[i].end(),CutCmp);
        }
        /*cout<<"ɾ��������ݿ�:"<<endl;
       for(int i=0;i<num;i++)
        {
            for(int j=0;j!=deal[i].size();j++)
                cout<<deal[i][j].id<<" ";
            cout<<endl;
        }*/
        return  HeadTable;
}
//�������ݿ⣬����FPtree
vector<TreeNode> CreatFptree(vector<ItemNode>deal[],int num)
{
     //ÿ�εݹ���ã���֮ǰ����������
     vector<TreeNode>gather;
     fptree=vector<vector<int> >(10000,vector<int>(20000,0));//����
     //gather.clear();
     gather.push_back(TreeNode());//�Ѹ��ڵ����,�����±���Ψһ��
     //ɨ�����ݿ⣬����
     int sum=1;//��ʼֻ�и��ڵ�,���׽ڵ��Ӧ���������±�
     for(int i=0;i<num;i++)
     {
         int position=0;//ÿ�δӸ��ڵ㿪ʼ
         //cout<<"1"<<endl;
         for(int j=0;j!=deal[i].size();j++)
         {
              int  state=deal[i][j].id;
              if(fptree[position][state]==0)//����ýڵ�û�д˺��ӽڵ�
              {
                  //ע�⣬����ط������½ڵ��ʱ��һ�����ܰ�cntд����1
                   TreeNode  node=TreeNode(state,deal[i][j].cnt,position);
                   gather.push_back(node);
                   //Ҫ�ҵ����׽ڵ㣬�Ѻ��ӽڵ����ӽ�ȥ
                   gather[position].children.push_back(state);
                   fptree[position][state]=sum;
                   sum++;
              }
              else//���ýڵ�������Ӽ���
              {
                    gather[fptree[position][state]].cnt+=deal[i][j].cnt;
              }
             position=fptree[position][state];//ȥ����һ�����ڵ�
         }
     }
    /*for(int i=0;i<gather.size();i++)
    {
        cout<<gather[i].id<<"  "<< gather[i].cnt<<endl;
    }*/
         return gather;
}
//�����������fpgrowth
//˼·��ÿһ��Ҫ���ݵĲ�������Щ��1.�����ɵ����ݿ� 2.���ݿ�Ĵ�С 3.ͷ�������ڷ��ʵ�Ԫ��
//һ�����⣺��ÿ�ν���ݹ�ʱ��֮ǰ��ͷ���fptree���ƻ���Ҫ����
void  dfs(vector<ItemNode>database[],int a,vector<ItemNode>VisitItem)
{
    vector<ItemNode>HeadTable=GetHeadTable(database,a);
    vector<TreeNode>gather=CreatFptree(database,a);
    vector<TreeNode> OldGather=gather;
    /*for(int i=0;i<OldGather.size();i++)
        cout<<OldGather[i].id<<" ";
    cout<<endl;*/
    //��ֻ�и��ڵ�ʱ���˳��ݹ�Ĺ���
    if(gather[0].parent==0&&gather[0].children.size()==0&&gather[0].id==0&&gather[0].cnt==0)
         return;
         //cout<<"1"<<endl;
    for(vector<ItemNode>::iterator i=HeadTable.begin();i!=HeadTable.end();i++)
        {
            cout<<(*i).id<<" ";
              for(vector<ItemNode>::iterator  j=VisitItem.begin();j!=VisitItem.end();j++)
            {
                  cout<<(*j).id<<" ";
                  ItemSum++;
            }
                 cout<<endl;
        }
     vector<ItemNode>::reverse_iterator it;
     //�������ͷ���õ���һ�������ģʽ��
     for(it=HeadTable.rbegin();it!=HeadTable.rend();it++)
     {
          vector<ItemNode>NewVisitItem;
          NewVisitItem.push_back(ItemNode((*it).id,ItemNode(*it).cnt));
          if(VisitItem.size()!=0)//��֮ǰ��ͷ���Ԫ�ؼ���
          {
                for(vector<ItemNode>::iterator  iter=VisitItem.begin();iter!=VisitItem.end();iter++)
                {
                    NewVisitItem.push_back(ItemNode((*iter).id,(*iter).cnt));
                }
          }
          /*cout<<"�µ�����"<<endl;
          for(int i=0;i!=NewVisitItem.size();i++)
                cout<<NewVisitItem[i].id;
            cout<<endl;
            cout<<"ԭʼ������"<<endl;
          for(int i=0;i!=VisitItem.size();i++)
                cout<<VisitItem[i].id;
            cout<<endl;*/
          vector<ItemNode>NewDeal[10000];
          int  CntDeal=0;
          for(int i=0;i!=gather.size();i++)
          {
                if(gather[i].id==(*it).id)
                {
                    TreeNode point=gather[i];
                    while(point.parent!=0)//��ǰ��ǰ׺·���������µ����ݿ�
                    {
                        NewDeal[CntDeal].push_back(ItemNode(gather[point.parent].id,gather[i].cnt));
                        point=gather[point.parent];
                    }
                    CntDeal++;
                }
          }
        //cout<<CntDeal<<endl;
        //cout<<endl;
        /*cout<<"��������ݿ���:"<<endl;
        for(int i=0;i<CntDeal;i++)
        {
          for(int j=0;j!=NewDeal[i].size();j++)
              cout<<NewDeal[i][j].id<<" "<<NewDeal[i][j].cnt<<"      ";
          cout<<endl;
         }*/
        dfs(NewDeal,CntDeal,NewVisitItem);
        //�ͷ��ڴ�
        for(int i=0;i<CntDeal;i++)
        {
            vector<ItemNode>().swap(NewDeal[i]);
        }
        gather=OldGather;
     }
}
/*void show()
{
    vector<ItemNode>::iterator it;
    for(it=HeadTable.begin();it!=HeadTable.end();it++)
        cout<<(*it).id<<"  "<<(*it).cnt<<endl;
     for(int i=0;i<num;i++)
     {
        for(int j=0;j!=deal[i].size();j++)
            cout<<deal[i][j].id<<" ";
        cout<<endl;
      }
}
void showtree()
{
    for(int i=0;i<gather.size();i++)
    {
        cout<<gather[i].id<<"  "<< gather[i].cnt<<"  "<<gather[gather[i].parent].id<<"    ";
        for(int j=0;j<gather[i].children.size();j++)
            cout<<gather[i].children[j]<<" ";
        cout<<endl;
    }
}*/
int main()
{
    init_database();
    support=num*rate;
    vector<ItemNode>VisitItem;
    dfs(deal,num,VisitItem);
    cout<<"һ����:"<<ItemSum<<endl;
    //GetHeadTable(HeadTable,deal,num);
    //CreatFptree(deal,num);
    //show();
    //showtree();
}
