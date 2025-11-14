#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
    Implementation of the Banker's Algorithm input parsing and safety check.

    What this file does:
    - Parses a textual input describing resources, processes,
    the Available vector, the Max and Allocation matrices, and an optional request.
    - Computes the Need matrix and runs the Banker's safety algorithm.
    - Simulates granting a single request (if present) and prints formatted output
    describing whether granting the request leaves the system in a safe state.
*/


// Banker's Algorithm implementation
class BankersAlgorithm {
    private:
        int numProcesses;                // Number of processes
        int numResources;                // Number of resources
        vector<vector<int>> allocation;  // Allocation matrix
        vector<vector<int>> max;         // Maximum demand matrix
        vector<vector<int>> need;        // Need matrix
        vector<int> available;           // Available resources
    public:
        BankersAlgorithm(int processes, int resources) {    // Constructor to initialize the matrices and vectors
            numProcesses = processes;
            numResources = resources;
            allocation.resize(numProcesses, vector<int>(numResources, 0));
            max.resize(numProcesses, vector<int>(numResources, 0));
            need.resize(numProcesses, vector<int>(numResources, 0));
            available.resize(numResources, 0);
        }

        // Setters so main can populate the matrices after parsing input
        void setAvailable(const vector<int>& av) {
            if ((int)av.size() != numResources) return;
            available = av;
        }

        void setMaxRow(int pid, const vector<int>& row) {
            if (pid < 0 || pid >= numProcesses) return;
            if ((int)row.size() != numResources) return;
            max[pid] = row;
        }

        void setAllocationRow(int pid, const vector<int>& row) {
            if (pid < 0 || pid >= numProcesses) return;
            if ((int)row.size() != numResources) return;
            allocation[pid] = row;
        }

        // Compute need = max - allocation for each process/resource
        void computeNeed() {
            for (int i = 0; i < numProcesses; ++i) {
                for (int j = 0; j < numResources; ++j) {
                    need[i][j] = max[i][j] - allocation[i][j];
                    if (need[i][j] < 0) need[i][j] = 0; // guard
                }
            }
        }

        // Safety algorithm: returns true if the current state is safe
        bool isSafe() const {
            vector<int> work = available;
            vector<bool> finish(numProcesses, false);

            while (true) {
                bool progressed = false;
                for (int i = 0; i < numProcesses; ++i) {
                    if (finish[i]) continue;
                    bool ok = true;
                    for (int j = 0; j < numResources; ++j) {
                        if (need[i][j] > work[j]) { ok = false; break; }
                    }
                    if (ok) {
                        // this process can finish
                        for (int j = 0; j < numResources; ++j) work[j] += allocation[i][j];
                        finish[i] = true;
                        progressed = true;
                    }
                }
                if (!progressed) break;
            }

            for (int i = 0; i < numProcesses; ++i) if (!finish[i]) return false;
            return true;
        }

        // Check if a request can be considered: req <= need and req <= available
        bool canRequest(int pid, const vector<int>& req) const {
            if (pid < 0 || pid >= numProcesses) return false;

            for (int j = 0; j < numResources; ++j) {
                if (req[j] > need[pid][j]) return false;
                if (req[j] > available[j]) return false;
            }
            return true;
        }

        // Apply the request (assumes it's valid). Modifies allocation, available, need.
        void applyRequest(int pid, const vector<int>& req) {
            if (pid < 0 || pid >= numProcesses) return;
            for (int j = 0; j < numResources; ++j) {
                allocation[pid][j] += req[j];
                available[j] -= req[j];
                need[pid][j] -= req[j];
                if (need[pid][j] < 0) need[pid][j] = 0;
            }
        }

        // Print just the need matrix with a header (used for 'New Need')
        void printNeedWithHeader(const string& header) const {
            cout << header << '\n';
            for (int i = 0; i < numProcesses; ++i) {
                for (int j = 0; j < numResources; ++j) {
                    if (j) cout << ' ';
                    cout << need[i][j];
                }
                cout << '\n';
            }
        }

        // Print current state (for verification)
        void printState() const {
            cout << "Resources: " << numResources << ", Processes: " << numProcesses << "\n";
            cout << "Available\n";
            for (int j = 0; j < numResources; ++j) {
                if (j) cout << ' ';
                cout << available[j];
            }
            cout << "\n";

            cout << "Max\n";
            for (int i = 0; i < numProcesses; ++i) {
                for (int j = 0; j < numResources; ++j) {
                    if (j) cout << ' ';
                    cout << max[i][j];
                }
                cout << '\n';
            }

            cout << "Allocation\n";
            for (int i = 0; i < numProcesses; ++i) {
                for (int j = 0; j < numResources; ++j) {
                    if (j) cout << ' ';
                    cout << allocation[i][j];
                }
                cout << '\n';
            }

            cout << "Need\n";
            for (int i = 0; i < numProcesses; ++i) {
                for (int j = 0; j < numResources; ++j) {
                    if (j) cout << ' ';
                    cout << need[i][j];
                }
                cout << '\n';
            }
        }
};

int main(){
    // Parse input from stdin to populate the Banker's Algorithm data structures
    string token;
    int numResources = 0;
    int numProcesses = 0;

    if (!(cin >> token)) return 0;
    if (token == "R" || token == "r") {
        cin >> numResources;
    } else {
        cerr << "Expected 'R' at start\n";
        return 1;
    }

    if (!(cin >> token)) { cerr << "Expected 'P' after resources\n"; return 1; }
    if (token == "P") {
        cin >> numProcesses;
    } else {
        cerr << "Expected 'P' token\n";
        return 1;
    }

    // Create the Banker's Algorithm instance with the parsed number of processes and resources
    BankersAlgorithm bankers(numProcesses, numResources);

    // Available
    if (!(cin >> token)) { cerr << "Unexpected EOF reading Available\n"; return 1; }
    if (token != "Available") {
        cerr << "Expected 'Available' but found '" << token << "'\n";
        return 1;
    }
    vector<int> available(numResources);
    for (int j = 0; j < numResources; ++j) cin >> available[j];
    bankers.setAvailable(available);

    // Max
    if (!(cin >> token)) { cerr << "Unexpected EOF reading Max\n"; return 1; }
    if (token != "Max") { cerr << "Expected 'Max' but found '" << token << "'\n"; return 1; }
    for (int i = 0; i < numProcesses; ++i) {
        vector<int> row(numResources);
        for (int j = 0; j < numResources; ++j) cin >> row[j];
        bankers.setMaxRow(i, row);
    }

    // Allocation
    if (!(cin >> token)) { cerr << "Unexpected EOF reading Allocation\n"; return 1; }
    if (token != "Allocation") { cerr << "Expected 'Allocation' but found '" << token << "'\n"; return 1; }
    for (int i = 0; i < numProcesses; ++i) {
        vector<int> row(numResources);
        for (int j = 0; j < numResources; ++j) cin >> row[j];
        bankers.setAllocationRow(i, row);
    }

    // Optional request line: e.g. "P1 1 0 2"
    string procName;
    vector<int> request(numResources, 0);
    if (cin >> procName) {
        int pid = -1;
        if (!procName.empty() && (procName[0] == 'P' || procName[0] == 'p')) {
            try {
                pid = stoi(procName.substr(1));
            } catch (...) { pid = -1; }
        }
        for (int j = 0; j < numResources; ++j) cin >> request[j];
    }

    // Compute need for the current state
    bankers.computeNeed();

    // If we have a request, run checks and simulate granting
    if (!procName.empty()) {
        int pid = -1;
        if (!procName.empty() && (procName[0] == 'P' || procName[0] == 'p')) {
            try { pid = stoi(procName.substr(1)); } catch(...) { pid = -1; }
        }

        // Check if the current state is safe before granting the request
        if(bankers.isSafe()){
            // Before granting
            cout << "Before granting the request of " << procName << ", the system is in safe state." << "\n";


            // Check request validity
            if (!bankers.canRequest(pid, request)) {
                cout << procName << "'s request cannot be granted (exceeds need or available)." << "\n";
                return 0;
            }

            // Simulate granting
            cout << "Simulating granting " << procName << "'s request." << "\n";
            bankers.applyRequest(pid, request);

            // Print new Need matrix
            bankers.printNeedWithHeader("New Need");

            // Check safety after granting
            if (bankers.isSafe()) {
                cout << procName << "'s request can be granted. The system will be in safe state." << "\n";
            } else {
                cout << procName << "'s request cannot be granted. The system will be in unsafe state." << "\n";
            }

        }
        else {
            cout << "The current system is in unsafe state." << "\n";
        }
    }

    return 0;
}
