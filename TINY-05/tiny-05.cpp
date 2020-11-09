#include <complex.h>
#include "tiny-05.hpp"

namespace pjc {

    complex::complex(double real, double imaginary) :
            m_real(real),
            m_imag(imaginary) {}

    double complex::real() const {
        return m_real;
    }

    void complex::real(double d) {
        m_real = d;
    }

    double complex::imag() const {
        return m_imag;
    }

    void complex::imag(double d) {
        m_imag = d;
    }

    complex operator+(const complex &lhs, const complex &rhs) {
        return {lhs.real() + rhs.real(), lhs.imag() + rhs.imag()};
    }

    complex operator+(double lhs, const complex &rhs) {
        return {lhs + rhs.real(), rhs.imag()};
    }

    complex operator+(complex rhs, const double &lhs) {
        return {lhs + rhs.real(), rhs.imag()};
    }

    complex operator-(const complex &lhs, const complex &rhs) {
        return {lhs.real() - rhs.real(), lhs.imag() - rhs.imag()};
    }

    complex operator-(double lhs, const complex &rhs) {
        return {lhs - rhs.real(), -rhs.imag()};
    }

    complex operator-(complex rhs, const double &lhs) {
        return {rhs.real() - lhs, rhs.imag()};
    }

    complex operator*(const complex &lhs, const complex &rhs) {
        return {(lhs.real() * rhs.real()) - (lhs.imag() * rhs.imag()),
                rhs.imag() * lhs.real() + lhs.imag() * rhs.real()};
    }

    complex operator*(double lhs, const complex &rhs) {
        return {lhs * rhs.real(), lhs * rhs.imag()};
    }

    complex operator*(complex rhs, const double &lhs) {
        return {lhs * rhs.real(), lhs * rhs.imag()};
    }


}


