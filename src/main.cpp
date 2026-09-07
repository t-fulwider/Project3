#include <iostream>
#include <sstream>
#include <cctype>
#include "CampusCompass.h"

using namespace std;

int main() {
    CampusCompass c;

    string numCommands;
    getline(cin, numCommands);

    for (int i = 0; i < stoi(numCommands); i++)
    {
        string line;
        getline(cin, line);

        istringstream inStream(line);
        string function;
        getline(inStream, function, ' ');

        if (function == "insert")
        {
            string garbage;
            getline(inStream, garbage, '\"');
            string name;
            getline(inStream, name, '\"');
            getline(inStream, garbage, ' ');
            string id;
            getline(inStream, id, ' ');
            string residence;
            getline(inStream, residence, ' ');

            string numClasses;
            getline(inStream, numClasses, ' ');
            if (numClasses.length() > 1 || (!isdigit(numClasses[0])))
                cout << "unsuccessful";
            else
            {
                string classCode;
                vector<string> classes;
                bool b = true;
                for (int j = 0; j < stoi(numClasses); j++)
                {
                    if (!(inStream >> classCode))
                        b = false;
                    classes.push_back(classCode);
                }
                string extra;
                if (inStream >> extra || !b)
                    cout << "unsuccessful";
                else
                {
                    bool res = c.AddStudent(id, name, residence, classes);
                    if (res)
                        cout << "successful";
                    else
                        cout << "unsuccessful";
                }
            }
        }
        else if (function == "remove")
        {
            string id;
            getline(inStream, id);

            bool res = c.RemoveStudent(id);
            if (res)
                cout << "successful";
            else
                cout << "unsuccessful";
        }
        else if (function == "dropClass")
        {
            string id;
            getline(inStream, id, ' ');

            string classCode;
            getline(inStream, classCode);

            bool res = c.DropClass(id, classCode);
            if (res)
                cout << "successful";
            else
                cout << "unsuccessful";
        }
        else if (function == "replaceClass")
        {
            string id;
            getline(inStream, id, ' ');
            string cc1;
            getline(inStream, cc1, ' ');
            string cc2;
            getline(inStream, cc2);

            bool res = c.ReplaceClass(id, cc1, cc2);
            if (res)
                cout << "successful";
            else
                cout << "unsuccessful";
        }
        else if (function == "removeClass")
        {
            string ccode;
            getline(inStream, ccode);

            int res = c.RemoveClass(ccode);
            if (res == 0)
                cout << "unsuccessful";
            else
                cout << res;
        }
        else if (function == "toggleEdgesClosure")
        {
            int num;
            inStream >> num;
            vector<pair<int, int>> edges;
            int from, to;
            bool b = true;
            for (int j = 0; j < num; j++)
            {
                if (!(inStream >> from) || !(inStream >> to))
                    b = false;
                edges.emplace_back(from, to);
            }
            string extra;
            if (inStream >> extra || !b)
                cout << "unsuccessful";
            else
            {
                bool res = c.ToggleEdges(edges);
                if (res)
                    cout << "successful";
                else
                    cout << "unsuccessful";
            }
        }
        else if (function == "checkEdgeStatus")
        {
            int one;
            int two;
            bool b = true;
            if (!(inStream >> one) || !(inStream >> two))
                b = false;

            string extra;
            if (inStream >> extra || !b)
                cout << "unsuccessful";
            else
                c.EdgeStatus(one, two);
        }
        else if (function == "isConnected")
        {
            int one;
            int two;
            bool b = true;
            if (!(inStream >> one) || !(inStream >> two))
                b = false;

            string extra;
            if (inStream >> extra || !b)
                cout << "unsuccessful";
            else
            {
                bool res = c.IsConnected(one, two);
                if (res)
                    cout << "successful";
                else
                    cout << "unsuccessful";
            }
        }
        else if (function == "printShortestEdges")
        {
            string id;
            inStream >> id;

            vector<pair<string, int>> res = c.ShortestEdges(id);
            if (res.empty())
                cout << "unsuccessful";
        }
        else if (function == "printStudentZone")
        {
            string id;
            inStream >> id;

            c.StudentZone(id);
        }
        else
            cout << "unsuccessful";

        cout << endl;
    }
    return 0;
}