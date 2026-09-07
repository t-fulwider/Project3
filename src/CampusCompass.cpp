#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <queue>
#include <climits>
#include <algorithm>
#include "CampusCompass.h"
using namespace std;

// ==== CONSTRUCTOR ==== //
CampusCompass::CampusCompass()
{
    ParseCSV("data/edges.csv", "data/classes.csv");
}

// ==== HELPERS ==== //
bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath)
{
    // return boolean based on whether parsing was successful or not

    // read the edges
    string text;
    ifstream inFile(edges_filepath);
    if (!inFile.is_open())
        return false;

    getline(inFile, text);
    while (getline(inFile, text))
    {
        stringstream stream(text);
        string from;
        string to;
        string weight;
        string garbage;

        getline(stream, from, ',');
        getline(stream, to, ',');
        getline(stream, garbage, ',');
        getline(stream, garbage, ',');
        getline(stream, weight);

        _adjList[stoi(from)].emplace_back(stoi(to), stoi(weight));
        _adjList[stoi(to)].emplace_back(stoi(from), stoi(weight));
    }
    inFile.close();

    //read the classes
    ifstream inFile2(classes_filepath);
    if (!inFile2.is_open())
        return false;

    getline(inFile2, text);
    while (getline(inFile2, text))
    {
        stringstream stream(text);
        string className;
        string location;
        string garbage;

        getline(stream, className, ',');
        getline(stream, location, ',');
        getline(stream, garbage);

        _classes[className] = stoi(location);
    }

    inFile2.close();
    return true;
}
pair<vector<int>, vector<int>> CampusCompass::Djikstras(int source)
{
    // standard dijkstra's using a priority heap
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.emplace(0, source);
    // get all the vertices that are not the source
    for (auto& vertex : _adjList)
    {
        if (vertex.first != source)
            pq.emplace(INT_MAX, vertex.first);
    }
    int maxVertex = 0;
    // get the largest node for d and p[v] size as graph could be unconnected
    for (const auto& vertex : _adjList)
        maxVertex = max(maxVertex, vertex.first);

    vector<int> p(maxVertex + 1, -1);
    vector<int> d(maxVertex + 1, INT_MAX);

    d[source] = 0;
    p[source] = source;

    while (!pq.empty())
    {
        auto [dist, u] = pq.top();
        pq.pop();
        // relaxing doesn't remove, just adds with new distance
        // make sure it is grabbing the lowest distance for each edge
        if (dist > d[u])
            continue;
        for (const auto& edge : _adjList[u])
        {
            if (edge._active)
            {
                int v = edge._to;
                if (d[u] != INT_MAX && d[u] + edge._weight < d[v])
                {
                    // relaxes by inserting new pair into heap with smaller distance
                    d[v] = d[u] + edge._weight;
                    p[v] = u;
                    pq.emplace(d[v], v);
                }
            }
        }
    }
    return {d, p};
}

// ==== MUTATORS ==== //
bool CampusCompass::AddStudent(string id, string name, string residence, vector<string> classes)
{
    // validation checks
    for (const auto& cls : classes)
    {
        if (_classes.find(cls) == _classes.end())
            return false;
    }
    regex constraint = regex("^[A-Za-z\\s]+$");
    if (!regex_match(name, constraint))
        return false;

    constraint = regex("[0-9]{8}");
    if (!regex_match(id, constraint))
        return false;

    for (const auto& stu : _students)
    {
        if (stu._id == id)
            return false;
    }
    // end of validation checks

    // create student object, add each class, add to student vector
    Student student(std::move(id), std::move(name), stoi(residence));
    for (auto& cls : classes)
        student._stClasses.insert(std::move(cls));
    _students.emplace_back(student);

    return true;
}
bool CampusCompass::RemoveStudent(string id)
{
    // check that UFID is valid
    regex constraint = regex("[0-9]{8}");
    if (!regex_match(id, constraint))
        return false;
    for (int i = 0; i < _students.size(); i++)
    {
        // erase student from vector once position found
        if (_students[i]._id == id)
        {
            _students.erase(_students.begin() + i);
            return true;
        }
    }
    return false;
}

bool CampusCompass::DropClass(string id, string className)
{
    // remove class from student set of classes
    for (int i = 0; i < _students.size(); i++)
    {
        if (_students[i]._id == id)
        {
            Student& target = _students[i];
            //for (int j = 0; j < target._stClasses.size(); j++)
           if (target._stClasses.count(className) == 1)
            {
                target._stClasses.erase(className);
               // student has no classess, they are removed from _students vector
                if (target._stClasses.empty())
                {
                    _students.erase(_students.begin() + i);
                    return true;
                }
                return true;
            }

        }
    }
    return false;
}
bool CampusCompass::ReplaceClass(string id, string oldClass, string newClass)
{
    // find student, remove oldClass, then add newClass if:
    // newClass is not in student classes already, and is in the classes data file
    // also make sure student does have oldClass
    for (auto& student : _students)
    {
        if (student._id == id)
        {
           if (student._stClasses.count(oldClass) == 1)
           {
               if (student._stClasses.count(newClass) == 0 && _classes.count(newClass) == 1)
               {
                   student._stClasses.erase(student._stClasses.find(oldClass));
                   student._stClasses.emplace(newClass);
                   return true;
               }
           }
        }
    }
    return false;
}
int CampusCompass::RemoveClass(string code)
{
    // check for every student that has the class, remove it
    // if student then has no classes, remove said student from _students
    int count = 0;
    for (int i = 0; i < _students.size(); i++)
    {
        if (_students[i]._stClasses.count(code) == 1)
        {
            _students[i]._stClasses.erase(code);
            count++;
            if (_students[i]._stClasses.empty())
            {
                _students.erase(_students.begin() + i);
                --i;
            }
        }
    }
    // if count = 0, print unsuccessful, else print count
    return count;
}

bool CampusCompass::ToggleEdges(vector<pair<int, int> >& edges)
{
    // check every edge in given edges, search to find it in map
    // because it is undirected, the "to" must also be found in the map
    for (auto& edge : edges)
    {
        // find {from , to} edge and switch its "active" value
        for (auto& vertex : _adjList[edge.first])
        {
            if (vertex._to == edge.second)
            {
                vertex._active = !vertex._active;
                break;
            }
        }
        // the same, but for {to, from} because edges are undirected
        for (auto& vertex : _adjList[edge.second])
        {
            if (vertex._to == edge.first)
            {
                vertex._active = !vertex._active;
                break;
            }
        }
    }
    return true;
}
bool CampusCompass::EdgeStatus(int from, int to)
{
    // make sure edge is in list
    // print its "active" value if it exists, otherwise "DNE"
    if (_adjList.count(from) == 1)
    {
        for (auto vertex : _adjList[from])
        {
            if (vertex._to == to)
            {
                if (vertex._active)
                {
                    cout << "open";
                    return true;
                }
                cout << "closed";
                return false;
            }
        }
    }
    cout << "DNE";
    return false;
}

// ==== GRAPH ALGORITHMS ==== //
bool CampusCompass::IsConnected(int from, int to)
{
    // simple s-t path using BFS
    unordered_set<int> visited;
    queue<int> q;
    visited.insert(from);
    q.push(from);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        vector<Edge> neighbors = _adjList[current];
        for (auto vertex : neighbors)
        {
            // make sure edge is actually active
            if (vertex._active && vertex._to == to)
                return true;
            if (vertex._active && visited.count(vertex._to) == 0)
            {
                visited.insert(vertex._to);
                q.push(vertex._to);
            }
        }
    }
    return false;
}
vector<pair<string, int>> CampusCompass::ShortestEdges(string id)
{
    // uses dijkstra's as helper function
    // then, uses returned d[v] to output shortes time
    // if d[v] was never changed, switch that value to -1 for unreached
    vector<pair<string, int>> output;
    for (auto& student : _students)
    {
        if (student._id == id)
        {
            auto [edges, pre] = Djikstras(student._residence);

            cout << "Time For Shortest Edges: " << student._name << endl;
            for (const auto& cls : student._stClasses)
            {
                int weight;
                if (_classes[cls] >= edges.size())
                    weight = -1;
                else
                    weight = edges[_classes[cls]];
                if (weight == INT_MAX)
                    weight = -1;
                output.push_back(make_pair(cls, weight));\
            }

            sort(output.begin(), output.end());
            if (!output.empty())
            {
                for (int i = 0; i <  output.size() - 1; i++)
                    cout << output[i].first << ": " << output[i].second << endl;
                cout << output[output.size() - 1].first << ": " << output[output.size() - 1].second;
            }

        }
    }
    return output;
}
void CampusCompass::StudentZone(string id)
{
    // performs dijkstra's
    unordered_map<int, vector<Edge>> graph;
    unordered_set<int> subVertices;
    vector<pair<string, int>> output;
    for (const auto& student : _students)
    {
        if (student._id == id)
        {
            auto [edges, p] = Djikstras(student._residence);
            subVertices.insert(student._residence);
            for (const auto& cls : student._stClasses)
            {
                int node = _classes[cls];
                int j = node;

                // get predecessors all the way to root
                while (j != student._residence)
                {
                    graph[j].emplace_back(p[j], edges[j] - edges[p[j]]);
                    graph[p[j]].emplace_back(j, edges[j] - edges[p[j]]);

                    subVertices.insert(j);
                    subVertices.insert(p[j]);

                    j = p[j];
                }
            }
            // add any other edges that directly connect edges in shortest paths
            for (int u : subVertices)
            {
                for (const auto& edge : _adjList[u])
                {
                    int v = edge._to;
                    if (subVertices.count(v) == 1)
                        graph[u].push_back(edge);
                }
            }

            // ==== PRIM'S ==== //

            int totalCost = 0;
            unordered_set<int> visited;

            priority_queue<pair<int, int>, vector<pair<int,int>>, greater<>> pq;
            pq.push({0, student._residence});

            while (!pq.empty())
            {
                auto [cost, to] = pq.top();
                pq.pop();

                if(visited.count(to) == 1)
                    continue;
                visited.insert(to);
                // update total cost when shortest edge is added to MST
                totalCost += cost;

                for (auto& edge : graph[to])
                {
                    if (visited.count(edge._to) == 0)
                    {
                        pq.push({edge._weight, edge._to});
                    }
                }
            }
            cout << "Student Zone Cost For " << student._name << ": " << totalCost;
            return;
        }
    }
}

// ==== ACCESSORS ==== //
vector<string> CampusCompass::PrintStudents()
{
    // used for testing, print all students in structure if there are some
    vector<string> students;
    for (int i = 0; i < _students.size(); i++)
    {
        cout << _students[i]._name << endl;
        students.push_back(_students[i]._name);
    }
    if (students.empty())
        cout << "No students!";

    return students;
}
vector<string> CampusCompass::GetClasses(string id)
{
    // used for testing, print all classes for a given student
    vector<string> classes;
    for (const auto& student : _students)
    {
        if (student._id == id)
        {
            for (const auto& cls : student._stClasses)
                classes.push_back(cls);
            break;
        }
    }

    return classes;
}