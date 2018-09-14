//
//  EuclideanVector.cpp
//  Assignment2
//
//  Created by Ethan Xu on 2/9/17.
//  Copyright © 2017 Ethan Xu. All rights reserved.
//

#include "EuclideanVector.h"
#include <cmath>

evec::EuclideanVector::EuclideanVector(unsigned dimensions) {
    this->dimensions = dimensions;
    magnitudes = new double[dimensions];
    for (unsigned i = 0; i < dimensions; i++) {
        magnitudes[i] = 0;
    }
    norm = 0;
}

evec::EuclideanVector::EuclideanVector() {
    EuclideanVector{1};
}

evec::EuclideanVector::EuclideanVector(unsigned dimensions, double magnitude) {
    this->dimensions = dimensions;
    magnitudes = new double[dimensions];
    for (unsigned i = 0; i < dimensions; i++) {
        magnitudes[i] = magnitude;
    }
    norm = 0;
}

evec::EuclideanVector::EuclideanVector(const std::initializer_list<double>& list) {
    dimensions = list.size();
    magnitudes = new double[dimensions];
    unsigned i = 0;
    for (auto magnitude: list) {
        magnitudes[i] = magnitude;
        i++;
    }
    norm = 0;
}

evec::EuclideanVector::EuclideanVector(const EuclideanVector& ev) {
    dimensions = ev.dimensions;
    magnitudes = new double[dimensions];
    for (unsigned i = 0; i < dimensions; i++) {
        magnitudes[i] = ev.magnitudes[i];
    }
    norm = 0;
}

evec::EuclideanVector::EuclideanVector(EuclideanVector&& ev) {
    dimensions = std::move(ev.dimensions);
    magnitudes = ev.magnitudes;
    norm = std::move(ev.norm);
    ev.dimensions = 0;
    ev.magnitudes = nullptr;
    ev.norm = 0;
}

evec::EuclideanVector::~EuclideanVector() {
    delete[] magnitudes;
}

unsigned evec::EuclideanVector::getNumDimensions() {
    return dimensions;
}

double evec::EuclideanVector::get(unsigned dimension) {
    return magnitudes[dimension];
}

double evec::EuclideanVector::getEuclideanNorm() {
    if (norm) {
        return norm;
    } else {
        norm = 0;
        for (unsigned i = 0; i < dimensions; i++) {
            norm += magnitudes[i] * magnitudes[i];
        }
        norm = sqrt(norm);
        return norm;
    }
}

evec::EuclideanVector& evec::EuclideanVector::createUnitVector() {
    getEuclideanNorm();
    std::vector<double> v;
    for (unsigned i = 0; i < dimensions; i++) {
        v.push_back(magnitudes[i] / norm);
    }
    return *new evec::EuclideanVector(v.cbegin(), v.cend());
}

double& evec::EuclideanVector::operator[](unsigned dimension) {
    return magnitudes[dimension];
}

void evec::EuclideanVector::operator+=(const EuclideanVector& ev) {
    if (dimensions != ev.dimensions) {
        throw std::runtime_error("The dimensions must be same");
    }
    for (unsigned i = 0; i < dimensions; i++) {
        magnitudes[i] += ev.magnitudes[i];
    }
}

void evec::EuclideanVector::operator-=(const EuclideanVector& ev) {
    if (dimensions != ev.dimensions) {
        throw std::runtime_error("The dimensions must be same");
    }
    for (unsigned i = 0; i < dimensions; i++) {
        magnitudes[i] -= ev.magnitudes[i];
    }
}

void evec::EuclideanVector::operator*=(const double& d) {
    for (unsigned i = 0; i < dimensions; i++) {
        magnitudes[i] *= d;
    }
}
void evec::EuclideanVector::operator/=(const double& d) {
    if (d == 0) {
        throw std::runtime_error("The divisor cannot be 0");
    }
    for (unsigned i = 0; i < dimensions; i++) {
        magnitudes[i] /= d;
    }
}

evec::EuclideanVector::operator std::vector<double> () const {
    std::vector<double> v;
    for (unsigned i = 0; i < dimensions; i++) {
        v.push_back(magnitudes[i]);
    }
    return v;
}

evec::EuclideanVector::operator std::list<double> () const {
    std::list<double> l;
    for (unsigned i = 0; i < dimensions; i++) {
        l.push_back(magnitudes[i]);
    }
    return l;
}

evec::EuclideanVector& evec::EuclideanVector::operator=(const EuclideanVector& ev) {
    if (this != &ev) {
        dimensions = ev.dimensions;
        magnitudes = new double[dimensions];
        for (unsigned i = 0; i < dimensions; i++) {
            magnitudes[i] = ev.magnitudes[i];
        }
        norm = 0;
    }
    return *this;
}

evec::EuclideanVector& evec::EuclideanVector::operator=(EuclideanVector&& ev) {
    if (this != &ev) {
        dimensions = std::move(ev.dimensions);
        magnitudes = ev.magnitudes;
        norm = std::move(ev.norm);
        ev.dimensions = 0;
        ev.magnitudes = nullptr;
        ev.norm = 0;
    }
    return *this;
}
