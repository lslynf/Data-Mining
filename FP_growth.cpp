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
int num=0;//交易的总数
double rate=0.05;
int support=0;
int ItemSum=0;//统计个数
//存放项和头表的结构体，查找时根据数值查找,都为int型，直接根据数值
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
//思路：数据中都是int类型的，可以在结构体里只存放int类型
//存放树节点的结构体
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
/* 一个问题：在利用vector存父子关系时，一开始利用键值存放父亲节点的id号
，在创建树的过程中，由于树的父亲节点的编号不唯一，在加入孩子节点时会出
错，发现把树存到一个容器里面，容器的下标是唯一的，利用下标唯一标识一个节点。
*/
vector< vector<int> >fptree(10000,vector<int>(20000,0));//存树父子节点的关系
set<int >sum;//利用set得到有多少不重复的项
vector<ItemNode>deal[90000];//存放数据库的数据
int CountNum[20000]={0};//记录每一项出现的次数
int CutOrder[10000];//记录频繁一项集的顺序，定义排序规则
bool cmp(ItemNode a,ItemNode b)
{
    return a.cnt>b.cnt;//按照支持度排序
}
bool CutCmp(ItemNode a,ItemNode b)//扫描数据库时得到频繁项的顺序
{
     return CutOrder[a.id]<CutOrder[b.id];
}
//初始化数据库
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
               deal[num].push_back(ItemNode(item,1));//每一项建立一个节点存入
               sum.insert(item);
           }
         num++;
    }
}
//得到删减的数据库和头表
vector<ItemNode> GetHeadTable(vector<ItemNode>*deal,int num)
{
    //每一次需清空，建立新的数据库
    memset(CountNum,0,sizeof(CountNum));
    memset(CutOrder,0,sizeof(CutOrder));
   // sum.clear();
    vector<ItemNode>HeadTable;
    //扫描数据库，得到频繁一项集
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
    cout<<"项头表的元素是："<<endl;
    for(itw=HeadTable.begin();itw!=HeadTable.end();itw++)
        cout<<(*itw).id<<"  "<<(*itw).cnt<<endl;*/
    for(int i=0;i<HeadTable.size();i++)//利用vector的下标排序
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
                      deal[i].erase(it);//删除非频繁项
             }
             sort(deal[i].begin(),deal[i].end(),CutCmp);
        }
        /*cout<<"删减后的数据库:"<<endl;
       for(int i=0;i<num;i++)
        {
            for(int j=0;j!=deal[i].size();j++)
                cout<<deal[i][j].id<<" ";
            cout<<endl;
        }*/
        return  HeadTable;
}
//传入数据库，构建FPtree
vector<TreeNode> CreatFptree(vector<ItemNode>deal[],int num)
{
     //每次递归调用，把之前存的数据清空
     vector<TreeNode>gather;
     fptree=vector<vector<int> >(10000,vector<int>(20000,0));//清零
     //gather.clear();
     gather.push_back(TreeNode());//把根节点放入,数组下标是唯一的
     //扫描数据库，建树
     int sum=1;//初始只有根节点,父亲节点对应的是数组下标
     for(int i=0;i<num;i++)
     {
         int position=0;//每次从根节点开始
         //cout<<"1"<<endl;
         for(int j=0;j!=deal[i].size();j++)
         {
              int  state=deal[i][j].id;
              if(fptree[position][state]==0)//如果该节点没有此孩子节点
              {
                  //注意，这个地方建立新节点的时候一定不能把cnt写成是1
                   TreeNode  node=TreeNode(state,deal[i][j].cnt,position);
                   gather.push_back(node);
                   //要找到父亲节点，把孩子节点增加进去
                   gather[position].children.push_back(state);
                   fptree[position][state]=sum;
                   sum++;
              }
              else//若该节点存在增加计数
              {
                    gather[fptree[position][state]].cnt+=deal[i][j].cnt;
              }
             position=fptree[position][state];//去往下一个父节点
         }
     }
    /*for(int i=0;i<gather.size();i++)
    {
        cout<<gather[i].id<<"  "<< gather[i].cnt<<endl;
    }*/
         return gather;
}
//深度优先搜索fpgrowth
//思路：每一次要传递的参数有哪些。1.新生成的数据库 2.数据库的大小 3.头表中正在访问的元素
//一个问题：在每次进入递归时，之前的头表和fptree被破坏，要保存
void  dfs(vector<ItemNode>database[],int a,vector<ItemNode>VisitItem)
{
    vector<ItemNode>HeadTable=GetHeadTable(database,a);
    vector<TreeNode>gather=CreatFptree(database,a);
    vector<TreeNode> OldGather=gather;
    /*for(int i=0;i<OldGather.size();i++)
        cout<<OldGather[i].id<<" ";
    cout<<endl;*/
    //当只有根节点时，退出递归的过程
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
     //反向遍历头表，得到各一项集的条件模式基
     for(it=HeadTable.rbegin();it!=HeadTable.rend();it++)
     {
          vector<ItemNode>NewVisitItem;
          NewVisitItem.push_back(ItemNode((*it).id,ItemNode(*it).cnt));
          if(VisitItem.size()!=0)//把之前项头表的元素加入
          {
                for(vector<ItemNode>::iterator  iter=VisitItem.begin();iter!=VisitItem.end();iter++)
                {
                    NewVisitItem.push_back(ItemNode((*iter).id,(*iter).cnt));
                }
          }
          /*cout<<"新的条件"<<endl;
          for(int i=0;i!=NewVisitItem.size();i++)
                cout<<NewVisitItem[i].id;
            cout<<endl;
            cout<<"原始的条件"<<endl;
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
                    while(point.parent!=0)//往前找前缀路径，加入新的数据库
                    {
                        NewDeal[CntDeal].push_back(ItemNode(gather[point.parent].id,gather[i].cnt));
                        point=gather[point.parent];
                    }
                    CntDeal++;
                }
          }
        //cout<<CntDeal<<endl;
        //cout<<endl;
        /*cout<<"传入的数据库是:"<<endl;
        for(int i=0;i<CntDeal;i++)
        {
          for(int j=0;j!=NewDeal[i].size();j++)
              cout<<NewDeal[i][j].id<<" "<<NewDeal[i][j].cnt<<"      ";
          cout<<endl;
         }*/
        dfs(NewDeal,CntDeal,NewVisitItem);
        //释放内存
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
    cout<<"一共有:"<<ItemSum<<endl;
    //GetHeadTable(HeadTable,deal,num);
    //CreatFptree(deal,num);
    //show();
    //showtree();
}
