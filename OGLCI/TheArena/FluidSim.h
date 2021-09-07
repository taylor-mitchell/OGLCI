#pragma once

#include "MathDefs.h"
#include "array2D.h"
#include "PCGSolver.h"
#include <vector>
#include "Renderer.h"
#include <gtx/norm.hpp>
#include "Circle.h"
class sorter;

class FluidSim {
public:
    virtual ~FluidSim();

    scalar rho;

    enum BOUNDARY_TYPE
    {
        BT_CIRCLE,
        BT_BOX,
        BT_HEXAGON,
        BT_TRIANGLE,
        BT_TORUS,
        BT_CYLINDER,

        BT_INTERSECTION,
        BT_UNION,

        BT_COUNT
    };

    struct Boundary
    {
        Boundary(const Vector2s& center_, const Vector2s& parameter_, BOUNDARY_TYPE type_, bool inside);

        Boundary(Boundary* op0_, Boundary* op1_, BOUNDARY_TYPE type_);

        Vector2s center;
        Vector2s parameter;

        Boundary* op0;
        Boundary* op1;

        BOUNDARY_TYPE type;
        scalar sign;
    };

    void initialize(const Vector2s& origin_, scalar width, int ni_, int nj_, scalar rho_,
        bool draw_grid_ = true, bool draw_particles_ = true, bool draw_velocities_ = true, bool draw_boundaries_ = true);
    void advance(scalar dt);
    void update_boundary();
    void init_random_particles();
    void render(const Renderer& renderer);
    void render_boundaries(Boundary& b, const Renderer& renderer);
    scalar compute_phi(const Vector2s& pos) const;
    scalar compute_phi(const Vector2s& pos, Boundary& b) const;

    // Boundaries
    Boundary* root_boundary;
    Boundary* root_sources;

    // Grid Origin
    Vector2s origin;

    // Grid dimensions
    int ni, nj;
    scalar dx;

    // Fluid velocity
    Array2s u, v;
    Array2s temp_u, temp_v;
    Array2s saved_u, saved_v;

    // Tracer particles
    std::vector<Circle> particles;

    // Static geometry representation
    Array2s nodal_solid_phi;
    Array2s liquid_phi;
    Array2s u_weights, v_weights;

    // Data arrays for extrapolation
    Array2c valid, old_valid;
    Array2c u_valid, v_valid;

    sorter* m_sorter;

    // Solver data
    robertbridson::PCGSolver<scalar> solver;
    robertbridson::SparseMatrix<scalar> matrix;
    std::vector<scalar> rhs;
    std::vector<scalar> pressure;

    Vector2s get_velocity(const Vector2s& position);
    Matrix2s get_affine_matrix(const Vector2s& position);

    Vector2s get_saved_velocity(const Vector2s& position);
    Matrix2s get_saved_affine_matrix(const Vector2s& position);
    void add_particle(const Circle& position);

    void map_p2g();
    void map_g2p_pic();
    void map_g2p_apic();
    void map_g2p_aflip(const scalar coeff = 0.95);
    void map_g2p_flip(const scalar coeff = 0.95);

    void save_velocity();

    void compute_cohesion_force();
    void compute_density();
    void compute_normal();
    void correct(scalar dt);
    void resample(Vector2s& p, Vector2s& u, Matrix2s& c);

    bool draw_grid;
    bool draw_particles;
    bool draw_velocities;
    bool draw_boundaries;

protected:
    inline scalar circle_phi(const Vector2s& position, const Vector2s& centre, scalar radius) const {
        return (glm::l2Norm(glm::vec3(position - centre, 0.0f)) - radius);
    }

    inline scalar box_phi(const Vector2s& position, const Vector2s& centre, const Vector2s& expand) const {
        scalar dx = fabs(position[0] - centre[0]) - expand[0];
        scalar dy = fabs(position[1] - centre[1]) - expand[1];
        scalar dax = max(dx, 0.0f);
        scalar day = max(dy, 0.0f);
        return min(max(dx, dy), 0.0f) + sqrt(dax * dax + day * day);
    }

    inline scalar hexagon_phi(const Vector2s& position, const Vector2s& centre, scalar radius) const {
        scalar dx = fabs(position[0] - centre[0]);
        scalar dy = fabs(position[1] - centre[1]);
        return max((dx * 0.866025f + dy * 0.5f), dy) - radius;
    }

    inline scalar triangle_phi(const Vector2s& position, const Vector2s& centre, scalar radius) const {
        scalar px = position[0] - centre[0];
        scalar py = position[1] - centre[1];
        scalar dx = fabs(px);
        return max(dx * 0.866025f + py * 0.5f, -py) - radius * 0.5f;
    }
/*
    inline scalar cylinder_phi(const Vector2s& position, const Vector2s& centre, scalar theta, scalar radius) const {
        Matrix2s nhat;
        nhat[0].x = cos(theta);
        nhat[0].y = sin(theta);
        nhat[1].x = sin(theta);
        nhat[1].y = cos(theta) * sin(theta);

        Vector2s dx = position - centre;
        return sqrt(dx * (Matrix2s(1.0f) - nhat) * dx) - radius;
    }
    */

    inline scalar union_phi(const scalar& d1, const scalar& d2) const {
        return min(d1, d2);
    }

    inline scalar intersection_phi(const scalar& d1, const scalar& d2) const {
        return max(d1, d2);
    }

    inline scalar substraction_phi(const scalar& d1, const scalar& d2) const {
        return max(-d1, d2);
    }

    inline scalar torus_phi(const Vector2s& position, const Vector2s& centre, scalar radius0, scalar radius1) const {
        return max(-circle_phi(position, centre, radius0), circle_phi(position, centre, radius1));
    }

    Vector2s trace_rk2(const Vector2s& position, scalar dt);

    //tracer particle operations

    void advect_particles(scalar dt);

    //fluid velocity operations
    void advect(scalar dt);
    void add_force(scalar dt);

    void project(scalar dt);
    void compute_weights();
    void solve_pressure(scalar dt);
    void compute_phi();

    void constrain_velocity();

    scalar cfl();
};
