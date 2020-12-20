#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <cctype>
#include <regex>

using namespace std;

#define MENU "n: 다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료"
#define SECTIONLINE "-------------------------------------------------------------------------------------"

class myTextEditor
{
private:
	vector<string> MyWords;
	static myTextEditor* INSTANCE;

	int MAX_LINE_LEN = 20;
	int curPage = 1;
	int lastPage;
	int firstPage;
	int linecount;
	string consoleMessage = "";
	string inputcmd;

	myTextEditor()
	{
		MyWords = {};
	}
public:
	int minLine;
	int maxLine;
	string inputCmd() {
		return this->inputcmd; //입력받은 명령의 getter
	}
	string consoleMsg() {
		return this->consoleMessage; //상황별 consoleMessage getter
	}
	void setConsoleMsg(string newConsoleMsg) {
		this->consoleMessage = newConsoleMsg;
	}

	vector<string> myWords() //getter
	{
		return MyWords;
	}
	void setMyWords(vector<string>& newMyWords) //setter
	{
		MyWords = newMyWords;
	}

	static myTextEditor* instance()
	{
		if (!INSTANCE)
			INSTANCE = new myTextEditor();
		return INSTANCE;
	}
	//singleTon의 주체인 editor객체를 반환하는 instance()함수 

	void viewPage(string consoleMsg);
	void viewLine();
	void viewLine(int line);
	//콘솔에 화면을 출력할때 사용되는 함수들
	int wordInStr(string J, string S);
	//인자로 준 문자열내에 두번째 인자에 해당하는 문자의 개수를 반환
	//명령에서 ',' 를 확인하여 숫자를 반환하는데 사용.

	void commandManager(string cmd);
	void commandN();
	void commandP();
	void commandI(int LineNuber, int wordNumber, string word);
	void commandD(int LineNuber, int wordNumber);
	void commandC(string targetWorld, string newWord);
	void commandS(string targetWord);
	void commandT();
	//명령에 관련된 함수들

	void reset(string consoleMsg);
	//명령처리를 한 뒤 페이지를 리셋하는 용도
	void resetForCommandS(string consoleMsg, int minline);
	//명령처리를 한 뒤 페이지를 리셋하는 용도(command S 를 위해 사용.)
	vector<string> split(string input, char delimiter);
	//명령에서 필요한 부분만 뽑아 사용하기 위해 선언.
	void inputCommand();
	//입력을 위한 함수
	vector<string> divideTxtToEachLine(vector<string> base);
	//하나의 스트링을 75바이트씩 분할된 라인으로 저장한 벡터를 반환
};

myTextEditor* myTextEditor::INSTANCE = 0;

int myTextEditor::wordInStr(string J, string S) {
	int count = 0;
	for (unsigned int n = 0; n < J.size(); n++) {
		char temp = J[n];
		for (unsigned int i = 0; i < S.size(); i++) {
			if (temp == S[i]) {
				count++;
			}
		}
	}
	return count;
}



void myTextEditor::commandManager(string cmd) {
	int count = wordInStr(cmd, ",");
	int numOfexept1 = wordInStr(cmd, ")");
	int numOfexept2 = wordInStr(cmd, "(");

	vector<string> splitedCmd = split(cmd, '(');
	setConsoleMsg("");
	regex r("^[a-zA-Z0-9]*$");

	if (cmd == "N" || cmd == "n") {
		commandN();
	}
	else if (cmd == "P" || cmd == "p") {
		commandP();
	}
	else if (cmd == "T" || cmd == "t") {
		commandT();
	}
	else if (numOfexept1 == 0 || numOfexept2 == 0) {
		setConsoleMsg("명령 내에 공백이 있거나 명령이 잘못되었습니다. 다시 입력해주세요.");
		reset(consoleMessage);
	}
	else if (splitedCmd[0] == "I" || splitedCmd[0] == "i") {
		if (count == 2) {
			vector<string> splitedArgNum = split(splitedCmd[1], ',');
			vector<string> splitedArgStr = split(splitedArgNum[2], ')');

			bool check1 = regex_match(splitedArgNum[0], r);
			bool check2 = regex_match(splitedArgNum[1], r);
			if (!check1 || !check2) {
				setConsoleMsg("명령 인자의 양식을 맞춰서 입력해주세요. 특수문자는 안됩니다.");
				reset(consoleMessage);
			}
			else if (minLine > stoi(splitedArgNum[0]) || stoi(splitedArgNum[0]) > maxLine) {
				setConsoleMsg("현재 페이지에 해당 라인이 존재하지 않습니다. 확인 후 다시 입력하십시오.");
				reset(consoleMessage);
			}
			else {
				commandI(stoi(splitedArgNum[0]), stoi(splitedArgNum[1]), splitedArgStr[0]);
			}
		}
		else {
			setConsoleMsg("해당 명령의 인자의 수가 잘못되었습니다. 다시 제대로 입력해주십시오.");
			reset(consoleMessage);
		}
	}
	else if (splitedCmd[0] == "D" || splitedCmd[0] == "d") {
		if (count == 1) {
			vector<string> splitedArgNum1 = split(splitedCmd[1], ',');
			vector<string> splitedArgNum2 = split(splitedArgNum1[1], ')');

			bool check1 = regex_match(splitedArgNum1[0], r);
			bool check2 = regex_match(splitedArgNum2[0], r);
			if (!check1 || !check2) {
				setConsoleMsg("명령 인자의 양식을 맞춰서 입력해주세요. 특수문자는 안됩니다.");
				reset(consoleMessage);
			}
			else if (minLine > stoi(splitedArgNum1[0]) || stoi(splitedArgNum1[0]) > maxLine) {
				setConsoleMsg("현재 페이지에 해당 라인이 존재하지 않습니다. 확인 후 다시 입력하십시오.");
				reset(consoleMessage);
			}
			else {
				commandD(stoi(splitedArgNum1[0]), stoi(splitedArgNum2[0]));
			}
		}
		else {
			setConsoleMsg("해당 명령의 인자의 수가 잘못되었습니다. 다시 제대로 입력해주십시오.");
			reset(consoleMessage);
		}
	}
	else if (splitedCmd[0] == "S" || splitedCmd[0] == "s") {
		if (count < 2) {
			vector<string> splitedArgStr = split(splitedCmd[1], ')');
			commandS(splitedArgStr[0]);
		}
		else {
			setConsoleMsg("해당 명령의 인자의 수가 잘못되었습니다. 다시 제대로 입력해주십시오.");
			reset(consoleMessage);
		}
	}
	else if (splitedCmd[0] == "C" || splitedCmd[0] == "c") {
		if (count == 1) {
			vector<string> splitedArgStr1 = split(splitedCmd[1], ',');
			vector<string> splitedArgStr2 = split(splitedArgStr1[1], ')');

			bool check1 = regex_match(splitedArgStr1[0], r);
			bool check2 = regex_match(splitedArgStr2[0], r);
			if (!check1 || !check2) {
				setConsoleMsg("명령 인자의 양식을 맞춰서 입력해주세요. 특수문자는 안됩니다.");
				reset(consoleMessage);
			}
			else {
				commandC(splitedArgStr1[0], splitedArgStr2[0]);
			}
		}
		else {
			setConsoleMsg("해당 명령의 인자의 수가 잘못되었습니다. 다시 제대로 입력해주십시오.");
			reset(consoleMessage);
		}
	}
	else {
		setConsoleMsg("명령이 잘못되었습니다. 옵션을 다시 확인하신 후 입력하십시오.");
		reset(consoleMessage);
	}
}

void myTextEditor::inputCommand() {
	string cmd;
	cin >> cmd;
	this->inputcmd = cmd;
	cout << SECTIONLINE << endl;

}

void myTextEditor::viewPage(string consoleMsg) {
	cout << SECTIONLINE << endl;
	cout << MENU << endl;
	cout << SECTIONLINE << endl;
	cout << "(콘솔메시지)" << consoleMsg << endl;
	cout << SECTIONLINE << endl;
	myTextEditor::inputCommand();
};

void myTextEditor::viewLine() {

	linecount = myWords().size();

	minLine = MAX_LINE_LEN * (curPage - 1)+1;
	maxLine = (MAX_LINE_LEN * curPage);


	if (myWords().size() % 20 == 0) {
		lastPage = myWords().size() / 20;
	}
	else {
		lastPage = myWords().size() / 20 + 1;
	}

	if (curPage != lastPage) {
		for (int i = MAX_LINE_LEN * (curPage - 1); i < (MAX_LINE_LEN * curPage); i++) {
			if (i / 9 == 0) {
				cout << " " << i + 1 << "|" << " " << myWords()[i] << endl;
			}
			else {
				cout << i + 1 << "|" << " " << myWords()[i] << endl;
			}
		}
	}
	else {
		int indexFromMid = myWords().size() - 20;
		for (int i = indexFromMid; i < myWords().size(); i++) {
			if (i / 9 == 0) {
				cout << " " << i + 1 << "|" << " " << myWords()[i] << endl;
			}
			else {
				cout << i + 1 << "|" << " " << myWords()[i] << endl;
			}
		}
	}
};

void myTextEditor::viewLine(int line) {
	for (int i = line; i < line + 20; i++) {
		if (i / 9 == 0) {
			cout << " " << i + 1 << "|" << " " << myWords()[i] << endl;
		}
		else {
			cout << i + 1 << "|" << " " << myWords()[i] << endl;
		}
	}

}

void myTextEditor::reset(string consoleMsg) {
	consoleMsg = myTextEditor::consoleMsg();
	myTextEditor::instance()->viewLine();
	myTextEditor::instance()->viewPage(consoleMsg);
}

vector<string> myTextEditor::split(string input, char delimiter) {
	vector<string> answer;
	stringstream ss(input);
	string temp;

	while (getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}

vector<string> myTextEditor::divideTxtToEachLine(vector<string> base)
{
	string temp;
	vector<string> dividedTxt;

	for (int i = 0; i < (int)base.size(); i++) {
		temp = temp + base[i] + " ";

		if (temp.length() > 75) {
			if (temp.length() == 76 && temp[temp.length() - 1] == ' ') { //길이가 딱 맞을때 
				temp.erase(temp.length() - 1, 1);
				dividedTxt.push_back(temp);
				temp = "";

				continue;
			}

			int wordSize = base[i].length() + 1;
			temp.erase(temp.length() - wordSize, temp.length());
			dividedTxt.push_back(temp);

			temp = base[i] + " ";

		}
		if (i == base.size() - 1 && temp.length() <= 75) {
			dividedTxt.push_back(temp);
			temp = "";
		}
	}
	return dividedTxt;
}


//==============================명령 함수===============================
void myTextEditor::commandN() {
	if (curPage == (lastPage - 1)) {
		curPage++;
		setConsoleMsg("마지막 페이지 입니다.");
		reset(consoleMsg());
	}
	else if (curPage != lastPage) {
		curPage++;
		reset(consoleMsg());
	}
	else {
		setConsoleMsg("현재 마지막 페이지이기 때문에 더이상 넘어갈 수 없습니다.");
		reset(consoleMsg());
	}

};
void myTextEditor::commandP() {
	if (curPage == 1) {
		setConsoleMsg("첫번째 페이지입니다.");
		reset(consoleMsg());
	}
	else {
		curPage--;
		reset(consoleMsg());
	}
};

void myTextEditor::commandI(int LineNumber, int wordNumber, string word) {
	if (LineNumber > (MAX_LINE_LEN * curPage) || LineNumber <= (MAX_LINE_LEN * (curPage - 1)) || word.size() > 75) {
		setConsoleMsg("잘못된 입력입니다. 다시 입력하시오.");
		reset(consoleMessage);
	}
	int lineIndex = LineNumber - 1;
	int wordIndex = wordNumber - 1;
	word = ' ' + word + ' ';
	int count = 0;
	string buf = "";
	string newStr = "";
	string temp2;
	vector<string> newMyWords;
	for (vector<string>::iterator i = MyWords.begin(); i != MyWords.end(); i++) {
		if (count == lineIndex) {
			vector<string> temp = split(MyWords[count], ' ');
			int numOfWords = temp.size();
			if (numOfWords < wordNumber) {
				setConsoleMsg("두번째 인자가 너무 큽니다. 다시 입력하시오.");
				reset(consoleMessage);
			}
			temp[wordIndex].insert(0, word);
			for (int i = 0; i < temp.size(); i++) {
				buf = buf + ' ' + temp[i];
			}


			stringstream s(newStr);
			MyWords[count] = buf;
			for (int i = 0; i < MyWords.size(); i++) {
				newStr = newStr + ' ' + MyWords[i];
			}
			stringstream s2(newStr);
			while (s2 >> temp2) {
				newMyWords.push_back(temp2);
			}
			vector<string> divided = myTextEditor::instance()->divideTxtToEachLine(newMyWords);
			MyWords = divided;
			break;
		}
		else {
			count++;
		}
	}
	setConsoleMsg("해당 문자 : " + word + " 이(가) 삽입되었습니다.");
	reset(consoleMessage);
};

void myTextEditor::commandD(int LineNumber, int wordNumber) {
	int lineIndex = LineNumber - 1;
	int wordIndex = wordNumber - 1;
	int count = 0;
	string buf = "";
	string newStr = "";
	string temp2;
	vector<string> newMyWords;

	for (vector<string>::iterator i = MyWords.begin(); i != MyWords.end(); i++) {
		if (LineNumber > (MAX_LINE_LEN * curPage) || LineNumber <= (MAX_LINE_LEN * (curPage - 1))) {
			setConsoleMsg("잘못된 입력입니다. 다시 입력하시오.");
			reset(consoleMessage);
		}
		if (count == lineIndex) {
			vector<string> temp = split(MyWords[count], ' ');
			temp.erase(temp.begin() + wordIndex);
			for (int i = 0; i < temp.size(); i++) {
				buf = buf + ' ' + temp[i];
			}

			stringstream s(newStr);
			MyWords[count] = buf;
			for (int i = 0; i < MyWords.size(); i++) {
				newStr = newStr + ' ' + MyWords[i];
			}
			stringstream s2(newStr);
			while (s2 >> temp2) {
				newMyWords.push_back(temp2);
			}
			vector<string> divided = myTextEditor::instance()->divideTxtToEachLine(newMyWords);
			MyWords = divided;
			break;
		}
		else {
			count++;
		}

	}
	setConsoleMsg("해당 문자가 삭제되었습니다.");
	reset(consoleMessage);
};

void myTextEditor::commandC(string targetWord, string newWord) {
	if (targetWord.size() > 75 || newWord.size() > 75) {
		setConsoleMsg("잘못된 입력입니다. 다시 입력하시오.");
		reset(consoleMessage);
	}

	int count = 0;
	int countTarget = 0;

	string buf = "";
	string newStr = "";
	string temp2;
	vector<string> newMyWords;
	for (vector<string>::iterator i = MyWords.begin(); i != MyWords.end(); i++) {
		vector<string> temp = split(MyWords[count], ' ');
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == targetWord) {
				temp[i] = newWord;
				countTarget++;
			}
			buf = buf + ' ' + temp[i];
		}
		count++;
	}

	if (countTarget == 0) {
		setConsoleMsg("바꾸고자 하는 문자가 text내에 포함되어있지 않습니다. 단어를 다시 확인한 뒤 교체하시오.");
		reset(consoleMessage);
	}
	else {
		stringstream s2(buf);
		while (s2 >> temp2) {
			newMyWords.push_back(temp2);
		}
		vector<string> divided = myTextEditor::instance()->divideTxtToEachLine(newMyWords);
		MyWords = divided;

		setConsoleMsg("해당 문자들이 바뀌었습니다.");
		reset(consoleMessage);
	}

};



void myTextEditor::commandS(string targetWord) {
	if (targetWord.size() > 75) {
		setConsoleMsg("잘못된 입력입니다. 다시 입력하시오.");
		reset(consoleMessage);
	}

	int count = 0;
	int countForLine = 0;
	string buf = "";
	string newStr = "";
	string temp2;

	vector<string> newMyWords;
	for (vector<string>::iterator i = MyWords.begin(); i != MyWords.end(); i++) {
		vector<string> temp = split(MyWords[count], ' ');
		for (int j = 0; j < temp.size(); j++) {
			if (temp[j] == targetWord) {
				this->minLine = count;
				countForLine = count;
				break;
			}
		}
		count++;
	}
	if (countForLine == 0) {
		setConsoleMsg("찾으시는 단어가 문서에 존재하지 않습니다. 다시 입력하시오.");
		reset(consoleMessage);
	}
	else {
		setConsoleMsg("찾으시는 단어가 해당된 줄이 상단에 나타났습니다.");
		resetForCommandS(consoleMessage, (this->minLine-1));
	}

};

void myTextEditor::resetForCommandS(string consoleMsg, int minline) {
	consoleMsg = myTextEditor::consoleMsg();
	myTextEditor::instance()->viewLine(minLine);
	myTextEditor::instance()->viewPage(consoleMsg);
}


void myTextEditor::commandT() {
	ofstream out("./test.txt");
	if (out.is_open()) {
		for (auto i : MyWords) {
			out << i;
			out << " ";
		}
	}
	setConsoleMsg("저장되었습니다.");
	myTextEditor::instance()->viewLine();
	cout << SECTIONLINE << endl;
	cout << MENU << endl;
	cout << SECTIONLINE << endl;
	cout << "(콘솔메시지)" << consoleMsg() << endl;
	cout << SECTIONLINE << endl;
	exit(0);
};
//==========================================================================


int main() {
	vector <string> allWordInText;
	string temp;
	ifstream Fopen;
	Fopen.open("./test.txt", ios_base::in);
	Fopen.seekg(0, ios::end);

	int end = Fopen.tellg();
	Fopen.clear();
	temp.resize(end);
	Fopen.seekg(0, ios::beg);

	Fopen.read(&temp[0], end);

	string buf;
	stringstream s(temp);

	while (s >> buf) {
		allWordInText.push_back(buf);
	}

	Fopen.close();
	vector<string> divided = myTextEditor::instance()->divideTxtToEachLine(allWordInText);
	myTextEditor::instance()->setMyWords(divided);

	myTextEditor::instance()->viewLine();
	myTextEditor::instance()->viewPage(myTextEditor::instance()->consoleMsg());

	while (1) {
		myTextEditor::instance()->commandManager(myTextEditor::instance()->inputCmd());
	}
}







