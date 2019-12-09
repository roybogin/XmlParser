#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

template <class T>
class XmlTree
{
private:
	string name;
	T value;
	vector<XmlTree<T>*> list;
public:
	XmlTree()
	{
		
	}

	XmlTree(string name)
	{
		this->name = name;
	}

	XmlTree(string name, T value)
	{
		this->value = value;
		this->name = name;
	}

	T getValue()
	{
		return value;
	}

	vector<XmlTree<T>*> getList()
	{
		return list;
	}

	void setName(string name)
	{
		this->name = name;
	}

	void setValue(T value)
	{
		this->value = value;
	}

	void addToList(XmlTree<T>* value)
	{
		this->list.push_back(value);
	}

	bool isLeaf()
	{
		return list.size() == 0;
	}
};

XmlTree<string>* textToTree(string text)
{
	XmlTree<string>* tree = new XmlTree<string>;
	regex xmlTag("<([^/>]*)>(.*)</\\1>");
	regex openTag("<([^/>]*)>");
	smatch matcher;
	if (regex_match(text, matcher, xmlTag))
	{
		tree->setName(matcher[1].str());
		string next = matcher[2].str();
		smatch match2;
		if (regex_search(next, match2, openTag))
		{
			while (regex_search(next, match2, openTag))
			{
				string val = match2[1].str();
				string closeTag = "</" + val + ">";
				int closeIdx = next.find(closeTag);
				tree->addToList(textToTree(next.substr(0, closeIdx + closeTag.length())));
				next = next.substr(closeIdx + closeTag.length());
			}
		}
		else
		{
			tree->setValue(next);
		}
		
	}
	return tree;
	
}

string fileToString(string fileName)
{
	ifstream file;
	string line;
	file.open(fileName);
	string text;
	try
	{
		while (getline(file, line))
		{
			while (isspace(line[0]))
				line = line.substr(1);
			text += line;
		}
	}
	catch (...)
	{

	}
	file.close();
	return text;
}



int main()
{
	cout << "Insert file name" << endl;
	string fileName;
	cin >> fileName;
	string text = fileToString(fileName);
	XmlTree<string>* tree = textToTree(text);

	return 0;
}