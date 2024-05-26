#ifndef ENGINE_ADVINPUT_H
#define ENGINE_ADVINPUT_H
#include <iostream>
#include "setupapi.h"
#include "hidapi.h"
#include <thread>

class AdvInput {
private:
    hid_device* device;
    bool running;

public:
    AdvInput() : device(nullptr), running(false) {}

    ~AdvInput() {
        if (device != nullptr) {
            hid_close(device);
            hid_exit();
        }
    }

    bool init() {
        // Inicjalizacja HIDAPI
        if (hid_init() != 0) {
            std::cerr << "Failed to initialize HIDAPI" << std::endl;
            return false;
        }

        // Otwarcie urządzenia HID o określonym VID (Vendor ID) i PID (Product ID)
        device = hid_open(0x045E, 0x0B12, nullptr);
        if (device == nullptr) {
            //hid_error(nullptr);
            std::cerr << "Failed to open Xbox controller" << std::endl;
            return false;
        }

        // Uruchomienie wątku do nasłuchiwania na przycisku "A"
        running = true;
        std::thread inputThread(&AdvInput::inputListener, this);
        inputThread.detach(); // Odepnij wątek od głównego wątku

        return true;
    }

private:
    void inputListener() {
        while (running) {
            unsigned char data[8];
            // Odczytaj dane z kontrolera Xboxa
            int bytesRead = hid_read(device, data, sizeof(data));

            if (bytesRead > 0 && data[2] == 0x14 && data[3] == 0x01) {
                // Jeśli przycisk "A" został naciśnięty
                std::cout << "Button A pressed!" << std::endl;

                // Aktywacja wibracji
                activateVibration();
            }
        }
    }

    void activateVibration() {
        unsigned char vibrationData[9] = {0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        // Wysłanie danych o wibracji do kontrolera Xboxa
        hid_write(device, vibrationData, sizeof(vibrationData));
    }
};
#endif
