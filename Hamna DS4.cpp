// File: Hamna_UNIDISC.cpp
// Single-file, fully functional UNIDISC Engine (Hamna's modules)
// Constraints: only <iostream>, <vector>, <string>, <fstream>, <functional>, <algorithm>
// Uses cout/cin and endl. No map, no advanced libs beyond above.

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <functional>
#include <algorithm>

using namespace std;

// --------------------------- CourseManager ---------------------------
class CourseManager {
public:
    struct Course {
        int id;
        string code;
        string name;
        int credits;
        Course(int i = -1, string c = "", string n = "", int cr = 0) : id(i), code(c), name(n), credits(cr) {}
    };

    int addCourse(const string& code, const string& name, int credits) {
        int id = static_cast<int>(courses.size());
        courses.push_back(Course(id, code, name, credits));
        return id;
    }

    const Course* getCourse(int id) const {
        if (id < 0 || id >= static_cast<int>(courses.size())) return nullptr;
        return &courses[id];
    }

    int count() const { return static_cast<int>(courses.size()); }

    void listAll() const {
        cout << "Courses:" << endl;
        for (size_t i = 0; i < courses.size(); ++i) {
            cout << courses[i].id << " | " << courses[i].code << " | " << courses[i].name
                << " | credits: " << courses[i].credits << endl;
        }
    }

private:
    vector<Course> courses;
};

// --------------------------- PeopleManager ---------------------------
class PeopleManager {
public:
    struct Student {
        int id;
        string name;
        vector<int> enrolled; // course ids
        Student(int i = -1, string n = "") : id(i), name(n) {}
    };

    struct Faculty {
        int id;
        string name;
        Faculty(int i = -1, string n = "") : id(i), name(n) {}
    };

    struct Room {
        int id;
        string name;
        Room(int i = -1, string n = "") : id(i), name(n) {}
    };

    int addStudent(const string& name) {
        int id = static_cast<int>(students.size());
        students.push_back(Student(id, name));
        return id;
    }
    int addFaculty(const string& name) {
        int id = static_cast<int>(faculties.size());
        faculties.push_back(Faculty(id, name));
        return id;
    }
    int addRoom(const string& name) {
        int id = static_cast<int>(rooms.size());
        rooms.push_back(Room(id, name));
        return id;
    }

    Student* getStudent(int id) {
        if (id < 0 || id >= static_cast<int>(students.size())) return nullptr;
        return &students[id];
    }
    const Student* getStudentConst(int id) const {
        if (id < 0 || id >= static_cast<int>(students.size())) return nullptr;
        return &students[id];
    }
    const vector<Student>& listStudents() const { return students; }
    const vector<Faculty>& listFaculties() const { return faculties; }
    const vector<Room>& listRooms() const { return rooms; }

    Faculty* getFaculty(int id) {
        if (id < 0 || id >= static_cast<int>(faculties.size())) return nullptr;
        return &faculties[id];
    }
    Room* getRoom(int id) {
        if (id < 0 || id >= static_cast<int>(rooms.size())) return nullptr;
        return &rooms[id];
    }

    void listAllStudents() const {
        cout << "Students:" << endl;
        for (size_t i = 0; i < students.size(); ++i) {
            cout << students[i].id << " | " << students[i].name << " | enrolled: ";
            for (size_t j = 0; j < students[i].enrolled.size(); ++j) {
                cout << students[i].enrolled[j];
                if (j + 1 < students[i].enrolled.size()) cout << ",";
            }
            cout << endl;
        }
    }

    void listAllFaculties() const {
        cout << "Faculty:" << endl;
        for (size_t i = 0; i < faculties.size(); ++i) {
            cout << faculties[i].id << " | " << faculties[i].name << endl;
        }
    }

    void listAllRooms() const {
        cout << "Rooms:" << endl;
        for (size_t i = 0; i < rooms.size(); ++i) {
            cout << rooms[i].id << " | " << rooms[i].name << endl;
        }
    }

private:
    vector<Student> students;
    vector<Faculty> faculties;
    vector<Room> rooms;
};

// --------------------------- Relations ---------------------------
// relations over course ids: prereq and implication
class Relations {
public:
    Relations() {}

    void ensureSize(int n) {
        if (n <= 0) return;
        int old = static_cast<int>(prereq.size());
        if (n == old) return;
        prereq.assign(n, vector<bool>(n, false));
        implication.assign(n, vector<bool>(n, false));
    }

    void addPrereq(int a, int b) {
        if (!validIndexForEnsure(a) || !validIndexForEnsure(b)) return;
        prereq[a][b] = true;
    }
    void addImplication(int a, int b) {
        if (!validIndexForEnsure(a) || !validIndexForEnsure(b)) return;
        implication[a][b] = true;
    }

    bool getPrereq(int a, int b) const {
        if (!validIndex(a) || !validIndex(b)) return false;
        return prereq[a][b];
    }

    int size() const { return static_cast<int>(prereq.size()); }

    // compute transitive closure of prereq
    vector<vector<bool>> closurePrereq() const {
        int n = size();
        vector<vector<bool>> c = prereq;
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) if (c[i][k]) {
                for (int j = 0; j < n; ++j) if (c[k][j]) c[i][j] = true;
            }
        }
        return c;
    }

    // compute closure of implication
    vector<vector<bool>> closureImplication() const {
        int n = size();
        vector<vector<bool>> c = implication;
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) if (c[i][k]) {
                for (int j = 0; j < n; ++j) if (c[k][j]) c[i][j] = true;
            }
        }
        return c;
    }

    bool hasPrereqCycle() const {
        vector<vector<bool>> c = closurePrereq();
        int n = size();
        for (int i = 0; i < n; ++i) if (c[i][i]) return true;
        return false;
    }

    const vector<vector<bool>>& getPrereqMatrix() const { return prereq; }
    const vector<vector<bool>>& getImplicationMatrix() const { return implication; }

private:
    vector<vector<bool>> prereq;
    vector<vector<bool>> implication;

    bool validIndex(int x) const {
        return x >= 0 && x < static_cast<int>(prereq.size());
    }
    bool validIndexForEnsure(int x) {
        if (x < 0) return false;
        if (x >= static_cast<int>(prereq.size())) {
            int newn = x + 1;
            ensureSize(newn);
        }
        return true;
    }
};

// --------------------------- Scheduling (topological orders) ---------------------------
class Scheduling {
public:
    Scheduling(const Relations& r) : relations(r) {}

    // list up to limit topological orders for n courses (0..n-1)
    vector<vector<int>> listOrders(int n, int limit = 200) const {
        vector<vector<int>> orders;
        if (n <= 0) return orders;
        // adjacency from relations.prereq
        vector<vector<bool>> adj = relations.getPrereqMatrix();
        if (static_cast<int>(adj.size()) < n) adj.assign(n, vector<bool>(n, false));
        vector<int> order;
        vector<bool> used(n, false);
        int counter = 0;
        backtrack(adj, used, order, orders, n, counter, limit);
        return orders;
    }

private:
    const Relations& relations;

    void backtrack(const vector<vector<bool>>& adj, vector<bool>& used, vector<int>& order,
        vector<vector<int>>& out, int n, int& counter, int limit) const {
        if (counter >= limit) return;
        if (static_cast<int>(order.size()) == n) {
            out.push_back(order);
            ++counter;
            return;
        }
        for (int i = 0; i < n; ++i) {
            if (used[i]) continue;
            bool incoming = false;
            for (int j = 0; j < n; ++j) if (!used[j] && adj[j][i]) { incoming = true; break; }
            if (!incoming) {
                used[i] = true;
                order.push_back(i);
                backtrack(adj, used, order, out, n, counter, limit);
                order.pop_back();
                used[i] = false;
                if (counter >= limit) return;
            }
        }
    }
};

// --------------------------- Induction Module ---------------------------
class Induction {
public:
    Induction(const Relations& r) : relations(r) {}

    // recursive check: verify prerequisites using DFS-like recursion.
    bool checkRecursive(int courseId, const vector<bool>& completed) const {
        int n = relations.size();
        if (courseId < 0 || courseId >= n) return false;
        vector<vector<bool>> adj = relations.getPrereqMatrix();
        vector<int> visited(n, 0);
        function<bool(int)> dfs = [&](int v)->bool {
            if (v >= 0 && v < static_cast<int>(completed.size()) && completed[v]) return true;
            if (visited[v]) return false;
            visited[v] = 1;
            for (int u = 0; u < n; ++u) if (adj[u][v]) {
                if (!dfs(u)) return false;
            }
            if (v >= 0 && v < static_cast<int>(completed.size())) return completed[v];
            return false;
            };
        return dfs(courseId);
    }

    // strong induction: check all indirect prerequisites are in completed
    bool checkStrong(int courseId, const vector<bool>& completed) const {
        int n = relations.size();
        if (courseId < 0 || courseId >= n) return false;
        vector<vector<bool>> cl = relations.closurePrereq();
        for (int i = 0; i < n; ++i) if (cl[i][courseId]) {
            if (!(i >= 0 && i < static_cast<int>(completed.size()))) return false;
            if (!completed[i]) return false;
        }
        return true;
    }

    // list indirect prereqs
    vector<int> listIndirect(int courseId) const {
        vector<int> out;
        int n = relations.size();
        if (courseId < 0 || courseId >= n) return out;
        vector<vector<bool>> cl = relations.closurePrereq();
        for (int i = 0; i < n; ++i) if (cl[i][courseId]) out.push_back(i);
        return out;
    }

private:
    const Relations& relations;
};

// --------------------------- Logic & Inference ---------------------------
class Logic {
public:
    Logic(const Relations& r) : relations(r) {}

    // apply implication closure to an initial facts vector<bool>
    vector<bool> applyImplications(vector<bool> facts) const {
        int n = relations.size();
        if (static_cast<int>(facts.size()) < n) facts.resize(n, false);
        vector<vector<bool>> impl = relations.closureImplication();
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < n; ++i) if (facts[i]) {
                for (int j = 0; j < n; ++j) if (impl[i][j] && !facts[j]) {
                    facts[j] = true;
                    changed = true;
                }
            }
        }
        return facts;
    }

private:
    const Relations& relations;
};

// --------------------------- Set Operations ---------------------------
class SetOps {
public:
    static vector<int> unionSet(const vector<int>& A, const vector<int>& B) {
        vector<int> out = A;
        for (int x : B) {
            if (!contains(out, x)) out.push_back(x);
        }
        return out;
    }
    static vector<int> intersection(const vector<int>& A, const vector<int>& B) {
        vector<int> out;
        for (int x : A) if (contains(B, x)) out.push_back(x);
        return out;
    }
    static vector<int> difference(const vector<int>& A, const vector<int>& B) {
        vector<int> out;
        for (int x : A) if (!contains(B, x)) out.push_back(x);
        return out;
    }
    static vector<vector<int>> powerSet(const vector<int>& A) {
        vector<vector<int>> out;
        int n = static_cast<int>(A.size());
        if (n > 20) return out; // guard
        int total = 1 << n;
        for (int mask = 0; mask < total; ++mask) {
            vector<int> subset;
            for (int i = 0; i < n; ++i) if (mask & (1 << i)) subset.push_back(A[i]);
            out.push_back(subset);
        }
        return out;
    }

private:
    static bool contains(const vector<int>& v, int x) {
        for (int a : v) if (a == x) return true;
        return false;
    }
};

// --------------------------- Functions Module ---------------------------
class Functions {
public:
    // domain: list of domain elements (course ids), codomain: assigned faculty ids
    static bool isFunction(const vector<int>& domain, const vector<int>& codomain) {
        return domain.size() == codomain.size();
    }
    static bool isInjective(const vector<int>& domain, const vector<int>& codomain) {
        if (!isFunction(domain, codomain)) return false;
        for (size_t i = 0; i < codomain.size(); ++i) {
            for (size_t j = i + 1; j < codomain.size(); ++j) {
                if (codomain[i] == codomain[j]) return false;
            }
        }
        return true;
    }
    static bool isSurjective(const vector<int>& domain, const vector<int>& codomain, int codomainSize) {
        if (!isFunction(domain, codomain)) return false;
        if (codomainSize <= 0) return false;
        vector<bool> seen(codomainSize, false);
        for (int v : codomain) if (v >= 0 && v < codomainSize) seen[v] = true;
        for (int i = 0; i < codomainSize; ++i) if (!seen[i]) return false;
        return true;
    }
    static bool isBijective(const vector<int>& domain, const vector<int>& codomain, int codomainSize) {
        return isInjective(domain, codomain) && isSurjective(domain, codomain, codomainSize);
    }
};

// --------------------------- Consistency ---------------------------
class Consistency {
public:
    Consistency(const Relations& r, const CourseManager& cm, PeopleManager& pm,
        const vector<int>& courseToFaculty, const vector<int>& courseToRoom)
        : relations(r), courseMgr(cm), peopleMgr(pm),
        cToF(courseToFaculty), cToR(courseToRoom) {
    }

    // run various checks and print results
    void runAllChecks() const {
        cout << "=== Consistency Report ===" << endl;
        checkPrereqCycles();
        checkStudentsPrereqs();
        checkStudentOverload();
        checkFacultyRoomConflicts();
        cout << "=== End Report ===" << endl;
    }

private:
    const Relations& relations;
    const CourseManager& courseMgr;
    PeopleManager& peopleMgr;
    const vector<int>& cToF;
    const vector<int>& cToR;

    void checkPrereqCycles() const {
        if (relations.hasPrereqCycle()) cout << "Error: prerequisite cycles detected." << endl;
        else cout << "Prerequisite relation appears acyclic." << endl;
    }

    void checkStudentsPrereqs() const {
        vector<vector<bool>> cl = relations.closurePrereq();
        int n = relations.size();
        const vector<PeopleManager::Student>& students = peopleMgr.listStudents();
        for (const auto& s : students) {
            vector<bool> enrolled(n, false);
            for (int c : s.enrolled) if (c >= 0 && c < n) enrolled[c] = true;
            for (int c : s.enrolled) {
                if (c < 0 || c >= n) continue;
                for (int p = 0; p < n; ++p) if (cl[p][c]) {
                    if (!enrolled[p]) {
                        const CourseManager::Course* pc = courseMgr.getCourse(p);
                        const CourseManager::Course* cc = courseMgr.getCourse(c);
                        string pname = pc ? pc->code : ("C#" + to_string(p));
                        string cname = cc ? cc->code : ("C#" + to_string(c));
                        cout << "Student " << s.name << " missing prereq " << pname << " for " << cname << "." << endl;
                    }
                }
            }
        }
    }

    void checkStudentOverload() const {
        const vector<PeopleManager::Student>& students = peopleMgr.listStudents();
        for (const auto& s : students) {
            if (static_cast<int>(s.enrolled.size()) > 6) {
                cout << "Warning: student " << s.name << " enrolled in " << s.enrolled.size() << " courses (possible overload)." << endl;
            }
        }
    }

    void checkFacultyRoomConflicts() const {
        int n = courseMgr.count();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int f1 = (i < static_cast<int>(cToF.size()) ? cToF[i] : -1);
                int f2 = (j < static_cast<int>(cToF.size()) ? cToF[j] : -1);
                int r1 = (i < static_cast<int>(cToR.size()) ? cToR[i] : -1);
                int r2 = (j < static_cast<int>(cToR.size()) ? cToR[j] : -1);
                if (f1 != -1 && f1 == f2 && r1 != -1 && r1 == r2) {
                    const PeopleManager::Faculty* fa = peopleMgr.getFaculty(f1);
                    const PeopleManager::Room* ro = peopleMgr.getRoom(r1);
                    cout << "Conflict: Faculty " << (fa ? fa->name : ("F#" + to_string(f1)))
                        << " assigned to multiple courses in room " << (ro ? ro->name : ("R#" + to_string(r1))) << "." << endl;
                }
            }
        }
    }
};

// --------------------------- Recommender ---------------------------
class Recommender {
public:
    Recommender(const Relations& r, const CourseManager& cm) : relations(r), courseMgr(cm) {}

    // suggest courses whose all indirect prereqs are completed
    vector<int> suggest(const vector<bool>& completed, int maxSuggest = 10) const {
        vector<int> out;
        int n = relations.size();
        vector<vector<bool>> cl = relations.closurePrereq();
        for (int c = 0; c < n; ++c) {
            if (c < static_cast<int>(completed.size()) && completed[c]) continue;
            bool ok = true;
            for (int p = 0; p < n; ++p) if (cl[p][c]) {
                if (!(p < static_cast<int>(completed.size()) && completed[p])) { ok = false; break; }
            }
            if (ok) out.push_back(c);
            if (static_cast<int>(out.size()) >= maxSuggest) break;
        }
        return out;
    }

private:
    const Relations& relations;
    const CourseManager& courseMgr;
};

// --------------------------- Proof Exporter ---------------------------
class ProofExporter {
public:
    ProofExporter(const Relations& r, const CourseManager& cm) : relations(r), courseMgr(cm) {}

    // generate textual proof for prerequisites of course c given completed vector
    string generateProof(int c, const vector<bool>& completed) const {
        string proof;
        const CourseManager::Course* course = courseMgr.getCourse(c);
        string cname = course ? course->code : ("C#" + to_string(c));
        proof += "Proof for prerequisites of ";
        proof += cname;
        proof += ".";
        proof += "\n";
        int n = relations.size();
        if (c < 0 || c >= n) {
            proof += "Invalid course id.";
            proof += "\n";
            return proof;
        }
        vector<vector<bool>> cl = relations.closurePrereq();
        bool any = false;
        for (int i = 0; i < n; ++i) if (cl[i][c]) {
            any = true;
            const CourseManager::Course* pc = courseMgr.getCourse(i);
            string pname = pc ? pc->code : ("C#" + to_string(i));
            proof += "- Indirect prerequisite: ";
            proof += pname;
            proof += " : ";
            proof += (i < static_cast<int>(completed.size()) && completed[i]) ? "COMPLETED" : "MISSING";
            proof += ".";
            proof += "\n";
        }
        if (!any) {
            proof += "- No prerequisites found.";
            proof += "\n";
        }
        bool ok = true;
        for (int i = 0; i < n; ++i) if (cl[i][c]) {
            if (!(i < static_cast<int>(completed.size()) && completed[i])) { ok = false; break; }
        }
        if (ok) {
            proof += "Conclusion: All prerequisites satisfied for ";
            proof += cname;
            proof += ".";
            proof += "\n";
        }
        else {
            proof += "Conclusion: Prerequisites NOT satisfied for ";
            proof += cname;
            proof += ".";
            proof += "\n";
        }
        return proof;
    }

    bool saveToFile(const string& filename, const string& content) const {
        ofstream ofs(filename.c_str());
        if (!ofs.is_open()) return false;
        ofs << content;
        ofs.close();
        return true;
    }

private:
    const Relations& relations;
    const CourseManager& courseMgr;
};

// --------------------------- Interactive CLI ---------------------------
int main() {
    CourseManager cm;
    PeopleManager pm;
    Relations rel;
    vector<int> courseToFaculty; // index by course id
    vector<int> courseToRoom;    // index by course id

    cout << "Welcome to UNIDISC ENGINE " << endl;

    while (true) {
        cout << endl;
        cout << "===== MENU =====" << endl;
        cout << "1 Add Course" << endl;
        cout << "2 Add Student" << endl;
        cout << "3 Add Faculty" << endl;
        cout << "4 Add Room" << endl;
        cout << "5 Add Prerequisite (A -> B) by course id" << endl;
        cout << "6 Add Implication (A -> B) by course id" << endl;
        cout << "7 Assign Faculty to Course" << endl;
        cout << "8 Assign Room to Course" << endl;
        cout << "9 Enroll Student in Course" << endl;
        cout << "10 List Courses/Students/Faculty/Rooms" << endl;
        cout << "11 List valid course sequences (topological orders, limited)" << endl;
        cout << "12 Induction checks (recursive & strong)" << endl;
        cout << "13 Apply inference engine (implications)" << endl;
        cout << "14 Set operations for two students (union/intersection/diff/power set)" << endl;
        cout << "15 Functions checks for course->faculty mapping" << endl;
        cout << "16 Consistency report" << endl;
        cout << "17 Smart recommendations for a student" << endl;
        cout << "18 Generate & save proof for a course" << endl;
        cout << "0 Exit" << endl;
        cout << "Enter choice: ";
        int choice; cin >> choice;

        if (choice == 0) {
            cout << "Exiting. Goodbye." << endl;
            break;
        }

        if (choice == 1) {
            string code, name; int credits;
            cout << "Course code (no spaces): "; cin >> code;
            cout << "Course name (no spaces): "; cin >> name;
            cout << "Credits: "; cin >> credits;
            int id = cm.addCourse(code, name, credits);
            rel.ensureSize(cm.count());
            if (static_cast<int>(courseToFaculty.size()) < cm.count()) {
                courseToFaculty.resize(cm.count(), -1);
                courseToRoom.resize(cm.count(), -1);
            }
            cout << "Added course id=" << id << endl;
        }
        else if (choice == 2) {
            string name; cout << "Student name (no spaces): "; cin >> name;
            int id = pm.addStudent(name);
            cout << "Added student id=" << id << endl;
        }
        else if (choice == 3) {
            string name; cout << "Faculty name (no spaces): "; cin >> name;
            int id = pm.addFaculty(name);
            cout << "Added faculty id=" << id << endl;
        }
        else if (choice == 4) {
            string name; cout << "Room name (no spaces): "; cin >> name;
            int id = pm.addRoom(name);
            cout << "Added room id=" << id << endl;
        }
        else if (choice == 5) {
            int a, b; cout << "Enter prerequisite course id A (prereq): "; cin >> a;
            cout << "Enter dependent course id B (dependent): "; cin >> b;
            rel.ensureSize(cm.count());
            rel.addPrereq(a, b);
            cout << "Prerequisite added: " << a << " -> " << b << endl;
        }
        else if (choice == 6) {
            int a, b; cout << "Enter implication A id: "; cin >> a;
            cout << "Enter implication B id: "; cin >> b;
            rel.ensureSize(cm.count());
            rel.addImplication(a, b);
            cout << "Implication added: " << a << " -> " << b << endl;
        }
        else if (choice == 7) {
            int c, f; cout << "Course id: "; cin >> c; cout << "Faculty id: "; cin >> f;
            if (c < 0 || c >= cm.count()) { cout << "Invalid course id." << endl; continue; }
            if (f < 0 || f >= static_cast<int>(pm.listFaculties().size())) { cout << "Invalid faculty id." << endl; continue; }
            if (static_cast<int>(courseToFaculty.size()) < cm.count()) courseToFaculty.resize(cm.count(), -1);
            courseToFaculty[c] = f;
            cout << "Assigned faculty " << f << " to course " << c << endl;
        }
        else if (choice == 8) {
            int c, r; cout << "Course id: "; cin >> c; cout << "Room id: "; cin >> r;
            if (c < 0 || c >= cm.count()) { cout << "Invalid course id." << endl; continue; }
            if (r < 0 || r >= static_cast<int>(pm.listRooms().size())) { cout << "Invalid room id." << endl; continue; }
            if (static_cast<int>(courseToRoom.size()) < cm.count()) courseToRoom.resize(cm.count(), -1);
            courseToRoom[c] = r;
            cout << "Assigned room " << r << " to course " << c << endl;
        }
        else if (choice == 9) {
            int s, c; cout << "Student id: "; cin >> s; cout << "Course id: "; cin >> c;
            PeopleManager::Student* st = pm.getStudent(s);
            if (!st) { cout << "Invalid student id." << endl; continue; }
            if (c < 0 || c >= cm.count()) { cout << "Invalid course id." << endl; continue; }
            bool already = false;
            for (int x : st->enrolled) if (x == c) already = true;
            if (!already) st->enrolled.push_back(c);
            cout << "Enrolled student " << s << " in course " << c << endl;
        }
        else if (choice == 10) {
            cout << "1 List Courses" << endl;
            cout << "2 List Students" << endl;
            cout << "3 List Faculty" << endl;
            cout << "4 List Rooms" << endl;
            cout << "Enter: ";
            int sub; cin >> sub;
            if (sub == 1) cm.listAll();
            else if (sub == 2) pm.listAllStudents();
            else if (sub == 3) pm.listAllFaculties();
            else if (sub == 4) pm.listAllRooms();
            else cout << "Unknown option." << endl;
        }
        else if (choice == 11) {
            int n = cm.count();
            if (n == 0) { cout << "No courses available." << endl; continue; }
            if (rel.hasPrereqCycle()) { cout << "Prereq cycle exists; cannot list orders." << endl; continue; }
            Scheduling sched(rel);
            vector<vector<int>> orders = sched.listOrders(n, 200);
            cout << "Topological orders (show up to 200):" << endl;
            for (size_t i = 0; i < orders.size(); ++i) {
                for (size_t j = 0; j < orders[i].size(); ++j) {
                    const CourseManager::Course* cptr = cm.getCourse(orders[i][j]);
                    if (cptr) cout << cptr->code;
                    else cout << orders[i][j];
                    if (j + 1 < orders[i].size()) cout << " -> ";
                }
                cout << endl;
            }
            cout << "Total shown: " << orders.size() << endl;
        }
        else if (choice == 12) {
            int c; cout << "Course id for induction check: "; cin >> c;
            int n = rel.size();
            vector<bool> completed(n, false);
            cout << "Enter completed course ids (-1 to stop):" << endl;
            while (true) {
                int x; cin >> x; if (x == -1) break; if (x >= 0 && x < n) completed[x] = true;
            }
            Induction ind(rel);
            bool rec = ind.checkRecursive(c, completed);
            bool strong = ind.checkStrong(c, completed);
            cout << "Recursive check: " << (rec ? "SATISFIED" : "NOT SATISFIED") << endl;
            cout << "Strong induction check: " << (strong ? "SATISFIED" : "NOT SATISFIED") << endl;
            vector<int> indirect = ind.listIndirect(c);
            cout << "Indirect prerequisites: ";
            for (int id : indirect) cout << id << " ";
            cout << endl;
        }
        else if (choice == 13) {
            int n = rel.size();
            vector<bool> facts(n, false);
            cout << "Enter currently taken course ids (-1 to stop):" << endl;
            while (true) {
                int x; cin >> x; if (x == -1) break; if (x >= 0 && x < n) facts[x] = true;
            }
            Logic logic(rel);
            vector<bool> after = logic.applyImplications(facts);
            cout << "Facts after applying implications:" << endl;
            for (int i = 0; i < static_cast<int>(after.size()); ++i) if (after[i]) {
                const CourseManager::Course* cptr = cm.getCourse(i);
                cout << (cptr ? cptr->code : to_string(i)) << " ";
            }
            cout << endl;
        }
        else if (choice == 14) {
            cout << "Enter two student ids:" << endl;
            int s1, s2; cin >> s1 >> s2;
            PeopleManager::Student* st1 = pm.getStudent(s1);
            PeopleManager::Student* st2 = pm.getStudent(s2);
            if (!st1 || !st2) { cout << "Invalid student ids." << endl; continue; }
            vector<int> u = SetOps::unionSet(st1->enrolled, st2->enrolled);
            vector<int> it = SetOps::intersection(st1->enrolled, st2->enrolled);
            vector<int> di = SetOps::difference(st1->enrolled, st2->enrolled);
            cout << "Union: "; for (int x : u) { const CourseManager::Course* cptr = cm.getCourse(x); cout << (cptr ? cptr->code : to_string(x)) << " "; } cout << endl;
            cout << "Intersection: "; for (int x : it) { const CourseManager::Course* cptr = cm.getCourse(x); cout << (cptr ? cptr->code : to_string(x)) << " "; } cout << endl;
            cout << "Difference (s1 - s2): "; for (int x : di) { const CourseManager::Course* cptr = cm.getCourse(x); cout << (cptr ? cptr->code : to_string(x)) << " "; } cout << endl;
            cout << "Power set of student1 courses (limited):" << endl;
            vector<vector<int>> ps = SetOps::powerSet(st1->enrolled);
            for (size_t i = 0; i < ps.size(); ++i) {
                cout << "{";
                for (size_t j = 0; j < ps[i].size(); ++j) {
                    const CourseManager::Course* cptr = cm.getCourse(ps[i][j]);
                    cout << (cptr ? cptr->code : to_string(ps[i][j]));
                    if (j + 1 < ps[i].size()) cout << ",";
                }
                cout << "}" << endl;
            }
        }
        else if (choice == 15) {
            int n = cm.count();
            vector<int> domain, codomain;
            domain.reserve(n); codomain.reserve(n);
            for (int i = 0; i < n; ++i) {
                domain.push_back(i);
                int f = (i < static_cast<int>(courseToFaculty.size()) ? courseToFaculty[i] : -1);
                codomain.push_back(f);
            }
            bool isFunc = Functions::isFunction(domain, codomain);
            bool inj = Functions::isInjective(domain, codomain);
            bool sur = Functions::isSurjective(domain, codomain, static_cast<int>(pm.listFaculties().size()));
            bool bij = Functions::isBijective(domain, codomain, static_cast<int>(pm.listFaculties().size()));
            cout << "Function? " << (isFunc ? "Yes" : "No") << endl;
            cout << "Injective? " << (inj ? "Yes" : "No") << endl;
            cout << "Surjective? " << (sur ? "Yes" : "No") << endl;
            cout << "Bijective? " << (bij ? "Yes" : "No") << endl;
        }
        else if (choice == 16) {
            Consistency cons(rel, cm, pm, courseToFaculty, courseToRoom);
            cons.runAllChecks();
        }
        else if (choice == 17) {
            cout << "Enter student id for recommendations: "; int sid; cin >> sid;
            PeopleManager::Student* st = pm.getStudent(sid);
            if (!st) { cout << "Invalid student id." << endl; continue; }
            int n = rel.size();
            vector<bool> completed(n, false);
            for (int c : st->enrolled) if (c >= 0 && c < n) completed[c] = true;
            Recommender rec(rel, cm);
            vector<int> suggestions = rec.suggest(completed, 10);
            cout << "Recommended next courses:" << endl;
            for (int c : suggestions) {
                const CourseManager::Course* cp = cm.getCourse(c);
                cout << (cp ? cp->code : to_string(c)) << " ";
            }
            cout << endl;
        }
        else if (choice == 18) {
            cout << "Enter course id to generate proof: "; int c; cin >> c;
            int n = rel.size();
            vector<bool> completed(n, false);
            cout << "Enter completed course ids (-1 to stop):" << endl;
            while (true) {
                int x; cin >> x; if (x == -1) break; if (x >= 0 && x < n) completed[x] = true;
            }
            ProofExporter pe(rel, cm);
            string proof = pe.generateProof(c, completed);
            cout << "Proof generated:" << endl << proof;
            cout << "Save to file? Enter filename or 'no': "; string fname; cin >> fname;
            if (fname != "no") {
                bool ok = pe.saveToFile(fname, proof);
                cout << (ok ? "Saved." : "Failed to save.") << endl;
            }
        }
        else {
            cout << "Unknown choice." << endl;
        }
    }

    return 0;
}
//laiba kashif