#include <iostream>

enum class side {LEFT, RIGHT, NONE};
side branchPositions[6];

int main() {
    // for (int i = 0; i < 6; i++) {
    //     std::cout << static_cast<std::underlying_type<side>::type>(branchPositions[i]) << std::endl;
    // }
    int arr[] {1, 2, 3, 4, 5};
    std::cout << sizeof(arr) / sizeof(arr[0]) << std::endl;
    return 0;
}