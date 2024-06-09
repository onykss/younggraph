#include <iostream>
#include <string.h>
#include <cstdlib>
#include <queue>
#include <fstream>
#include <vector>
#include <windows.h>
#include <sstream>
#include <iomanip>

using namespace std;

int check = 0;

struct Head
{
    struct Diagram * first;
    int CountOfElements;
};

struct Diagram
{
    int * ivals;
    int len;
    int level;
    struct Diagram ** Children;
    int ways;
    int number;
    int CountOfChildren;
    int OutFlag;
    int color;
    int * prime_c;
    int CountOfPrimeChildren;

    Diagram()
    {
        Children = nullptr;
        CountOfChildren = 0;
        ivals = new int[200];
        prime_c = new int[201];
        CountOfPrimeChildren = 0;
        ways = 1;
        color = -1;
    }

    void addChild()
    {
        CountOfChildren++;
        Diagram** temp_array = new Diagram*[CountOfChildren];
        for (int i = 0; i < CountOfChildren-1; i++)
        {
            temp_array[i] = Children[i];
        }
        delete[] Children;
        Children = temp_array;
        Children[CountOfChildren-1] = new Diagram;
    }

    Diagram* CheckRep(int * ival, int level, int length)
    {
        if (CountOfChildren == 0)
            return nullptr;
        int flag = 0;

        queue<Diagram*> q;
        q.push(this);

        while (!q.empty())
        {
            int level_size = q.size();
            for (int i = 0; i < level_size; i++)
            {
                Diagram* current = q.front();

                q.pop();
                int coin = 1;

                if (current->level == level && current->len == length)
                {
                    for (int i = 0; i < length; i++)
                    {
                        if (current->ivals[i] != ival[i])
                        {
                            coin = 0;
                            break;
                        }
                    }

                }
                else coin = 0;

                if (coin == 1)
                {
                    return current;
                    flag = 1;
                }
                else if (current->level < level)
                {
                    for (int j = 0; j < current->CountOfPrimeChildren; j++)
                    {
                        q.push(current->Children[current->prime_c[j]]);
                    }

                }
            }
        }
        if (flag == 0) return nullptr;
        return this;
    }


    int ** FillMatrix(Head * Head)
    {
        if (CountOfChildren == 0)
            return nullptr;

        int** matrix = new int*[Head->CountOfElements];
        for (int i = 0; i < Head->CountOfElements; i++)
        {
            matrix[i] = new int[Head->CountOfElements];
        }

        for (int i = 0; i < Head->CountOfElements; i++)
        {
            for (int j = 0; j < Head->CountOfElements; j++)
            {
                matrix[i][j] = 0;
            }
        }

        queue<Diagram*> q;
        q.push(this);

        while (!q.empty())
        {
            int level_size = q.size();
            for (int i = 0; i < level_size; i++)
            {
                Diagram* current = q.front();
                q.pop();

                for (int j = 0; j < current->CountOfChildren; j++)
                {
                    q.push(current->Children[j]);
                    matrix[current->number][current->Children[j]->number] = 1;
                }
            }
        }
        return matrix;
    }

    void checkShow()
    {
        cout << endl;
        for (int i = 0; i < len; i++)
        {
            cout << ivals[i];
        }
    }

    ~Diagram()
    {
        for (int i = 0; i < CountOfChildren-1; i++)
        {
            delete Children[i];
        }
        delete[] Children;
        delete ivals;
    }
};

typedef struct Diagram drgm;
typedef struct Head head;

void AddChildren(int levels, drgm * El, drgm * first, head * Head, int subgraph_check, int K, int n, int m);
void IncrWays(drgm* El, int x);
bool Shura(drgm * el);
float GetOpacity(drgm* El, vector<vector<int>> wsys, int* stats);

bool SmallerTK(drgm * el, int k);
bool MNrectangle(drgm * el, int n, int m);

int main()
{
    fstream fout;
    fout.open("vizualize\\data.txt", ofstream::out | ofstream::trunc);
    int CountOfLevels;
    int subgraph_check;
    int ribs, clr;
    int nes_of_contrast;
    int r, g, b;
    int cr, cg, cb;
    int K, n, m;

    drgm * First;
    First = new(drgm);
    First->ivals[0] = 1;
    First->ivals[1] = 0;
    First->len = 2;
    First->level = 0;
    First->Children = new(drgm*);
    First->CountOfChildren = 0;
    First->OutFlag = 0;

    head * Head;
    Head = new(head);
    Head->first = First;
    Head->CountOfElements = 1;

    int * stats;
    vector<vector<int>> ways_system;

    cout << "Count of levels: ";
    cin >> CountOfLevels;
    cout << "\nChose the way of generate:\n0 - classic young graph\n1 - Shura's subgraph\n2 - <=K subgraph\n3 - MNrectangle subgraph\n";
    cout << "Enter: ";
    cin >> subgraph_check;
    if (subgraph_check == 2)
    {
        cout << "\nEnter K: ";
        cin >> K;
    }
    else if (subgraph_check == 3)
    {
        cout << "\nEnter n (height): ";
        cin >> n;
        cout << "Enter m (width): ";
        cin >> m;
    }
    cout << "\nEdge mapping (1 - Yes | 0 - No): ";
    cin >> ribs;
    cout << "\nTint diagrams (1 - Yes | 0 - No): ";
    cin >> clr;
    if (clr)
    {
        cout << "\nValue will be in range [0, 255]";
        cout << "\nEnter RED scale: ";
        cin >> r;
        cout << "Enter GREEN scale: ";
        cin >> g;
        cout << "Enter BLUE scale: ";
        cin >> b;
        cout << "\nIs a contrasting color necessary? (1 - Yes | 0 - No): ";
        cin >> nes_of_contrast;
        if (nes_of_contrast)
        {
            cout << "\nEnter RED scale: ";
            cin >> cr;
            cout << "Enter GREEN scale: ";
            cin >> cg;
            cout << "Enter BLUE scale: ";
            cin >> cb;
        }
    }
    ostringstream MainColor;
    ostringstream ConColor;
    MainColor << "#" << hex << setfill('0') << setw(2) << r << setw(2) << g << setw(2) << b;
    if (nes_of_contrast)
    {
        ConColor << "#" << hex << setfill('0') << setw(2) << cr << setw(2) << cg << setw(2) << cb;
    }

    stats = (int*)calloc(CountOfLevels, sizeof(int));
    for (int i = 0; i < CountOfLevels; i++)
    {
        ways_system.push_back({1});
    }

    cout << "\n10%  |##------------------| Start of generation";

    AddChildren(CountOfLevels-1, Head->first, Head->first, Head, subgraph_check, K, n, m);

    queue<drgm*> qu;
    qu.push(Head->first);
    while (!qu.empty())
    {
        int level_size = qu.size();
        for (int i = 0; i < level_size; i++)
        {
            drgm* current = qu.front();
            qu.pop();
            if (current->color == -1)
            {
                stats[current->level]++;
                ways_system[current->level].push_back(current->ways);
                current->color = -2;
            }
            for (int j = 0; j < current->CountOfChildren; j++)
            {
                qu.push(current->Children[j]);
            }
        }
    }

    int num = 0;
    queue<drgm*> q;
    q.push(Head->first);

    fout << "'--args0' " << "'" << ribs << clr << "' ";
    fout << "'--args1' ";
    while (!q.empty())
    {
        int level_size = q.size();
        for (int i = 0; i < level_size; i++)
        {
            drgm* current = q.front();
            q.pop();

            if (current->OutFlag == 0)
            {
                current->number = num++;

                fout << "'";
                for (int i = 0; i < current->len-1; i++)
                {
                    if (i < current->len-2) fout << current->ivals[i] << ";";
                    else fout << current->ivals[i];
                }

                float opacity = GetOpacity(current, ways_system, stats);
                if (nes_of_contrast)
                {
                    if (opacity == 1
                        ) fout << "$" << opacity << "$" << ConColor.str() << "' ";
                    else
                    {
                        fout << "$" << opacity << "$" << MainColor.str() << "' ";
                    }
                }
                else
                {
                    fout << "$" << opacity << "$" << MainColor.str() << "' ";
                }

                current->OutFlag = 1;
            }
            for (int j = 0; j < current->CountOfChildren; j++)
            {
                q.push(current->Children[j]);
            }
        }
    }

    cout << "\n30%  |######--------------| Start of filling in the adjacency matrix";

    int ** matrix;
    matrix = Head->first->FillMatrix(Head);

    cout << "\n40%  |########------------| Start of visualization";

    string command = "vizualize\\.venv\\Scripts\\python.exe vizualize\\viz.py";

    fout << "'--args2' ";
    string MatLine;
    Head->CountOfElements;
    for (int i = 0; i < Head->CountOfElements; i++)
    {
        for (int j = 0; j < Head->CountOfElements; j++)
        {
            MatLine += to_string(matrix[i][j]);

        }
        fout << "'" + MatLine + "' ";
        MatLine = "";
    }
    fout.close();

    system(command.c_str());

    cout << "\n100% |####################|\nSaved to file: graph.pdf\n";
    cout << "\nStats: \n";
    stats[0] = 1;
    for (int i = 0; i < CountOfLevels; i++)
    {
        cout << "Level " << i+1 << ": " << stats[i] << " elements" << endl;
    }

    int cmd;
    cout << "\nDo you want to open \"graph.pdf\"? (1 - Yes | 0 - No): ";
    cin >> cmd;
    const char* graphPath = "graph.pdf";
    if (cmd)
    {
        ShellExecuteA(NULL, "open", graphPath, NULL, NULL, SW_SHOWNORMAL);
    }


    delete stats;
    delete First;

    return 0;
}

void AddChildren(int levels, drgm * El, drgm * first, head * Head, int subgraph_check, int K, int n, int m)
{
    for (int j = 0; j < El->len; j++)
    {
        if (j == 0 || j == El->len-1 || El->ivals[j-1] > El->ivals[j])
        {

            drgm * Child;
            Child = new(drgm);

            for (int i = 0; i < El->len; i++)
            {
                Child->ivals[i] = El->ivals[i];
            }

            Child->len = El->len;
            Child->ivals[j]++;

            Child->level = (El->level + 1);
            Child->OutFlag = 0;

            if (Child->ivals[Child->len - 1] != 0)
            {
                Child->ivals[Child->len] = 0;
                Child->len++;
            }

            bool accordance = true;

//A section for checking conditions for subgraphs. It is necessary to set the value of the variable "accordance" depending on the match.

            switch(subgraph_check)
            {
            case 1:
                if (!Shura(Child))
                {
                    delete Child;
                    accordance = false;
                }
                break;
            case 2:
                if (!SmallerTK(Child, K))
                {
                    delete Child;
                    accordance = false;
                }
                break;
            case 3:
                if (!MNrectangle(Child, n, m))
                {
                    delete Child;
                    accordance = false;
                }
                break;
            }

//-------------------------------------------------------------------------------------------------------------------------------------
            if (accordance)
            {

                drgm * Rep = nullptr;
                if (Child->len > 2) Rep = first->CheckRep(Child->ivals, (El->level + 1), Child->len);
                El->addChild();
                if (Rep)
                {
                    Rep->ways += El->ways;
                    IncrWays(Rep, El->ways);
                    El->Children[El->CountOfChildren-1] = Rep;
                    delete Child;
                }
                else
                {
                    Child->ways = El->ways;
                    El->Children[El->CountOfChildren-1] = Child;
                    El->prime_c[El->CountOfPrimeChildren] = El->CountOfChildren-1;
                    El->CountOfPrimeChildren += 1;
                    Head->CountOfElements++;

                    if (Child->level < levels) AddChildren(levels, El->Children[El->CountOfChildren-1], first, Head, subgraph_check, K, n, m);
                }

            }

        }
    }
}

void IncrWays(drgm* El, int x)
{
    for (int i = 0; i < El->CountOfChildren; i++)
    {
        El->Children[i]->ways += x;
        IncrWays(El->Children[i], x);
    }
}

float GetOpacity(drgm* El, vector<vector<int>> wsys, int* stats)
{
    int wMax = 0;
    for (int i = 0; i < stats[El->level]; i++)
    {
        if (wsys[El->level][i] > wMax) wMax = wsys[El->level][i];
    }
    float part = El->ways * (100.0/wMax);
    float res = part/100.0;
    if (El->ways == 1) res = 0.0;
    return res;
}

bool Shura(drgm * el)
{
    bool res = true;
    for (int i = 0; i < el->len-1; i++)
    {
        if (el->ivals[i] == el->ivals[i+1]) res = false;
    }
    return res;
}

bool SmallerTK(drgm * el, int k)
{
    bool res = true;
    if (el->len-1 > k) res = false;
    return res;
}

bool MNrectangle(drgm* el, int n, int m)
{
    bool res = true;
    if (el->len-1 > m || el->ivals[0] > n) res = false;
    return res;
}
