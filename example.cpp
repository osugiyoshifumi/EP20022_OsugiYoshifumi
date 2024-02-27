#include <arm_neon.h>
#include <stdint.h>
#include <pybind11/pybind11.h>

uint64_t _mm_popcnt_u64(uint64_t x)
{
    uint64_t count = 0;
    uint8x8_t input_val, count8x8_val;
    uint16x4_t count16x4_val;
    uint32x2_t count32x2_val;
    uint64x1_t count64x1_val;

    input_val = vld1_u8(reinterpret_cast<const uint8_t*>(&x));
    count8x8_val = vcnt_u8(input_val);
    count16x4_val = vpaddl_u8(count8x8_val);
    count32x2_val = vpaddl_u16(count16x4_val);
    count64x1_val = vpaddl_u32(count32x2_val);
    vst1_u64(&count, count64x1_val);
    return count;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("_mm_popcnt_u64", &_mm_popcnt_u64, "A function that adds two numbers");
}
