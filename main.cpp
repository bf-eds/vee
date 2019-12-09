#include "lib/include/route.h"

#include <iostream>

using namespace std;

void printMenu();
void loadDB(unique_ptr<veeroute::Route> &route);
void addRoad(unique_ptr<veeroute::Route> &route);
void editRoad(unique_ptr<veeroute::Route> &route);
void deleteRoad(unique_ptr<veeroute::Route> &route);
void findRoute(unique_ptr<veeroute::Route> &route);

const std::string WARN_MSG1 = {"You should load database first "};
const std::string WARN_MSG2 = {"There isn't such road "};

int main()
{
    unique_ptr<veeroute::Route> route;
    int choice = -1;

    do
    {
        printMenu();
        cin >> choice;

        switch (choice)
        {
            case 1:
                loadDB(route);
                break;
            case 2:
                addRoad(route);
                break;
            case 3:
                deleteRoad(route);
                break;
            case 4:
                editRoad(route);
                break;
            case 5:
                findRoute(route);
                break;
            case 0:
                break;
            default:
                cout << "Invalid key" << endl;
        }
    }
    while (choice != 0);

    return 0;
}

void printMenu()
{
    cout << "\n     Router menu" << endl;
    cout << "1. Load database from file" << endl;
    cout << "2. Add road to database" << endl;
    cout << "3. Delete road from database" << endl;
    cout << "4. Edit road" << endl;
    cout << "5. Find route" << endl;
    cout << "0. Exit" << endl;
    cout << "\nEnter key: ";
}

void loadDB(unique_ptr<veeroute::Route> &route)
{
    cout << "Enter file path: ";
    std::string path;
    cin >> path;

    route = make_unique<veeroute::Route>(path.data());
}

void addRoad(unique_ptr<veeroute::Route> &route)
{
    if (route)
    {
        cout << "Enter <begin_id> <end_id> <length> <velocity>: ";
        uint64_t begin_id;
        uint64_t end_id;
        double velocity;
        double length;

        cin >> begin_id >> end_id >> length >> velocity;
        route->AddRoad(begin_id, end_id, length, velocity);
    }
    else
    {
        cout << WARN_MSG1 << endl;
    }
}

void editRoad(unique_ptr<veeroute::Route> &route)
{
    if (route)
    {
        cout << "Enter <begin_id> <end_id> <length> <velocity>: ";
        uint64_t begin_id;
        uint64_t end_id;
        double velocity;
        double length;

        cin >> begin_id >> end_id >> length >> velocity;
        if (!route->EditRoad(begin_id, end_id, length, velocity))
        {
            cout << WARN_MSG2 << endl;
        }
    }
    else
    {
        cout << WARN_MSG1 << endl;
    }
}

void deleteRoad(unique_ptr<veeroute::Route> &route)
{
    if (route)
    {
        cout << "Enter <begin_id> <end_id>: ";
        uint64_t begin_id;
        uint64_t end_id;

        cin >> begin_id >> end_id;
        if (!route->DeleteRoad(begin_id, end_id))
        {
            cout << WARN_MSG2 << endl;
        }
    }
    else
    {
        cout << WARN_MSG1 << endl;
    }
}

void findRoute(unique_ptr<veeroute::Route> &route)
{
    if (route)
    {
        cout << "Enter <begin_id> <end_id>: ";
        uint64_t begin_id;
        uint64_t end_id;
        uint64_t path[1024];
        int path_count = sizeof(path);


        cin >> begin_id >> end_id;
        if (route->FindRoute(begin_id, end_id, path, &path_count))
        {
            cout << begin_id;

            for (size_t i = 0; i < path_count; ++i)
            {
                cout << " -> " << path[i];
            }

            cout << endl;
        }
        else
        {
            cout << "There isn't route between " << begin_id << " and " << end_id << endl;
        }
    }
    else
    {
        cout << WARN_MSG1 << endl;
    }
}
