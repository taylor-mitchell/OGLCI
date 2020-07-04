#pragma once
#include "MathDefs.h"
#include <algorithm>
#include <gtx/norm.hpp>

namespace kernel {

    // Equ. (2) in [Akinci et al. 2013]
    inline scalar akinci_cohesion_kernel(const scalar& r, const scalar& h) {
        scalar coeff = 32.f / (scalar)M_PI / pow(h, 9.f);

        if (2.f * r > h && r <= h)
        {
            return coeff * (h - r) * (h - r) * (h - r) * r * r * r;
        }
        else if (r > 0.0f && 2.f * r <= h)
        {
            return coeff * (2.f * (h - r) * (h - r) * (h - r) * r * r * r - pow(h, 6.f) / 64.f);
        }
        else {
            return 0.0f;
        }
    }

    inline scalar smooth_kernel(const scalar& r2, const scalar& h) {
        return std::max(pow(1.0f - r2 / (h * h), 3.0f), 0.0f);
    }

    inline scalar poly6_kernel(const scalar& r2, const scalar& h) {
        if (r2 <= h * h) return 4.0f / ((scalar)M_PI * pow(h, 8.0f)) * pow(h * h - r2, 3.0f);
        else return 0.0f;
    }

    inline Vector2s grad_poly6_kernel(const Vector2s& d, const scalar& h) {
        scalar norm = glm::l2Norm(glm::vec3(d, 0.0f));
        scalar r2 = norm * norm;
        if (r2 <= h * h) return -24.0f / ((scalar)M_PI * pow(h, 8.0f)) * pow(h * h - r2, 2.0f) * d;
        else return Vector2s(0.0f);
    }

    inline scalar smooth_kernel_laplacian(const scalar& r2, const scalar& h) {
        scalar x2 = sqrt(r2 / (h * h));
        return x2 > 1.0f ? 0.0f : (1.0f - x2);
    }

    inline scalar sharp_kernel(const scalar& r2, const scalar& h) {
        return std::max(h * h / std::max(r2, 1.0e-5f) - 1.0f, 0.0f);
    }

    inline scalar linear_kernel(const Vector2s& d, const scalar& h) {
        return std::max((1.0f - fabs(d.x / h)) * (1.0f - fabs(d.y / h)), 0.0f);
    }
}