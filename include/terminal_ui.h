#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include "interceptor_controller.h"
#include "enemy_missile.h"  
#include "detection_system.h"
#include "target.h"

class TerminalUI 
{
public:
    // Menu options enum for better code readability
    enum MenuOption
    {
        LIST = 1,
        LAUNCH = 2,
        DETECT = 3,
        AUTO_INTERCEPT = 4,
        LIVE_VIEW = 5,
        EXIT = 6
    };

    // Display methods
    void displayMainMenu() const;
    void displayAutoInterceptMenu() const;
    void displayLiveBattlefield(const InterceptorController &controller,
                               const std::vector<EnemyMissile> &enemyMissiles,
                               const std::vector<Target> &targets,
                               const std::vector<ThreatReport> &threats) const;

    // Input methods
    int getChoice(const std::string &prompt, int min, int max) const;
    
    // Live view functionality
    void runLiveView(InterceptorController &controller,
                    std::vector<EnemyMissile> &enemyMissiles,
                    const std::vector<Target> &targets,
                    DetectionSystem &radar) const;

    // New methods for handling main menu options
    void handleLaunchMissile(InterceptorController &controller, const std::vector<Target> &targets) const;
    void handleThreatDetection(InterceptorController &controller,
                               std::vector<EnemyMissile> &enemyMissiles,
                               DetectionSystem &radar) const;
    void handleAutoInterceptMenu(InterceptorController &controller) const;

    // Utility methods
    void clearScreen() const;
    void resetCursor() const;
    bool removeEnemyMissileById(std::vector<EnemyMissile> &enemies, int id) const;

private:
    // Color constants
    static constexpr const char* RESET = "\033[0m";
    static constexpr const char* BOLD = "\033[1m";
    static constexpr const char* GREEN = "\033[32m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* CYAN = "\033[36m";
    static constexpr const char* RED = "\033[31m";
    static constexpr const char* BLUE = "\033[34m";
    static constexpr const char* MAGENTA = "\033[35m";

    // Display constants
    static constexpr int LIVE_VIEW_UPDATE_MS = 1500;
    static constexpr int STARTUP_DELAY_SECONDS = 2;
    static constexpr int MAX_AUTO_INTERCEPT_MISSILES = 10;
};

#endif