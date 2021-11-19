#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <limits.h>

using namespace std;

typedef pair<int, int> ii;

const int noduriMAX = 100001;

class Graf
{
private:
	int nrNoduri;
	int nrMuchii;
	int plecareBFS;
	vector <vector<int>> adiacenta;
	vector <vector<int>> ans; //pt muchie critica si pt biconexe
	vector <vector<ii>> adiacentaCosturi;

	void Tarjan(int nod, int nrPasi[], int minimPasi[],
		stack<int>* st, bool inSt[], vector<vector<int>>& afisare);
	void DFSTopologic(int start, bool vizitat[], stack <int>& st);
	void TBFA(int nod, int parinte, int nrPasi[], int minimPasi[]);
	void DFSBiconex(int nod, int nrPasi[], int minimPasi[], int parinte[], stack <pair<int, int>>& muchii);
	
public:
	void GrafNeorientat(string fisier);
	void GrafOrientat(string fisier);
	void DFS(int start, bool vizitat[]);
	void NumaraCompCnx();
	void CitireBFS();
	void BFS();
	void CTC();
	void HavelHakimi();
	void SortareTopologica();
	vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections);
	void Biconexe();
	void citireAPM(string fisier);
	void citireDjikstra(string fisier);
	void primAPM();
	void Dijkstra();
	void BellmanFord();
};

void Graf::GrafNeorientat(string fisier)
{
	ifstream in(fisier);
	in >> nrNoduri >> nrMuchii;
	int start;
	int capat;
	adiacenta.resize(nrNoduri + 1);
	for (int i = 0; i < nrMuchii; ++i)
	{
		in >> start >> capat;
		adiacenta[start].push_back(capat);
		adiacenta[capat].push_back(start);
	}
	in.close();
}

void Graf::GrafOrientat(string fisier)
{
	ifstream in(fisier);
	in >> nrNoduri >> nrMuchii;
	int start;
	int capat;
	adiacenta.resize(nrNoduri + 1);
	for (int i = 0; i < nrMuchii; ++i)
	{
		in >> start >> capat;
		adiacenta[start].push_back(capat);
	}
	in.close();
}

void Graf::DFS(int start, bool vizitat[])
{
	vizitat[start] = true;
	for (int vecin : adiacenta[start])
	{
		if (!vizitat[vecin])
			DFS(vecin, vizitat);
	}
}

void Graf::NumaraCompCnx()
{
	ofstream out("dfs.out");
	int nrCompCnx = 0;
	bool vizitat[noduriMAX] = { false };
	for (int i = 1; i <= nrNoduri; ++i)
	{
		if (!vizitat[i])
		{
			DFS(i, vizitat);
			nrCompCnx++;
		}
	}
	out << nrCompCnx;
	out.close();
}

void Graf::CitireBFS()
{
	ifstream in("bfs.in");
	in >> nrNoduri >> nrMuchii >> plecareBFS;
	int start;
	int capat;
	adiacenta.resize(nrNoduri + 1);
	for (int i = 0; i < nrMuchii; ++i)
	{
		in >> start >> capat;
		adiacenta[start].push_back(capat);
	}
	in.close();
}

void Graf::BFS()
{
	queue <int> coadaBFS;
	int distanta[noduriMAX] = { 0 };
	ofstream out("bfs.out");
	int copiePlecareBFS = plecareBFS;
	bool vizitat[noduriMAX] = { false };
	vizitat[copiePlecareBFS] = true;
	coadaBFS.push(copiePlecareBFS);
	while (!coadaBFS.empty())
	{
		copiePlecareBFS = coadaBFS.front();
		coadaBFS.pop();
		for (int vecin : adiacenta[copiePlecareBFS])
			if (!vizitat[vecin])
			{
				vizitat[vecin] = true;
				distanta[vecin] = distanta[copiePlecareBFS] + 1;
				coadaBFS.push(vecin);
			}		
	}
	for (int i = 1; i <= nrNoduri; ++i)
	{
		if (!vizitat[i])
			out << -1 << " ";
		else
			out << distanta[i] << " ";
	}
		
	out.close();
}

void Graf::Tarjan(int nod, int nrPasi[], int minimPasi[], stack<int>* st, bool inSt[], vector<vector<int>>& afisare)
{
	//calc nr de pasi pt fiecare nod in DFS si pt fiecare nod dupa ce termin recursia pe vecinii sai 
	//actualizez minimul cu minimul dintre nod si cel mai jos copil al sau din arborele DFS
	static int pas = 0;
	nrPasi[nod] = minimPasi[nod] = ++pas;
	st->push(nod);
	inSt[nod] = true;

	for (int vecin : adiacenta[nod])
	{
		if (nrPasi[vecin] == -1)
		{
			Tarjan(vecin, nrPasi, minimPasi, st, inSt, afisare);
			minimPasi[nod] = min(minimPasi[nod], minimPasi[vecin]);
		}
		else if (inSt[vecin] == true)
			minimPasi[nod] = min(minimPasi[nod], nrPasi[vecin]);
	}
	if (minimPasi[nod] == nrPasi[nod])
	{
		vector <int> auxAfisare;
		while (st->top() != nod)
		{
			auxAfisare.push_back(st->top());
			inSt[st->top()] = false;
			st->pop();
		}
		auxAfisare.push_back(st->top());
		inSt[st->top()] = false;
		st->pop();
		afisare.push_back(auxAfisare);
	}
}

void Graf::CTC()
{
	int* nrPasi = new int[nrNoduri + 1];
	int* minimPasi = new int[nrNoduri + 1];
	bool* inSt = new bool[nrNoduri + 1];
	stack<int>* st = new stack<int>();
	vector < vector<int>> afisare;

	for (int i = 1; i <= nrNoduri; i++)
	{
		nrPasi[i] = -1;
		minimPasi[i] = -1;
		inSt[i] = false;
	}

	for (int i = 1; i <= nrNoduri; i++)
		if (nrPasi[i] == -1)
			Tarjan(i, nrPasi, minimPasi, st, inSt, afisare);

	ofstream out("ctc.out");
	out << afisare.size() << "\n";
	for (int i = 0; i < (int)afisare.size(); ++i)
	{
		for (int j = 0; j < (int)afisare[i].size(); ++j)
			out << afisare[i][j] << " ";
		out << "\n";
	}
	out.close();
}

void Graf::HavelHakimi()
{
	//facem urm:
	//sortam vect desc
	//scot primul elem (val v) si scad din celelalte v elem 1
	//pana cand:
	//toate elem sunt 0 (->exista)
	//v > vector.size() (->nu exista)
	//dupa ce scadem 1 din fiecare exista un elem care sa fie neg(->nu exista)
	ifstream in("havel.in");
	int x;
	bool ok = true;
	vector <int> v;
	while (in >> x)
		v.push_back(x);
	in.close();
	ofstream out("havel.out");
	while (true)
	{
		sort(v.begin(), v.end(), greater<>());
		if (v[0] == 0)
			break;

		int aux = v[0];
		v.erase(v.begin());
		if (aux > (int)v.size())
		{
			ok = false;
			break;
		}
		for (int i = 0; i < aux; ++i)
		{
			v[i]--;
			if (v[i] < 0)
			{
				ok = false;
				break;
			}
		}
		if (!ok)
			break;
	}
	ok ? out << "Da" : out << "Nu";
	out.close();
	
}

void Graf::DFSTopologic(int start, bool vizitat[], stack <int>& st)
{
	//dfs cu stiva in care punem elem curent
	//dupa ce am terminat recursia
	vizitat[start] = true;

	for (int vecin : adiacenta[start])
	{
		if (!vizitat[vecin])
			DFSTopologic(vecin, vizitat, st);
	}

	st.push(start);
}

void Graf::SortareTopologica()
{
	ofstream out("sortaret.out");
	stack <int> st;
	bool* vizitat = new bool[nrNoduri + 1];
	for (int i = 1; i <= nrNoduri; ++i)
		vizitat[i] = false;
	for (int i = 1; i <= nrNoduri; ++i)
		if (!vizitat[i])
			DFSTopologic(i, vizitat, st);
	
	while (!st.empty())
	{
		out << st.top() << " ";
		st.pop();
	}
	
	out.close();
}

void Graf::TBFA(int nod, int parinte, int nrPasi[], int minimPasi[])
{
	//la fel ca la CTC doar ca gasesc o muchie critica 
	//atunci cand un copil nu se poate duce mai sus in arborele de DFS decat nivelul tatalui
	static int pas = 0;
	nrPasi[nod] = minimPasi[nod] = ++pas;
	for (int vecin : adiacenta[nod])
	{
		if (nrPasi[vecin] == -1)
		{
			TBFA(vecin, nod, nrPasi, minimPasi);
			minimPasi[nod] = min(minimPasi[vecin], minimPasi[nod]);
		}
		else if (vecin != parinte)
			minimPasi[nod] = min(minimPasi[nod], nrPasi[vecin]);
		if (minimPasi[vecin] > nrPasi[nod])
			ans.push_back({ nod, vecin });
	}
}

vector<vector<int>> Graf::criticalConnections(int n, vector<vector<int>>& connections) {

	adiacenta.resize(n + 1);
	for (auto muchie : connections)
	{
		adiacenta[muchie[0]].push_back(muchie[1]);
		adiacenta[muchie[1]].push_back(muchie[0]);
	}
	int* nrPasi = new int[n];
	int* minimPasi = new int[n];
	for (int i = 0; i < n; i++)
	{
		nrPasi[i] = -1;
		minimPasi[i] = -1;
	}
	TBFA(0, -1, nrPasi, minimPasi);
	return ans;
}

void Graf::DFSBiconex(int nod, int nrPasi[], int minimPasi[], int parinte[], stack <pair<int, int>>& muchii)
{
	static int pas = 0;
	nrPasi[nod] = minimPasi[nod] = ++pas;
	int nrFii = 0;
	for (int vecin : adiacenta[nod])
	{
		if (nrPasi[vecin] == -1)
		{
			nrFii++;
			parinte[vecin] = nod;
			muchii.push({ nod, vecin });
			DFSBiconex(vecin, nrPasi, minimPasi, parinte, muchii);
			minimPasi[nod] = min(minimPasi[nod], minimPasi[vecin]);

			if ((parinte[nod] == -1 && nrFii > 1)	// pt rad
				|| (nrPasi[nod] != -1 && minimPasi[vecin] >= nrPasi[nod]))	//pt celelate noduri
			{
				vector <int> aux;
				while (muchii.top().first != nod && muchii.top().second != vecin)
				{
					aux.push_back(muchii.top().second);
					muchii.pop();
				}
				aux.push_back(muchii.top().second);
				aux.push_back(muchii.top().first);
				muchii.pop();
				ans.push_back(aux);
			}
		}
		else if (vecin != parinte[nod])
			minimPasi[nod] = min(minimPasi[nod], nrPasi[vecin]);
	}
}

void Graf::Biconexe()
{
	int* nrPasi = new int[nrNoduri + 1];
	int* minimPasi = new int[nrNoduri + 1];
	int* parinte = new int[nrNoduri + 1];
	stack<pair<int, int>> muchii;
	for (int i = 1; i <= nrNoduri; ++i)
	{
		nrPasi[i] = -1;
		minimPasi[i] = -1;
		parinte[i] = -1;
	}

	for (int i = 1; i <= nrNoduri; i++)
		if (nrPasi[i] == -1)
			DFSBiconex(i, nrPasi, minimPasi, parinte, muchii);

	ofstream out("biconex.out");
	out << ans.size() << "\n";
	for (auto comp : ans)
	{
		for (int nod : comp)
			out << nod << " ";
		out << "\n";
	}
	out.close();
}

void Graf::citireAPM(string fisier)
{
	ifstream in(fisier);
	in >> nrNoduri >> nrMuchii;
	int start;
	int capat;
	int cost;
	adiacentaCosturi.resize(nrNoduri + 1);
	for (int i = 0; i < nrMuchii; ++i)
	{
		in >> start >> capat >> cost;
		adiacentaCosturi[start].push_back(make_pair(capat, cost));
		adiacentaCosturi[capat].push_back(make_pair(start, cost));
	}
	in.close();
}

void Graf::primAPM()
{
	priority_queue<ii, vector<ii>, greater<ii>> noduriAPM;
	vector<int> costuri(nrNoduri + 1, 1001);
	vector<bool> inAPM(nrNoduri + 1, false);
	vector<int> parinte(nrNoduri + 1, -1);
	noduriAPM.push(make_pair(0, 1));
	costuri[1] = 0;
	
	while (!noduriAPM.empty())
	{
		int nod = noduriAPM.top().second;
		noduriAPM.pop();
		
		if (inAPM[nod])
			continue;

		inAPM[nod] = true;
		for (auto muchie : adiacentaCosturi[nod])
		{
			int vecin = muchie.first;
			int cost = muchie.second;
			if (cost < costuri[vecin])
			{
				costuri[vecin] = cost;
				noduriAPM.push(make_pair(cost, vecin));
				parinte[vecin] = nod;
			}
		}
	}
	ofstream out("apm.out");
	int s = 0;
	for (int i = 1; i <= nrNoduri; i++)
		s += costuri[i];
	out << s << "\n";
	out << nrNoduri - 1 << "\n";
	for (int i = 2; i <= nrNoduri; i++)
	{
		out << i << " " << parinte[i] << "\n";
	}
	
}

void Graf::citireDjikstra(string fisier)
{
	ifstream in(fisier);
	in >> nrNoduri >> nrMuchii;
	int start;
	int capat;
	int cost;
	adiacentaCosturi.resize(nrNoduri + 1);
	for (int i = 0; i < nrMuchii; ++i)
	{
		in >> start >> capat >> cost;
		adiacentaCosturi[start].push_back(make_pair(capat, cost));
	}
	in.close();
}

void Graf::Dijkstra() 
{

	vector<int> dist(nrNoduri + 1, INT_MAX);
	vector<bool>viz(nrNoduri + 1, false);
	priority_queue<ii, vector<ii>, greater<ii>> costuri;

	dist[1] = 0;
	costuri.push({ 0, 1 });

	while (!costuri.empty()) {
		int nod = costuri.top().second;
		costuri.pop();

		if (!viz[nod]) {
			viz[nod] = true;

			for (auto vecin : adiacentaCosturi[nod]) 
			{
				if (dist[nod] + vecin.second < dist[vecin.first]) 
				{
					dist[vecin.first] = dist[nod] + vecin.second;
					costuri.push({ dist[vecin.first], vecin.first });
				}
			}
		}
	}

	ofstream out("dijkstra.out");

	for (int i = 2; i <= nrNoduri; ++i) 
	{
		if (dist[i] != INT_MAX) 
			out << dist[i] << " ";
		else 
			out << "0 ";
	}
	out.close();
}

void Graf::BellmanFord()
{
	ofstream out("bellmanford.out");

	vector<int> dist(nrNoduri + 1, INT_MAX);
	vector<bool> inCoada(nrNoduri + 1, false);
	vector<int> relax(nrNoduri + 1, 0);
	queue<int> noduri;
	
	dist[1] = 0;
	inCoada[1] = true;
	noduri.push(1);

	while (!noduri.empty())
	{
		int nod = noduri.front();
		noduri.pop();
		inCoada[nod] = false;

		for (auto muchie : adiacentaCosturi[nod])
		{
			int vecin = muchie.first;
			int cost = muchie.second;

			if (dist[nod] + cost < dist[vecin])
			{
				dist[vecin] = dist[nod] + cost;
				relax[vecin]++;

				if (relax[vecin] == nrNoduri)
				{
					out << "Ciclu negativ!";
					return;
				}
				
				if (!inCoada[vecin])
				{
					noduri.push(vecin);
					inCoada[vecin] = true;	
				}
				
			}
		}
	}
	for (int i = 2; i <= nrNoduri; i++)
	{
		if (dist[i] == INT_MAX)
			out << "0 ";
		else
			out << dist[i] << " ";
	}
	out.close();
}

class Disjoint {
private:
	int nrElem;
	int nrOp;
	int* parinte;
	int* inaltime;

	int findRoot(int x)
	{
		if (parinte[x] != x)
			parinte[x] = findRoot(parinte[x]);

		return parinte[x];
	}

	void combin(int x, int y)
	{
		int xRoot = findRoot(x);
		int yRoot = findRoot(y);

		if (xRoot == yRoot)
			return;

		if (inaltime[xRoot] < inaltime[yRoot])
			parinte[xRoot] = yRoot;
		else if (inaltime[xRoot] > inaltime[yRoot])
			parinte[yRoot] = xRoot;
		else
		{
			parinte[yRoot] = xRoot;
			inaltime[xRoot] ++;
		}


	}
public:

	void afisare()
	{
		ifstream in("disjoint.in");
		ofstream out("disjoint.out");
		
		in >> nrElem >> nrOp;
		parinte = new int[nrElem + 1];
		inaltime = new int[nrElem + 1];
		for (int i = 1; i <= nrElem; i++)
			parinte[i] = i;
		int cod, x1, x2;
		for (int i = 0; i < nrOp; i++)
		{
			in >> cod >> x1 >> x2;
			if (cod == 1)
			{
				combin(x1, x2);
			}
			else
			{
				if (findRoot(x1) == findRoot(x2))
					out << "DA\n";
				else
					out << "NU\n";
					
			}
		}
		in.close();
		out.close();
	}
};

int main()
{
	Graf g;
	g.citireDjikstra("dijkstra.in");
	g.Dijkstra();
	return 0;
}