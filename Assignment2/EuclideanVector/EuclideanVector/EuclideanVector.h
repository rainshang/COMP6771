//
//  EuclideanVector.h
//  Assignment2
//
//  Created by Ethan Xu on 2/9/17.
//  Copyright © 2017 Ethan Xu. All rights reserved.
//

#ifndef EuclideanVector_h
#define EuclideanVector_h

#include <ostream>
#include <initializer_list>
#include <vector>
#include <list>
#include <stdexcept>

namespace evec {
    class EuclideanVector {
    private:
        double* magnitudes;
        unsigned dimensions;
        double norm;
    public:
        EuclideanVector(unsigned);
        EuclideanVector();
        EuclideanVector(unsigned, double);
        
        template <typename Iterator>
        EuclideanVector(const Iterator& begin, const Iterator& end) {
            dimensions = std::distance(begin, end);
            magnitudes = new double[dimensions];
            unsigned i = 0;
            for (Iterator pos = begin; pos != end; pos++, i++) {
                magnitudes[i] = *pos;
            }
            norm = 0;
        };
        
        EuclideanVector(const std::initializer_list<double>&);
        EuclideanVector(const EuclideanVector&);
        EuclideanVector(EuclideanVector&&);
        ~EuclideanVector();
        
        unsigned getNumDimensions();
        double get(unsigned);
        double getEuclideanNorm();
        EuclideanVector& createUnitVector();
        
        double& operator[](unsigned);
        void operator+=(const EuclideanVector&);
        void operator-=(const EuclideanVector&);
        void operator*=(const double&);
        void operator/=(const double&);
        operator std::vector<double> () const;
        operator std::list<double> () const;
        EuclideanVector& operator=(const EuclideanVector&);//copy assignment operator
        EuclideanVector& operator=(EuclideanVector&&);//move assignment operator
        
        friend bool operator==(const evec::EuclideanVector& ev1, const evec::EuclideanVector& ev2) {
            if (ev1.dimensions != ev2.dimensions) {
                return false;
            } else {
                for (unsigned i = 0; i < ev1.dimensions; i++) {
                    if (ev1.magnitudes[i] != ev2.magnitudes[i]) {
                        return false;
                    }
                }
                return true;
            }
        }
        
        friend bool operator!=(const evec::EuclideanVector& ev1, const evec::EuclideanVector& ev2) {
            return !(ev1 == ev2);
        }
        
        friend evec::EuclideanVector& operator+(const evec::EuclideanVector& ev1, const evec::EuclideanVector& ev2) {
            if (ev1.dimensions != ev2.dimensions) {
                throw std::runtime_error("The dimensions must be same");
            }
            std::vector<double> newMagnitudes;
            for (unsigned i = 0; i < ev1.dimensions; i++) {
                newMagnitudes.push_back(ev1.magnitudes[i] + ev2.magnitudes[i]);
            }
            return *new evec::EuclideanVector(newMagnitudes.cbegin(), newMagnitudes.cend());
        }
        
        friend evec::EuclideanVector& operator-(const evec::EuclideanVector& ev1, const evec::EuclideanVector& ev2) {
            if (ev1.dimensions != ev2.dimensions) {
                throw std::runtime_error("The dimensions must be same");
            }
            std::vector<double> newMagnitudes;
            for (unsigned i = 0; i < ev1.dimensions; i++) {
                newMagnitudes.push_back(ev1.magnitudes[i] - ev2.magnitudes[i]);
            }
            return *new evec::EuclideanVector(newMagnitudes.cbegin(), newMagnitudes.cend());
        }
        
        friend double operator*(const evec::EuclideanVector& ev1, const evec::EuclideanVector& ev2) {
            if (ev1.dimensions != ev2.dimensions) {
                throw std::runtime_error("The dimensions must be same");
            }
            double sum = 0;
            for (unsigned i = 0; i < ev1.dimensions; i++) {
                sum += ev1.magnitudes[i] * ev2.magnitudes[i];
            }
            return sum;
        }
        
        friend evec::EuclideanVector& operator*(const evec::EuclideanVector& ev, const double& d) {
            std::vector<double> newMagnitudes;
            for (unsigned i = 0; i < ev.dimensions; i++) {
                newMagnitudes.push_back(ev.magnitudes[i] * d);
            }
            return *new evec::EuclideanVector(newMagnitudes.cbegin(), newMagnitudes.cend());
        }
        
        friend evec::EuclideanVector& operator/(const evec::EuclideanVector& ev, const double& d) {
            if (d == 0) {
                throw std::runtime_error("The divisor cannot be 0");
            }
            std::vector<double> newMagnitudes;
            for (unsigned i = 0; i < ev.dimensions; i++) {
                newMagnitudes.push_back(ev.magnitudes[i] / d);
            }
            return *new evec::EuclideanVector(newMagnitudes.cbegin(), newMagnitudes.cend());
        }
        
        friend std::ostream& operator<<(std::ostream& os, const evec::EuclideanVector& ev) {
            os << '[';
            for (unsigned i = 0; i < ev.dimensions; i++) {
                os << ev.magnitudes[i];
                if (i != ev.dimensions - 1) {
                    os << ' ';
                }
            }
            os << ']';
            return os;
        }
        
    };
}

#endif /* EuclideanVector_h */
