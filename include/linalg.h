#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include "constants.h" // for flags

template <typename T>
class Vector3
{
private:
public:
    T x, y, z;
    // normal constructors
    Vector3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
    // construct from std vector
    Vector3(const std::vector<T> &v)
    {
        if (v.size() != 3)
            throw std::invalid_argument("Vector dimension must be 3");
        x = v[0];
        y = v[1];
        z = v[2];
    }

    // accessing elements (write)
    T &operator[](int i)
    {
        if (i < 0 or i > 2)
            throw std::out_of_range("Vector dimension exceeded");
        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
        throw std::runtime_error("Unexpected error accessing vector component");
    }

    // accessing elements (read, const)
    T operator[](const int i) const
    {
        if (i < 0 or i > 2)
            throw std::out_of_range("Vector dimension exceeded");
        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
        throw std::runtime_error("Unexpected error accessing vector component");
    }

    // equality
    friend bool operator==(const Vector3 &a, const Vector3 &b)
    {
        return a.x == b.x and a.y == b.y and a.z == b.z;
    }

    // printing vectors
    friend std::ostream &operator<<(std::ostream &os, const Vector3 &v)
    {
        os << v[0] << " " << v[1] << " " << v[2] << std::endl;
        return os;
    };

    // add vectors
    Vector3<T> operator+(const Vector3<T> &v) const
    {
        Vector3<T> result;

        for (int i = 0; i < 3; i++)
            result[i] = (*this)[i] + v[i];

        return result;
    }

    // substract vectors
    Vector3<T> operator-(const Vector3<T> &v) const
    {
        Vector3<T> result;

        for (int i = 0; i < 3; i++)
            result[i] = (*this)[i] - v[i];

        return result;
    }

    // dot product
    T operator*(const Vector3<T> &v) const
    {
        T result = (T)0;
        for (int i = 0; i < 3; i++)
            result += (*this)[i] * v[i];
        return result;
    }
    // cross product
    friend Vector3<T> cross(const Vector3<T> &v, const Vector3<T> &w)
    {
        Vector3<T> result;
        result.x = v.y * w.z - w.y * v.z;
        result.y = v.z * w.x - w.z * v.x;
        result.z = v.x * w.y - w.x * v.y;
        return result;
    }

    // vector-scalar operations
    Vector3<T> operator*(const T n) const
    {
        Vector3<T> result;
        for (int i = 0; i < 3; i++)
            result[i] = (*this)[i] * n;
        return result;
    }
    friend Vector3<T> operator*(const T n, const Vector3<T> &v)
    {
        return v * n;
    }

    // like is used to compare vectors of (maybe) different types with a certain tolerance (double precision)
    template <typename V>
    bool like(const Vector3<V> &v, double TOLERANCE = 1e-14 /*tolerance should be enough for most physical applications*/) const
    {
        for (int i = 0; i < 3; i++)
        {
            double v1 = static_cast<double>((*this)[i]);
            double v2 = static_cast<double>(v[i]);
            if (std::abs(v1 - v2) >= TOLERANCE)
                return false;
        }
        return true;
    }

    // vector properties and spherical interface
    double r() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    T r2() const // usually more efficient
    {
        return x * x + y * y + z * z;
    }
    double theta() const // angle in xy plane
    {
        if (y == (T)0 and x == (T)0)
            return 0;
        return std::atan2(y, x);
    }
    double phi() const // drop from z-axis
    {
        double rr = r();
        if (rr == 0)
            return 0;
        return std::acos(z / rr);
    }

    static Vector3 fromSpherical(T r, T theta, T phi)
    {
        return Vector3(
            r * std::sin(phi) * std::cos(theta),
            r * std::sin(phi) * std::sin(theta),
            r * std::cos(phi));
    }
};

template <typename T>
class Matrix
{
private:
    int cols;
    int rows;
    T *data;

public:
    // normal constructor
    Matrix(int rows, int cols) : rows(rows), cols(cols)
    {
        data = new T[cols * rows];
        for (int i = 0; i < cols * rows; i++)
            data[i] = (T)0;
    }
    // construct from 2d std vector
    Matrix(std::vector<std::vector<T>> v)
    {
        if (v.size() == 0)
        {
            throw std::invalid_argument("Matrix cannot have dimension 0");
        }
        if (v[0].size() == 0)
        {
            throw std::invalid_argument("Matrix cannot have dimension 0");
        }
        rows = v.size();
        cols = v[0].size();
        data = new T[cols * rows];
        for (int j = 0; j < rows; j++)
            for (int i = 0; i < cols; i++)
                (*this)(j, i) = v[j][i];
    }

    // copy constructor
    Matrix(const Matrix &other)
    {
        rows = other.rows;
        cols = other.cols;
        data = new T[rows * cols];
        for (int i = 0; i < rows * cols; i++)
            data[i] = other.data[i];
    }

    // assignment
    Matrix &operator=(const Matrix &other)
    {
        if (this == &other)
            return *this;
        delete[] data;
        rows = other.rows;
        cols = other.cols;
        data = new T[rows * cols];
        for (int i = 0; i < rows * cols; i++)
            data[i] = other.data[i];
        return *this;
    }

    // get rid of the memory held in data
    ~Matrix()
    {
        delete[] data;
    }

    // accessing elements (write)
    T &operator()(int y, int x)
    {
        if constexpr (Math::SAFE_MATRICES)
        {
            if (x >= cols)
            {
                throw std::out_of_range("Matrix width exceeded");
            }
            if (y >= rows)
            {
                throw std::out_of_range("Matrix height exceeded");
            }
        }
        return data[cols * y + x];
    }

    // accessing elements (read, const)
    T operator()(int y, int x) const
    {
        if constexpr (Math::SAFE_MATRICES)
        {
            if (x >= cols)
            {
                throw std::out_of_range("Matrix width exceeded");
            }
            if (y >= rows)
            {
                throw std::out_of_range("Matrix height exceeded");
            }
        }
        return data[cols * y + x];
    }

    // getters for rows and cols
    int getRows() const
    {
        return rows;
    }
    int getCols() const
    {
        return cols;
    }

    // equality
    friend bool operator==(const Matrix &a, const Matrix &b)
    {
        if (a.cols != b.cols or a.rows != b.rows)
            return false;
        for (int i = 0; i < a.cols; i++)
            for (int j = 0; j < a.rows; j++)
            {
                if (a(j, i) != b(j, i))
                    return false;
            }
        return true;
    }

    // printing matrices
    friend std::ostream &operator<<(std::ostream &os, const Matrix &m)
    {
        for (size_t j = 0; j < m.rows; j++)
        {
            for (size_t i = 0; i < m.cols; i++)
                os << m((int)j, (int)i) << " ";
            os << std::endl;
        }
        return os;
    };

    // add matrices
    Matrix<T> operator+(const Matrix<T> &m) const
    {
        if constexpr (Math::SAFE_MATRICES)
        {
            if (m.cols != cols or m.rows != rows)
                throw std::invalid_argument("Matrix dimensions are not compatible");
        }
        Matrix<T> result(rows, cols);

        for (int j = 0; j < rows; j++)
            for (int i = 0; i < cols; i++)
                result(j, i) = (*this)(j, i) + m(j, i);

        return result;
    }

    // substract matrices
    Matrix<T> operator-(const Matrix<T> &m) const
    {
        if constexpr (Math::SAFE_MATRICES)
        {
            if (m.cols != cols or m.rows != rows)
                throw std::invalid_argument("Matrix dimensions are not compatible");
        }
        Matrix<T> result(rows, cols);

        for (int j = 0; j < rows; j++)
            for (int i = 0; i < cols; i++)
                result(j, i) = (*this)(j, i) - m(j, i);

        return result;
    }

    // multiply matrices
    Matrix<T> operator*(const Matrix<T> &m) const
    {
        if constexpr (Math::SAFE_MATRICES)
        {
            if (cols != m.rows)
                throw std::invalid_argument("Matrix dimensions are not compatible");
        }
        Matrix<T> result(rows, m.cols);

        for (int i = 0; i < m.cols; i++)
            for (int j = 0; j < rows; j++)
                for (int k = 0; k < cols; k++)
                    result(j, i) += (*this)(j, k) * m(k, i);

        return result;
    }

    // matrix-scalar operations
    Matrix<T> operator*(const T n) const
    {
        Matrix<T> result(rows, cols);
        for (int j = 0; j < rows; j++)
            for (int i = 0; i < cols; i++)
                result(j, i) = (*this)(j, i) * n;
        return result;
    }
    friend Matrix<T> operator*(const T n, const Matrix<T> &m)
    {
        return m * n;
    }

    // returns identity matrix in type T of size n
    static Matrix<T> id(int n)
    {
        // keep in mind 1 must be castable into T
        Matrix<T> result(n, n);
        for (int i = 0; i < n; i++)
            result(i, i) = (T)1;
        return result;
    }

    // returns transpose, does not transpose in-place
    Matrix<T> transpose() const
    {
        Matrix<T> result(cols, rows);
        for (int j = 0; j < rows; j++)
            for (int i = 0; i < cols; i++)
                result(i, j) = (*this)(j, i);
        return result;
    }

    // determinant (calls minor)
    T det() const
    {
        T determinant = (T)0;
        if (rows != cols)
            throw std::domain_error("Cannot compute determinant of non-square matrix");
        int n = rows;
        if (n == 1)
            return data[0];
        int sign = 1;
        for (int i = 0; i < rows; i++)
        {
            determinant += (*this)(0, i) * minor(0, i) * sign;
            sign *= -1;
        }
        return determinant;
    }

    // minor (calls determinant)
    T minor(int x, int y) const
    {
        if (rows != cols)
            throw std::domain_error("Cannot compute minor of non-square matrix");
        int n = rows;
        if (n == 1)
            return data[0];
        Matrix<T> m(n - 1, n - 1);
        for (int i = 0; i < n; i++)
        {
            if (i == x)
                continue;
            for (int j = 0; j < n; j++)
            {
                if (j == y)
                    continue;
                m((i < x) ? i : i - 1, (j < y) ? j : j - 1) = (*this)(i, j);
            }
        }
        return m.det();
    }

    // inverse
    Matrix<T> inverse() const
    {
        if (rows != cols)
            throw std::domain_error("Cannot compute inverse of non-square matrix");
        T determinant = det();
        if (determinant == 0)
            throw std::runtime_error("Matrix determinant is 0");
        int n = rows;
        Matrix<T> inverse(n, n);
        int isign = 1;
        for (int i = 0; i < n; i++)
        {
            int jsign = 1;
            for (int j = 0; j < n; j++)
            {
                inverse(j, i) = (T)(isign * jsign) * minor(i, j) / determinant;
                jsign *= -1;
            }
            isign *= -1;
        }
        return inverse;
    }

    // like is used to compare matrices of (maybe) different types with a certain tolerance (double precision)
    template <typename V>
    bool like(const Matrix<V> &m, double TOLERANCE = 1e-14 /*tolerance should be enough for most physical applications*/) const
    {
        if (rows != m.getRows() or cols != m.getCols())
            return false;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
            {
                double v1 = static_cast<double>((*this)(i, j));
                double v2 = static_cast<double>(m(i, j));
                if (std::abs(v1 - v2) >= TOLERANCE)
                    return false;
            }
        return true;
    }

    // vector operations
    Vector3<T> operator*(const Vector3<T> &v) const
    {
        if (rows != 3 or cols != 3)
            throw std::invalid_argument("Matrix must be 3x3 for vector multiplication");

        Vector3<T> result;
        for (int rcoord = 0; rcoord < 3; rcoord++)
            for (int vcoord = 0; vcoord < 3; vcoord++)
                result[rcoord] += (*this)(rcoord, vcoord) * v[vcoord];
        return result;
    }
};
