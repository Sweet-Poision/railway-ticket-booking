#include <iostream>
#include <vector>
#include <string>
#include <optional> // For std::optional

class Compartment {
public:
    Compartment();

    [[nodiscard]] bool checkAvailability(int seatCount) const;  // Ensure return value is used
    std::optional<std::vector<std::string>> allocateSeats(int seatCount);

private:
    std::vector<std::vector<int>> seats;  // Seating layout (1 for available, 0 for booked)
    int availableSeats;                   // Total available seats

    std::string seatLabel(int rowIndex, int seatIndex) const;   // Helper to generate seat names
    bool allocateConsecutiveSeats(int rowIndex, int seatCount, std::vector<std::string>& allocatedSeats);
};

Compartment::Compartment()
    : seats({
        {1, 1, 1, 1, 1, 1, 1},  // Row A
        {1, 1, 1, 1, 1, 1, 1},  // Row B
        {1, 1, 1, 1, 1, 1, 1},  // Row C
        {1, 1, 1, 1, 1, 1, 1},  // Row D
        {1, 1, 1, 1, 1, 1, 1},  // Row E
        {1, 1, 1, 1, 1, 1, 1},  // Row F
        {1, 1, 1, 1, 1, 1, 1},  // Row G
        {1, 1, 1, 1, 1, 1, 1},  // Row H
        {1, 1, 1, 1, 1, 1, 1},  // Row I
        {1, 1, 1, 1, 1, 1, 1},  // Row J
        {1, 1, 1, 1, 1, 1, 1},  // Row K
        {1, 1, 1}               // Row L (3 seats)
    }), availableSeats(80) {}

[[nodiscard]] bool Compartment::checkAvailability(int seatCount) const {
    return availableSeats >= seatCount;
}

std::string Compartment::seatLabel(int rowIndex, int seatIndex) const {
    const std::string rowNames = "ABCDEFGHIJKL";
    return rowNames[rowIndex] + std::to_string(seatIndex + 1);
}

bool Compartment::allocateConsecutiveSeats(int rowIndex, int seatCount, std::vector<std::string>& allocatedSeats) {
    int consecutiveSeats = 0;
    std::vector<int> tempSeats;

    // Directly use reference to the row vector
    auto& rowSeats = seats[rowIndex];

    for (int seatIndex = 0; seatIndex < rowSeats.size(); ++seatIndex) {
        if (rowSeats[seatIndex] == 1) {  // 1 indicates the seat is available
            tempSeats.push_back(seatIndex);
            ++consecutiveSeats;
        } else {
            tempSeats.clear();
            consecutiveSeats = 0;
        }

        if (consecutiveSeats == seatCount) {
            for (const auto& index : tempSeats) {
                rowSeats[index] = 0;  // Mark seat as allocated with '0'
                allocatedSeats.push_back(seatLabel(rowIndex, index));
            }
            availableSeats -= seatCount;
            return true;
        }
    }
    return false;
}

std::optional<std::vector<std::string>> Compartment::allocateSeats(int seatCount) {
    std::vector<std::string> allocatedSeats;

    // Priority 1: Try to allocate consecutive seats in a single row
    for (int rowIndex = 0; rowIndex < seats.size(); ++rowIndex) {
        if (allocateConsecutiveSeats(rowIndex, seatCount, allocatedSeats)) {
            return allocatedSeats; // Return early if seats are allocated
        }
    }

    // Priority 2: Allocate any available seats
    for (int rowIndex = 0; rowIndex < seats.size(); ++rowIndex) {
        for (int seatIndex = 0; seatIndex < seats[rowIndex].size(); ++seatIndex) {
            if (seats[rowIndex][seatIndex] == 1) {  // 1 indicates the seat is available
                allocatedSeats.push_back(seatLabel(rowIndex, seatIndex));
                seats[rowIndex][seatIndex] = 0;  // Mark seat as allocated
                --seatCount;

                if (seatCount == 0) {
                    availableSeats -= allocatedSeats.size();
                    return allocatedSeats;
                }
            }
        }
    }

    // If we couldn't allocate the requested number of seats, return std::nullopt
    return std::nullopt;
}

int main() {
    Compartment compartment;

    int n;
    std::cout << "Enter number of booking requests: ";
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        int requestedSeats;
        std::cout << "Enter number of seats to book: ";
        std::cin >> requestedSeats;

        // Check availability using the modern `[[nodiscard]]` function
        if (compartment.checkAvailability(requestedSeats)) {
            auto allocatedSeatsOpt = compartment.allocateSeats(requestedSeats);

            if (allocatedSeatsOpt) {
                const auto& allocatedSeats = allocatedSeatsOpt.value();
                for (const auto& seat : allocatedSeats) {
                    std::cout << seat << " ";
                }
                std::cout << '\n';
            } else {
                std::cout << "No seats available\n";
            }
        } else {
            std::cout << "No seats available\n";
        }
    }
    
    return EXIT_SUCCESS;
}