#pragma once

#include<diffgeomeng/classes/diff/Metric.hpp>

class SchwarzschildMetric : public Metric {
public:
    SchwarzschildMetric(double mass);
private:
    double mass;
};