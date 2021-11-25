#include <iostream>
#include <stack>
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;

class Calculator {//model과 result를 여러 클래스에서 쓰기 위해 만들었다.
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

class Inf_to_pof:Calculator {// 입력된 것을 후위표기로 바꾸는 클래스
private:
	int col_count = 0;
	stack<char> stk;// 연산자 스택
	stack<int> stkp; //stk스택에 있는 우선순위
public:
	vector<char> posfix_arr;//후위표기로 바꿔진 식, '|'은 붙어있는 숫자들을 구분하기 위해 있다.
	void priority(string::iterator iter, int p) {//연산자의 우선순위에 따라 스택 작업이 이뤄짐
		try {
			if (p == 0) {//p == 0 이란 말은 *가 iter')'란 뜻
				while (!stk.empty()) {
					if (stk.top() != '(') {
						posfix_arr.push_back(stk.top()); stk.pop(); stkp.pop();
					}
					else {
						stk.pop(); stkp.pop();
						return;
					}
				}
				cout << "'('와 ')'순서가 맞지 않습니다!" << endl;
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
				cout << "스택관련 오류가 발생했습니다!" << endl;
				clearcalc();
			}
	}


	vector<char> convert(string str) {//후위표기로 바꾸는 함수, posfix_arr에 저장한다.

		for (string::iterator iter = str.begin(); iter != str.end(); ++iter) {
			//cout << "iter: " << *iter << endl;
			if (!stk.empty()) {
				char a = stk.top();
			}
			if(isdigit(*iter) || *iter == 'x' || *iter == 'b' || ('A'<=(signed)*iter && (signed)*iter <='F')) {
				if (*iter == 'x' || *iter == 'b') {
					if (posfix_arr.back() != '0') {//x나 b가 붙었을 때 진수표기인지 오타인지 확인
						cout << "잘못된 입력입니다 (0x, 0b형식이 아님)"<< endl;
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
				}// 나중에 두자리 이상의 숫자입력을 분별하기 위해 |로 구분함
				switch (*iter) {//연산자를 감지하고 그에 우선순위를 부여 및 작업
					case '+': this->priority(iter, 1); break;
					case '-': this->priority(iter, 1); break;

					case '*': this->priority(iter, 2); break;
					case '/': this->priority(iter, 2); break;

					case '(': col_count++; stk.push(*iter); stkp.push(0); break;
					case ')': col_count--; this->priority(iter, 0); break;

					default: cout << "잘못된 문자가 있습니다." << endl; clearcalc();
				}
			}
		}
		while (!stk.empty()) {
			posfix_arr.push_back(stk.top());
			stk.pop(); stkp.pop();
		}
		if (col_count != 0) {
			cout << "'('가 ')'보다 " << col_count << "개 더 많습니다!" << endl;
			clearcalc();
		}
		vector<char> rtv = posfix_arr;
		posfix_arr.clear();
		return rtv;//후위표기로 바뀐 벡터를 리턴
	}

};

class Displayed : Calculator{//MVC의 V
private:

public:
	void viewpostfix(vector<char> vec) {//후위표기된 것을 표시
		cout << endl;
		for (int i = 0; i < vec.size(); i++) {
			cout<< vec[i];
		}
		cout << endl;
	}
	void viewresult(int result) {//결과값을 표시
		cout << endl<<result<<endl;
	}
};



class Calcmodel:public Calculator{//MVC의 model
private:
	stack<int> cstk;//후위표기된 식을 계산할때 값을 저장할 때 쓰이는 스택
	friend class Control;
	//static Calcmodel* model;//싱글턴 패턴
	Calcmodel(string str) {//전의 계산결과를 이용해서 수식 만들기, 
		//다만 숫자가 먼저 입력되면 전의 결과는 사라진다.
		if (isdigit(str[0])) {
			line = str;
		}
		else {
			cout << "잘못된 입력입니다." << endl;
			clearcalc();
		}
	}
	string s;//char형으로 한글자씩 저장되어있는 숫자들을 묶어서 두자리수 이상이 되게 한다
public:
	~Calcmodel() {}
	string line;//최종적으로 실행할 식 
	static Calculator* getstr(string str) {//싱글턴 패턴
		if (model==nullptr){
			model = new Calcmodel(str);
		}
		return model;
	}
	
	void get_inputline(string str) {//첫 계산 후 다음입력을 받을 떄 쓰이는 함수
		if (isdigit(str.at(0))) {
			result = 0;
			line = str;
		}
		else {
			line = to_string(result) + str;
		}
	}

	int hexconv(vector<char> vec, int i) {//16진수를 10진수로 변환시켜준다. 끝난 후 끝났을때의 vec위치인덱스를 리턴해서 기존 Calc함수에서 달라진 i로 진행한다
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
				return j - 1;//리턴 한 후 Calc함수 내의 i는 이 리턴값이 된다, 즉 16진수형식이 끝난 상태에서 시작한다.
			}
		}
	}
	int biconv(vector<char> vec, int i) {//2진수를 10진수로 변환시켜준다. 끝난 후 끝났을때의 vec위치인덱스를 리턴해서 기존 Calc함수에서 달라진 i로 진행한다
		int num = 0;

		for (int j = i + 1; j < vec.size(); j++) {//원래 진행하던 index를 계속 진행한다
			if (vec[j] == '0' || vec[j] == '1') {
				s += vec[j];
			}
			else if (isdigit(vec[j])) {
				cout << "0과 1 외의 숫자가 있음!!!" << endl;
				//clearcalc();
			}
			else {
				for (int k = 0; k < s.size(); k++) {
					num = num + (s.at(k) - '0') * pow(2, (s.size() - k - 1));
				}
				s = "";
				cstk.push(num);
				return j - 1;//16진수쪽 함수의 리턴값의 역할이랑 같다.
			}
		}
	}
	int Calc(vector<char> vec) {//계산하는 함수
			string s = "";
			int a;//계산할때 임시로 피연산자를 저장한다.
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
					//string을 int로 변환 후 스택에 넣기
					s = "";
				}
				else {
					if (s != "") {//숫자가 끝난거니 묶어놨던 것을 스택에 push한다.
						cstk.push(stoi(s));
						s = "";
					}


						if (cstk.size() <2){ cout << "연산자가 연속으로 들어있습니다." << endl; clearcalc(); }
						switch (vec[i]) {//주어진 연산자에 따라 스택의 탑부터 1,2번째에 있는 정수끼리 연산한다 
						case '+': a = cstk.top(); cstk.pop(); a += cstk.top(); cstk.pop(); cstk.push(a); break;
						case '-': a = cstk.top(); cstk.pop(); a = cstk.top() - a; cstk.pop(); cstk.push(a); break;
						case '*': a = cstk.top(); cstk.pop(); a *= cstk.top(); cstk.pop(); cstk.push(a); break;
						case '/': a = cstk.top(); cstk.pop(); a = cstk.top() / a; cstk.pop(); cstk.push(a); break;
						}


				}
			}
			a = cstk.top();
			cstk.pop();
			return a;//계산된 값을 int로 반환한다.
	}
};
class Control:Calculator {//MVC의 control
private:
	Calculator* model;//싱글턴으로서 이 객체로 계속 굴릴 예정
	Displayed *view;//Displayed 클래스에 접근하기 위한 객체
	Inf_to_pof converter;//Inf_to_of 의 후위표현 변환기에 접근하기 위한 변수
public:
	Control(Calculator *model, Displayed *view) {//생성자, model과 view를 객체로 받는다.
		this->model = model;
		this->view = view;
	}

	void start() {//입력 받은 후 계산기를 작동시키는 함수
		vector<char> povec = converter.convert(((Calcmodel*)model)->line);//다운캐스팅으로 함수 접근
		view->viewpostfix(povec);
		result = ((Calcmodel*)model)->Calc(povec);// 일시적 다운캐스팅으로 함수 접근
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
	while (model->line != "ac") {//ac입력하면 종료
		getline(cin, str);
		((Calcmodel*)model)->get_inputline(str);//다운캐스팅을 통한 함수 접근
		control->start();
	}
}