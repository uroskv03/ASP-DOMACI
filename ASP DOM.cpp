#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Lista {
	string s;
	Lista* sled;
	Lista(string s) :s(s), sled(nullptr) {}
};

void DodajElemListe(Lista*& ls, string s) {
	if (ls == nullptr) {
		ls = new Lista(s);
	}
	else {
		Lista* pom = ls;
		while (pom->sled != nullptr) {
			pom = pom->sled;
		}
		pom->sled = new Lista(s);
	}
}

struct Stablo {
	string s;
	Stablo* sin = nullptr;
	Stablo* brat = nullptr;
	Stablo(string s) : s(s), sin(nullptr), brat(nullptr) {}
};

struct Stek {
	Stablo* s;
	Stek* sled;
	Stek(Stablo* s) : s(s), sled(nullptr) {}
};

struct Graf {
	bool** matrica =nullptr;
	Lista* ls = nullptr;
	Graf(bool** matrica, Lista* ls): matrica(matrica),ls(ls) {}
};

Stablo* POP(Stek*& stek) {
	Stablo* r = stek->s;
	Stek* pom = stek;
	stek = stek->sled;
	delete pom;
	return r;
}

Stek* PUSH(Stek* stek, Stablo* stablo) {
	if (stek == nullptr) {
		stek = new Stek(stablo);
	}
	else {
		Stek* pom = new Stek(stablo);
		pom->sled = stek;
		stek = pom;
	}
	return stek;
}

Stablo* DodajStekStablu(Stablo* s, Lista* b) {
	Lista* ls = b;
	Stablo* pom = s;
	if (s == nullptr) {
		s = new Stablo(ls->s);
		ls = ls->sled;
		pom = s;
		while (ls != nullptr) {
			pom->sin = new Stablo(ls->s);
			pom = pom->sin;
			ls = ls->sled;
		}
	}
	else {
		pom = s;
		Stablo* pom1 = nullptr;
		while (ls != nullptr) {
			if (pom == nullptr) {
				pom = pom1;
				break;
			}
			pom1 = pom;
			while (pom1 != nullptr) {
				if ((pom1->s == ls->s)){
					pom = pom1->sin;
					break;
				}
				pom1 = pom1->brat;
			}
			if (pom1 == nullptr) {
				pom1 = pom;
				while (pom1->brat != nullptr) {
					pom1 = pom1->brat;
				}
				pom1->brat = new Stablo(ls->s);
				pom = pom1->brat;
				ls = ls->sled;
				break;
			}
			ls = ls->sled;
		}
	}
	if (ls != nullptr) {
		while (ls != nullptr && pom != nullptr) {
			pom->sin = new Stablo(ls->s);
			pom = pom->sin;
			ls = ls->sled;
		}
	}
	return s;
}

void Ocisti(Lista* l) {
	Lista* pom = l;
	while (pom != nullptr) {
		l = pom;
		pom = pom->sled;
		delete l;
	}
}

void Obrisi(Stablo*& stablo);

void Ucitaj(Stablo*& stablo) {
	Lista* ls = nullptr;
	string naziv;
	cout << "Uneti naziv file-a (bez .txt)" << endl;
	cin >> naziv;
	ifstream file(naziv += ".txt");
	if (!file.is_open()) {
		cout << "Ne moze da se otvori file";
	}
	if (stablo != nullptr) {
		Obrisi(stablo);
	}

	string l;
	while (getline(file, l)) {
		int j = 0;
		int i = 0;
		for (; l[i] != '\0'; i++) {
			if (l[i] == ' ') {
				string s = l.substr(j, i - j);
				j = i + 1;
				DodajElemListe(ls, s);
			}
		}
		string s = l.substr(j, i - j);
		j = i + 1;
		DodajElemListe(ls, s);
		stablo = DodajStekStablu(stablo, ls);
		Ocisti(ls);
		ls = nullptr;
	}
	file.close();
}

void Dodaj(Stablo*& stablo) {
	Lista* ls = nullptr;
	cout << "Unesi stek:" << endl;
	string l;
	getline(cin, l);
	getline(cin, l);
	if (l.size() == 0) {
		return;
	}
	int j = 0;
	int i = 0;
	for (; l[i] != '\0'; i++) {
		if (l[i] == ' ') {
			string s = l.substr(j, i - j);
			j = i + 1;
			DodajElemListe(ls, s);
		}
	}
	string s = l.substr(j, i - j);
	DodajElemListe(ls, s);
	stablo = DodajStekStablu(stablo, ls);
	Ocisti(ls);
	ls = nullptr;
}

void Ukloni(Stablo*& stablo) {
	if (stablo == nullptr) {
		cout << "Stablo je prazno pa ne moze nista ni da se brise iz njega" << endl;
		return;
	}
	Lista* ls = nullptr;
	cout << "Unesi stek:" << endl;
	string l;
	getline(cin, l);
	getline(cin, l);
	if (l.size() == 0) {
		return;
	}
	int j = 0;
	int i = 0;
	for (; l[i] != '\0'; i++) {
		if (l[i] == ' ') {
			string s = l.substr(j, i - j);
			j = i + 1;
			DodajElemListe(ls, s);
		}
	}
	string s = l.substr(j, i - j);
	DodajElemListe(ls, s);
	Stek* stek = nullptr;
	Lista* pom = ls;
	Stablo* st = stablo;
	while (pom != nullptr) {
		while (pom->s != st->s) {
			st = st->brat;
			if (st == nullptr) {
				while (stek != nullptr) {
					POP(stek);
				}
				Ocisti(ls);
				cout << "Nema takvog steka unutar stabla" << endl;
				return;
			}
		}
		stek = PUSH(stek, st);
		st = st->sin;
		pom = pom->sled;
		if (st == nullptr && pom != nullptr) {
			while (stek != nullptr) {
				POP(stek);
			}
			Ocisti(ls);
			cout << "Nema takvog steka unutar stabla" << endl;
			return;
		}
	}

	Stablo* a = POP(stek);
	while (stek != nullptr && a->sin == nullptr) {
		Stablo* b = POP(stek);
		if (b->sin == a) {
			b->sin = a->brat;
			delete a;
		}
		else {
			Stablo* p = b->sin;
			while (p->brat != a) {
				p = p->brat;
			}
			p->brat = a->brat;
			delete a;
		}
		a = b;
	}

	if (stablo->sin == nullptr && stablo->brat== nullptr) {
		Stablo* st = stablo;
		stablo = nullptr;
		delete st;
	}

	while (stek != nullptr) {
		POP(stek);
	}
	Ocisti(ls);
}

void Ispisi(Stablo* stablo) {
	if (stablo == nullptr) {
		cout << endl;
		return;
	}
	Stek* stek = nullptr;
	stek = PUSH(stek, stablo);
	while (stek != nullptr) {
		Stablo* pom = POP(stek);
		cout << pom->s << " ";
		if (pom->brat != nullptr) {
			stek = PUSH(stek, pom->brat);
		}
		while (pom->sin != nullptr) {
			pom = pom->sin;
			cout << pom->s << " ";
			if (pom->brat != nullptr) {
				stek = PUSH(stek, pom->brat);
			}
		}
	}
	cout << endl;
}

void Obrisi(Stablo*& stablo) {
	if (stablo == nullptr) {
		return;
	}
	Stek* stek = nullptr;
	stek = PUSH(stek, stablo);
	while (stek != nullptr) {
		Stablo* pom = POP(stek);
		if (pom->brat != nullptr) {
			stek = PUSH(stek, pom->brat);
		}
		while (pom->sin != nullptr) {
			Stablo* d = pom;
			pom = pom->sin;
			delete d;
			if (pom->brat != nullptr) {
				stek = PUSH(stek, pom->brat);
			}

		}
		delete pom;
	}
	stablo = nullptr;
}

bool uListi(Lista* ls, string s) {
	while (ls != nullptr) {
		if (ls->s == s) {
			return true;
		}
		ls = ls->sled;
	}
	return false;
}

int PozListe(Lista* ls, string s) {
	int r = 0;
	while (ls != nullptr) {
		if (ls->s == s) {
			return r;
		}
		r++;
		ls = ls->sled;
	}
	return -1;
}

string ElemNaPozicijiListe(Lista* ls, int poz) {
	Lista* pom = ls;
	for (int i = 0; i < poz; i++) {
		if (pom != nullptr) {
			pom = pom->sled;
		}
		else {
			return "";
		}
	}
	return pom->s;
}

void ObradiCvor(bool**& r,Stablo* pom,Lista* ls) {
	int poz = PozListe(ls, pom->s);
	pom = pom->sin;
	while (pom != nullptr) {
		r[poz][PozListe(ls, pom->s)] = true;
		pom = pom->brat;
	}
}

void IspisiMatricu(Graf* g) {
	if (g == nullptr) {
		return;
	}
	bool** a = g->matrica;
	Lista* pom = g->ls;
	if (a == nullptr) {
		cout << endl;
		return;
	}
	int dim = 0;
	while (pom != nullptr) {
		dim++;
		pom = pom->sled;
	}
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
}

Graf* StabloUGraf(Stablo* stablo) {
	Graf* g;
	bool** r;
	Lista* ls = nullptr;
	if (stablo == nullptr) {
		return nullptr;
	}
	Stek* stek = nullptr;
	stek = PUSH(stek, stablo);
	while (stek != nullptr) {
		Stablo* pom = POP(stek);
		if (!uListi(ls, pom->s)) {
			DodajElemListe(ls, pom->s);
		}
		if (pom->brat != nullptr) {
			stek = PUSH(stek, pom->brat);
		}
		while (pom->sin != nullptr) {
			pom = pom->sin;
			if (!uListi(ls, pom->s)) {
				DodajElemListe(ls, pom->s);
			}
			if (pom->brat != nullptr) {
				stek = PUSH(stek, pom->brat);
			}
		}
	}
	Lista* p = ls;
	int br = 0;
	while (p != nullptr) {
		p = p->sled;
		br++;
	}
	if (br > 0) {
		r = new bool* [br];
		for (int i = 0; i < br; i++) {
			r[i] = new bool[br];
		}
	}
	else {
		return nullptr;
	}
	for (int i = 0; i < br; i++) {
		for (int j = 0; j < br; j++) {
			r[i][j] = false;
		}
	}
	stek = PUSH(stek, stablo);
	while (stek != nullptr) {
		Stablo* pom = POP(stek);
		ObradiCvor(r, pom, ls);
		if (pom->brat != nullptr) {
			stek = PUSH(stek, pom->brat);
		}
		while (pom->sin != nullptr) {
			pom = pom->sin;
			ObradiCvor(r, pom, ls);
			if (pom->brat != nullptr) {
				stek = PUSH(stek, pom->brat);
			}
		}
	}
	g = new Graf(r, ls);
	//IspisiMatricu(g);
	return g;
}

void Ispisi(Graf* g) {
	if (g == nullptr) {
		return;
	}
	Lista* ls = g->ls;
	bool** a = g->matrica;
	int dim = 0;
	while (ls != nullptr) {
		dim++;
		ls = ls->sled;
	}
	ls = g->ls;
	bool* visit = new bool[dim];
	for (int i = 0; i < dim; i++) {
		visit[i] = false;
	}
	string p = ls->s;
	cout << p << " ";
	visit[0] = true;
	Lista* red = new Lista(p);
	while (red != nullptr) {
		p = red->s;
		Lista* pom = red;
		red = red->sled;
		delete pom;
		int poz = PozListe(ls, p);
		for (int i = 0; i < dim; i++) {
			if (a[poz][i]) {
				if (!visit[i]) {
					visit[i] = true;
					DodajElemListe(red, ElemNaPozicijiListe(ls, i));
					cout << ElemNaPozicijiListe(ls,i) <<" ";
				}
			}
		}
	}
	delete[] visit;
}

bool Rekurzija(Graf* g) {
	if (g == nullptr) {
		cout << "nije rekurzija" << endl;
		return false;
	}
	bool** a = g->matrica;
	Lista* ls = g->ls;
	int dim = 0;
	while (ls != nullptr) {
		dim++;
		ls = ls->sled;
	}
	ls = g->ls;
	for (int i = 0; i < dim; i++) {
		if (a[i][i]) {
			cout << "direktna rekurzija" << endl;
			return true;
		}
	}
	for (int k = 0; k < dim; k++) {
		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
				a[i][j] = a[i][j] || (a[i][k] && a[k][j]);
			}
		}
	}
	for (int i = 0; i < dim; i++) {
		if (a[i][i]) {
			cout << "indirektna rekurzija" << endl;
			return true;
		}
	}
	cout << "nije rekurzija" << endl;
	return false;
}

void Ocisti(Graf*& graf) {
	int dim = 0;
	if (graf == nullptr) {
		return;
	}
	Lista* l = graf->ls;
	Lista* pom = l;
	while (pom != nullptr) {
		dim++;
		l = pom;
		pom = pom->sled;
		delete l;
	}
	for (int i = 0; i < dim; i++) {
		delete[] graf->matrica[i];	
	}
	delete[] graf->matrica;
	graf = nullptr;
}

int main()
{
	Stablo* stablo = nullptr;
	bool** matr = nullptr;
	Lista* ls = nullptr;
	Graf* graf = nullptr;
	while (true) {
		cout << "Unesti broj od 0 do 9 u zavisnosti od instrukcije koju zelite da izvrstie:" << endl;
		cout << "0 Prekid programa" << endl;
		cout << "1 Ucitavanje skupa programskih stekova iz tekstualnog fajla zadatog formata i formiranje stabla" << endl;
		cout << "2 Dodavanje novog programskog steka u stablo" << endl;
		cout << "3 Uklanjanje programskog steka iz stabla" << endl;
		cout << "4 Ispis stabla" << endl;
		cout << "5 Brisanje stabla iz memorije" << endl;
		cout << "6 Konverziju formiranog stabla u graf" << endl;
		cout << "7 Ispis grafa na pogodan nacin" << endl;
		cout << "8 Detekciju rekurzivnih poziva funkcija." << endl;
		int a;
		cin >> a;
		switch (a)
		{
		case 1: Ucitaj(stablo); break;
		case 2: Dodaj(stablo); break;
		case 3: Ukloni(stablo); break;
		case 4: Ispisi(stablo); break;
		case 5: Obrisi(stablo); break;
		case 6:	graf = StabloUGraf(stablo); break;
		case 7: Ispisi(graf); break; 
		case 8: Rekurzija(graf); break;
		case 0: Obrisi(stablo); Ocisti(graf); exit(0);
		default: cout << endl << "Uneli ste neposojecu opciju probajte ponovo:" << endl; break;
		}
		cout << endl;
	}
}