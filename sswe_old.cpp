#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
using namespace std;


vector<int> NULL_IDX(1,-1);


void print_R_arr(vector< vector<string> > R_arr) {
    vector <string> R;
    for (int i=0; i<R_arr.size(); i++){
        R = R_arr[i];
        for (int j=0; j<R.size(); j++){
            cout << R[j] << " ";
        }
        cout << endl;
    }
}


void print_idx(vector<int> idx){
    for (int i=0; i<idx.size(); i++)
        cout << idx[i] << " ";

    cout << endl;
}


void print_opts(vector< set<int> > opts) {
    set<int> idx_set;
    set<int>::iterator it;
    for (int i=0; i<opts.size(); i++) {
        idx_set = opts[i];
        it = idx_set.begin();
        while (it != idx_set.end()) {
            cout << (*it) << " ";
            it++;
        }
        cout << endl;
    }
}


void print_opts_vec(vector< vector<int> > opts) {
    for (int i=0; i<opts.size(); i++) {
        for (int j=0; j<opts[i].size(); j++){
            cout << opts[i][j] << " ";
        }
        cout << endl;
    }
}


void print_sol(vector<int> idx_arr, vector< vector<string> > R_arr) {
    for (int i=0; i<R_arr.size(); i++) {
        cout << (char)(i+65) << ":" << R_arr[i][idx_arr[i]] << endl;
    }
}


bool verify_input(int k, string s, vector<string> t_arr, vector< vector<string> > R_arr) {
    /* 
    TODO: Implement checks. 
        1) All characters are {0,..,9}, lower case or upper case letters 
        2) If len(R_arr) <= 26
    */

    if (R_arr.size() > 26)
        return false;

    if (t_arr.size() != k)
        return false;

    // Nothing but upper/lower case in t_arr
    string t;
    int c;
    for (int i=0; i<t_arr.size(); i++) {
        t = t_arr[i];
        for (int j=0; j<t.length(); j++) {
            c = (int)t.at(j);
            if (c < 65 || (c > 90 && c < 97) || c > 122)
                return false;
            if (c > 64 && c < 91) {
                if (R_arr.size() < c-64 || R_arr[c-65].size() == 0)
                    return false;
            }
        }
    }

    // S all lower case
    for (int i=0; i<s.length(); i++) {
        c = (int)s.at(i);
        if (c < 97 || c > 122)
            return false;
    }

    return true;
}


vector<string> get_r(vector<int> idx_arr, vector< vector<string> > R_arr) {
    vector<string> r;
    for (int i=0; i<R_arr.size(); i++) {
        if (idx_arr[i] != -1)
            r.push_back(R_arr[i][idx_arr[i]]);
        else
            r.push_back(R_arr[i][0]);
    }

    return r;
}


bool test(string s, string t, vector<string> r) {
    string exp;
    int c;
    for (int i=0; i<t.length(); i++) {
        c = (int) t.at(i);
        if (c < 97)
            exp += r[c-65];
        else 
            exp += t.at(i);
    }

    if (s.find(exp) != string::npos)
        return true;
    else
        return false;
}


bool test_all(string s, vector<string> t_arr, vector<string> r) {
    for (int i=0; i<t_arr.size(); i++) {
        if (!test(s, t_arr[i], r))
            return false;
    }

    return true;
}


vector<int> increment(vector<int> idx_arr, vector<int> max_arr) {
    // TODO: Skip indexes that don't match previous solutions in opts.
    //       If we know idx_arr[0] must be in the set [0,1,4,6] don't bother
    //       checking other possibilities.
    if (idx_arr == max_arr)
        return NULL_IDX;

    for (int i=idx_arr.size()-1; i>=0; i--) {
        if (idx_arr[i] != -1){
            idx_arr[i]++;
            if (idx_arr[i] > max_arr[i])
                idx_arr[i] = 0;
            else
                break;
        }
    }

    return idx_arr;
}


vector< set<int> > get_opts(string s, string t, vector< vector<string> > R_arr, vector< set<int> > opts){
    // Init idx and max arrays
    vector<int> idx_arr(26,-1);
    vector<int> max_arr(26,-1);
    int c;
    for (int i=0; i<t.length(); i++){
        c = (int)t.at(i);
        // Upper case letters start at 65, lower case start at 97
        if (c < 97) {
            idx_arr[c-65] = 0;
            max_arr[c-65] = R_arr[c-65].size() - 1;
        }
    }

    vector<string> r;
    while (idx_arr != NULL_IDX) {
        r = get_r(idx_arr, R_arr);
        if (test(s, t, r)){
            for(int i=0; i<idx_arr.size(); i++) {
                // TODO: Only insert if set is empty or contains only -1
                //       since the solution must work for previous examples too
                if (i == opts.size()) {
                    set<int> opt;
                    opt.insert(idx_arr[i]);
                    opts.push_back(opt);
                } else {
                    opts[i].insert(idx_arr[i]);
                }
            }
        }

        idx_arr = increment(idx_arr, max_arr);
    }

    return opts;
}


vector< vector<int> > get_opts_vec(vector< set<int> > opts, int m) {
    vector< vector<int> > opts_vec;
    set<int> opt;
    set<int>::iterator it;
    for (int i=0; i<m; i++) {
        opt = opts[i];
        // If opt contains only -1, replace it with 0
        if (opt.size() == 1 && opt.find(-1) != opt.end()) {
            opt.insert(0);
        } 

        // Remove -1 from the option pool
        if (opt.find(-1) != opt.end())
            opt.erase(opt.find(-1));

        // Add the cleaned option pool to clean_opts
        vector<int> opt_vec;
        for (it=opt.begin(); it!=opt.end(); it++){
            opt_vec.push_back((*it));
        }

        opts_vec.push_back(opt_vec);
    }

    return opts_vec;
}


int main() {
    /* READ INPUT */
    int k; cin >> k;
    string s; cin >> s;

    if (k==0) {
        cout << "NO" << endl;
        return 0;
    }

    vector< vector<string> > R_arr;
    vector<string> t_arr;
    string str;
    while (cin >> str){
        if (str.length() < 2 || (int)str.at(1) != 58) {
            if (R_arr.size() == 0) {
                t_arr.push_back(str);
            } else {
                cout << "NO" << endl;
                return 0;
            }
        } else if ((int)str.at(0) <= 90) {
            str = str.substr(2);  // Cut out key letter and ':'
            vector<string> R;
            stringstream str_stream(str); //create string stream from the string
            while(str_stream.good()) {
                string substr;
                getline(str_stream, substr, ',');  //get first string delimited by comma
                if (s.find(substr) != string::npos)
                    R.push_back(substr);
            }
            R_arr.push_back(R);
        } else {
            cout << "NO" << endl;
            return 0;
        }
    }

    /* VERIFY INPUT */
    if(!verify_input(k, s, t_arr, R_arr)) {
        cout << "NO" << endl;
        return 0;
    }

    /* COMPUTE SOLUTION TO EACH TEST CASE */
    vector< vector<int> > opts_vec;
    vector< set<int> > opts;
    string t;
    for (int i=0; i<t_arr.size(); i++){
        t = t_arr[i];
        opts = get_opts(s, t, R_arr, opts);
    }
    opts_vec = get_opts_vec(opts, R_arr.size());

    /* CHECK ALL PERMUTATIONS AND COMBINATIONS OF OPTS */
    vector<int> idx_arr(26,0);
    vector<int> max_arr(26,0);
    for (int i=0; i<opts_vec.size(); i++){
        max_arr[i] = opts_vec[i].size() - 1;
    }

    bool solved = false;
    vector<string> r;
    while (idx_arr != NULL_IDX) {
        r = get_r(idx_arr, R_arr);
        if (test_all(s, t_arr, r)) {
            solved = true;
            break;
        }

        idx_arr = increment(idx_arr, max_arr);
    }

    if (solved) {
        print_sol(idx_arr, R_arr);
    } else {
        cout << "NO" << endl;
    }
}