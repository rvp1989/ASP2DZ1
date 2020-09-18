#include <iostream>
#include <stack>
#include <queue>
#include <string> 
using namespace std;

//ispitivao sam po level orderu
struct Cvor {

	Cvor(int k) {
		kljuc = k;

		levi = NULL;
		desni = NULL;
		br = 0;
		visina_podstabala = 0;//za avl

	}
	Cvor* levi;
	Cvor* desni;
	int kljuc;
	int br; //zbog lepseg ispisa
	int visina_podstabala;

};
//int ni - povecana tabela,ne gledamo prividne kljuceve
Cvor* napravi_stablo(int* ni, int velicina, int* v, int br) {
	int* niz = new int[br] {0};
	int j = 0;
	for (int i = 0; i < velicina; i++) { //formiramo niz od validnih kljuceva povecane tabele
		if (v[i] == 1)
			niz[j++] = ni[i];

	}
	//koristio sam 3 steka posto ne smemo da koristimo rekurziju
	Cvor* koren = new Cvor(0);
	stack<Cvor*> cvorovi;
	stack<int> levi_indeksi;
	stack<int> desni_indeksi;
	levi_indeksi.push(0);
	desni_indeksi.push(br - 1);
	cvorovi.push(koren);
	while (!cvorovi.empty()) {
		Cvor* trenutni = cvorovi.top();//top da vratim vrednost sa steka a pop da ga uklonim

		int levi = levi_indeksi.top();//skidamo levi sa steka
		int desni = desni_indeksi.top();//skidamo desni sa steka
		int sredina = levi + (desni - levi) / 2;

		trenutni->kljuc = niz[sredina]; //ubacivanje kljuca sa sredine u stablo

		cvorovi.pop();
		levi_indeksi.pop();
		desni_indeksi.pop();


		if (levi <= sredina - 1) {
			trenutni->levi = new Cvor(0);


			cvorovi.push(trenutni->levi); //levo podstablo dobija donju polovinu niza
			levi_indeksi.push(levi);
			desni_indeksi.push(sredina - 1);
		}
		if (sredina + 1 <= desni) {
			trenutni->desni = new Cvor(0);


			cvorovi.push(trenutni->desni); //desno dobija gornju polovinu
			levi_indeksi.push(sredina + 1);
			desni_indeksi.push(desni);
		}

	}
	return koren;
}
//pretraga - dok ne dodjemo do null
Cvor* pretraga(int kljuc, Cvor* koren) {

	Cvor* trenutni = koren;
	while (trenutni != NULL) {
		if (trenutni->kljuc == kljuc) return trenutni;
		if (trenutni->kljuc > kljuc) trenutni = trenutni->levi;//ukoliko je kljuc koji trazimo vecin od tog onda nam treba levo podstablo
		else trenutni = trenutni->desni;//u suprotnom desno
	}
	cout << "Kljuc nije pronadjen!" << endl;
	return NULL;
}

//isto,trazim ga,moram da pratim predhodni jer on ce tacno da ide po stablu do mesta gde treba da se umetne dok ga trazi
void umetni(int kljuc, Cvor* koren) {
	Cvor* trenutni = koren;
	Cvor* prethodni = koren;

	while (trenutni != NULL) {
		if (trenutni->kljuc == kljuc) {
			cout << "Kljuc vec postoji u stablu!" << endl;
			return;
		}
		if (trenutni->kljuc > kljuc) {

			prethodni = trenutni;
			trenutni = trenutni->levi;
		}
		else {
			prethodni = trenutni;
			trenutni = trenutni->desni;
		}
	}
	Cvor* novi = new Cvor(kljuc);//pravimo novi cvor vrednosti kljuca koji umecemo
	if (prethodni != nullptr && prethodni->kljuc > kljuc) {
		prethodni->levi = novi;
	}
	else {
		prethodni->desni = novi;
	}

}
//level order,ukolio ima levog sina da stavim zarez ispred njega a u koliko ima desnog zarez iza
void ispis(Cvor* koren) {

	if (koren == NULL) return;

	queue<Cvor*> red;

	red.push(koren);
	int br = 0;
	int br_stari = 0;
	int j = 20;
	while (red.empty() == false)
	{

		int brojac = red.size();

		for (int i = 0; i < j - brojac; i++) cout << " ";
		while (brojac > 0)
		{
			Cvor* cvor = red.front();
			for (int i = 0; i < cvor->br; i++) cout << " ";
			if (cvor->levi != NULL)cout << ",";	cout << cvor->kljuc; if (cvor->desni != NULL)cout << ",  "; else cout << "  ";
			red.pop();

			if (cvor->levi != NULL) {
				cvor->levi->br = br;

				if (br > 0) { br_stari += br; br = 0; } //brojaci razmaka

				red.push(cvor->levi);

			}
			else br++;

			if (cvor->desni != NULL) {
				cvor->desni->br = br;
				if (br > 0) { br_stari += br; br = 0; }

				red.push(cvor->desni);
			}
			else br++;

			brojac--; //brojac koliko u jednom redu ima elemenata
		}
		br = br_stari;
		br_stari = 0;
		cout << endl;

	}


}
//stavljam u red,pop je da ga ukloni sa steka
void brisanje(Cvor*& koren) {
	if (koren == NULL) return;

	queue<Cvor*> red;

	red.push(koren);


	while (red.empty() == false)
	{



		Cvor* cvor = red.front();
		red.pop();

		if (cvor->levi != NULL) {

			red.push(cvor->levi);

		}


		if (cvor->desni != NULL) {

			red.push(cvor->desni);
		}

		delete cvor;

	}
	koren = NULL;

}




bool da_li_je_AVL(Cvor* koren) {
	//obilazimo stablto po post-order, i za svaki cvor racunamo na kojoj je "visini"- cvor je na visini svog viseg sina +1 
	//ako u jednom trenutku otkrijemo da se visina sinova nekog cvora razlikuje za vise od 1, stablo nije AVL balansirano
	stack<Cvor*> cvorovi;
	Cvor* k = koren;
	if (koren == NULL)
		return false;

	do
	{

		while (k)
		{

			if (k->desni)
				cvorovi.push(k->desni);
			cvorovi.push(k);

			k = k->levi;
		}

		k = cvorovi.top();
		cvorovi.pop();


		if (!cvorovi.empty() && k->desni && cvorovi.top() == k->desni)
		{
			cvorovi.pop();
			cvorovi.push(k);
			k = k->desni;
		}
		else
		{
			if (k->desni == NULL && k->levi == NULL) k->visina_podstabala = 0;
			else if (k->desni && k->levi == NULL) k->visina_podstabala = k->desni->visina_podstabala + 1;
			else if (k->levi && k->desni == NULL) k->visina_podstabala = k->levi->visina_podstabala + 1;
			else {
				if (abs(k->levi->visina_podstabala - k->desni->visina_podstabala) > 1) return false;//provera da je avl
				else k->visina_podstabala = (k->levi->visina_podstabala > k->desni->visina_podstabala ? k->levi->visina_podstabala + 1 : k->desni->visina_podstabala + 1);

			}
			k = nullptr;
		}
	} while (!cvorovi.empty());
	return true;
}

int pretrazi(int kljuc, int*& v, int*& tabela, int& velicina) {

	int donja = 0, gornja = (velicina - 1);
	int sred = donja + (gornja - donja) * (kljuc - tabela[donja]) /
		(tabela[gornja] - tabela[donja]); //interpolaciona pretraga
	while (donja <= gornja && kljuc >= tabela[donja] && kljuc <= tabela[gornja])
	{
		if (donja == gornja)
		{
			if (tabela[donja] == kljuc)
				return donja;

		}

		sred = donja + (gornja - donja) * (kljuc - tabela[donja]) /
			(tabela[gornja] - tabela[donja]);


		if (tabela[sred] == kljuc)
		{
			return sred;

		}


		if (tabela[sred] < kljuc)
			donja = sred + 1;

		else
			gornja = sred - 1;
	}
	return sred;


}
int* kreiraj_tabelu(int broj, int faktor, int* niz, int* v) {

	int* tabela = new int[broj * faktor];

	int index = 0;
	int d = floor(broj * faktor / broj); //distanca izmedju kljuceva 

	for (int i = 0; i < broj; i++) {
		tabela[index] = niz[i]; //upisivanje kljuca
		v[index] = 1;
		for (int j = 0; j < d && j < broj * faktor; j++) {
			tabela[index + j] = niz[i]; //upisivanje prividnih kljuceva
		}

		index += d;
	}

	return tabela;


}
void brisanje(int kljuc, int* tabela, int*& v, int velicina, int& br) {
	int poz = pretrazi(kljuc, v, tabela, velicina); //pozicija na kojoj je kljuc ili na kojoj bi trebalo da bude
	if (v[poz] == 0) {
		while (v[poz] == 0 && tabela[poz] == kljuc) { // ukoliko smo nasli prividan kljuc, potrebno je naci pravi
			poz--;
		}
		if (v[poz] == 1 && tabela[poz] == kljuc) { v[poz] = 0; br--; cout << "Kljuc " << kljuc << " je obrisan!" << endl; }
		else { cout << "Kljuc ne postoji!" << endl; return; }
	}
	else {
		if (poz >= velicina) { cout << "Kljuc ne postoji!" << endl; return; }
		v[poz] = 0; br--; cout << "Kljuc " << kljuc << " je obrisan!" << endl;
	}


}
int pretrazivanje(int kljuc, int* tabela, int*& v, int velicina) {
	int poz = pretrazi(kljuc, v, tabela, velicina);
	if (v[poz] == 0) {
		while (v[poz] == 0 && tabela[poz] == kljuc) { // ukoliko smo nasli prividan kljuc, potrebno je naci pravi
			poz--;
		}
		if (v[poz] == 1 && tabela[poz] == kljuc) { return poz; }
		else { cout << "Kljuc ne postoji!"; return -1; }
	}
	else { if (poz >= velicina) { cout << "Kljuc ne postoji!"; return -1; } }
	return poz;

}


void pomeri(int kljuc, int*& tabela, int*& v, int& velicina, int poz, int k) {

	int i = k; //pozicija prvog narednog prividnog kljuca
	if (i > poz) {
		while (i > poz) { // pomeranje validnih kljuceva u levo
			tabela[i] = tabela[i - 1];
			v[i] = v[i - 1];
			i--;
		}
	}
	else {
		while (i < poz) { //pomeranje validnih kljuceva u desno
			tabela[i] = tabela[i + 1];
			v[i] = v[i + 1];
			i++;
		}

	}
	tabela[poz] = kljuc;
	v[poz] = 1;

}
void umetanje_novog_kljuca(int kljuc, int*& v, int*& tabela, int& velicina, int& br) {
	int sred = pretrazi(kljuc, v, tabela, velicina);
	if (v[sred] == 1 && tabela[sred] == kljuc) { cout << "Kljuc " << kljuc << " vec postoji!"; return; } //nasli smo validan kljuc
	if (v[sred] == 0) {
		int p = sred;
		while (v[p] == 0 && tabela[p] == kljuc) {
			p--;
		}
		if (v[p] == 1 && tabela[p] == kljuc) { cout << "Kljuc " << kljuc << " vec postoji!"; return; } //prvo smo nasli prividan a zatim i validan kljuc
		tabela[sred] = kljuc;
		br++;
		v[sred] = 1; //umetanje kljuca i postavljanje bita validnosti
		int j = sred + 1;
		while (v[j++] == 0) { //svi naredni prividni kljucevi dobijaju vrednost novog kljuca
			tabela[j] = kljuc;

		}
		j = sred - 1;
		while (v[j] == 0 && j > 0) { j--; }
		if (j > 0) {
			int prethodni = tabela[j];
			while (j < sred) { tabela[j] = prethodni; j++; } //a svi prethodni prividni kljucevi vrednost prvog prethodnog validnog kljuca
		}




	}
	else {


		if (br == velicina) { //tabela je puna, potrebno je udvostruciti njenu velicinu i ponovo rasporediti kljuceve

			int* nova_tabela = new int[velicina * 2]{ 0 };

			int index = 0;

			int d = floor(velicina * 2 / br); //distanca izmedju kljuceva 
			int* novi_v = new int[velicina * 2]{ 0 };
			for (int i = 0; i < velicina; i++) {
				if (v[i]) {
					nova_tabela[index] = tabela[i];
					novi_v[index] = 1;
					for (int j = 0; j < d && j < velicina * 2; j++) {
						nova_tabela[index + j] = tabela[i];
					}
					index += d;

				}

			}

			velicina = velicina * 2;
			tabela = new int[velicina];
			v = new int[velicina];
			for (int i = 0; i < velicina; i++) {
				if (nova_tabela[i] != 0)tabela[i] = nova_tabela[i];
				else tabela[i] = nova_tabela[i - 1];

				v[i] = novi_v[i];
			}
			int poz = pretrazi(kljuc, v, tabela, velicina); //trazenje pozicije u realociranoj tabeli
			if (v[poz] == 0) {
				tabela[poz] = kljuc;
				v[poz] = 1;
				br++; //umetanje i postavljanje bita validnosti 
				int j = poz + 1;
				while (v[j++] == 0) { //svi naredni prividni kljucevi dobijaju vrednost novog kljuca
					tabela[j] = kljuc;

				}
				j = poz - 1;
				while (v[j] == 0 && j > 0) { j--; }
				if (j > 0) {
					int prethodni = tabela[j];
					while (j < poz) { tabela[j] = prethodni; j++; } //a svi prethodni prividni kljucevi vrednost prvog prethodnog validnog kljuca
				}

			}
			else { //dobijena pozicija nije slobodna, potrebno je pomeranje validnih kljuceva
				if (poz >= velicina) poz = velicina - 1;
				if (poz < 0) poz = 0;
				int k = poz;
				if (tabela[poz] > kljuc) {
					while (v[k] == 1 && k < velicina) { k++; }
					if (k == velicina) {
						k--;
						while (v[k] == 1 && k >= 0) { k--; }
						poz--;
					}
				}
				else {
					while (v[k] == 1 && k >= 0) { k--; }
					if (k == 0) {

						while (v[k] == 1 && k < velicina) { k++; }
						poz++;
					}
				}
				pomeri(kljuc, tabela, v, velicina, poz, k); //kljuc bi trebalo da bude na poziciji k, a poz predstavlja poziciju prvog prividnog kljuca
				br++;
			}



		}
		else { //dobijena pozicija nije slobodna, potrebno je pomeranje validnih kljuceva
			if (sred >= velicina) sred = velicina - 1;
			if (sred < 0) sred = 0;
			int k = sred;

			if (tabela[sred] > kljuc) {
				while (v[k] == 1 && k < velicina) { k++; }
				if (k == velicina) {
					k--;
					while (v[k] == 1 && k >= 0) { k--; }
					sred--;
				}
			}
			else {
				while (v[k] == 1 && k >= 0) { k--; }
				if (k == 0) {

					while (v[k] == 1 && k < velicina) { k++; }
					sred++;
				}
			}
			pomeri(kljuc, tabela, v, velicina, sred, k); //kljuc bi trebalo da bude na poziciji k, a poz predstavlja poziciju prvog prividnog kljuca
			br++;

		}

	}



}
int* kreiraj(int& broj, int& velicina, int& faktor_uvecanja, int*& v) {


	cout << "Unesite velicinu inicijalnog skupa kljuceva:" << endl;
	cin >> broj;

	int* pomocni_niz = new int[broj];

	cout << "Unesite faktor uvecanja:" << endl;
	cin >> faktor_uvecanja;

	velicina = broj * faktor_uvecanja;

	v = new int[broj * faktor_uvecanja]{ 0 };//vektor validnosti

	for (int i = 0; i < broj; i++) { //unos kljuceva u pomocni niz zbog sortiranja
		int kljuc;
		cout << "Unesite kljuc:" << endl;
		cin >> kljuc;
		pomocni_niz[i] = kljuc;
	}

	for (int i = 0; i < broj - 1; i++) { //u slucaju da nam korisnik ne unosi kljuceve u rastucem poretku potrebno je sortiranje unesenih kljuceva
		int min = i;
		for (int j = i + 1; j < broj; j++)
			if (pomocni_niz[j] < pomocni_niz[min])
				min = j;
		int temp = pomocni_niz[i];
		pomocni_niz[i] = pomocni_niz[min];
		pomocni_niz[min] = temp;

	}

	int* tabela = kreiraj_tabelu(broj, faktor_uvecanja, pomocni_niz, v);

	return tabela;


}

int main()
{
	int broj = 0;
	int velicina = 0;
	int faktor_uvecanja = 0;

	int kljuc;
	int izbor;
	int* tabela = { 0 };
	int* v = { 0 };
	Cvor* koren = 0;
	bool kraj = false;
	while (!kraj) {
		cout << "Izaberite opciju: " << endl
			<< "1. Forimiranje povecane tabele" << endl
			<< "2. Umetanje novog kljuca" << endl
			<< "3. Pretraga kljuca" << endl
			<< "4. Uklanjanje postojeceg kljuca" << endl
			<< "5. Formiranje stabla binarne pretrage na osnovu validnih kljuceva " << endl
			<< "6. Pretraga stabla na zadatu vrednost kljuca" << endl
			<< "7. Umetanje novog kljuca u stablo" << endl
			<< "8. Ispis stabla " << endl
			<< "9. Brisanje stabla " << endl
			<< "10. Provera da li je stablo balansirano po AVL kriterijumu" << endl
			<< "11. Kraj rada" << endl;
		cin >> izbor;
		switch (izbor) {
		case 1: {
			tabela = kreiraj(broj, velicina, faktor_uvecanja, v);
		}break;
		case 2: {
			cout << "Unesite kljuc koji zelite da umetnete" << endl;
			cin >> kljuc;
			umetanje_novog_kljuca(kljuc, v, tabela, velicina, broj);
		}break;
		case 3: {
			cout << "Unesite kljuc koji pretrazujete" << endl;
			cin >> kljuc;
			int pozicija = pretrazivanje(kljuc, tabela, v, velicina);
			if (pozicija != -1) cout << "Kljuc " << kljuc << " je na poziciji " << pozicija << endl;
		} break;
		case 4: {
			cout << "Unesite kljuc koji uklanjate" << endl;
			cin >> kljuc;
			brisanje(kljuc, tabela, v, velicina, broj);

		} break;
		case 5: {
			koren = napravi_stablo(tabela, velicina, v, broj);

		}break;
		case 6: {
			cout << "Unesite kljuc koji pretrazujete" << endl;
			cin >> kljuc;
			Cvor* c = pretraga(kljuc, koren);
		} break;
		case 7: {
			cout << "Unesite kljuc koji zelite da umetnete u stablo" << endl;
			cin >> kljuc;
			umetni(kljuc, koren);
		} break;
		case 8: {
			ispis(koren);
		} break;
		case 9: {
			brisanje(koren);
		} break;
		case 10: {
			bool balansirano = da_li_je_AVL(koren);
			if (balansirano) cout << "Stablo je balansirano po AVL kriterijumu!" << endl;
			else cout << "Stablo nije balansirano po AVL kriterijumu" << endl;
		}break;
		case 11: {
			brisanje(koren);
			delete[] v;
			delete[] tabela;
			kraj = true;
		} break;
		}

	}
}
