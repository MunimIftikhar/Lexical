#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;


#define MAXSIZE 50

struct token{
	string name;
	string type;
};

vector <int> asciiMap;
vector <vector<int>> mapping;
vector <vector<string>> symbolTable;
vector <int> acceptingStates;
vector <string> keywords;
vector <string> operators;

ofstream fout("token.txt");
ofstream tout("error.txt");

void readSourceCode();
void fillAsciiMapArray();
void fillMappingArray();
void fillAcceptingStatesArray();
void fillKeyWordsArray();
void tokenization(string line);
bool checkifFinalState(int state, int &i);
void fillOperatorsArray();
void createSymbolTable();
token fillTokenStruct(string line, int startingPoint, int endingPoint);
void createSymbolTable(token t);
void print2dArray();



int main()
{

	fillAsciiMapArray();
	fillMappingArray();
	fillAcceptingStatesArray();
	fillKeyWordsArray();
	fillOperatorsArray();
	//print2dArray();
	readSourceCode();

	return 0;
}



bool checkifFinalState(int state, int &i)
{
	for (int k = 0; k < acceptingStates.size(); k++)
	{
		if (acceptingStates[k] < 0)
		{

			int s;
			s = acceptingStates[k] * (-1);
			if (s == state)
			{
				i=i-1;
				return true;
			}
		}
		else if (acceptingStates[k] == state)
		{
			return true;
		}
	}
	return false;
}

void tokenization(string line)
{

	token t;
	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] == '#'){ i++; }
		if (line[i] == '"'){
			while (line[i] != '"'){
				i++;
			}
			i++;
		}
		if (line[i] == ' '){ i++; }
		if (line[i] == ';'){ i++; }
		if (line[i] == '\t'){ i++; }
		if (asciiMap[int(line[i])] == -1)
		{
			tout << line[i] << endl;
		}
		else
		{
			int state = mapping[0][asciiMap[int(line[i])]];
			int startingPoint = i;
			while (1)
			{
				int endingp = i;
				if (checkifFinalState(state, i))
				{
					t = fillTokenStruct(line, startingPoint, i);
					if (t.name.size()>0 && t.type.size()>0)
					{
						fout << t.name << " " << t.type << endl;
						string id = "identifier";
						if (t.name.compare(id) > 0)
						{
							createSymbolTable(t);
						}
						
					}
					else
					{
						for (int k = startingPoint; k <= i; k++)
						{
							tout << line[k];
						}
						tout << endl;
					}
					break;
				}
				if (line[i] == '+' && line[i + 1] != '+' && line[i + 1] != '=' && !(line[i + 1] >= 48 && line[i + 1] <= 57))
				{
					state = mapping[state][23];
				}

				else if (line[i] == '-' && line[i + 1] != '-' && line[i + 1] != '=' && (line[i + 1] >= 48 && line[i + 1] <= 57))
				{
					state = mapping[state][23];
				}
				else if (line[i] == '<' && line[i + 1] != '<' && line[i + 1] != '=')
				{
					state = mapping[state][23];
				}
				else if (line[i] == '>' && line[i + 1] != '>' && line[i + 1] != '=')
				{
					state = mapping[state][23];
				}
				else if (line[i] == '=' && line[i + 1] != '=')
				{
					state = mapping[state][23];
				}
				else if ((line[i] >= 48 && line[i] <= 57) && ((line[startingPoint] >= 48 && line[startingPoint] <= 57)
					|| line[startingPoint] == '+' || line[startingPoint] == '-') && 
					(!(line[i+1] >= 48 && line[i+1] <= 57)))
				{
					state = mapping[state][23];
				}
				else if (((line[startingPoint] >= 65 && line[startingPoint] <= 90) || (line[startingPoint] >= 97 && line[startingPoint] <= 122)
					|| line[startingPoint] == '_') && ((line[i] >= 48 && line[i] <= 57) || line[i] == '_') && (!(line[i+1] >= 48 && line[i+1] <= 57) || line[i+1] != '_'))
				{
					state = mapping[state][23];
				}
				else if (((line[startingPoint] >= 65 && line[startingPoint] <= 90) || (line[startingPoint] >= 97 && line[startingPoint] <= 122)) &&
					!((line[i + 1] >= 65 && line[i + 1] <= 90) || (line[i + 1] >= 97 && line[i + 1] <= 122))
					&& ((line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122)) && (!(line[i + 1] >= 48 && line[i + 1] <= 57)
					&& line[i + 1] != '_'))
				{
					state = mapping[state][23];
				}
				else{
					if (asciiMap[int(line[i])] == -1)
					{
						tout << line[i] << endl;
					}
					else if (state!=-1)
					{
						//cout << asciiMap[int(line[i])];
						state = mapping[state][asciiMap[int(line[i])]];
					}

				}
				i++;
			}
		}

	}
}

void readSourceCode()
{
	string line;
	ifstream fin("sourceCode.txt");
	if (!fin.is_open())
	{
		cout << "Unable to open file.";
		return;
	}

	while (getline(fin, line))
	{
		//cout << line << '\n';
		if (!line.empty())
		{
			tokenization(line);
		}

	}
	fin.close();
	fout.close();
	tout.close();
}

int findLength(string line)
{
	int cnt = 0;
	for (int i = 0; line[i] != '\0'; i++)
	{
		cnt++;
	}
	return cnt;
}

token fillTokenStruct(string line, int startingPoint, int endingPoint)
{
	token t;
	if (line[endingPoint] == '_' || (line[endingPoint] >= 48 && line[endingPoint] <= 57) && !(line[startingPoint] >= 48 && line[startingPoint] <= 57))
	{
		t.type = "Identifier";
		t.name.resize(1024);
		int k=0;
		for (int i = startingPoint; i <= endingPoint; i++)
		{
			t.name[k] = line[i];
			k++;
		}
		t.name[k] = '\0';
		
		//int s = findLength(t.name);
		t.name.resize(k);
		return t;
	}
	if ((line[endingPoint] >= 48 && line[endingPoint] <= 57) && (line[startingPoint] >= 48 && line[startingPoint] <= 57))
	{
		t.type = "Number";
		t.name.resize(1024);
		int k=0;
		for (int i = startingPoint; i <= endingPoint; i++)
		{
			t.name[k] = line[i];
			k++;
		}
		t.name[k] = '\0';
		int s = t.name.size();
		t.name.resize(s);
		return t;
	}
	int m = startingPoint;
	if ((line[endingPoint] >= 65 && line[endingPoint] <= 90) || (line[endingPoint] >= 97 && line[endingPoint] <= 122))
	{
		for (int i = 0; i < keywords.size(); i++)
		{
			bool check = true;
			m = startingPoint;
			for (int k = 0; k < keywords[i].length(); k++)
			{
				if (m > endingPoint || line[m] != keywords[i][k])
				{
					check = false;
					break;
				}
				m++;
			}
			if (check == true)
			{
				t.name = keywords[i];
				t.type = "Keyword";
				return t;
			}
		}
	}
	m = startingPoint;
	for (int i = 0; i < operators.size(); i++)
	{
		bool check = true;
		m = startingPoint;

		for (int k = 0; k < operators[i].length(); k++)
		{
			if (m > endingPoint || line[m] != operators[i][k])
			{
				check = false;
				break;
			}
			m++;
		}
		if (check == true)
		{
			t.name = operators[i];
			t.type = "Operator";
			return t;
		}
	}
	return t;

}

void createSymbolTable(token t)
{
	//0 - lexical
	//1 - datatype
	//2 - classes
	//3 - size
	vector <string> ss;
	ss.push_back(t.name);
	ss.push_back(t.type);
	symbolTable.push_back(ss);
}

void fillAsciiMapArray()
{
	ifstream fin("ascii_map.txt");
	if (!fin.is_open())
	{
		cout << "Unable to open file.";
		return;
	}
	int i = 0, num;
	while (fin >> num){
		asciiMap.push_back(num);
		i++;
	}
	fin.close();
}

void fillMappingArray()
{
	int row = 0, col = 0;
	ifstream fin("mapping.txt");
	if (!fin.is_open())
	{
		cout << "Unable to open file.";
		return;
	}
	int i = 0, num = 0;
	fin >> row; fin >> col;
	while (i<row)
	{
		int k;
		vector<int>num1;
		for (k = 0; k < col; k++)
		{
			fin >> num;
			num1.push_back(num);
		}
		mapping.push_back(num1);
		i++;
	}
}
void fillAcceptingStatesArray()
{
	ifstream fin("acceptingStates.txt");
	if (!fin.is_open())
	{
		cout << "Unable to open file.";
		return;
	}
	int i = 0, num = 0;
	while (fin >> num)
	{
		acceptingStates.push_back(num);
		i++;
	}
	fin.close();
}

void fillKeyWordsArray()
{
	ifstream fin("keywords.txt");
	if (!fin.is_open())
	{
		cout << "Unable to open file.";
		return;
	}
	string data;
	while (fin >> data)
	{
		keywords.push_back(data);
	}
	fin.close();
}

void fillOperatorsArray()
{
	ifstream fin("operators.txt");
	if (!fin.is_open())
	{
		cout << "Unable to open file.";
		return;
	}
	string data;
	while (getline(fin, data))
	{
		operators.push_back(data);
	}
	fin.close();
}

void print2dArray()
{
	for (int i = 0; i < mapping.size(); i++)
	{
		for (int k = 0; k < mapping[i].size(); k++)
		{
			cout << mapping[i][k] << " ";
		}
		cout << endl;
	}
}