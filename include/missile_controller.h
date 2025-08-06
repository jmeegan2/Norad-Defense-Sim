#ifndef MISSILE_CONTROLLER_H
#define MISSILE_CONTROLLER_H

#include <vector>
#include <algorithm>
#include "interceptor_missile.h"
#include "detection_system.h"

class MissileController
{
public:
    void addMissile(const InterceptorMissile &missile);
    void moveAllMissiles(double dx, double dy, double dz);
    void printAllStatuses() const;
    void launchMissile(InterceptorMissile &missile, const Position &target);
    InterceptorMissile *getMissileById(int id);
    bool removeDefensiveMissile(int id);
    void detectIncomingMissiles();
    int interceptThreat(const ThreatReport& threat);
    
    // Auto-intercept functionality
    void setAutoIntercept(bool enabled);
    bool isAutoInterceptEnabled() const;
    void setAutoInterceptThreshold(double threshold);
    void setMaxAutoInterceptMissiles(int maxMissiles);
    std::vector<int> autoInterceptThreats(const std::vector<ThreatReport>& threats);
    void printAutoInterceptStatus() const;
    
    // Utility methods for auto-intercept
    int getAvailableMissileCount() const;
    bool hasAvailableMissiles() const;

private:
    std::vector<InterceptorMissile> missiles;
    
    // Auto-intercept settings
    bool autoInterceptEnabled = false;
    double autoInterceptThreshold = 2000.0;  // Only auto-intercept if threat is within this distance (km)
    int maxAutoInterceptMissiles = 3;        // Maximum missiles to use for auto-intercept
    int usedAutoInterceptMissiles = 0;       // Track how many we've used this session
    
    // Helper methods
    std::vector<ThreatReport> prioritizeThreats(const std::vector<ThreatReport>& threats) const;
    bool shouldInterceptThreat(const ThreatReport& threat) const;
    InterceptorMissile* selectBestInterceptor(const ThreatReport& threat);
    
};

#endif