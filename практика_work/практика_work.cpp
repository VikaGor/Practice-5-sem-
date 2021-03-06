// практика_work.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <locale.h>
#include <stdio.h>
#include <conio.h>
using namespace std;

void Show(float **a, float *y, int n)
{
	cout << "  _\n";
	for (int i = 0; i<n; i++) {
		cout << " |";
		for (int j = 0; j<n; j++) {
			printf("%6.10f*x%d", *(*(a + i) + j), j + 1);
			if (j<n - 1)
				cout << " + ";
		}
		printf(" = %6.3f\n", *(y + i));
	}
	cout << "  -";
	return;
}

float *MethodOfGauss(float **a, float *y, int n)//решение СЛАУ методом Гаусса
{
	float *x = new float[n], max;
	int k = 0, m;
	while (k<n) {
		max = abs(*(*(a + k) + k));
		m = k;
		for (int i = k + 1; i<n; i++) {
			if (abs(*(*(a + i) + k))>max) {
				max = abs(*(*(a + i) + k));
				m = i;
			}
		}
		if (max == 0) {
			cout << "\nРешение получить невозможно";
			system("pause");
			return 0;
		}

		for (int j = 0; j<n; j++) {
			float temp = *(*(a + k) + j);
			*(*(a + k) + j) = *(*(a + m) + j);
			*(*(a + m) + j) = temp;
		}

		float temp = *(y + k);
		*(y + k) = *(y + m);
		*(y + m) = temp;

		for (int i = k; i<n; i++) {
			float temp = *(*(a + i) + k);
			if (temp != 0) {
				for (int j = 0; j<n; j++)
					*(*(a + i) + j) = *(*(a + i) + j) / temp;
				*(y + i) = *(y + i) / temp;
			}
			if ((i != k) && (temp != 0)) {
				for (int j = 0; j<n; j++)
					*(*(a + i) + j) = *(*(a + i) + j) - *(*(a + k) + j);
				*(y + i) = *(y + i) - *(y + k);
			}
		}
		k++;
		cout << "\nИттерация " << k << ":";
		cout << "\n///////////////////////////////////////////////////////\n";
		Show(a, y, n);
		cout << "\n///////////////////////////////////////////////////////\n";
	}

	for (k = n - 1; k >= 0; k--) {
		*(x + k) = *(y + k);
		for (int i = 0; i<k; i++)
			*(y + i) = *(y + i) - (*(*(a + i) + k)*(*(x + k)));
	}
	return x;
}

class Vect
{
protected:
	int size;
	float *E;
public:

	Vect(){
		size = 0;
		E = NULL;
	}
	Vect(const Vect &r) :size(r.size){
		E = new float[r.size];
		for (int i = 0; i < r.size; i++)
			E[i] = r.E[i];
	}

	Vect(int n, float a, float b, float P, float h) :size(n) //расставляем точки
	{
		E = new float[n];
		float eps = 0.001;
		E[0] = eps;
		float p = E[0];
		for (int i = 1; i < n; i++){
			E[i] = p + h;
			if (E[i] == a && E[i] == (a + b) && E[i] == (2 * a + b)){
				E[i] = E[i] + eps;
				cout << "СМЕЩЕНИЕ:   ";
				printf("%3.3f\t", E[i]);
			}
			p = E[i];
		}
	}
	
	~Vect() { delete E; }//деструктор

	float &operator[](int ind) const //оператор индексирования [] 
	{
		float k = 0;
		if (ind > size) return k;
		return E[ind];
	}
	void print() {
		for (int i = 0; i < size; i++)
			printf("%3.3f\t", E[i]);
		printf("\n");
	}
	int sizevect() //возвращает размер вектора (количество точек E)
	{
		return size;
	}
	friend ostream &operator<<(ostream &out, Vect el);
};

ostream &operator<<(ostream &out, Vect el) //операция печати вектора 
{
	for (int i = 0; i < el.size; i++)
		out << el.E[i] << "     ";
	return out;
}

int main()
{
	setlocale(0, "rus");
	srand((unsigned)time(NULL));
	float a, b, n, P, k = 1;
	float h;
	float *E_x, *E_y, *N_x, *N_y;
	const double PI = 3.1415926535897932384626433832795;
	cout << "Введите количество точек E: ";
	cin >> n;
	E_x = new float[n];
	E_y = new float[n];
	N_x = new float[n];
	N_y = new float[n];
	cout << "Введите ширину прямоугольника а= ";
	cin >> a;
	cout << "Введите длину прямоугольника b= ";
	cin >> b;
	P = 2 * (a + b);
	cout << "Периметр прямоугольника равен P=2*(a+b)=" << "2" << "*" << "(" << a << "+" << b << ")=" << P << endl;
	h = P / n;
	cout << "Шаг разбиения h=P/n=" << P << "/" << n << "=" << h << endl;
	Vect V1(n, a, b, P, h);
	cout << endl << endl;
	//ищем координаты расставленных точек
	for (int i = 0; i < n; i++) {
		if (V1[i] < a) {
			E_x[i] = 0;
			E_y[i] = V1[i];
			N_x[i] = 1;
			N_y[i] = 0;
		}
		if ((V1[i] > a) && (V1[i] < (a + b))) {
			E_x[i] = V1[i] - a;
			E_y[i] = a;
			N_x[i] = 0;
			N_y[i] = -1;
		}
		if ((V1[i] > (a + b)) && (V1[i] < (2 * a + b))) {
			E_x[i] = b;
			E_y[i] = (2 * a + b) - V1[i];
			N_x[i] = -1;
			N_y[i] = 0;
		}
		if ((V1[i] > (2 * a + b)) && (V1[i] < P)) {
			E_x[i] = P - V1[i];
			E_y[i] = 0;
			N_x[i] = 0;
			N_y[i] = 1;
		}
	}
	for (int i = 0; i < n; i++) {
		//cout << "E[" << i + 1 << "]=" << V1[i];
		//printf("\t(%3.3f; %3.3f)\n", E_x[i], E_y[i]);
		printf("E=(%3.3f; %3.3f), n =(%3.3f;%3.3f)\n", E_x[i], E_y[i], N_x[i], N_y[i]);
	}
	cout << endl;
	float cosnx = 0;
	float cosne = 0;
	float G = 0;
	//вектор b
	float b_x = 0;
	float b_y = 0;
	//переменные для слау
	float **slau = new float*[n], *y = new float[n], *x;
	for (int i = 0; i < n; i++) {
		*(slau + i) = new float[n];
	}
	//получение нормали 
	for (int i = 0; i < n; i++) {
		//по i - цикл для фиксированных точек
		//по j - цикл для временных точек
		for (int j = 0; j < n; j++) {
			if (i != j) {
				b_x = E_x[j] - E_x[i]; cout << "b_x[" << j + 1 << "]=" << b_x;
				b_y = E_y[j] - E_y[i]; cout << "\tb_y[" << j + 1 << "]=" << b_y;
				cosne = fabs(((b_x * N_x[j] + b_y * N_y[j]) / (sqrt(b_x*b_x + b_y * b_y))));
				cosnx = (b_x * N_x[i] + b_y * N_y[i]) / (sqrt(b_x*b_x + b_y * b_y));
				G = (1 / (PI * sqrt(b_x * b_x + b_y * b_y))) * cosne * cosnx; //расчет ядра
				printf("\t(%3.3f;%3.3f)\n", cosnx, cosne);
			}
			//заполнение матрицы СЛАУ
			if (i==j) 
				slau[i][j] = 1;
			else {
				if ((j!=n-1)||(j!=0))
					slau[i][j] = -k * h * G;
				else
					if((j==0)|| (j == n-1))
						slau[i][j] = -k * (h / 2) * G;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		y[i] = 5;
	}
	system("pause");
	system("cls");
	cout << "Проверка на правильность составления матрицы\n\n";
	float sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%3.3f\t", slau[i][j]);
		}
		cout << endl;
	}
	cout << endl;
	for (int j = 0; j < n; j++){
		for (int i = 0; i < n; i++){
			sum = sum + slau[i][j];
		}
		cout << "sum[" << j + 1 << "]=" << sum << endl;
		sum = 0;
	}
	system("pause");
	system("cls");
	Show(slau, y, n);
	system("cls");
	puts("\n\nПроцесс работы:\n");
	y = MethodOfGauss(slau, y, n);
	system("pause");
	system("cls");
	cout << "Ответ:\n\n";
	cout << "q(x):" << endl;
	for (int i = 0; i<n; i++)
		printf("%6.4lf\n", *(y + i));
	_getch();
	return 0;
}


