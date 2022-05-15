#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <clocale>
#include <windows.h>
#include <algorithm>

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

	bool goodLearner() const;
	float avgGrade() const;
	bool operator < (const Student& st);

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

bool Student::operator<(const Student& st) {
	return avgGrade() < st.avgGrade();
}

bool Student::goodLearner() const{
	int cnt_good = 0;
	int cnt_bad = 0;

	for (auto& session : grades)
	{
		for (auto& subject : session.second)
		{
			if (subject.second > 3)
				++cnt_good;
			else
				++cnt_bad;
		}
	}

	return cnt_good >= cnt_bad;
}

float Student::avgGrade() const {
	int count = 0;
	float res = 0;
	for (auto& session : grades) {
		for (auto& subject : session.second)
		{
			count++;
			res += subject.second;
		}
	}

	return res / count;
}

void Student::print()
{
	string d = " ";
	cout << setw(3) << d << ". " << name << d << m_name << d << surname << endl;
	cout << setw(5) << d << "Дата рождения: " << b_day << "." << b_month << "." << b_year << endl;
	cout << setw(5) << d << "Год поступления: " << adm_year << endl;
	cout << setw(5) << d << "Факультет: " << faculty << endl;
	cout << setw(5) << d << "Кафедра: " << departm << endl;
	cout << setw(5) << d << "Группа: " << group << endl;
	cout << setw(5) << d << "Номер зачетной книжки: " << record_book_n << endl;
	cout << setw(5) << d << "Пол: " << (sex ? "М" : "Ж") << endl;
	cout << setw(5) << d << "Оценки: " << endl;

	for (auto& s : grades)
	{
		cout << setw(7) << " " << "Сессия " << s.first + 1 << endl;

		for (auto ss : s.second)
			cout << setw(9) << " " << ss.first << " - " << ss.second << endl;
	}

	cout << endl;
}

void Student::setData() {
	cout << "Имя: "; cin >> name;
	cout << "Отчество: "; cin >> m_name;
	cout << "Фамилия: "; cin >> surname;

	do {
		cout << "День рождения: "; 	cin >> b_day;
	} while (b_day <= 0 || b_day > 31);

	do {
		cout << "Месяц рождения: "; cin >> b_month;
	} while (b_month <= 0 || b_month > 12);

	do {
		cout << "Год рождения: "; cin >> b_year;
	} while (b_year <= 1980 || b_year > 2005);

	do {
		cout << "Год поступления: "; cin >> adm_year;
	} while (adm_year <= 1980 || adm_year > 2022);

	cout << "Факультет: "; cin >> faculty;
	cout << "Кафедра: "; cin >> departm;
	cout << "Группа: "; cin >> group;
	cout << "Номер зачетной книжки: "; cin >> record_book_n;
	cout << "Пол: "; cin >> sex;

	int s_count;
	do
	{
		cout << "Количество сессий: "; cin >> s_count;
	} while (s_count <= 0 || s_count > 9);

	int ss_count;
	int grade;
	std::string sbj;
	for (int i = 0; i < s_count; ++i) {
		cout << "Количество предметов: "; cin >> ss_count;
		for (int j = 0; j < ss_count; ++j) {
			cout << "Предмет: "; cin >> sbj;
			do {
				cout << "Оценка: "; cin >> grade;
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

	void printCustom();

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
	cout << "Номер для редактирования: "; cin >> i;
	if (!i) {
		cout << "Операция отменена\n";
		return;
	}

	if (i < 0 || i > _data.size())
	{
		cout << "Неправильный номер\n";
		return;
	}

	_data[i].setData();
}

void Group::removeItem()
{
	int i;
	cout << "Номер для удаления: "; cin >> i;
	if (!i) {
		std::cout << "Операция отменена\n";
		return;
	}
	if (i <= 0 || i > _data.size())
	{
		cout << "Неправильный номер\n";
		return;
	}

	_data.erase(_data.begin() + i);
}

void Group::save()
{
	ofstream out(fileName);
	if (!out.is_open())
	{
		cout << "Невозможно открыть файл" << endl;
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
		std::cout << "Файл еще не создан, нечего загрузить\n";
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

void Group::printCustom()
{
	int yearMin, yearMax;
	std::cout << "Min birth year: "; std::cin >> yearMin;
	std::cout << "Max birth year: "; std::cin >> yearMax;

	std::vector<Student> filtered;
	std::copy_if(_data.begin(), _data.end(), std::back_inserter(filtered),
		[&](Student& st) { 
			return st.b_year >= yearMin && st.b_year <= yearMax; });

	auto it = std::partition(filtered.begin(), filtered.end(), [](Student& st) {return st.goodLearner(); });
	auto sz = std::distance(filtered.begin(), it);
	auto sz_n = filtered.size();
	std::sort(filtered.begin(), it);
	std::sort(it, filtered.end());

	std::cout << "Первая группа: " << std::endl;
	if (sz < 4) {
		for (int i = 0; i < sz; ++i) {
			filtered[i].print();
		}
	}
	else {
		std::cout << "Мене успевающие: ";
		filtered[0].print();
		filtered[1].print();
		std::cout << "Более успевающие:";
		filtered[sz - 1].print();
		filtered[sz - 2].print();
	}

	std::cout << "Вторая группа: " << std::endl;
	if (sz < 4) {
		for (int i = sz; i < sz_n; ++i) {
			filtered[i].print();
		}
	}
	else {
		std::cout << "Мене успевающие: ";
		filtered[sz].print();
		filtered[sz].print();
		std::cout << "Болеее успевающие:";
		filtered[sz_n - 1].print();
		filtered[sz_n - 2].print();
	}


}

void menu() {
	cout << "1. Добавить студента\n";
	cout << "2. Изменить студента\n";
	cout << "3. Удалить студента\n";
	cout << "4. Распечатать список студентов\n";
	cout << "5. Распечатать список согласно фильтру\n";
	cout << "0. Выход\n>";
}

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

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
			gr->printCustom();
			break;

		case 0:
			break;

		default:
			std::cout << "Неизвестная команда\n";
		}

		system("pause");
		system("cls");
	} while (answer);

	return 0;
}