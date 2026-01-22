#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

template <typename T>
class Matrix
{
private:
    int cols;
    int rows;
    T *data;

public:
    // normal constructor
    Matrix(int rows, int cols)
    {
        this->cols = cols;
        this->rows = rows;
        data = new T[cols * rows];
        for (int i = 0; i < cols * rows; i++)
            data[i] = (T)0;
    }
    // construct from 2d vector
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
        this->rows = v.size();
        this->cols = v[0].size();
        data = new T[cols * rows];
        for (int j = 0; j < rows; j++)
            for (int i = 0; i < cols; i++)
                (*this)(j, i) = v[j][i];
    }

    // copy constructor
    Matrix(const Matrix &other)
    {
        this->rows = other.rows;
        this->cols = other.cols;
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
        if (x >= cols)
        {
            throw std::out_of_range("Matrix width exceeded");
        }
        if (y >= rows)
        {
            throw std::out_of_range("Matrix height exceeded");
        }
        return data[this->cols * y + x];
    }

    // accessing elements (read, const)
    T operator()(int y, int x) const
    {
        if (x >= cols)
        {
            throw std::out_of_range("Matrix width exceeded");
        }
        if (y >= rows)
        {
            throw std::out_of_range("Matrix height exceeded");
        }
        return data[this->cols * y + x];
    }

    // getters for rows and cols
    int getRows() const
    {
        return this->rows;
    }
    int getCols() const
    {
        return this->cols;
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
        if (m.cols != this->cols or m.rows != this->rows)
            throw std::invalid_argument("Matrix dimensions are not compatible");
        Matrix<T> result(this->rows, this->cols);

        for (int j = 0; j < this->rows; j++)
            for (int i = 0; i < this->cols; i++)
                result(j, i) = (*this)(j, i) + m(j, i);

        return result;
    }

    // substract matrices
    Matrix<T> operator-(const Matrix<T> &m) const
    {
        if (m.cols != this->cols or m.rows != this->rows)
            throw std::invalid_argument("Matrix dimensions are not compatible");
        Matrix<T> result(this->rows, this->cols);

        for (int j = 0; j < this->rows; j++)
            for (int i = 0; i < this->cols; i++)
                result(j, i) = (*this)(j, i) - m(j, i);

        return result;
    }

    // multiply matrices
    Matrix<T> operator*(const Matrix<T> &m) const
    {
        if (this->cols != m.rows)
            throw std::invalid_argument("Matrix dimensions are not compatible");
        Matrix<T> result(this->rows, m.cols);

        for (int i = 0; i < m.cols; i++)
            for (int j = 0; j < this->rows; j++)
                for (int k = 0; k < this->cols; k++)
                    result(j, i) += (*this)(j, k) * m(k, i);

        return result;
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
        Matrix<T> result(this->cols, this->rows);
        for (int j = 0; j < this->rows; j++)
            for (int i = 0; i < this->cols; i++)
                result(i, j) = (*this)(j, i);
        return result;
    }

    // determinant (calls minor)
    T det() const
    {
        T determinant = (T)0;
        if (this->rows != this->cols)
            throw std::domain_error("Cannot compute determinant of non-square matrix");
        int n = this->rows;
        if (n == 1)
            return this->data[0];
        int sign = 1;
        for (int i = 0; i < this->rows; i++)
        {
            determinant += (*this)(0, i) * this->minor(0, i) * sign;
            sign *= -1;
        }
        return determinant;
    }

    // minor (calls determinant)
    T minor(int x, int y) const
    {
        if (this->rows != this->cols)
            throw std::domain_error("Cannot compute minor of non-square matrix");
        int n = this->rows;
        if (n == 1)
            return this->data[0];
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
        if (this->rows != this->cols)
            throw std::domain_error("Cannot compute inverse of non-square matrix");
        T determinant = this->det();
        if (determinant == 0)
            throw std::runtime_error("Matrix determinant is 0");
        int n = this->rows;
        Matrix<T> inverse(n, n);
        int isign = 1;
        for (int i = 0; i < n; i++)
        {
            int jsign = 1;
            for (int j = 0; j < n; j++)
            {
                inverse(j, i) = (T)(isign * jsign) * this->minor(i, j) / determinant;
                jsign *= -1;
            }
            isign *= -1;
        }
        return inverse;
    }

    // like is used to compare matrices of different types with a certain tolerance (double precision)
    template <typename V>
    bool like(const Matrix<V> &m, double TOLERANCE = 1e-14 /*tolerance should be enough for most physical applications*/) const
    {
        if (this->rows != m.getRows() or this->cols != m.getCols())
            return false;
        for (int i = 0; i < this->rows; i++)
            for (int j = 0; j < this->cols; j++)
            {
                double v1 = static_cast<double>((*this)(i, j));
                double v2 = static_cast<double>(m(i, j));
                if (std::abs(v1 - v2) >= TOLERANCE)
                    return false;
            }
        return true;
    }
};
