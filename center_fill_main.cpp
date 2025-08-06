#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <atomic>
#include <csignal>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <iomanip>


/* Assumptions

World coordinates range from -10 to +10 (Y axis), and -10 to +10 (X axis)
There is alwasys at least one Central Fill Facility
There is always at least one medication in a facility

Coordinates are represented as integers
Medication is represented as a string
Price per Medication is represented as a float
Price is in USD
Distance is calculated using Manhattan distance
The final result is a list of the top 3 closest Central Fill Facilities to a given location in order of distance

*/


/*
Central Fill Facility Data Structure

Unique Identifier: 1
Coordinates: (x, y)  ; -10 <= x <=10 , -10 <= y <= 10
Medication : A, B, C, D, E...
Price per Medication: 10.0, 20.0, 30.0, 40.0, 50.0...
*/

// Random data ranges
int max_facilities = 10;  // max number of facilities
int max_coordinates = 10;  // coordinates range from -10 to +10
int min_coordinates = -10; // coordinates range from -10 to +10
int max_medications = 5; // max number of medications per facility
float max_price = 300.0f;    // max price per medication


// Central Fill Facility Class
class CentralFillFacility {
public:
    CentralFillFacility(int id, int x, int y, const std::vector<std::string>& meds, const std::vector<float>& prices)
        : id(id), x(x), y(y), medications(meds), prices(prices) { dist = 0; }

    // Getter Functions
    int getId() const { return id; }
    std::pair<int, int> getCoordinates() const { return {x, y}; }
    std::vector<std::string> getMedications() const { return medications; }
    std::vector<float> getPrices() const { return prices; }
    int getDistance() const { return dist; }

    // Setters for distance
    void setDistance(int distance) { dist = distance; }

    // Comparison operator for sorting (compares by ID)
    bool operator<(const CentralFillFacility& other) const {
        return id < other.id;
    }

private:
    int id;
    int x;
    int y;
    int dist; // distance from user location, this will be set later per user request
    std::vector<std::string> medications;
    std::vector<float> prices;
};


// Central Fill Facility Manager Class
class CentralFillFacilityManager {
public:
    void addFacility(const CentralFillFacility& facility) {
        facilities.push_back(facility);
    }

    // get all facilities
    const std::vector<CentralFillFacility>& getFacilities() {
        return facilities;
    }

    // Find Top 3 Central Fill Facilities with closest distance from a given location
    std::vector<CentralFillFacility> findTop3Facilities(const std::pair<int, int>& location) {
        std::vector<std::pair<int, CentralFillFacility>> distances;
        // corner case: no facilities
        if (facilities.empty()) {
            std::cerr << "No facilities available for search." << std::endl;
            return {}; // Return empty if no facilities
        }
        for (auto& facility : facilities) {
            int distance = calculateDistance(location, facility.getCoordinates());
            facility.setDistance(distance); // Set distance in facility
            distances.emplace_back(distance, facility);
        }
        std::sort(distances.begin(), distances.end());
        ////  if multiple facilities have the same distance, sort by ID
        // std::stable_sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        //     if (a.first == b.first) {
        //         return a.second.getId() < b.second.getId();
        //     }
        //     return a.first < b.first;
        // });
        ////  if multiple facilities have the same distance, sort number of medications
        // std::stable_sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        //     if (a.first == b.first) {
        //         return a.second.getMedications().size() < b.second.getMedications().size();
        //     }
        //     return a.first < b.first;
        // });
        //// if multiple facilities have the same distance, select the one with the cheapest medication
        // std::stable_sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        //     if (a.first == b.first) {
        //         auto aCheapest = std::min_element(a.second.getPrices().begin(), a.second.getPrices().end());
        //         auto bCheapest = std::min_element(b.second.getPrices().begin(), b.second.getPrices().end());
        //         return *aCheapest < *bCheapest;
        //     }
        //     return a.first < b.first;
        // });
        std::vector<CentralFillFacility> result;
        for (int i = 0; i < 3 && i < distances.size(); ++i) {
            result.push_back(distances[i].second);
        }

        return result;
    }

    // find cheapest medication in a facility
    std::pair<std::string, float> findCheapestMedication(const CentralFillFacility& facility) {
        // corner case: no medications
        if (facility.getPrices().empty()) {
            return {"No medications available", 0.0f};
        }
        std::vector<float> prices = facility.getPrices();
        std::vector<std::string> meds = facility.getMedications();

        float minPrice = 0.0f;
        int minIndex = -1;
        for (size_t i = 0; i < prices.size(); ++i) {
            if (i == 0 || prices[i] < minPrice) {
                minPrice = prices[i];
                minIndex = static_cast<int>(i);
            }
        }
        if (minIndex != -1) {
            return {meds[minIndex], minPrice};
        }
        return {"No medications available", 0.0f};
    }

private:
    std::vector<CentralFillFacility> facilities;

    int calculateDistance(const std::pair<int, int>& loc1, const std::pair<int, int>& loc2) {
        return std::abs(loc1.first - loc2.first) + std::abs(loc1.second - loc2.second);
    }
};


/*
Main Function to Demonstrate Usage
1. Initialize Central Fill Facilities
    1.1 Create random number of facilities with random coordinates, medications, and prices
2. Receive a location from the user
3. Find the top 3 closest facilities to the given location
*/

int main(void) {
    // Seed the random number generator with current time for different random data each run
    srand(static_cast<unsigned int>(time(0)));

    // Create Central Fill Facility Manager
    CentralFillFacilityManager IA_manager;

    // Initialize Central Fill Facilities
    int numFacilities = rand() % max_facilities + 1; // Random number between 1 and max_facilities
    if (numFacilities < 1) numFacilities = 1; // Ensure at least one facility
    for (int i = 0; i < numFacilities; ++i) {
        int x = rand() % (max_coordinates - min_coordinates + 1) + min_coordinates;
        int y = rand() % (max_coordinates - min_coordinates + 1) + min_coordinates;

        // Random medications and prices
        int numMeds = rand() % max_medications + 1; // Random number of medications between 1 and max_medications
        if(numMeds < 1) numMeds = 1; // Ensure at least one medication
        std::vector<std::string> medications(numMeds);
        std::vector<float> prices(numMeds);
        for (int j = 0; j < numMeds; ++j) {
            // use alphanumeric medication names Medication_A, Medication_B, etc.
            medications[j] = "Medication_" + std::string(1, 'A' + j);
            // medication prices are random floats between 1 and max_price with 2 decimal places
            prices[j] = static_cast<float>(rand() % static_cast<int>(max_price * 100) + 1) / 100.0f; // Random price between 1 and max_price
        }

        CentralFillFacility facility(i + 1, x, y, medications, prices);
        IA_manager.addFacility(facility);
    }

    // If you were working with a much larger world size
    // you might want to consider using a more efficient data structure
    // for spatial queries, such as a quadtree or a k-d tree.
    // quadtree is a good choice for 2D spatial data.
        // Large datasets (1000+ facilities)
        // Frequent spatial queries (many users asking for nearest facilities)
        // Clustered data (facilities grouped in cities/regions)
        // Range queries ("find all facilities within 5km")
    // k-d tree is better for high-dimensional data
    // but for our 2D case, quadtree is simpler and more efficient.
    // For now, we will use a simple vector to store facilities and sort them by distance later.

    // Print all facilities
    std::cout << "Central Fill Facilities:\n";
    std::cout << "Total Facilities: " << IA_manager.getFacilities().size() << "\n";
    std::cout << "-------------------------\n";
    for (const auto& facility : IA_manager.getFacilities()) {
        std::cout << "Facility ID: " << facility.getId() << "\n";
        std::cout << "Location: (" << facility.getCoordinates().first << ", " << facility.getCoordinates().second << ")\n";
        std::cout << "Medications and Prices:\n";
        auto meds = facility.getMedications();
        auto prices = facility.getPrices();
        for (size_t i = 0; i < meds.size(); ++i) {
            std::cout << "  " << meds[i] << ": $" << std::fixed << std::setprecision(2) << prices[i] << "\n";
        }
        std::cout << "\n";
    }   
    std::cout << "-------------------------\n";


    // get user input for location
    int userX, userY;
    std::cout << "Enter your location (x y): ";
    std::cin >> userX >> userY;
    // corner case : invalid input
    if (std::cin.fail() || userX < min_coordinates || userX > max_coordinates || userY < min_coordinates || userY > max_coordinates) {
        std::cerr << "Invalid input. Please enter coordinates between -10 and 10.\n";
        return 1; // Exit with error code
    }
    std::pair<int, int> userLocation(userX, userY);

    // Find the top 3 closest facilities
    auto closestFacilities = IA_manager.findTop3Facilities(userLocation);

    // Display the closest facilities
    std::cout << "Closest Facilities:\n";
    // corner case: no facilities found
    if (closestFacilities.empty()) {
        std::cout << "No facilities found.\n";
        return 0; // Exit gracefully
    }
    for (const auto& facility : closestFacilities) {
        std::pair<std::string, float> cheapestMedication = IA_manager.findCheapestMedication(facility);
        // Central Fill 002 - $30.29, Medication A, Distance 3
        std::cout << "-------------------------\n";
        std::cout << "Central Fill: " << facility.getId() << " - $" << std::fixed << std::setprecision(2) << cheapestMedication.second << ", " << cheapestMedication.first << ", Distance " << facility.getDistance() << "\n";
    }


    return 0;
}   