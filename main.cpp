#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define INF 0x3f3f3f
//INFINITO --> 1.061.109.567 em decimal

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

	Grafo(int quant_vertices, int quant_arestas)
	{
		n = quant_vertices;
		m = quant_arestas;
		arestas = new Aresta[m];
		for (int i = 0; i < n; i++)
		{
		}
	}
	void printArestas()
	{
		for(int i=0; i<m; i++)
		{
			arestas[i].print();
		}
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
	int **pai = new int*[N]; //vetor que armazena os pais do vertido.
	int npath = -1; //numero de caminhos de s até t.
	bool inPath = false;

	//Inicializa os vetores auxiliares
	for(int i=0;i<N;i++) //preenche 'd' com infinito.
	{
		d[i] = INF;
		pai[i]=NULL;
	}
	d[s]=0; //a distancia da origem até ele mesmo é zero.
	pai[0]=new int[s];

	/*RELAXA*/
	for(int i=0;i<M;i++)
	{
		int u = g->arestas[i].origem;
		int v = g->arestas[i].destino;
		int w = g->arestas[i].peso; //w --> weight
		if(d[v] > (d[u] + w)) //Relaxa
		{
			d[v] = d[u] + w;
			pai[v] = new int[u];
			//cout << "u:" << u << " v:" << v << " w:" << w << " d[v]:" << d[v] << endl;
			if(u==s)
			{
				inPath=true;
				path.push_back(vector<int>());
				path[++npath].push_back(d[v]); //armazeno o peso na primeira posição do vector
				path[npath].push_back(s); //insiro a origem no vector
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
		if(d[v] > (d[u] + w))
		{
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
	vector<vector<int>> path;

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
