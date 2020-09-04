#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

#define MAX 100

class Node {

public:

	string identifier, scope, type;
	Node* next;


	Node()
	{
		next = NULL;
	}

	Node(string key, string value, string type)
	{
		this->identifier = key;
		this->scope = value;
		this->type = type;
		next = NULL;
	}

	void print()
	{
		cout << "Identifier's Name:" << this->identifier << endl;
		cout << "Type:" << this->type << endl;
		cout << "Scope: " << this->scope << endl;
	}
};

class SymbolTable {
	Node* head[MAX];

public:
	SymbolTable()
	{
		for (int i = 0; i < MAX; i++)
			head[i] = NULL;
	}


	int hashFunction(string id)
	{
		int asciiTableSum = 0;

		for (int i = 0; i < id.length(); i++) {
			asciiTableSum = asciiTableSum + id[i];
		}

		return (asciiTableSum % 100);
	}

	// Function to modify an identifier 
	bool update(string id, string s,
		string t)
	{
		int index = hashFunction(id);
		Node* start = head[index];

		if (start == NULL)
			return "-1";

		while (start != NULL) {
			if (start->identifier == id) {
				start->scope = s;
				start->type = t;
				return true;
			}
			start = start->next;
		}

		return false; // id not found 
	}

	// Function to delete an identifier 
	bool deleteRecord(string id)
	{
		int index = hashFunction(id);
		Node* tmp = head[index];
		Node* par = head[index];

		// no identifier is present at that index 
		if (tmp == NULL) {
			return false;
		}
		// only one identifier is present 
		if (tmp->identifier == id && tmp->next == NULL) {
			tmp->next = NULL;
			delete tmp;
			return true;
		}

		while (tmp->identifier != id && tmp->next != NULL) {
			par = tmp;
			tmp = tmp->next;
		}
		if (tmp->identifier == id && tmp->next != NULL) {
			par->next = tmp->next;
			tmp->next = NULL;
			delete tmp;
			return true;
		}

		// delete at the end 
		else {
			par->next = NULL;
			tmp->next = NULL;
			delete tmp;
			return true;
		}
		return false;
	}

	// Function to find an identifier 
	bool find(string id)
	{
		int index = hashFunction(id);
		Node* start = head[index];

		if (start == NULL)
			return "-1";

		while (start != NULL) {

			if (start->identifier == id) {
				start->print();
				//return start->scope;
				return true;
			}

			start = start->next;
		}

		return "-1"; // not found 
	}

	// Function to insert an identifier 
	bool insert(string id, string scope,
		string Type)
	{
		int index = hashFunction(id);
		Node* p = new Node(id, scope, Type);

		if (head[index] == NULL) {
			head[index] = p;
			cout << "\n"
				<< id << " inserted";

			return true;
		}

		else {
			Node* start = head[index];
			while (start->next != NULL)
				start = start->next;

			start->next = p;
			cout << "\n"
				<< id << " inserted";

			return true;
		}

		return false;
	}


};