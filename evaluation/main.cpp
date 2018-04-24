/****
**  This is only a test to make an evaluation without extern 
**  frameworks like boost.spirit and others as said in the 
**  repository name "useless things" 
*/

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <stack>
#include <queue>

using namespace std;

// 100 * ((10 + (2 * (1 + 1)) * 3) / 11) =  200       (by Google too)
// 100 * 10 + 2 * 1 + 1 * 3 / 11 =          1002.27   (by Google 1002.27272727)
// -100 * ((10 + (2 * (1 + 1)) * 3) / 11) = -200      (by Google too)
// -100 * 10 + 2 * 1 + 1 * 3 / 11 =         -997.727  (by Google -997.727272727)

class evaluate {
public:
	evaluate(const string &expr, double &result) : _valid(false) {
		string _expr = expr;
		if (verify(_expr) && tokenizer(_expr))
		{ _valid = solve(_tks, result); }
	}
	explicit operator bool() { return _valid; }

private:
	typedef struct _tk {
		_tk() : _val(0), _tp(in) {}
		enum _tp { op, nu, ob, cb, in } _tp;
		double _val;
	} _tk;

	bool _valid;
	vector<_tk> _tks;

	bool verify(string &expr) {
		// First verification
		if (expr.find_first_not_of("0123456789+-*/()[]{} ") != string::npos) 
		{ return false; }

		// Remove all espaces characters
		for (auto pos = expr.find_first_of(" "); pos != string::npos; pos = expr.find_first_of(" "))  
		{ expr.erase(expr.begin() + pos); }

		if (expr.size()) {
			// Verify first ([^\/*]) and last ([0-9)\]}]) character 
			if ((expr[0] == '/') || (expr[0] == '*')) 
			{ return false; }
			if ((expr[expr.size() - 1] == '-') ||
				(expr[expr.size() - 1] == '+') ||
				(expr[expr.size() - 1] == '/') ||
				(expr[expr.size() - 1] == '*')) 
			{ return false; }

			// Verifying if it has double "[-+*\/]{2}"
			if ((expr.find("++") != string::npos) &&
				(expr.find("--") != string::npos) &&
				(expr.find("+-") != string::npos) &&
				(expr.find("-+") != string::npos) &&
				(expr.find("**") != string::npos) &&
				(expr.find("//") != string::npos) &&
				(expr.find("*/") != string::npos) &&
				(expr.find("*+") != string::npos) && // not yet
				(expr.find("*-") != string::npos) && // not yet
				(expr.find("/+") != string::npos) && // not yet
				(expr.find("/-") != string::npos) && // not yet
				(expr.find("/*") != string::npos) &&
				(expr.find("+*") != string::npos) &&
				(expr.find("-*") != string::npos) &&
				(expr.find("+/") != string::npos) &&
				(expr.find("-/") != string::npos)) 
			{ return false; }

			// Validate brackets
			if (expr.find_first_of("()[]{}") != string::npos) {
				stack<char> q;
				for (auto &c : expr) {
					if (c == '{' || c == '[' || c == '(')
						q.push(c);
					else if (!q.empty() && 
						((c == '}' && q.top() == '{') ||
						 (c == ']' && q.top() == '[') ||
						 (c == ')' && q.top() == '(')))
					{ q.pop(); }
					else if (q.empty() && 
						((c == '}') ||
						 (c == ']') ||
						 (c == ')')))
					{ return false; }
				}
				if (!q.empty()) 
				{ return false; }
			}
			return true;
		}
		return false;
	}
	bool tokenizer(const string &expr) {
		if (!expr.size()) 
		{ return false; }
		string::const_iterator act = expr.begin();
		while (act != expr.end()) {
			if ((*act) >= '0' && (*act) <= '9') {
				_tk p;
				p._tp = _tk::_tp::nu;
				do {
					p._val = 10 * p._val + ((*act) - '0');
					act++;
				} while (act != expr.end() && (*act) >= '0' && (*act) <= '9');
				_tks.push_back(p);
				if (act == expr.end()) break;
			}
			if ((*act) == '+' || (*act) == '-' || (*act) == '*' || (*act) == '/') {
				_tk p;
				p._tp = _tk::_tp::op;
				p._val = (*act);
				_tks.push_back(p);
			} else if ((*act) == '{' || (*act) == '[' || (*act) == '(') {
				_tk p;
				p._tp = _tk::_tp::ob;
				p._val = (*act);
				_tks.push_back(p);
			} else if ((*act) == '}' || (*act) == ']' || (*act) == ')') {
				_tk p;
				p._tp = _tk::_tp::cb;
				p._val = (*act);
				_tks.push_back(p);
			} 
			act++;
		}
		return true;
	}
	bool solve(vector<_tk> tks, double &result) {
		if (tks.empty()) 
		{ return false; }
		function<bool()> verify_signal = [&tks]()->bool {
			if (tks[0]._tp == _tk::_tp::op && tks[1]._tp == _tk::_tp::nu) {
				if ((tks[0]._val != '+') && (tks[0]._val != '-')) 
				{ return false; }
				if (tks[0]._val == '-') 
				{ tks[1]._val *= -1; }
				tks.erase(tks.begin());
			}
			return true;
		};
		if (!verify_signal()) 
		{ return false; }
		if (tks.size() < 3) {
			if ((tks.size() == 1) && (tks[0]._tp == _tk::_tp::nu)) {
				result = tks[0]._val;
				return true;
			} else if (tks[1]._tp == _tk::_tp::nu) {
				if ((tks[0]._tp == _tk::_tp::op) &&
					((tks[0]._val == '+') || (tks[0]._val == '-'))) 
				{ result = (tks[0]._val == '-') ? tks[1]._val * -1 : tks[1]._val; }
				return true;
			} 
			return false;
		}
		function<void(vector<_tk>::iterator, vector<_tk>::iterator, double)> replace;
		replace = [&tks](vector<_tk>::iterator f, vector<_tk>::iterator l, double val) {
			l->_tp = _tk::_tp::nu; l->_val = val; tks.erase(f, l);
		};
		vector<_tk>::iterator act = tks.begin();
		// Looking for innerly brackets
		do {
			for (act = tks.begin(); act != tks.end(); act++) {
				if (act->_tp == _tk::_tp::cb) 
				{ break; }
			}
			if (act == tks.end()) 
			{ break; }
			vector<_tk>::iterator open;
			for (open = act; open->_tp != _tk::_tp::ob;) { open--; }
			if (!solve(vector<_tk>(open + 1, act), result)) 
			{ return false; }
			replace(open, act, result);
		} while (true);
		if (!verify_signal()) 
		{ return false; }
		if (tks.size() < 3) return solve(tks, result);
		// Looking for */ operators
		do {
			for (act = tks.begin(); act != tks.end(); act++) {
				if ((act->_tp == _tk::_tp::op) && 
					((act->_val == '*' ) || (act->_val == '/'))) 
				{ break; }
			}
			if (act == tks.end()) 
			{ break; }
			if (act->_val == '*') 
			{ result = (act - 1)->_val * (act + 1)->_val; }
			else if (act->_val == '/') 
			{ result = (act - 1)->_val / (act + 1)->_val; }
			replace((act - 1), (act + 1), result);
		} while (true);
		if (!verify_signal()) 
		{ return false; }
		if (tks.size() < 3) 
		{ return solve(tks, result); }
		// Looking for +- operators
		do {
			for (act = tks.begin(); act != tks.end(); act++) {
				if ((act->_tp == _tk::_tp::op) &&
					((act->_val == '+') || (act->_val == '-'))) 
				{ break; }
			}
			if (act == tks.end()) 
			{ break; }
			if (act->_val == '+') 
			{ result = (act - 1)->_val + (act + 1)->_val; }
			else if (act->_val == '-') 
			{ result = (act - 1)->_val - (act + 1)->_val; }
			replace((act - 1), (act + 1), result);
		} while (true);
		return true;
	}
};

// First idea 
bool solve(string ex, int &result) {
	size_t solve_pos;
	int partial_res;
	size_t open_bks, close_bks;

	//First verification
	if (ex.find_first_not_of("0123456789+-*/()[]{} ") != string::npos) return false;

	//remoe all espaces characters
	for (auto pos = ex.find_first_of(" "); pos != string::npos; pos = ex.find_first_of(" ")) {
		ex.erase(ex.begin() + pos);
	}

	if (!ex.size()) 
	{ return false; }

	function<bool(const string&)> validate_brackets = [](const string &s)->bool {
		stack<char> q;
		for (auto &c : s) {
			if (c == '{' || c == '[' || c == '(')
			{ q.push(c); }
			else if (!q.empty() && (
				(c == '}' && q.top() == '{') ||
				(c == ']' && q.top() == '[') ||
				(c == ')' && q.top() == '(')))
			{ q.pop(); }
			else if (q.empty() && (
				(c == '}') ||
				(c == ']') ||
				(c == ')')))
			{ return false; }
		}
		return (q.empty());
	};

	// Validate brackets
	if ((ex.find_first_of("()[]{}") != string::npos) &&
		(!validate_brackets(ex))) 
	{ return false; }

	while (((open_bks = ex.find_last_of("([{")) != string::npos) && 
		   ((close_bks = ex.substr(open_bks).find_first_of(")]}")) != string::npos)) {
		int simple_res;
		stringstream ss;
		size_t simple_pos;
		string simple_expr = ex.substr(open_bks, close_bks + 1), res_str;
		if (!solve(simple_expr.substr(1, simple_expr.size() - 2), simple_res)) return false;
		ss << simple_res;
		res_str = ss.str();
		while ((simple_pos = ex.find(simple_expr)) != string::npos )
		{ ex.replace(simple_pos, simple_expr.size(), res_str); }
	}

	while ((solve_pos = ex.find_first_of("*/")) != string::npos) {
		size_t solve_end = solve_pos+1, solve_begin = solve_pos-1;
		int lval, rval;
		for (; solve_begin > 0 && ex[solve_begin] >= '0' && ex[solve_begin] <= '9'; --solve_begin);
		if (solve_begin) solve_begin++;
		for (; solve_end < ex.size() && ex[solve_end] >= '0' && ex[solve_end] <= '9'; solve_end++);
		lval = stoi(ex.substr(solve_begin, solve_pos));
		rval = stoi(ex.substr(solve_pos + 1, solve_end));
		if (ex[solve_pos] == '*') partial_res = lval * rval;
		else partial_res = lval / rval;
		stringstream ss;
		string exp = ex.substr(solve_begin, solve_end - solve_begin);
		ss << partial_res;
		while ((solve_pos = ex.find(exp)) != string::npos) {
			ex.replace(solve_pos, exp.size(), ss.str());
		}
	}

	while ((solve_pos = ex.find_first_of("+-")) != string::npos) {
		size_t solve_end = solve_pos + 1, solve_begin = solve_pos - 1;
		int lval, rval;
		for (; solve_begin > 0 && ex[solve_begin] >= '0' && ex[solve_begin] <= '9'; --solve_begin);
		if (solve_begin) solve_begin++;
		for (; solve_end < ex.size() && ex[solve_end] >= '0' && ex[solve_end] <= '9'; solve_end++);
		lval = stoi(ex.substr(solve_begin, solve_pos));
		rval = stoi(ex.substr(solve_pos + 1, solve_end));
		if (ex[solve_pos] == '+') partial_res = lval + rval;
		else partial_res = lval - rval;
		stringstream ss;
		string exp = ex.substr(solve_begin, solve_end - solve_begin);
		ss << partial_res;
		while ((solve_pos = ex.find(exp)) != string::npos) {
			ex.replace(solve_pos, exp.size(), ss.str());
		}
	}
	result = partial_res;
	return true;
}

int main(char *argv[], int argc) {
	int n;
	cin >> n;
	double result;
	while (n--) {
		string expression;
		cin >> expression;
		if (evaluate(expression, result)) {
			cout << result << endl;
		}
		else {
			cout << "Not valid" << endl;
		}
	}
	return 0;
}
