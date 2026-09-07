#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <unordered_set>

using namespace std;

class CampusCompass {
private:

    struct Edge
    {
        int _to;
        int _weight;
        bool _active;

        Edge(int to, int weight) : _to(to), _weight(weight), _active(true) {}
    };

    struct Student
    {
        string _id;
        string _name;
        int _residence;
        unordered_set<string> _stClasses;

        Student(string  id, string name, int residence) : _id(std::move(id)), _name(std::move(name)), _residence(residence) {}
    };

    unordered_map<int, vector<Edge>> _adjList;
    vector<Student> _students;
    unordered_map<string, int> _classes;

    // ==== HELPER ==== //
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    pair<vector<int>, vector<int>> Djikstras(int source);

public:

    CampusCompass(); // constructor

    // ==== MUTATORS ==== //
    bool AddStudent(string id, string name, string residence, vector<string> classes);
    bool RemoveStudent(string id);

    bool DropClass(string id, string className);
    bool ReplaceClass(string id, string oldClass, string newClass);
    int RemoveClass(string code);

    bool ToggleEdges(vector<pair<int, int>>& edges);
    bool EdgeStatus(int from, int to);

    // ==== GRAPH ALGORITHMS ==== //
    bool IsConnected(int from, int to);
    vector<pair<string, int>> ShortestEdges(string id);
    void StudentZone(string id);

    // ==== ACCESSORS ==== //
    vector<string> PrintStudents();
    vector<string> GetClasses(string id);
};