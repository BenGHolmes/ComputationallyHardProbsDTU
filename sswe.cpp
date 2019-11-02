#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;


const int IDX_LEN = 26;
const int VALID = 1;
const int INVALID = 0;


bool t_sort(string a, string b) {
    /* Sort by descinding number of GAMMA letters in string */
    int a_sum = 0;
    int b_sum = 0;
    for (char c : a) {
        if ((int)c < 97){
            ++a_sum;
        }
    }

    for (char c : b) {
        if ((int)c < 97){
            ++b_sum;
        }
    }

    return a_sum > b_sum;
}


void print_idx(int *idx){
    for (int i=0; i<IDX_LEN; i++)
        cout << idx[i] << " ";

    cout << endl;
}


void print_opts_vec(vector< vector<int> > opts) {
    for (int i=0; i<opts.size(); i++) {
        for (int j=0; j<opts[i].size(); j++){
            cout << opts[i][j] << " ";
        }
        cout << endl;
    }
}


int pass(int *idx, vector< vector<string> > R_arr) {
    for (int i=0; i<R_arr.size(); i++){
        cout << (char)(65+i) << ":" << R_arr[i][idx[i]] << endl;
    }
    return 0;
}


int fail() {
    cout << "NO" << endl;
    return 0;
}


bool equal(int *arr1, int *arr2) {
    for (int i=0; i<IDX_LEN; i++){
        if (arr1[i] != arr2[i])
            return false;
    }   
    return true;
}


void increment(int *idx_arr, int *max_arr) {
   for (int i=IDX_LEN-1; i>-1; i--) {
        if (idx_arr[i] != -1) {
            idx_arr[i]++;
            if (idx_arr[i] > max_arr[i])
                idx_arr[i] = 0;
            else
                break;
        }
    }
}


int validate(int k, string s, vector<string> t_arr, vector< vector<string> > R_arr) {
    if (k == 0)
        return INVALID;
    
    if (R_arr.size() > 26)
        return INVALID;

    if (t_arr.size() != k)
        return INVALID;

    // Check characters of t_arr
    string t;
    int c;
    for (int i=0; i<t_arr.size(); i++) {
        t = t_arr[i];
        for (int j=0; j<t.length(); j++) {
            c = (int)t.at(j);
            // Invalid if letter is something besides upper/lower case
            if (c < 65 || (c > 90 && c < 97) || c > 122)
                return INVALID;
            // Invalid if letter is Upper case but not in R_arr
            if (c > 64 && c < 91) {
                if (R_arr.size()-1 < c-65 || R_arr[c-65].size() == 0)
                    return INVALID;
            }
        }
    }

    // S all lower case
    for (int i=0; i<s.length(); i++) {
        c = (int)s.at(i);
        if (c < 97 || c > 122)
            return INVALID;
    }

    return VALID;
}


vector<string> get_r(int *idx_arr, vector< vector<string> > R_arr) {
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


vector< vector<int> > get_opts_vec(vector< set<int> > opts) {
    vector< vector<int> > opts_vec;
    set<int> opt;
    set<int>::iterator it;
    for (int i=0; i<IDX_LEN; i++) {
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


vector< set<int> > get_opts(string s, string t, vector< vector<string> > R_arr, vector< set<int> > opts){
    // Init idx and max arrays
    int idx_arr[IDX_LEN];
    int max_arr[IDX_LEN];

    for (int i=0; i<IDX_LEN; i++){
        idx_arr[i] = -1;
        max_arr[i] = -1;
    }

    int can_edit[IDX_LEN];
    for (int i=0; i<IDX_LEN; i++){
        can_edit[i] = (i >= opts.size()) || (opts[i].size() == 1 && opts[i].find(-1) != opts[i].end());
    }

    int c;
    int m = 1;
    for (int i=0; i<t.length(); i++){
        c = (int)t.at(i);
        // Upper case letters start at 65, lower case start at 97
        if (c < 97) {
            idx_arr[c-65] = 0;
            max_arr[c-65] = R_arr[c-65].size() - 1;
            m *= max_arr[c-65]+1;
        }
    }

    vector<string> r;
    for (int c=0; c<m; c++) {
        r = get_r(idx_arr, R_arr);
        if (test(s, t, r)){
            for(int i=0; i<IDX_LEN; i++) {
                // TODO: Only insert if set is empty or contains only -1
                //       since the solution must work for previous examples too
                if (i == opts.size()) {
                    set<int> opt;
                    opt.insert(idx_arr[i]);
                    opts.push_back(opt);
                } else {
                    if (can_edit[i])
                        opts[i].insert(idx_arr[i]);
                }
            }
        }
        increment(idx_arr, max_arr);
    }

    return opts;
}


int main() {
    /* READ INPUT */
    int k; cin >> k;
    string s; cin >> s;

    vector< vector<string> > R_arr;
    vector<string> t_arr;
    string str;
    while (cin >> str) {
        // If there is no colon in the word, assume it is t_i
        // else assume it is R_i
        if (str.length() < 2 || (int)str.at(1) != 58) {
            // If we've already started R_arr, input is invalid
            if (R_arr.size() > 0) {
                return fail();
            }
            t_arr.push_back(str);
        } else {
            // Fail if R_i key is lower case, or no colon in str
            if ((int)str.at(0) >= 97 || (int)str.at(1) != 58) {
                return fail();
            }
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
        }
    }

    /* VALIDATE INPUT */
    if (validate(k, s, t_arr, R_arr) == INVALID) {
        return fail();
    }

    /* Sort t_arr by descending number of upper case letters
       to tackle most restrictive cases first. */
    sort(t_arr.begin(), t_arr.end(), t_sort);

    /* COMPUTE SOLUTION TO EACH TEST CASE */
    vector< vector<int> > opts_vec;
    vector< set<int> > opts;
    string t;
    for (int i=0; i<t_arr.size(); i++){
        t = t_arr[i];
        opts = get_opts(s, t, R_arr, opts);
    }
    opts_vec = get_opts_vec(opts);

    /* CHECK ALL PERMUTATIONS AND COMBINATIONS OF OPTS */
    int opt_idx[26];
    int max_arr[26];
    int idx[26];
    int m=1;
    for (int i=0; i<IDX_LEN; i++){
        opt_idx[i] = 0;
        max_arr[i] = opts_vec[i].size() - 1;
        m *= max_arr[i]+1;
    }

    bool solved = false;
    vector<string> r;
    for (int c=0; c<m; c++) {
        for (int i=0; i<IDX_LEN; i++) {
            idx[i] = opts_vec[i][opt_idx[i]];
        }
        r = get_r(idx, R_arr);
        if (test_all(s, t_arr, r)) {
            solved = true;
            break;
        }
        increment(opt_idx, max_arr);
    }

    if (solved) {
        return pass(idx, R_arr);
    } else {
        return fail();
    }
}
