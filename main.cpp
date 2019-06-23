#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

/**
 * BRANCO - nodo ainda não encontrado
 * CINZA  - nodo encontrado
 * PRETO  - todos os vizinhos verificados
 */
#define BRANCO 0
#define CINZA 1
#define PRETO 2

//INFINITO --> 1.061.109.567 em decimal
#define INF 0x3f3f3f

//N --> vertices de 0 até N-1 [0,N)
//M --> m linhas - numero de arcos
//s --> vertice de origem
//t --> vertice de destino

using namespace std;
class Aresta
{
public:
	int origem;
	int destino;
	int peso;
	/*Aresta(int _origem, int _destino, int _peso)
	{
		origem=_origem;
		destino=_destino;
		peso=_peso;
	}*/
	void set(int _origem, int _destino, int _peso)
	{
		origem  = _origem;
		destino = _destino;
		peso    = _peso;
	}
	void print()
	{
		cout << "(" << origem << "," << destino << "," << peso << ")" << endl;
	}
};

class Grafo
{
public:
	int n; //quantidade de VERTICES no grafo
	int m; //quantidade de ARESTAS no grafo
	Aresta* arestas;
	vector<vector<int>> adj; //lista de adjacentes
	vector<int> container; //lista nodos

	Grafo(int quant_vertices, int quant_arestas)
	{
		n = quant_vertices;
		m = quant_arestas;
		arestas = new Aresta[m];
		for (int i = 0; i < n; i++)
		{
			adj.push_back(vector<int>());
		}
	}
	void criarAresta(int x, int y)
	{
		adj[x].push_back(y);
		
	}
	void printArestas()
	{
		for(int i=0; i<m; i++)
		{
			arestas[i].print();
		}
	}
	void printContainer()
	{
		cout << "container: ";
		for(int c : container)
		{
			cout << c << ",";
		}
		cout<<endl;
	}
};
class Info
{
public:
	int x;
	int y;
	int w;
	Info(int _x, int _y, int _w)
	{
		x = _x;
		y = _y;
		w = _w;
	}
	void print()
	{
		cout << "(" << x << "," << y << "," << w << ")" << endl;
	}
};

//------------------------------------------------------------------------------
void printArr(int dist[], int n) 
{ 
    printf("Vertex   Distance from Source\n"); 
    for (int i = 0; i < n; ++i) 
        printf("%d \t\t %d\n", i, dist[i]); 
}

void printPP(int *v, int n) 
{ 
    cout << ": [pai:filho] ";
    for (int i=0; i<n; ++i) 
        cout << "[" << v[i] << ":" << i << "] ";
    cout << endl;
}

void printVec(vector<int> v, int n) 
{ 
    cout << ": ";
    for (int i : v) 
        cout << v[i] << " -> ";
    cout << endl;
}

bool DFS(Grafo *g, int v, vector<int> &cor, int dest)
{
	cor[v]=CINZA;
	//cout << "v:"<<v<<" dest:"<<dest<<endl;
	g->container.push_back(v);
	if(v==dest)
		return true;
	for(int i=0; i<g->adj[v].size(); ++i)
	{
		int w = g->adj[v][i]; //vertice adjacente
		if(cor[w]==BRANCO)
		{
			return DFS(g,w,cor,dest);
		}
	}
	return false;
}
bool DFS(Grafo *g, int v, int dest)
{
	vector<int> cor(g->n,0);
	return DFS(g,v,cor,dest);
}

/**
	g -> grafo percorrido;
	s -> vertice de origem;
	t -> vertice de destino;
*/
bool BellmanFord(Grafo* g, int s, int t, vector<vector<int>> &path)
{
	int N = g->n; //quant vertices
	int M = g->m; //quant arestas
	int d[N]; //vetor auxiliar para armazenar a distanda até a origem.
	//int *pai = new int[N]; //vetor que armazena os pais do vertices.
	int npath = -1; //numero de caminhos de s até t.
	bool inPath = false;
	vector<bool> mark(N,false);

	//Inicializa os vetores auxiliares
	for(int i=0;i<N;i++) //preenche 'd' com infinito.
	{
		d[i] = INF;
		//pai[i]=-1;
	}
	d[s]=0; //a distancia da origem até ele mesmo é zero.
	//pai[0]=s; //por padronização o pai da origem é ele mesmo.

	/*RELAXA*/
	for(int i=0;i<M;i++)
	{
		int u = g->arestas[i].origem;
		int v = g->arestas[i].destino;
		int w = g->arestas[i].peso; //w --> weight
		if(d[v] > (d[u] + w)) //Relaxa
		{
			d[v] = d[u] + w;
			//pai[v] = u;
			//cout << "u:" << u << " v:" << v << " w:" << w << " d[v]:" << d[v] << endl;
			if(u==s)
			{
				inPath=true;
				path.push_back(vector<int>());
				path[++npath].push_back(d[v]); //armazena o peso na primeira posição do vector
				path[npath].push_back(s); //insere a origem no vector
			}
			if(inPath)
			{
				path[npath].push_back(v);
			}
			if(v==t)
			{
				inPath=false;
				path[npath][0] = d[v];
			}
		}
	}

	/*VERIFICA CICLO NEGATIVO*/
	for(int i=0;i<M;i++)
	{
		int u = g->arestas[i].origem;
		int v = g->arestas[i].destino;
		int w = g->arestas[i].peso; //w --> weight
		//cout << "u:" << u << "--> v:" << v << "" << endl;
		//cout << "" << d[v] << " > (" << d[u] << " + " << w << ")" << endl;
		if(d[v] > (d[u] + w))
		{
			//printVec(path[npath], path[npath].size());
			/*
			int ant = -1;
			int prox = -1;
			int i=0;
			for(i=0; i<path.size(); i++)
			{
				for(int j=1; j<path[i].size(); j++)
				{
					//cout << path[i][j] << " -> ";
					if(mark[ path[i][j] ])
		        	{
		        		//cout << " mark: " << path[i][j] << endl;
		        		ant = mark[ path[i][j] ];
		        		prox = path[i][j];
		        		break;
		        	}
		        	mark[ path[i][j] ] = true;
				}
				//cout << endl;
				if(ant!=-1){break;}
			}
			
			bool resp=false;
			for(int j=1; j<path[i].size(); j++)
			{
				//cout << ant << " -> " << prox << " ";
				if(path[i][j] == ant)
		       	{
		       		//cout << "    >" << path[i][j] << endl;
		       		//pega o pai e passa para o DFS

		       		vector<int> cor(g->n,0);
		       		cor[prox] = CINZA;
		       		resp = DFS(g,ant,cor,t);

		       		if(!resp)
		       			cout << "Distancia infinito negativo 2" << endl;
		       		else
		       		{
		       			//g->printContainer();
		       			cout << path[i][0] << " (";
		       			for(int j=1; j<path[i].size(); j++)
						{
							if(path[i][j] != ant)
								cout << path[i][j] << " ";
							else
								break;
						}
						for(int j=0; j<g->container.size(); j++)
						{
							cout << g->container[j];
							if(j<g->container.size()-1){cout<<" ";}
						}
						cout << ")" << endl;
		       		}
		       		return false;
		       	}
			}
			*/
			cout << "Distancia infinito negativo" << endl;
			return false;
		}
	}
	//printArr(d, N);
	return true;
}

//------------------------------------------------------------------------------

Info *DecodificaInstrucao(string line)
{
	int x = 0;
	int y = 0;
	int w = 0;

    size_t found = line.find(" "); 
    if (found != string::npos) 
    {
    	x = atoi(line.substr(0, found).c_str());
    } 
  
    char arr[] = " ";
    int aux = found; 
    found = line.find(arr, found+1); 
    if (found != string::npos) 
    {
    	y = atoi(line.substr(aux, found).c_str());
		w = atoi(line.substr(found).c_str());
    }
    else
    	y = atoi(line.substr(aux).c_str());

	return new Info(x, y, w);
}

int main(int argc, char const *argv[])
{
	char line[50];
	int counter = 0; //contador de linhas lidas do arquivo de entrada;
	Grafo *g;
	Info *info = NULL;
	int m=0;//numero de linhas

	int vert_origem=0,vert_destino=0;
	vector<vector<int>> path; //a primeira coluna do path armazena o peso final do caminho, os demais elementos de cada linha gera um caminho.

	while (scanf("%[^\n]\n", line) == 1)
	{
		//cout << line << endl;
		counter++;
		if (counter == 1)
		{
			info = DecodificaInstrucao(line);
			//info->print();
			m = info->y;
			g = new Grafo(info->x, m);
			continue;
		}
		info = DecodificaInstrucao(line);
		//info->print();
		g->arestas[counter-2].set(info->x, info->y, info->w);
		g->criarAresta(info->x, info->y);

		if(counter == m+2)//+1 devido a primeira linha ser parametro e a ultima linha ser vertice de origem e destino
		{
			info = DecodificaInstrucao(line);
			vert_origem = info->x;
			vert_destino = info->y;
			//cout << "origem:"<<vert_origem<<" - destino:"<<vert_destino<<endl;
			break;
		}
	}

	bool resp = BellmanFord(g,vert_origem,vert_destino,path);
	if(resp)
	{
		int idmenor=0; //indice do menor distancia;
		int menor=path[0][0];
		int c=0;
		for(vector<int> item : path)
		{
			if(item[0]<menor)
			{
				menor = item[0];
				idmenor=c;
			}
			c++;
		}

		cout << path[idmenor][0] << " (";

		for(int i=1; i<path[idmenor].size(); i++)
		{
			cout << path[idmenor][i];
			if(i<path[idmenor].size()-1){cout<<" ";}
		}
		cout << ")" << endl;
	}

	return 0;
}
