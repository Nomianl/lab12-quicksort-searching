/*
 * Course: COEN 2220 - Programming 2
 * Name: Keanel A. Martínez Aguilar
 * Lab: Lab 12 - Quicksort, Searching, and Project Application
 * Description: Delivery order reporting view and ID lookup practice
 * Due date: November 2026
 */

#include <iostream>
#include <string>

using namespace std;

struct OrderPreview {
    int orderId;
    string orderType;
    string status;
    string building;
    int priority;
};

bool comesBeforeOrEqual(const OrderPreview *left, const OrderPreview *right);
void swapOrderPointers(OrderPreview *&left, OrderPreview *&right);
int partitionReport(OrderPreview *view[], int first, int last);
void quickSortReport(OrderPreview *view[], int first, int last);
int findOrderInView(OrderPreview *view[], int count, int targetId);
void displayOrders(const OrderPreview orders[], int count);
void displayReport(OrderPreview *view[], int count);

int main() {
    const int CAPACITY = 6;
    OrderPreview orders[CAPACITY] = {
        {420, "DELIVERY", "PENDING",     "Biblioteca",      1},
        {180, "DELIVERY", "IN_PROGRESS", "Ingenieria",      3},
        {650, "DINE_IN",  "PENDING",     "Cafeteria-Lunch", 1},
        {310, "DELIVERY", "PENDING",     "Biblioteca",      3},
        {205, "DELIVERY", "PENDING",     "Biblioteca",      3},
        {900, "DELIVERY", "COMPLETED",   "Ingenieria",      2}
    };

    OrderPreview *reportView[CAPACITY]; // Borrow addresses; do not delete these objects.
    int reportCount = 0;

    cout << boolalpha << "Original orders before filtering:" << endl;
    displayOrders(orders, CAPACITY);

    // E1: Filter active DELIVERY orders (PENDING or IN_PROGRESS)
    for (int index = 0; index < CAPACITY; index++) {
        if (orders[index].orderType == "DELIVERY" && 
           (orders[index].status == "PENDING" || orders[index].status == "IN_PROGRESS")) {
            reportView[reportCount] = &orders[index];
            reportCount++;
        }
    }

    cout << "\nFiltered active delivery orders (Unsorted Report View):" << endl;
    displayReport(reportView, reportCount);

    // E2: Sort the active delivery report view using quicksort
    quickSortReport(reportView, 0, reportCount - 1);

    cout << "\nSorted active delivery orders (Report View):" << endl;
    displayReport(reportView, reportCount);

    // E3: Search for an order ID in the report view
    int targetId;
    cout << "\nEnter an order ID to search: ";
    if (!(cin >> targetId)) {
        cout << "Invalid ID input." << endl;
        return 1;
    }

    int position = findOrderInView(reportView, reportCount, targetId);
    if (position == -1) {
        cout << "No active delivery order with that ID." << endl;
    } else {
        cout << "Found Order ID: " << reportView[position]->orderId
             << " | Building: " << reportView[position]->building
             << " | Status: " << reportView[position]->status
             << " | Priority: " << reportView[position]->priority << endl;
    }

    // E4: Prove original orders remain completely unchanged
    cout << "\nOriginal orders after generating report and search (unchanged):" << endl;
    displayOrders(orders, CAPACITY);

    return 0;
}

// Comparator following the 3 rules:
// 1. Building ascending
// 2. Priority descending (higher number first)
// 3. Order ID ascending (tie-breaker)
bool comesBeforeOrEqual(const OrderPreview *left, const OrderPreview *right) {
    if (left->building < right->building) return true;
    if (left->building > right->building) return false;

    // Buildings are equal, compare priority descending
    if (left->priority > right->priority) return true;
    if (left->priority < right->priority) return false;

    // Priorities are equal, compare orderId ascending
    return left->orderId <= right->orderId;
}

void swapOrderPointers(OrderPreview *&left, OrderPreview *&right) {
    OrderPreview *temp = left;
    left = right;
    right = temp;
}

int partitionReport(OrderPreview *view[], int first, int last) {
    int middle = first + (last - first) / 2;
    swapOrderPointers(view[first], view[middle]);

    int boundary = first;
    for (int scan = first + 1; scan <= last; scan++) {
        if (comesBeforeOrEqual(view[scan], view[first])) {
            boundary++;
            swapOrderPointers(view[boundary], view[scan]);
        }
    }
    swapOrderPointers(view[first], view[boundary]);
    return boundary;
}

void quickSortReport(OrderPreview *view[], int first, int last) {
    if (first >= last) {
        return;
    }
    int pivotIndex = partitionReport(view, first, last);
    quickSortReport(view, first, pivotIndex - 1);
    quickSortReport(view, pivotIndex + 1, last);
}

// E3: Linear search on the report view since it is ordered by building/priority rather than ID alone
int findOrderInView(OrderPreview *view[], int count, int targetId) {
    for (int index = 0; index < count; index++) {
        if (view[index]->orderId == targetId) {
            return index;
        }
    }
    return -1;
}

void displayOrders(const OrderPreview orders[], int count) {
    for (int index = 0; index < count; index++) {
        cout << "ID: " << orders[index].orderId
             << " | Type: " << orders[index].orderType
             << " | Status: " << orders[index].status
             << " | Building: " << orders[index].building
             << " | Priority: " << orders[index].priority << endl;
    }
}

void displayReport(OrderPreview *view[], int count) {
    for (int index = 0; index < count; index++) {
        cout << "View index: " << index
             << " | ID: " << view[index]->orderId
             << " | Building: " << view[index]->building
             << " | Status: " << view[index]->status
             << " | Priority: " << view[index]->priority << endl;
    }
}