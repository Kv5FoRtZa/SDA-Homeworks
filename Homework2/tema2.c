// Morus Alexandru 313 CC
// SDA tema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node
{
    int val, msj;
    int stanga, dreapta;
    char *mesaj;
} arbore;
typedef struct Heap
{
    int *num;
    int nr;
    int capacitate;
    char **vct;
} heap;
typedef struct afisare_1
{
    int *v, *rand;
    char **str;
    int cnt;
} af;
void Heapify(heap *h, int ind)
{
    // in aceasta funtie imi creez heap-ul
    int st = ind * 2 + 1;
    int dr = ind * 2 + 2;
    int min = ind;
    if (st < h->nr && st >= 0)
    {
        if (h->num[st] < h->num[ind])
        {
            min = st;
        }
        if (h->num[st] == h->num[ind] && strcmp(h->vct[st], h->vct[ind]) < 0)
        {
            min = st;
        }
    }
    if (dr < h->nr && dr >= 0)
    {
        if (h->num[dr] < h->num[min])
        {
            min = dr;
        }
        if (h->num[dr] == h->num[min] && strcmp(h->vct[dr], h->vct[min]) < 0)
        {
            min = dr;
        }
    }
    if (min != ind)
    {
        char *secundar;
        /// inversez cele 2 campuri
        int temp = h->num[min];
        secundar = (char *)malloc((strlen(h->vct[min]) + 1) * sizeof(char));
        strcpy(secundar, h->vct[min]);
        strcpy(h->vct[min], h->vct[ind]);
        strcpy(h->vct[ind], secundar);
        h->num[min] = h->num[ind];
        h->num[ind] = temp;
        Heapify(h, min);
        free(secundar);
    }
}
void insertHelper(heap *h, int ind)
{
    int tata = (ind - 1) / 2;
    if (h->num[tata] > h->num[ind])
    {
        // daca trebuie inversate din cauza marimii
        // le inversez
        char *secundar;
        secundar = (char *)malloc((strlen(h->vct[tata]) + 1) * sizeof(char));
        strcpy(secundar, h->vct[tata]);
        strcpy(h->vct[tata], h->vct[ind]);
        strcpy(h->vct[ind], secundar);
        int acum = h->num[tata];
        h->num[tata] = h->num[ind];
        h->num[ind] = acum;
        insertHelper(h, tata);
        free(secundar);
    }
    if (h->num[tata] == h->num[ind] && strcmp(h->vct[tata], h->vct[ind]) > 0)
    {
        // acelasi lucru pt lexicografie
        char *secundar;
        secundar = (char *)malloc((strlen(h->vct[tata]) + 1) * sizeof(char));
        strcpy(secundar, h->vct[tata]);
        strcpy(h->vct[tata], h->vct[ind]);
        strcpy(h->vct[ind], secundar);
        int temp = h->num[tata];
        h->num[tata] = h->num[ind];
        h->num[ind] = temp;
        insertHelper(h, tata);
        free(secundar);
    }
}
void insert(heap *h, int val, char *vct_nou)
{ // aici inserez un nod nou in heap
    h->num[h->nr] = val;
    strcpy(h->vct[h->nr], vct_nou);
    insertHelper(h, h->nr);
    h->nr = h->nr + 1;
}
void eliminare_varf(heap *h, int *nod, char *str)
{
    nod[0] = h->num[0];
    if (str != h->vct[0])
    {
        strcpy(str, h->vct[0]);
    }
    // bag ultimul element in 0, apoi elimin ultimul
    h->num[0] = h->num[h->nr - 1];
    if (h->vct[0] != h->vct[h->nr - 1])
    {
        strcpy(h->vct[0], h->vct[h->nr - 1]);
    }
    h->nr--;
    // apoi refac arborele
    Heapify(h, 0);
}
void legaturi(heap *h, FILE *f2, af *afs, int n, arbore **A, int *tati)
{
    int i;
    if (h->nr > 1)
    {
        int n1, n2, marime_1, marime_2, cnt_local, rand = 0;
        char *v1, *v2, *v_fin;
        // scot din heap cele 2 cele mai mici
        marime_1 = strlen(h->vct[0]);
        v1 = (char *)malloc((marime_1 + 1) * sizeof(char));
        eliminare_varf(h, &n1, v1);
        marime_2 = strlen(h->vct[0]);
        v2 = (char *)malloc((marime_2 + 1) * sizeof(char));
        eliminare_varf(h, &n2, v2);
        v_fin = (char *)malloc((marime_2 + marime_1 + 2) * sizeof(char));
        strcpy(v_fin, v1);
        // si le concatenez si bag iar in heap
        strcpy(v_fin + marime_1, v2);
        insert(h, n1 + n2, v_fin);
        // in afs salvez elementele in ordinea eliminarii lor
        afs->str[afs->cnt] = (char *)malloc((16 * n + 1) * sizeof(char));
        afs->v[afs->cnt] = n1;
        strcpy(afs->str[afs->cnt], v1);
        afs->cnt++;
        afs->str[afs->cnt] = (char *)malloc((16 * n + 1) * sizeof(char));
        afs->v[afs->cnt] = n2;
        strcpy(afs->str[afs->cnt], v2);
        afs->cnt++;
        cnt_local = afs->cnt;
        legaturi(h, f2, afs, n, A, tati);
        // si aici salvez randul pe care trebuie afisate
        // acesta este randul tatalui + 1
        for (i = 0; i < afs->cnt; i++)
        {
            if (afs->v[i] == n1 + n2 && strcmp(afs->str[i], v_fin) == 0)
            {
                // i este pozitia tatalui
                rand = afs->rand[i];
                break;
            }
        }
        tati[cnt_local - 1] = i;
        tati[cnt_local - 2] = i;
        // in A am arborele si in afs am afisarea pt cerina 1
        A[i]->val = n1 + n2;
        A[i]->stanga = cnt_local - 2;
        A[i]->dreapta = cnt_local - 1;
        afs->rand[cnt_local - 1] = rand + 1;
        afs->rand[cnt_local - 2] = rand + 1;
        free(v_fin);
        free(v1);
        free(v2);
    }
    else
    {
        afs->rand[afs->cnt - 1] = 0;
        tati[afs->cnt] = -1;
    }
}
int lca(int p1, int p2, int nr, int *am_fost, int *tati)
{
    // parcurg de la p1 pana la radacina
    while (tati[p1] != -1)
    {
        am_fost[p1] = nr;
        p1 = tati[p1];
    }
    // apoi parcurg de la p2 pana dau de un element deja parcurs
    // si returnez acel element
    am_fost[p1] = nr;
    while (tati[p2] != -1)
    {
        if (am_fost[p2] == nr)
        {
            return p2;
        }
        am_fost[p2] = nr;
        p2 = tati[p2];
    }
    am_fost[p2] = nr;
    return p2;
}
int distanta_noduri(int p1, int p2, int *am_fost, int *tati)
{
    // parcurg de la p1 pana la radacina
    int cnt1 = 0, cnt2 = 0;
    // in cnt1 si cnt2 salvez distanta nod ->lca
    while (tati[p1] != -1)
    {
        am_fost[p1] = cnt1;
        cnt1++;
        p1 = tati[p1];
    }
    // apoi parcurg de la p2 pana dau de un element deja parcurs
    // si returnez suma intre cnt1 si cnt2
    am_fost[p1] = cnt1;
    while (tati[p2] != -1)
    {
        if (am_fost[p2] != 0)
        {
            return cnt2 + am_fost[p2];
        }
        am_fost[p2] = cnt2;
        cnt2++;
        p2 = tati[p2];
    }
    am_fost[p2] = cnt2;
    return cnt1 + cnt2;
}
void eliberare(arbore **A, int n)
{
    // eliberez memoria din mesajul arborelui
    int i;
    for (i = 0; i <= n; i++)
    {
        if (A[i]->msj == 1)
        {
            free(A[i]->mesaj);
        }
    }
}
int main(int argc, char *argv[])
{
    //./a.out -c1 sateliti.in sateliti.out
    FILE *f = fopen(argv[2], "r");
    FILE *f2 = fopen(argv[3], "w");
    // char lol[6] = "-c5";
    int n, i, j, *tati, radacini, frunze, l, poz_rad, m, nr, poz;
    // declar variabile, aloc memorie initiala
    af *afs = (af *)malloc(sizeof(af));
    afs->cnt = 0;
    fscanf(f, "%d", &n);
    // arborele final va avea maxim 2 * n elemente
    arbore **A = (arbore **)malloc(2 * n * sizeof(arbore *));
    for (i = 0; i < 2 * n; i++)
    {
        A[i] = (arbore *)malloc(sizeof(arbore));
        A[i]->msj = 0;
        A[i]->dreapta = -1;
        A[i]->stanga = -1;
    }
    tati = (int *)malloc(2 * n * sizeof(int));
    afs->v = (int *)malloc(2 * n * sizeof(int));
    afs->rand = (int *)malloc(2 * n * sizeof(int));
    afs->str = (char **)malloc(2 * n * sizeof(char *));
    int *v = (int *)malloc(n * sizeof(int));
    char **mt = (char **)malloc(n * sizeof(char *));
    heap *sateliti = (heap *)malloc(sizeof(heap));
    sateliti->nr = n; // initializam heap=ul
    sateliti->capacitate = n;
    sateliti->num = (int *)malloc(n * sizeof(int));
    sateliti->vct = (char **)malloc(n * sizeof(char *));
    for (i = 0; i < n; i++)
    { // citesc datele despre sateliti
        fscanf(f, "%d", &v[i]);
        mt[i] = (char *)malloc(16 * sizeof(char));
        fscanf(f, "%s", mt[i]);
        sateliti->num[i] = v[i];
        sateliti->vct[i] = (char *)malloc((16 * n + 1) * sizeof(char));
        strcpy(sateliti->vct[i], mt[i]);
    }
    i = n / 2 - 1;
    while (i > -1)
    {
        // fac heapul initial
        Heapify(sateliti, i);
        i--;
    }
    legaturi(sateliti, f2, afs, n, A, tati);
    for (i = 0; i < afs->cnt; i++)
    {
        // introduc si mesajul in arbore
        A[i]->mesaj = (char *)malloc((strlen(afs->str[i]) + 1) * sizeof(char));
        strcpy(A[i]->mesaj, afs->str[i]);
        A[i]->msj = 1;
    }
    int sum = strlen(sateliti->vct[0]) + 1;
    A[afs->cnt]->mesaj = (char *)malloc(sum * sizeof(char));
    A[i]->msj = 1;
    strcpy(A[afs->cnt]->mesaj, sateliti->vct[0]);
    if (strcmp(argv[1], "-c1") == 0)
    {
        fprintf(f2, "%d-%s\n", sateliti->num[0], sateliti->vct[0]);
        for (i = 1; i <= afs->rand[0]; i++)
        { // in forul mare iau randurile
            for (j = 0; j < afs->cnt; j++)
            {
                // si apoi parcurg toate numerele afisate
                // si le afisez pe cele de pe reandul bun
                if (afs->rand[j] == i)
                {
                    fprintf(f2, "%d-%s ", afs->v[j], afs->str[j]);
                }
            }
            fprintf(f2, "\n");
        }
    }
    else if (strcmp(argv[1], "-c2") == 0)
    {
        char mesaj[1001];
        fscanf(f, "%d", &m);
        for (i = 0; i < m; i++)
        {
            fscanf(f, "%s", mesaj);
            nr = strlen(mesaj);
            poz = afs->cnt; // citesc mesajul si incep din radacina
            for (j = 0; j <= nr; j++)
            {
                if (A[poz]->dreapta == -1)
                {
                    // am ajuns la o frunza si o afisez
                    fprintf(f2, "%s ", A[poz]->mesaj);
                    poz = afs->cnt;
                    j--;
                }
                else if (mesaj[j] == '0')
                {
                    // altfel avansez in arbore
                    poz = A[poz]->stanga;
                }
                else if (mesaj[j] == '1')
                {
                    poz = A[poz]->dreapta;
                }
            }
            fprintf(f2, "\n");
        }
    }
    else if (strcmp(argv[1], "-c3") == 0)
    {
        int adancime, afisare_01[1000];
        char vct[16];
        fscanf(f, "%d", &m);
        for (i = 0; i < m; i++)
        {
            adancime = 0;
            fscanf(f, "%s", vct);
            for (j = 0; j < afs->cnt; j++)
            {
                if (strcmp(A[j]->mesaj, vct) == 0)
                {
                    // am gasit pozitia initiala
                    poz = j;
                    break;
                }
            }
            A[poz]->val = 0;
            while (A[poz]->val != A[afs->cnt]->val)
            {
                // verific daca am venit din stanga sau din dreapta
                if (poz == A[tati[poz]]->dreapta)
                {
                    afisare_01[adancime] = 1;
                }
                else
                {
                    afisare_01[adancime] = 0;
                }
                adancime++;
                poz = tati[poz];
            }
            // si apoi afisez
            for (j = adancime - 1; j >= 0; j--)
            {
                fprintf(f2, "%d", afisare_01[j]);
            }
        }
    }
    else if (strcmp(argv[1], "-c4") == 0)
    {
        int poz_ant, *am_fost;
        am_fost = (int *)malloc(2 * n * sizeof(int));
        for (i = 0; i < 2 * n; i++)
        {
            am_fost[i] = 0;
        }
        char vct[16], anterior[16];
        fscanf(f, "%d", &m);
        fscanf(f, "%s", anterior);
        for (j = 0; j < afs->cnt; j++)
        {
            if (strcmp(A[j]->mesaj, anterior) == 0)
            {
                // am gasit pozitia 1
                poz_ant = j;
                break;
            }
        }
        for (i = 1; i < m; i++)
        {
            fscanf(f, "%s", vct);
            for (j = 0; j < afs->cnt; j++)
            {
                if (strcmp(A[j]->mesaj, vct) == 0)
                {
                    // am gasit pozitia curenta
                    poz = j;
                    break;
                }
            }
            // poz = pozitia curenta
            // poz_ant = pozitia lca veche
            poz_ant = lca(poz, poz_ant, i, am_fost, tati);
        }
        fprintf(f2, "%s", A[poz_ant]->mesaj);
        free(am_fost);
    }
    else if (strcmp(argv[1], "-c5") == 0)
    {
        int ultim_vct_tati = (afs->cnt + 1), *am_fost, marime = 5 * n;
        char legatura_rad[16], mesj[16], mesj2[16];
        char rad[16];
        tati = (int *)realloc(tati, marime * sizeof(int));
        afs->str = (char **)realloc(afs->str, marime * sizeof(char *));
        fscanf(f, "%d", &m);
        for (i = 0; i < m; i++)
        {
            fscanf(f, "%s", legatura_rad);
            for (j = 0; j < afs->cnt; j++)
            {
                if (strcmp(A[j]->mesaj, legatura_rad) == 0)
                {
                    // am gasit pozitia legaturii
                    poz = j;
                    break;
                }
            }
            // imi leg noul arbore de arborele vechi
            fscanf(f, "%d%s", &nr, mesj);
            tati[ultim_vct_tati] = poz;
            afs->str[ultim_vct_tati] = malloc(strlen(mesj) + 1);
            strcpy(afs->str[ultim_vct_tati], mesj);
            ultim_vct_tati++;
            if (ultim_vct_tati >= marime)
            {
                marime = marime * 2;
                tati = (int *)realloc(tati, marime * sizeof(int));
                afs->str = (char **)realloc(afs->str, marime * sizeof(char *));
            }
            fscanf(f, "%d", &radacini);
            for (j = 0; j < radacini; j++)
            {
                fscanf(f, "%s", mesj);
                // caut pozitia radacinii
                for (l = afs->cnt + 1; l < ultim_vct_tati; l++)
                {
                    if (strcmp(afs->str[l], mesj) == 0)
                    {
                        // am gasit pozitia radacinii
                        poz_rad = l;
                        break;
                    }
                }
                int nr_frunze;
                fscanf(f, "%d", &nr_frunze);
                for (l = 0; l < nr_frunze; l++)
                {
                    // bag noile noduri in vectorul de tati
                    fscanf(f, "%d%s", &nr, mesj);
                    tati[ultim_vct_tati] = poz_rad;
                    afs->str[ultim_vct_tati] = malloc(strlen(mesj) + 1);
                    strcpy(afs->str[ultim_vct_tati], mesj);
                    ultim_vct_tati++;
                    if (ultim_vct_tati >= marime)
                    {
                        marime = marime * 2;
                        tati = (int *)realloc(tati, marime * sizeof(int));
                        afs->str = (char **)realloc(afs->str, marime * sizeof(char *));
                    }
                }
            }
        }
        int p1, p2;
        fscanf(f, "%s%s", mesj, mesj2);
        // folosesc o functie asemanatoare cu lca-ul pentru a vedea distanta
        am_fost = (int *)malloc(marime * sizeof(int));
        for (i = 0; i < marime; i++)
        {
            am_fost[i] = 0;
        }
        for (l = 0; l < afs->cnt; l++)
        {
            if (strcmp(afs->str[l], mesj) == 0)
            {
                // am gasit pozitia primului nod
                p1 = l;
                break;
            }
        }
        for (l = afs->cnt + 1; l < ultim_vct_tati; l++)
        {
            if (strcmp(afs->str[l], mesj) == 0)
            {
                // am gasit pozitia primului nod
                p1 = l;
                break;
            }
        }
        for (l = 0; l < afs->cnt; l++)
        {
            if (strcmp(afs->str[l], mesj2) == 0)
            {
                // am gasit pozitia nodului 2
                p2 = l;
                break;
            }
        }
        for (l = afs->cnt + 1; l < ultim_vct_tati; l++)
        {
            if (strcmp(afs->str[l], mesj2) == 0)
            {
                // am gasit pozitia nodului 2
                p2 = l;
                break;
            }
        }
        int af = distanta_noduri(p1, p2, am_fost, tati);
        for (i = afs->cnt + 1; i < ultim_vct_tati; i++)
        {
            free(afs->str[i]);
        }
        free(am_fost);
        fprintf(f2, "%d", af);
    }
    // eliberari
    free(v);
    eliberare(A, afs->cnt);
    for (i = 0; i < 2 * n; i++)
    {
        free(A[i]);
    }
    free(A);
    for (i = 0; i < afs->cnt; i++)
    {

        free(afs->str[i]);
    }
    free(tati);
    free(afs->rand);
    free(afs->v);
    free(afs->str);
    free(afs);
    for (i = 0; i < n; i++)
    {
        free(mt[i]);
        free(sateliti->vct[i]);
    }
    free(sateliti->vct);
    free(sateliti->num);
    free(sateliti);
    free(mt);
    fclose(f);
    fclose(f2);
    return 0;
}