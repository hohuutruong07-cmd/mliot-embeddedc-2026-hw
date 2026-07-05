// HW02: KIẾN TRÚC PHẦN MỀM VÀ QUẢN LÝ BỘ NHỚ SMART BIKE

#include <stdio.h>
#include <stdint.h>

// TASK 1: STRUCT, UNION & BIT-FIELDS

typedef union {
    uint16_t raw_value;
    struct {
        // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
        
        uint16_t PWR_ON       : 1;  // Bit [0]
        uint16_t ASSIST_LEVEL : 2;  // Bit [2:1]
        uint16_t LIGHT_BRIGHT : 4;  // Bit [6:3]
        uint16_t RESERVED     : 9;  // Bit [15:7]

        // HỌC VIÊN KẾT THÚC VIẾT CODE
    } fields;
} Bike_Status_t;


// TASK 2: FUNCTION POINTERS & TABLE-DRIVEN ARCHITECTURE

void drive_eco(void) {
    printf("Bike Mode: ECO.\n");
}
void drive_normal(void) {
    printf("Bike Mode: NORMAL.\n");
}
void drive_sport(void) {
    printf("Bike Mode: SPORT.\n");
}

// HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

void (*drive_modes[])(void) = {
    drive_eco,     // 0 = ECO
    drive_normal,  // 1 = NORMAL
    drive_sport    // 2 = SPORT
};

// HỌC VIÊN KẾT THÚC VIẾT CODE


// TASK 3: ARCHITECTURE CALLBACKS

void Battery_Monitor(void (*overheat_cb)(void)) {
    int battery_temp = 45; 
    
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

// Nhiệt độ vượt ngưỡng an toàn (> 40°C)
    if (battery_temp > 40) {
        // Kiểm tra tính hợp lệ của con trỏ hàm trước khi gọi (tránh Null Pointer Exception)
        if (overheat_cb != NULL) {
            (*overheat_cb)(); // Hoặc overheat_cb();
        }
    }

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

void Critical_Battery_Handler(void) {
    printf("[WARNING] Battery is overheated!\n");
}


// TASK 4: MEMORY MAP & CRASH LAB

const char BIKE_MODEL[] = "E-Bike X2026"; 
uint32_t total_odometer = 0;             

void crash_simulation(void) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

// Khai báo một mảng cục bộ để tiêu tốn không gian vùng nhớ Stack nhanh hơn
    volatile uint8_t stack_drainer[256];
    stack_drainer[0] = 0xAA;
    
    // Gọi đệ quy vô hạn không điều kiện dừng nhằm ép tràn bộ nhớ ngăn xếp
    crash_simulation();


    // HỌC VIÊN KẾT THÚC VIẾT CODE
}


// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm)

int main() {
    Bike_Status_t my_bike;
    
    // 1. Test Task 1
    my_bike.raw_value = 0x0025; 
    printf("REGISTER STRUCTURE: \n");
    printf("PWR_ON: %d | ASSIST_LEVEL: %d | LIGHT_BRIGHT: %d\n\n", 
           my_bike.fields.PWR_ON, my_bike.fields.ASSIST_LEVEL, my_bike.fields.LIGHT_BRIGHT);

    // 2. Test Task 2
    printf("ENGINE CONTROLLING: \n");
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    
    // Lấy cấp độ trợ lực từ bit-fields của thanh ghi trạng thái
    uint8_t current_level = my_bike.fields.ASSIST_LEVEL;
    
    // Tính toán số lượng phần tử trong mảng drive_modes để kiểm tra an toàn biên (Bounds checking)
    size_t total_modes = sizeof(drive_modes) / sizeof(drive_modes[0]);

    if (current_level < total_modes) {
        // Giải con trỏ hàm và thực thi hàm tương ứng trực tiếp từ mảng
        (*drive_modes[current_level])(); // Hoặc viết gọn: drive_modes[current_level]();
    } else {
        printf("[Error] Invalid Assist Level: Index out of bounds!\n");
    }

    // HỌC VIÊN KẾT THÚC VIẾT CODE

    // 3. Test Task 3
    Battery_Monitor(Critical_Battery_Handler);
    printf("\n");

    // 4. Test Task 4
    int current_speed = 25; 
    printf("MEMORY ADDRESSES OF VARIABLES \n");
    printf("BIKE_MODEL (ROM/.rodata):   %p\n", (void*)BIKE_MODEL);
    printf("total_odometer (RAM/.bss):  %p\n", (void*)&total_odometer);
    printf("current_speed (RAM/Stack):  %p\n", (void*)&current_speed);

    // Bỏ comment dòng dưới để chạy thử bài Crash Lab
    // crash_simulation();

    return 0;
}