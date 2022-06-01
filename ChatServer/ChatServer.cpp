#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <regex>
#include <cctype>
#include <uwebsockets/App.h>

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

struct UserData {
	string name;
	int user_id;
};


int main()
{
	uWS::App().get("/hello", [](auto* res, auto* req) {
		res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");
		}).ws<UserData>("/*", {
			.open = [](auto* ws) {
				UserData* data = ws->getUserData();
				//ws->getUserData
				//ws->subscribe("sensors/+/house");
			},
			.message = [](auto* ws, string_view message, uWS::OpCode opCode) {
				//ws->send(message, opCode);
			},
			.close = [](auto *ws, int code, string_view message) {
				//	
			}
		}).listen(9001, [](auto* listenSocket) {
			if (listenSocket) {
				std::cout << "Listening on port " << 9001 << std::endl;
			}
	}).run();

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
