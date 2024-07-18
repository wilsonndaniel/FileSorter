/*The conference chair has set the following rules for paper acceptance:
1- The average of each individual reviewer must be above 5 (i.e., if the
average of any reviewer is less than 5, the paper is rejected,
regardless of other scores).
2- The overall average of all the reviewers is greater than or equal 7.
The chair also wanted to consider a round 2 possible acceptance by
marking papers as borderline. The paper is “borderline” if the overall
score is greater than or equal 5 and less than 7.*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

string Decision(int arr[4][5]) {
	int sums[] = { 0, 0, 0, 0 };
	int accumulator = 0;
	//Rejected 
	//"if the average of any reviewer is less than 5, the paper is rejected"
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			sums[i] += arr[i][j];
			accumulator += arr[i][j];
		}
	}
	for (int i = 0; i < 4; i++) {
		if (sums[i] / 5.0 < 5) {
			return "Declined";
		}
	}
	//"The paper is “borderline” if the overall score is greater than or equal 5 and less than 7"
	if (accumulator / 20.0 < 7) {
		return "borderline";
	}
	return "accepted";
}

int main() {
	//Reviewer # then Criteria
	int arr[4][5];
	int id;
	int count = 0;
	string line;

	ifstream in("input.txt");
	ofstream out("Conference Manager.txt");

	if (in.fail()) {
		cout << "Error";
		exit(1);
	}
	getline(in, line);
	out << setw(12) << left << "Paper ID" << "Decision" << endl;
	while (!in.eof()) {
		in >> id;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 5; j++) {
				in >> arr[i][j];
			}
		}
		out << setw(12) << left << id << Decision(arr) << endl;
	}
	cout << "Output to 'Conference Manager.txt' Successful" << endl;
	return 0;
}