#pragma GCC optimize(3,"Ofast","inline")
#include<iostream>
#include<cstring>
#include<algorithm>
#include<ctime>
#include<vector>
#include<queue>
using namespace std;
const double INF=1000000.0;
queue <int> q;
bool visited[401];
int T, m, E, n, s, t, u, v;
double t0, a1, a2;
int Q[8001][2], k[401],path[401][2];
double dist[401];
void SPFA(int,int);
void printAndUpdate(int, int);//更新road的te并打印该路径结果

struct node
{
	int end;//目的地
	double te;//行驶时间（会变化，需更新）
	double t0;//空载时（即没有车辆通过）的行驶时间
	double a;//拥堵参数
			 //int fe;//流量fe为所有通过路段e的车辆总和
	int next;
}road[10001 * 2];

int main()
{
	cin >> T;
	for (int i = 0; i < T; i++)
	{
		memset(k, -1, sizeof(k));
		cin >> m >> E >> n;//分别表示路口结点总数、双向路段总数、导航请求总数n
		for (int j = 0; j < n; j++)
		{
			cin >> s >> t;//(0<=s<n)、(0<=t<n)、s≠t，请求的出发点和目的地点
			Q[j][0] = s;
			Q[j][1] = t;
		}
		for (int j = 0; j < 2 * E-1; j += 2)
		{
			cin >> u >> v >> t0 >> a1 >> a2;
			road[j].end = v;
			road[j].te = t0;//最初行驶时间定义
			road[j].t0 = t0;
			road[j].a = a1;
			road[j].next = k[u];
			//road[j].fe = 0;
			k[u] = j;
			road[j + 1].end = u;
			road[j + 1].te = t0;
			road[j + 1].t0 = t0;
			road[j + 1].a = a2;
			road[j + 1].next = k[v];
			//road[j + 1].fe = 0;
			k[v] = j + 1;
		}
		for (int j = 0; j < n; j++)
		{
			SPFA(Q[j][0], Q[j][1]);//最短路径：dist(Q[j][0], Q[j][1])
			printAndUpdate(Q[j][0], Q[j][1]);//更新road的te并打印该路径结果
		}
	}
	return 0;
}
void SPFA(int s, int t)

{
	for (int i = 0; i < m; i++)
		dist[i] = DBL_MAX;
	memset(visited, 0, sizeof(visited));
	memset(path, -1, sizeof(path));
	for (int j = 0; j < 2 * E - 1; j += 2)
	{
		v = road[j].end; 
		u = road[j + 1].end;
		path[v][0] = u;
		path[v][1] = j;
		path[u][0] = v;
		path[u][1] = j + 1;
	}
	int start = s;
	while (!q.empty())q.pop();
	dist[s] = 0.0;
	visited[s] = 1;
	q.push(s);
	while (!q.empty())
	{
		start = q.front();
		q.pop();
		visited[start] = 0;
		for (int x = k[start]; x != -1; x = road[x].next)//所有以start为起点的路在road中的位置 
		{ 
			int y= road[x].end;//y表示这个路的终点
			if (dist[y] > dist[start] + road[x].te)
			{
				path[y][0] = start;
				path[y][1] = x;//用于打印路径
				dist[y] = dist[start]+ road[x].te;
				if (!visited[y])
				{
					visited[y] = 1;//从s到y的路程发生了改变，则以y为起点的路的长度可能也改变 
					q.push(y);
				}
			}
		}
	}
}

void printAndUpdate(int start, int end)
{
	int result[10001];
	int num=1,num_e=0;
	for (int i = end; i !=start; i = path[i][0])
	{
		result[num_e++] = i; num += 2;
		road[path[i][1]].te += road[path[i][1]].a * road[path[i][1]].t0;
	}
	cout << num << endl;
	cout << start << " ";
	for (int i = num_e - 1; i >= 0; i--)
	{
		cout << path[result[i]][1] << " " << result[i];
		if (i != 0)cout << " ";
		else cout << endl;
	}
}