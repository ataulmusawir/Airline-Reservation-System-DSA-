#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
#include <queue>      
#include <map>        
#include <set>        



#define MAXCITY 30
#define MAXFLIGHT 200
#define ROUNDTRIP 0
#define ONEWAY 1
#define MINLAYOVER 30

using namespace std;

struct FlightType {
    int FlightNo;
    char *startCity;
    int timeDepart;
    char *endCity;
    int timeArrival;
    int noOfPassengers;
    FlightType *nextDeparture;
    FlightType *nextArrival;
};

struct CityListType {
    char *cityName;
    FlightType *nextDeparture;
    FlightType *nextArrival;
};

struct RouteType {
    int Day;
    int nHops;
    int FlightNo1;
    int FlightNo2;
};

struct ReservationType {
    char *firstName;
    char *lastName;
    int tripType;
    RouteType route1;
    RouteType route2;
    ReservationType *nextReserve;
};

CityListType cityList[MAXCITY];
int flightCount = 0;
ReservationType *reserveHead = nullptr;

//int Hash(char *cityName) {
 //   int hash = 0;
 //   while (*cityName) {
  //      hash = (hash * 31 + *cityName) % MAXCITY;
 //       cityName++;
  //  }
  //  return hash;
//}
int Hash(std::string cityName) {
    int hash = 0;
    for (char c : cityName) {
        hash = (hash * 31 + c) % MAXCITY;
    }
    return hash;
}


FlightType *MakeFlightNode(int FlightNo, char *startCity, int timeDepart, char *endCity, int timeArrival) {
    FlightType *newFlight = new FlightType;
    newFlight->FlightNo = FlightNo;
    newFlight->startCity = new char[strlen(startCity) + 1];
    strcpy(newFlight->startCity, startCity);
    newFlight->timeDepart = timeDepart;
    newFlight->endCity = new char[strlen(endCity) + 1];
    strcpy(newFlight->endCity, endCity);
    newFlight->timeArrival = timeArrival;
    newFlight->noOfPassengers = 0;
    newFlight->nextDeparture = nullptr;
    newFlight->nextArrival = nullptr;
    return newFlight;
}

void AddFlightToCityLists(FlightType *flight) {
    int startIdx = Hash(flight->startCity);
    int endIdx = Hash(flight->endCity);

    // Add to start city's departure list
    if (!cityList[startIdx].cityName) {
        cityList[startIdx].cityName = new char[strlen(flight->startCity) + 1];
        strcpy(cityList[startIdx].cityName, flight->startCity);
        cityList[startIdx].nextDeparture = nullptr;
    }
    flight->nextDeparture = cityList[startIdx].nextDeparture;
    cityList[startIdx].nextDeparture = flight;

    // Add to end city's arrival list
    if (!cityList[endIdx].cityName) {
        cityList[endIdx].cityName = new char[strlen(flight->endCity) + 1];
        strcpy(cityList[endIdx].cityName, flight->endCity);
        cityList[endIdx].nextArrival = nullptr;
    }
    flight->nextArrival = cityList[endIdx].nextArrival;
    cityList[endIdx].nextArrival = flight;
}

void AddFlight(int FlightNo, char *startCity, int timeDepart, char *endCity, int timeArrival) {
    FlightType *flight = MakeFlightNode(FlightNo, startCity, timeDepart, endCity, timeArrival);
    AddFlightToCityLists(flight);
}

void DisplayAllCities() {
    cout << "\nCities Serviced by Airline:\n";
    for (int i = 0; i < MAXCITY; i++) {
        if (cityList[i].cityName) {
            cout << cityList[i].cityName << endl;
        }
    }
}

void DisplayDepartureList(char *cityName) {
    int idx = Hash(cityName);
    FlightType *current = cityList[idx].nextDeparture;

    cout << "\nFlight Departures from " << cityName << ":\n";
    while (current) {
        cout << "Flight No: " << current->FlightNo << " Departure: " << current->timeDepart << " Destination: " << current->endCity << endl;
        current = current->nextDeparture;
    }
}

void DisplayArrivalList(char *cityName) {
    int idx = Hash(cityName);
    FlightType *current = cityList[idx].nextArrival;

    cout << "\nFlight Arrivals at " << cityName << ":\n";
    while (current) {
        cout << "Flight No: " << current->FlightNo << " Arrival: " << current->timeArrival << " Origin: " << current->startCity << endl;
        current = current->nextArrival;
    }
}

void DisplayReachableCities(char *startCity) {
    cout << "\nCities reachable from " << startCity << ":\n";
    int idx = Hash(startCity);
    FlightType *current = cityList[idx].nextDeparture;

    while (current) {
        cout << current->endCity << endl;
        current = current->nextDeparture;
    }
}
void ShowShortestPath(char *startCity, char *endCity) {
    priority_queue<pair<int, char *>, vector<pair<int, char *>>, greater<>> pq;
    map<string, int> minDist;
    map<string, string> prevCity;
    map<string, FlightType *> prevFlight;
    set<string> visited;

    pq.push({0, startCity});
    minDist[startCity] = 0;

    while (!pq.empty()) {
        auto [currDist, currCity] = pq.top();
        pq.pop();

        if (visited.count(currCity)) continue;
        visited.insert(currCity);

        int idx = Hash(currCity);
        FlightType *current = cityList[idx].nextDeparture;

        while (current) {
            int totalTime = currDist + (current->timeArrival - current->timeDepart);
            string nextCity = current->endCity;

            if (!minDist.count(nextCity) || totalTime < minDist[nextCity]) {
                minDist[nextCity] = totalTime;
                prevCity[nextCity] = currCity;
                prevFlight[nextCity] = current;
                pq.push({totalTime, current->endCity});
            }
            current = current->nextDeparture;
        }
    }

    if (!minDist.count(endCity)) {
        cout << "No route exists between " << startCity << " and " << endCity << ".\n";
        return;
    }

    cout << "Shortest path (in minutes): " << minDist[endCity] << "\n";
    vector<FlightType *> path;
    for (string city = endCity; city != startCity; city = prevCity[city]) {
        path.push_back(prevFlight[city]);
    }
    reverse(path.begin(), path.end());

    for (auto flight : path) {
        cout << "Flight " << flight->FlightNo << " from " << flight->startCity
             << " at " << flight->timeDepart << " to " << flight->endCity
             << " at " << flight->timeArrival << ".\n";
    }
}

void FindRoute(char *startCity, char *endCity) {
    queue<pair<string, vector<FlightType *>>> q;
    set<string> visited;
    vector<vector<FlightType *>> routes;

    q.push({startCity, {}});
    visited.insert(startCity);

    while (!q.empty()) {
        auto [currCity, path] = q.front();
        q.pop();

        int idx = Hash(currCity);
        FlightType *current = cityList[idx].nextDeparture;

        while (current) {
            if (path.empty() || current->timeDepart >= path.back()->timeArrival + MINLAYOVER) {
                vector<FlightType *> newPath = path;
                newPath.push_back(current);

                if (strcmp(current->endCity, endCity) == 0) {
                    routes.push_back(newPath);
                } else if (!visited.count(current->endCity)) {
                    q.push({current->endCity, newPath});
                }
            }
            current = current->nextDeparture;
        }
    }

    if (routes.empty()) {
        cout << "No route exists between " << startCity << " and " << endCity << ".\n";
        return;
    }

    cout << "Available routes:\n";
    for (auto &route : routes) {
        for (auto &flight : route) {
            cout << "Flight " << flight->FlightNo << " from " << flight->startCity
                 << " at " << flight->timeDepart << " to " << flight->endCity
                 << " at " << flight->timeArrival << ".\n";
        }
        cout << "---\n";
    }
}

void MakeReservation() {
    ReservationType *newReservation = new ReservationType;

    cout << "Enter first name: ";
    newReservation->firstName = new char[50];
    cin >> newReservation->firstName;

    cout << "Enter last name: ";
    newReservation->lastName = new char[50];
    cin >> newReservation->lastName;

    cout << "Enter trip type (0 for roundtrip, 1 for one-way): ";
    cin >> newReservation->tripType;

    cout << "Enter flight number for first leg: ";
    cin >> newReservation->route1.FlightNo1;

    if (newReservation->tripType == ROUNDTRIP) {
        cout << "Enter flight number for return leg: ";
        cin >> newReservation->route2.FlightNo2;
    }

    newReservation->nextReserve = reserveHead;
    reserveHead = newReservation;

    cout << "Reservation made successfully.\n";
}

void PrintSchedule() {
    char firstName[50], lastName[50];
    cout << "Enter first name: ";
    cin >> firstName;
    cout << "Enter last name: ";
    cin >> lastName;

    ReservationType *current = reserveHead;
    while (current) {
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            cout << "Passenger: " << current->firstName << " " << current->lastName << "\n";
            cout << "Trip Type: " << (current->tripType == ROUNDTRIP ? "Roundtrip" : "One-way") << "\n";

            cout << "First Leg: Flight " << current->route1.FlightNo1 << "\n";
            if (current->tripType == ROUNDTRIP) {
                cout << "Return Leg: Flight " << current->route2.FlightNo2 << "\n";
            }
            return;
        }
        current = current->nextReserve;
    }
    cout << "No reservation found for the passenger.\n";
}

void DeleteReservation() {
    char firstName[50], lastName[50];
    cout << "Enter first name: ";
    cin >> firstName;
    cout << "Enter last name: ";
    cin >> lastName;

    ReservationType *current = reserveHead, *prev = nullptr;

    while (current) {
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            if (prev) {
                prev->nextReserve = current->nextReserve;
            } else {
                reserveHead = current->nextReserve;
            }
            delete[] current->firstName;
            delete[] current->lastName;
            delete current;
            cout << "Reservation deleted successfully.\n";
            return;
        }
        prev = current;
        current = current->nextReserve;
    }
    cout << "No reservation found for the passenger.\n";
}

void PrintPassengers(int flightNo) {
    cout << "Passengers on Flight " << flightNo << ":\n";
    ReservationType *current = reserveHead;
    bool found = false;

    while (current) {
        if (current->route1.FlightNo1 == flightNo || current->route2.FlightNo2 == flightNo) {
            cout << current->firstName << " " << current->lastName << "\n";
            found = true;
        }
        current = current->nextReserve;
    }

    if (!found) {
        cout << "No passengers found for this flight.\n";
    }
}



void InitializeFlightsAndCities() {
    for (int i = 0; i < MAXCITY; i++) {
        cityList[i].cityName = nullptr;
        cityList[i].nextDeparture = nullptr;
        cityList[i].nextArrival = nullptr;
    }
    AddFlight(101, strdup("Lahore"), 800, strdup("Karachi"), 1100);
    AddFlight(102, strdup("Karachi"), 1200, strdup("Islamabad"), 1530);
    AddFlight(103, strdup("Islamabad"), 1600, strdup("Peshawar"), 1700);
    AddFlight(104, strdup("Peshawar"), 2000, strdup("Lahore"), 2045);
    AddFlight(105, strdup("Islamabad"), 2200, strdup("Lahore"), 2300);
    AddFlight(106, strdup("Karachi"), 1400, strdup("Lahore"), 1700);
    AddFlight(107, strdup("Peshawar"), 2100, strdup("Karachi"), 2330);
    AddFlight(108, strdup("Peshawar"), 1800, strdup("Islamabad"), 1830);
    AddFlight(109, strdup("Karachi"), 200, strdup("Peshawar"), 600);
    AddFlight(110, strdup("Islamabad"), 700, strdup("Karachi"), 1100);
    AddFlight(111, strdup("Lahore"), 900, strdup("Islamabad"), 1200);
    AddFlight(112, strdup("Lahore"), 2300, strdup("Peshawar"), 2400);
}

int main() {
    InitializeFlightsAndCities();

    int choice;
    char cityName[50], startCity[50], endCity[50];
    int flightNo;

    do {
        cout << "\n GIKI Airline Reservation System\n";
        cout << "1. Show all cities serviced by airline\n";
        cout << "2. Show flight departures for a city\n";
        cout << "3. Show flight arrivals for a city\n";
        cout << "4. Show all cities reachable from a city\n";
        cout << "5. Show shortest path between two cities\n";
        cout << "6. Find route between two cities\n";
        cout << "7. Make a reservation\n";
        cout << "8. Print a passenger's schedule\n";
        cout << "9. Delete a reservation\n";
        cout << "10. Print passengers for a flight\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                DisplayAllCities();
                break;
            case 2:
                cout << "Enter city name: ";
                cin >> cityName;
                DisplayDepartureList(cityName);
                break;
            case 3:
                cout << "Enter city name: ";
                cin >> cityName;
                DisplayArrivalList(cityName);
                break;
            case 4:
                cout << "Enter start city name: ";
                cin >> startCity;
                DisplayReachableCities(startCity);
                break;
            case 5:
                cout << "Enter start city name: ";
                cin >> startCity;
                cout << "Enter end city name: ";
                cin >> endCity;
                ShowShortestPath(startCity, endCity);
                break;
            case 6:
                cout << "Enter start city name: ";
                cin >> startCity;
                cout << "Enter end city name: ";
                cin >> endCity;
                FindRoute(startCity, endCity);
                break;
            case 7:
                MakeReservation();
                break;
            case 8:
                PrintSchedule();
                break;
            case 9:
                DeleteReservation();
                break;
            case 10:
                cout << "Enter flight number: ";
                cin >> flightNo;
                PrintPassengers(flightNo);
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
