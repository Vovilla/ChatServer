#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <regex>
#include <cctype>

using namespace std;

string ToLower(string text) {
	transform(text.begin(), text.end(),
		text.begin(), [](unsigned char c) {
			return tolower(c);
		});
	return text;
}

bool IsMatch(string text, string example) {
	text = ToLower(text);
	example = ToLower(example);
	regex pattern(".*" + example + ".*");
	return regex_match(text, pattern);
}

string UserQuestion() {
	string question;	
	cout << "[USER]: ";
	getline(cin, question);
	return question;
}

void BotReply(string reply) {
	cout << "[BOT]: " << reply << endl;
}

int main()
{
	map<string, string> AnswerDatabase{
		{"Hello", "Hi!"},
		{"How.*are.*you", "All good"}
	};

	cout << "Hello niga\n";
	string question = "";
	while (question != "exit") {
		question = UserQuestion();
		for (const auto& [key, value] : AnswerDatabase) {
			if (IsMatch(question, key)) {
				BotReply(value);
			}
		}
	}
}
