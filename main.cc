#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

const std::string fileName = "students.txt";

using u_short = unsigned short;
using u_int = unsigned;

using Subjects = map<string, u_short>;
using Grades = map<u_short, map<string, u_short>>;

struct Student {
public:
	void setData();
	void print();

public:
	string name;
	string m_name;
	string surname;
	u_short b_day;
	u_short b_month;
	u_short b_year;
	u_short adm_year;
	string faculty;
	string departm;
	u_int group;
	u_int record_book_n;
	bool sex;
	Grades grades;
};

std::ostream& operator <<(std::ostream& out, const Student& st);
std::istream& operator >>(std::istream& in, Student& st);

std::ostream& operator <<(std::ostream& out, const Student& st)
{
	const std::string delim = "  ";
	out << st.name << delim << st.m_name << delim << st.surname << delim << st.b_day << delim << st.b_month 
		<< delim << st.b_year << delim << st.adm_year << delim << st.faculty << delim << st.departm 
		<< delim << st.group << delim << st.record_book_n << delim << st.sex << delim;

	out << st.grades.size() << delim;
	for (auto& s : st.grades) {
		out << s.second.size() << delim;
		for (auto& sbj : s.second) {
			out << sbj.first << delim << sbj.second << delim;
		}
	}
	return out;
}

std::istream& operator >>(std::istream& in, Student& st)
{
	in >> st.name >> st.m_name >> st.surname >> st.b_day >> st.b_month >> st.b_year >> st.adm_year 
		>> st.faculty >> st.departm >> st.group >> st.record_book_n >> st.sex;
	int s_size, sbj_size;
	in >> s_size;
	for (int i = 0; i < s_size; ++i) {
		st.grades[i] = Subjects();
		in >> sbj_size;
		for (int j = 0; j < sbj_size; ++j) {
			std::string sbj;
			int grade;
			in >> sbj >> grade;
			st.grades[i][sbj] = grade;
		}

	}
	return in;
}

void Student::print()
{
	string d = " ";
	cout << setw(3) << d << ". " << name << d << m_name << d << surname << endl;
	cout << setw(5) << d << "Date of birth: " << b_day << "." << b_month << "." << b_year << endl;
	cout << setw(5) << d << "Entrance: " << adm_year << endl;
	cout << setw(5) << d << "Faculty: " << faculty << endl;
	cout << setw(5) << d << "Department: " << departm << endl;
	cout << setw(5) << d << "Group: " << group << endl;
	cout << setw(5) << d << "Record book: " << record_book_n << endl;
	cout << setw(5) << d << "Sex: " << (sex ? "М" : "Ж") << endl;
	cout << setw(5) << d << "Grades: " << endl;

	for (auto& s : grades)
	{
		cout << setw(7) << " " << "Session " << s.first << endl;

		for (auto ss : s.second)
			cout << setw(9) << " " << ss.first << " - " << ss.second << endl;
	}

	cout << endl;
}

void Student::setData() {
	cout << "Name: "; cin >> name;
	cout << "Middle name: "; cin >> m_name;
	cout << "Surname: "; cin >> surname;

	do {
		cout << "Birth day: "; 	cin >> b_day;
	} while (b_day <= 0 || b_day > 31);

	do {
		cout << "Birth month: "; cin >> b_month;
	} while (b_month <= 0 || b_month > 12);

	do {
		cout << "Birth year: "; cin >> b_year;
	} while (b_year <= 1980 || b_year > 2005);

	do {
		cout << "Entrance year: "; cin >> adm_year;
	} while (adm_year <= 1980 || adm_year > 2022);

	cout << "Faculty: "; cin >> faculty;
	cout << "Department: "; cin >> departm;
	cout << "Group: "; cin >> group;
	cout << "Record book: "; cin >> record_book_n;
	cout << "Sex: "; cin >> sex;

	int s_count;
	do
	{
		cout << "Session count: "; cin >> s_count;
	} while (s_count <= 0 || s_count > 9);

	int ss_count;
	int grade;
	std::string sbj;
	for (int i = 0; i < s_count; ++i) {
		cout << "Subjects count: "; cin >> ss_count;
		for (int j = 0; j < ss_count; ++j) {
			cout << "Subject: "; cin >> sbj;
			do {
				cout << "Grade: "; cin >> grade;
			} while (grade < 2 || grade > 5);
			grades[i][sbj] = grade;
		}
	}
}

struct FileSystem {
	virtual ~FileSystem() {}
	virtual void load() = 0;
	virtual void save() = 0;
};

struct Collection {
	virtual ~Collection() {}
	virtual void addItem() = 0;
	virtual void modifyItem() = 0;
	virtual void removeItem() = 0;
};

class Group : public FileSystem, public Collection
{
public:

	Group() { load(); }
	~Group() { save(); }

	void print();

	void addItem() override;
	void modifyItem() override;
	void removeItem() override;

	void printGoodLearners();
	void printNotSoGoodLearners();

private:
	void load() override;
	void save() override;

private:
	vector<Student> _data;
};

void Group::print()
{
	int i = 0;
	const char* d = " ";

	for (auto& s : _data)
	{
		s.print();
	}
}

void Group::addItem()
{
	Student s;
	s.setData();

	_data.push_back(s);
}

void Group::modifyItem()
{
	int i;
	cout << "Item to modify: "; cin >> i;
	if (!i) {
		cout << "Operation canceled\n";
		return;
	}

	if (i < 0 || i > _data.size())
	{
		cout << "Incorrect number\n";
		return;
	}

	_data[i].setData();
}

void Group::removeItem()
{
	int i;
	cout << "Item to modify:  "; cin >> i;
	if (!i) {
		std::cout << "Operation canceled\n";
		return;
	}
	if (i <= 0 || i > _data.size())
	{
		cout << "Icorrect number\n";
		return;
	}

	_data.erase(_data.begin() + i);
}

void Group::save()
{
	ofstream out;
	out.open(fileName, ios_base::out | ios_base::binary);

	if (!out.is_open())
	{
		cout << "Can't save file!" << endl;
		return;
	}

	for (auto& st : _data)
	{
		out << st << "\n";
	}
	out.close();
}

void Group::load()
{
	ifstream in(fileName);
	if (!in.is_open()) {
		std::cout << "File isn't exist yet\n";
	}

	std::string line;

	Student st;
	while (getline(in, line))
	{
		std::istringstream iss(line);
		iss >> st;
		_data.push_back(st);
	}

	in.close();
}

void Group::printGoodLearners()
{
	cout << "Group1: " << endl;

	int i = 0;

	for (auto& s : _data)
	{
		int cnt_good = 0;
		int cnt_bad = 0;

		for (auto& session : s.grades)
		{
			for (auto& subject : session.second)
			{
				if (subject.second > 3)
					++cnt_good;
				else
					++cnt_bad;
			}
		}

		if (cnt_bad > cnt_good)
			continue;

		s.print();
	}
}

void Group::printNotSoGoodLearners()
{
	cout << "Group2: " << endl;

	int i = 0;

	for (auto& s : _data)
	{
		int cnt_good = 0;
		int cnt_bad = 0;

		for (auto& session : s.grades)
		{
			for (auto& subject : session.second)
			{
				if (subject.second > 3)
					++cnt_good;
				else
					++cnt_bad;
			}
		}

		if (cnt_bad <= cnt_good)
			continue;

		s.print();
	}
}

void menu() {
	cout << "1. Add item\n";
	cout << "2. Modify item\n";
	cout << "3. Remove item\n";
	cout << "4. Print all\n";
	cout << "5. Print task\n";
	cout << "0. Exit\n>";
}

int main()
{
	using GroupPtr = std::shared_ptr<Group>;
	GroupPtr gr(new Group());
	int answer;

	do 	{
		menu();
		cin >> answer;
		system("cls");

		switch (answer)
		{
		case 1: gr->addItem(); 	  break;
		case 2: gr->modifyItem(); break;
		case 3:	gr->removeItem(); break;
		case 4: gr->print();      break;
		case 5:
			gr->printGoodLearners();
			gr->printNotSoGoodLearners();
			break;

		case 0:
			break;

		default:
			std::cout << "Unknown command\n";
		}

		system("pause");
		system("cls");
	} while (answer);

	return 0;
}