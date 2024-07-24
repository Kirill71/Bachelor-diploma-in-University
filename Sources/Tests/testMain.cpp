#include "Tests/testslib.hpp"
#include "Model/Matrix.hpp"

DECLARE_OOP_TEST(MATRIX_SIMPLE_TEST);

void MATRIX_SIMPLE_TEST()
{
    const Matrix m(3, 4);

    assert(m.rowSize() == 3);
    assert(m.collSize() == 4);
}
