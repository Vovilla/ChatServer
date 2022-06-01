#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <regex>
#include <cctype>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

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

const string COMMAND = "command";
const string PRIVATE_MSG = "PRIVATE_MSG";
const string PUBLIC_MSG = "PUBLIC_MSG";
const string USER_ID_TO = "user_id_to";
const string USER_ID_FROM = "user_id_from";
const string TEXT = "text";

void ProcessMessage(auto ws, auto message) {
	json data = json::parse(message);
	UserData* userData = ws->getUserData();
	string command = data[COMMAND];
	if(command == PRIVATE_MSG) {
		int user_id = data[USER_ID_TO];
		string text = data[TEXT];
		json response;
		response[COMMAND] = PRIVATE_MSG;
		response[USER_ID_FROM] = userData->user_id;
		response[TEXT] = text;
		ws->publish("user" + to_string(data->user_id), response.dump());
		cout << "User " << userData->user_id 
			<< " send message to User N " << user_id << endl;
	}
}

int main()
{
	int latest_user_id = 1;
	uWS::App().get("/hello", [](auto* res, auto* req) {
		res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");
		}).ws<UserData>("/*", {
			.open = [&latest_user_id](auto* ws) {
				UserData* data = ws->getUserData();
				data->user_id = latest_user_id++;
				cout << "User connected ID= " << data->user_id << endl;
				ws->subscribe("user" + to_string(data->user_id));
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
