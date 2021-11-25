#include <iostream>
#include <stack>
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;

class Calculator {//model�� result�� ���� Ŭ�������� ���� ���� �������.
	protected:
	public:
		stack<int> cstk;
		static int result;
		static Calculator* model;
		int Calc(vector<char> vec) { return 1; };
		void get_inputline(string str) {};
		string line;
		void clearcalc() {
			exit(1);
		}



};

class Inf_to_pof:Calculator {// �Էµ� ���� ����ǥ��� �ٲٴ� Ŭ����
private:
	int col_count = 0;
	stack<char> stk;// ������ ����
	stack<int> stkp; //stk���ÿ� �ִ� �켱����
public:
	vector<char> posfix_arr;//����ǥ��� �ٲ��� ��, '|'�� �پ��ִ� ���ڵ��� �����ϱ� ���� �ִ�.
	void priority(string::iterator iter, int p) {//�������� �켱������ ���� ���� �۾��� �̷���
		try {
			if (p == 0) {//p == 0 �̶� ���� *�� iter')'�� ��
				while (!stk.empty()) {
					if (stk.top() != '(') {
						posfix_arr.push_back(stk.top()); stk.pop(); stkp.pop();
					}
					else {
						stk.pop(); stkp.pop();
						return;
					}
				}
				cout << "'('�� ')'������ ���� �ʽ��ϴ�!" << endl;
				clearcalc();
			}
			if (stk.empty()) {
				stk.push(*iter);
				stkp.push(p);
				return;
			}
			else {
				if (stkp.top() < p) {
					stk.push(*iter); stkp.push(p);
				}
				else { posfix_arr.push_back(stk.top()); stk.pop(); stkp.pop(); stk.push(*iter); stkp.push(p); }
				return;
			}
			throw 1;
		}
			catch (int i) {
				cout << "���ð��� ������ �߻��߽��ϴ�!" << endl;
				clearcalc();
			}
	}


	vector<char> convert(string str) {//����ǥ��� �ٲٴ� �Լ�, posfix_arr�� �����Ѵ�.

		for (string::iterator iter = str.begin(); iter != str.end(); ++iter) {
			//cout << "iter: " << *iter << endl;
			if (!stk.empty()) {
				char a = stk.top();
			}
			if(isdigit(*iter) || *iter == 'x' || *iter == 'b' || ('A'<=(signed)*iter && (signed)*iter <='F')) {
				if (*iter == 'x' || *iter == 'b') {
					if (posfix_arr.back() != '0') {//x�� b�� �پ��� �� ����ǥ������ ��Ÿ���� Ȯ��
						cout << "�߸��� �Է��Դϴ� (0x, 0b������ �ƴ�)"<< endl;
						clearcalc();
					}
				}
				posfix_arr.push_back(*iter);


			}
			else if (!isdigit(*iter)) {
				if (*iter != ')' && isdigit(posfix_arr.back())) {
					posfix_arr.push_back('|');
				}
				else if (*iter != ')' && ('A' <= posfix_arr.back() || posfix_arr.back()<= 'F')) {
					posfix_arr.push_back('|');
				}// ���߿� ���ڸ� �̻��� �����Է��� �к��ϱ� ���� |�� ������
				switch (*iter) {//�����ڸ� �����ϰ� �׿� �켱������ �ο� �� �۾�
					case '+': this->priority(iter, 1); break;
					case '-': this->priority(iter, 1); break;

					case '*': this->priority(iter, 2); break;
					case '/': this->priority(iter, 2); break;

					case '(': col_count++; stk.push(*iter); stkp.push(0); break;
					case ')': col_count--; this->priority(iter, 0); break;

					default: cout << "�߸��� ���ڰ� �ֽ��ϴ�." << endl; clearcalc();
				}
			}
		}
		while (!stk.empty()) {
			posfix_arr.push_back(stk.top());
			stk.pop(); stkp.pop();
		}
		if (col_count != 0) {
			cout << "'('�� ')'���� " << col_count << "�� �� �����ϴ�!" << endl;
			clearcalc();
		}
		vector<char> rtv = posfix_arr;
		posfix_arr.clear();
		return rtv;//����ǥ��� �ٲ� ���͸� ����
	}

};

class Displayed : Calculator{//MVC�� V
private:

public:
	void viewpostfix(vector<char> vec) {//����ǥ��� ���� ǥ��
		cout << endl;
		for (int i = 0; i < vec.size(); i++) {
			cout<< vec[i];
		}
		cout << endl;
	}
	void viewresult(int result) {//������� ǥ��
		cout << endl<<result<<endl;
	}
};



class Calcmodel:public Calculator{//MVC�� model
private:
	stack<int> cstk;//����ǥ��� ���� ����Ҷ� ���� ������ �� ���̴� ����
	friend class Control;
	//static Calcmodel* model;//�̱��� ����
	Calcmodel(string str) {//���� ������� �̿��ؼ� ���� �����, 
		//�ٸ� ���ڰ� ���� �ԷµǸ� ���� ����� �������.
		if (isdigit(str[0])) {
			line = str;
		}
		else {
			cout << "�߸��� �Է��Դϴ�." << endl;
			clearcalc();
		}
	}
	string s;//char������ �ѱ��ھ� ����Ǿ��ִ� ���ڵ��� ��� ���ڸ��� �̻��� �ǰ� �Ѵ�
public:
	~Calcmodel() {}
	string line;//���������� ������ �� 
	static Calculator* getstr(string str) {//�̱��� ����
		if (model==nullptr){
			model = new Calcmodel(str);
		}
		return model;
	}
	
	void get_inputline(string str) {//ù ��� �� �����Է��� ���� �� ���̴� �Լ�
		if (isdigit(str.at(0))) {
			result = 0;
			line = str;
		}
		else {
			line = to_string(result) + str;
		}
	}

	int hexconv(vector<char> vec, int i) {//16������ 10������ ��ȯ�����ش�. ���� �� ���������� vec��ġ�ε����� �����ؼ� ���� Calc�Լ����� �޶��� i�� �����Ѵ�
		int num = 0;
		s = "";
		for (int j = i+1; j<vec.size(); j++) {
			if (isdigit(vec[j])) {
				s += vec[j];
			}
			else if(vec[j] >= 'A' && vec[j] <= 'F') {
				s += vec[j];
				
			}
			else {
				for (int k = 0; k < s.size(); k++) {
					if (s.at(k) >= 'A' && s.at(k) <= 'F') {
						num = num * 16 + s.at(k) - 'A' + 10;
					}
					else if (s.at(k) >= '0' && s.at(k) <= '9') {
						num = num * 16 + s.at(k) - '0';
					}
				}
				s = "";
				cstk.push(num);
				return j - 1;//���� �� �� Calc�Լ� ���� i�� �� ���ϰ��� �ȴ�, �� 16���������� ���� ���¿��� �����Ѵ�.
			}
		}
	}
	int biconv(vector<char> vec, int i) {//2������ 10������ ��ȯ�����ش�. ���� �� ���������� vec��ġ�ε����� �����ؼ� ���� Calc�Լ����� �޶��� i�� �����Ѵ�
		int num = 0;

		for (int j = i + 1; j < vec.size(); j++) {//���� �����ϴ� index�� ��� �����Ѵ�
			if (vec[j] == '0' || vec[j] == '1') {
				s += vec[j];
			}
			else if (isdigit(vec[j])) {
				cout << "0�� 1 ���� ���ڰ� ����!!!" << endl;
				//clearcalc();
			}
			else {
				for (int k = 0; k < s.size(); k++) {
					num = num + (s.at(k) - '0') * pow(2, (s.size() - k - 1));
				}
				s = "";
				cstk.push(num);
				return j - 1;//16������ �Լ��� ���ϰ��� �����̶� ����.
			}
		}
	}
	int Calc(vector<char> vec) {//����ϴ� �Լ�
			string s = "";
			int a;//����Ҷ� �ӽ÷� �ǿ����ڸ� �����Ѵ�.
			for (int i = 0; i < vec.size(); i++) {
				if (isdigit(vec[i])) {
					s += vec[i];
					//cout <<endl<< s;
				}
				else if (vec[i] == 'x') {
					s = "";
					i = hexconv(vec, i);
				}
				else if (vec[i] == 'b') {
					s = "";
					i = biconv(vec, i);
				}
				else if (vec[i] == '|') {
					if (s == "") {
						continue;
					}
					cstk.push(stoi(s));
					//string�� int�� ��ȯ �� ���ÿ� �ֱ�
					s = "";
				}
				else {
					if (s != "") {//���ڰ� �����Ŵ� ������� ���� ���ÿ� push�Ѵ�.
						cstk.push(stoi(s));
						s = "";
					}


						if (cstk.size() <2){ cout << "�����ڰ� �������� ����ֽ��ϴ�." << endl; clearcalc(); }
						switch (vec[i]) {//�־��� �����ڿ� ���� ������ ž���� 1,2��°�� �ִ� �������� �����Ѵ� 
						case '+': a = cstk.top(); cstk.pop(); a += cstk.top(); cstk.pop(); cstk.push(a); break;
						case '-': a = cstk.top(); cstk.pop(); a = cstk.top() - a; cstk.pop(); cstk.push(a); break;
						case '*': a = cstk.top(); cstk.pop(); a *= cstk.top(); cstk.pop(); cstk.push(a); break;
						case '/': a = cstk.top(); cstk.pop(); a = cstk.top() / a; cstk.pop(); cstk.push(a); break;
						}


				}
			}
			a = cstk.top();
			cstk.pop();
			return a;//���� ���� int�� ��ȯ�Ѵ�.
	}
};
class Control:Calculator {//MVC�� control
private:
	Calculator* model;//�̱������μ� �� ��ü�� ��� ���� ����
	Displayed *view;//Displayed Ŭ������ �����ϱ� ���� ��ü
	Inf_to_pof converter;//Inf_to_of �� ����ǥ�� ��ȯ�⿡ �����ϱ� ���� ����
public:
	Control(Calculator *model, Displayed *view) {//������, model�� view�� ��ü�� �޴´�.
		this->model = model;
		this->view = view;
	}

	void start() {//�Է� ���� �� ���⸦ �۵���Ű�� �Լ�
		vector<char> povec = converter.convert(((Calcmodel*)model)->line);//�ٿ�ĳ�������� �Լ� ����
		view->viewpostfix(povec);
		result = ((Calcmodel*)model)->Calc(povec);// �Ͻ��� �ٿ�ĳ�������� �Լ� ����
		view->viewresult(result);
		povec.clear();
		return;
		//view->viewresult(povec);

	}
};
Calculator* Calculator::model;
int Calculator::result;
int main() {
	string str;
	getline(cin, str);
	Calculator* model = Calcmodel::getstr(str) ;

	Displayed* view = new Displayed;

	Control* control = new Control(model, view);

	control->start();
	while (model->line != "ac") {//ac�Է��ϸ� ����
		getline(cin, str);
		((Calcmodel*)model)->get_inputline(str);//�ٿ�ĳ������ ���� �Լ� ����
		control->start();
	}
}